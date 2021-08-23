/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include "circlebuffer.h"

namespace sparrow
{

CircularBuffer::CircularBuffer(uint32_t size)
: m_size(size)
{
    m_buffer.reset(new uint8_t[m_size]);
    m_head = m_tail = m_buffer.get();
    m_state = m_size == 0 ? Full : Empty;
}

uint32_t CircularBuffer::WriteEnd(const void* ptr, uint32_t size) noexcept
{
    assert(m_tail <= m_head);
    const uint8_t* curPtr = reinterpret_cast<const uint8_t*>(ptr);
    uint32_t canWrite = std::min(size, uint32_t(Eob() - m_head));
    memcpy(m_head, curPtr, canWrite);
    size -= canWrite;
    m_head += canWrite;

    assert(m_head <= Eob());

    if (m_head == Eob())
        m_head = m_buffer.get();

    m_state = m_head == m_tail ? Full : Unknown;

    return size;
}

uint32_t CircularBuffer::WriteBegin(const void* ptr, uint32_t size) noexcept
{
    assert(m_tail > m_head);
    const uint8_t* curPtr = reinterpret_cast<const uint8_t*>(ptr);
    uint32_t canWrite = std::min(size, uint32_t(m_tail - m_head));
    memcpy(m_head, curPtr, canWrite);
    size -= canWrite;
    m_head += canWrite;
    assert(m_head <= m_tail);
    m_state = m_head == m_tail ? Full : Unknown;

    return size;
}

uint32_t CircularBuffer::Set(const void* ptr, uint32_t size) noexcept
{
    const uint8_t* curPtr = reinterpret_cast<const uint8_t*>(ptr);
    if (m_state == Full || size == 0)
        return 0;
    uint32_t sizeRest = size;

    if (m_tail <= m_head) {
        sizeRest = WriteEnd(ptr, size);
        curPtr += size - sizeRest;
        if (m_state == Full || sizeRest == 0)
            return size - sizeRest;
    }

    sizeRest = WriteBegin(curPtr, sizeRest);
    return size - sizeRest;
}

uint8_t CircularBuffer::Get()
{
    uint8_t v;
    if (ConsumeSize() > 0) {
        v = *Ptr();
        Consume(1);
    } else
        throw std::runtime_error("Not enough data in buffer.");
    return v;
}

uint32_t CircularBuffer::ConsumeSize() noexcept
{
    if (m_state == Empty)
        return 0;

    if (m_tail < m_head)
        return m_head - m_tail;
    else
        return Eob() - m_tail;
}

uint32_t CircularBuffer::PopulateSize() noexcept
{
    if (IsFull())
        return 0;
    if (m_head >= m_tail)
        return Eob() - m_head;
    else
        return m_tail - m_head;
}

void CircularBuffer::Populate(uint32_t size) noexcept
{
    if (size == 0)
        return;
    assert(m_state != Full);

    if (m_tail <= m_head) {
        assert(m_head + size <= Eob());
        m_head += size;
    } else {
        assert(m_head + size <= m_tail);
        m_head += size;
    }

    if (m_head == Eob())
        m_head = m_buffer.get();

    if (m_tail == m_head)
        m_state = Full;
    else
        m_state = Unknown;
}

void CircularBuffer::Consume(uint32_t size) noexcept
{
    if (size == 0)
        return;

    assert(m_state != Empty);

    assert(m_tail + size <= Eob());

    m_tail += size;

    if (m_tail == Eob())
        m_tail = m_buffer.get();

    if (m_tail == m_head)
        m_state = Empty;
    else
        m_state = Unknown;
}

uint32_t CircularBuffer::FilledSize() const noexcept
{
    if (m_state == Full)
        return m_size;

    if (m_tail <= m_head)
        return m_head - m_tail;
    else
        return (Eob() - m_tail) + m_head - m_buffer.get();
}

uint32_t CircularBuffer::Get(void* ptr, uint32_t size) noexcept
{

    uint32_t ts1 = std::min(ConsumeSize(), size);
    if (ts1 == 0)
        return 0;
    memcpy(ptr, Ptr(), ts1);
    Consume(ts1);

    uint32_t ts2 = std::min(ConsumeSize(), size - ts1);
    if (ts2 == 0)
        return ts1;
    memcpy(reinterpret_cast<uint8_t*>(ptr) + ts1, Ptr(), ts2);
    Consume(ts2);

    return ts1 + ts2;
}

uint32_t CircularBuffer::SetLatecomer(uint32_t lateSize, const void* ptr, uint32_t size) noexcept
{
    const uint8_t* point = static_cast<const uint8_t*>(ptr);
    if (IsEmpty() || size == 0) {
        return 0;
    } else if (m_head > m_tail) {
        uint8_t* stop = m_head - lateSize;
        uint8_t* start = stop - size;
        if (stop < m_tail)
            return 0;
        start = std::max(start, m_tail);
        if (start < m_buffer.get() + m_size) {
            memcpy(start, point, stop - start);
            return stop - start;
        }
        return 0;
    } else {

        uint32_t consumed = 0;

        uint32_t s1 = m_buffer.get() + m_size - m_tail;
        if (s1 < lateSize)
            lateSize -= s1;
        else {
            uint8_t* start = m_tail + lateSize;
            uint8_t* stop = std::min(m_buffer.get() + m_size, start + size);
            memcpy(start, point, stop - start);
            point += stop - start;
            consumed += stop - start;
            size -= stop - start;
            lateSize = 0;
        }

        uint32_t s2 = m_buffer.get() - m_head;
        if (s1 < lateSize)
            return consumed;
        else {
            uint8_t* start = m_buffer.get() + lateSize;
            uint8_t* stop = std::min(start + size, m_head);
            memcpy(start, point, stop - start);
            consumed += stop - start;
        }
        return consumed;
    }
}

uint32_t CircularBuffer::Blocks() const noexcept
{
    if (m_state == Empty) {
        return 0;
    }
    if (m_tail < m_head)
        return 1;
    else
        return 2;
}

} // namespace sparrow