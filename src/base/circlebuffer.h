/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <memory.h>
#include <memory>
#include <utils.h>

namespace sparrow
{

class CircularBuffer
{
    enum State {
        Unknown,
        Full,
        Empty
    };

public:
    CircularBuffer(uint32_t size);

    MOVEBLE_DEFAULT(CircularBuffer);

    uint32_t Push(const void* ptr, uint32_t size) noexcept;

    uint32_t Push(const char* ptr) noexcept { return Push(ptr, strlen(ptr)); }

    uint8_t Pop();

    bool IsEmpty() const noexcept { return m_state == Empty; }

    bool IsFull() const noexcept { return m_state == Full; }

    const uint8_t* Payload() const noexcept { return m_buffer.get(); }

    uint32_t ConsumeSize() noexcept;

    uint32_t PopulateSize() noexcept;

    void Consume(uint32_t size) noexcept;

    void Populate(uint32_t size) noexcept;

    const uint8_t* Ptr() const noexcept { return m_tail; }

    uint8_t* Ptr() noexcept { return m_tail; }

    uint32_t AvailableSize() const noexcept { return m_size - FilledSize(); }

    uint32_t FilledSize() const noexcept;

    uint32_t Pop(void* ptr, uint32_t size) noexcept;

    const uint8_t* Tail() const noexcept { return m_tail; }

    uint8_t* Tail() noexcept { return m_tail; }

    const uint8_t* Head() const noexcept { return m_head; }

    uint8_t* Head() noexcept { return m_head; }

    const uint8_t* Eob() const noexcept { return m_buffer.get() + m_size; }

    const uint8_t* Buffer() const noexcept { return m_buffer.get(); }

    uint32_t PushLatecomer(uint32_t lateSize, const uint8_t* ptr, uint32_t size) noexcept;

    uint32_t Size() const noexcept { return m_size; }

    uint32_t Blocks() const noexcept;

private:

    uint32_t WriteEnd(const void* ptr, uint32_t size) noexcept;
    uint32_t WriteBegin(const void* ptr, uint32_t size) noexcept;

    uint8_t* m_tail = nullptr;
    uint8_t* m_head = nullptr;

    uint32_t m_size;

    std::unique_ptr<uint8_t[]> m_buffer;
    State m_state;
};

} // namespace sparrow