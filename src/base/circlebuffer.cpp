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

uint32_t CircularBuffer::WriteToTheEnd(const void* ptr, uint32_t size) noexcept
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

uint32_t CircularBuffer::WriteToTheBegin(const void* ptr, uint32_t size) noexcept
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

uint32_t CircularBuffer::Push(const void* ptr, uint32_t size) noexcept
{
    const uint8_t* curPtr = reinterpret_cast<const uint8_t*>(ptr);
    if (m_state == Full || size == 0)
        return 0;
    uint32_t sizeRest = size;

    if (m_tail <= m_head) {
        sizeRest = WriteToTheEnd(ptr, size);
        curPtr += size - sizeRest;
        if (m_state == Full || sizeRest == 0)
            return size - sizeRest;
    }

    sizeRest = WriteToTheBegin(curPtr, sizeRest);
    return size - sizeRest;
}

uint8_t CircularBuffer::Pop()
{
    uint8_t v;
    if (SizeToConsume() > 0) {
        v = *Ptr();
        Consume(1);
    } else
        throw std::runtime_error("Not enough data in buffer.");
    return v;
}

uint32_t CircularBuffer::SizeToConsume() noexcept
{
    if (m_state == Empty)
        return 0;

    if (m_tail < m_head) {
        return m_head - m_tail;
    }

    return Eob() - m_tail;
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

uint32_t CircularBuffer::Pop(void* ptr, uint32_t size) noexcept
{

    uint32_t ts1 = std::min(SizeToConsume(), size);
    if (ts1 == 0)
        return 0;
    memcpy(ptr, Ptr(), ts1);
    Consume(ts1);

    uint32_t ts2 = std::min(SizeToConsume(), size - ts1);
    if (ts2 == 0)
        return ts1;
    memcpy(reinterpret_cast<uint8_t*>(ptr) + ts1, Ptr(), ts2);
    Consume(ts2);

    return ts1 + ts2;
}

uint32_t CircularBuffer::PushLatecomer(uint32_t lateSize, const uint8_t* ptr, uint32_t size) noexcept
{
    if (IsEmpty() || size == 0) {
        return 0;
    } else if (m_head > m_tail) {
        uint8_t* stop = m_head - lateSize;
        uint8_t* start = stop - size;
        if (stop < m_tail)
            return 0;
        start = std::max(start, m_tail);
        if (start < m_buffer.get() + m_size) {
            memcpy(start, ptr, stop - start);
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
            memcpy(start, ptr, stop - start);
            ptr += stop - start;
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
            memcpy(start, ptr, stop - start);
            consumed += stop - start;
        }
        return consumed;
    }
}

uint32_t CircularBuffer::Blocks() const noexcept
{
    if (m_size == 0)
        return 0;
    if (m_head > m_tail)
        return 1;
    else
        return 2;
}


} // namespace sparrow