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

/*! \brief Circular buffer.
 *
 *  Circular buffer is used to hold temporary date.
 */
class CircularBuffer
{
    enum State {
        Unknown, /**< buffer is neither Full nor empty */
        Full,    /**< buffer is full */
        Empty    /**< buffer is empty */
    };

public:
    //! Constructor
    /*!
      \param size is the size of buffer.
    */
    CircularBuffer(uint32_t size);

    //! Makes class moveable(and noncopyable)
    MOVEBLE_DEFAULT(CircularBuffer);

    //! Set data to the buffer
    /*!
      \param ptr point to the data buffer.
      \param size is the size of data buffer.
      \return size of consumed data 
      \sa uint32_t Set(const char* ptr) noexcept
    */
    uint32_t Set(const void* ptr, uint32_t size) noexcept;

    //! Set string to the buffer
    /*!
      \param ptr point to the data buffer.
      \return size of consumed data 
      \sa uint32_t Set(const void* ptr, uint32_t size) noexcept
    */
    uint32_t Set(const char* ptr) noexcept { return Set(ptr, strlen(ptr)); }

    //! Get single byte fron buffer and free one byte
    /*!
      \return single byte 
    */
    uint8_t Get();

    //! Return true if buffer is empty
    /*!
      \return true/false 
    */
    bool IsEmpty() const noexcept { return m_state == Empty; }

    //! Return true if buffer is full
    /*!
      \return true/false 
    */
    bool IsFull() const noexcept { return m_state == Full; }

    uint32_t ConsumeSize() noexcept;

    uint32_t PopulateSize() noexcept;

    void Consume(uint32_t size) noexcept;

    void Populate(uint32_t size) noexcept;

    const uint8_t* Ptr() const noexcept { return m_tail; }

    uint8_t* Ptr() noexcept { return m_tail; }

    uint32_t AvailableSize() const noexcept { return m_size - FilledSize(); }

    uint32_t FilledSize() const noexcept;

    uint32_t Pop(void* ptr, uint32_t size) noexcept;

    uint32_t PushLatecomer(uint32_t lateSize, const uint8_t* ptr, uint32_t size) noexcept;

    //! Return size of reserved bytes.
    /*!
      \return Return size of reserved bytes 
    */
    uint32_t Reserved() const noexcept { return m_size; }

    //! Return number of continuous blocks in buffer.
    //! May be 0 - buffer is empty,
    //!        1 - buffer has only one block (tail is less than head),
    //!        2 - buffer has two blocks(from tail to end of buffer, from begin of buffer to head)
    /*!
      \return number of block in buffer 
    */
    uint32_t Blocks() const noexcept;

    // Internal and auxillary methods  
    const uint8_t* Tail() const noexcept { return m_tail; }

    uint8_t* Tail() noexcept { return m_tail; }

    const uint8_t* Head() const noexcept { return m_head; }

    uint8_t* Head() noexcept { return m_head; }

    const uint8_t* Eob() const noexcept { return m_buffer.get() + m_size; }

    const uint8_t* Buffer() const noexcept { return m_buffer.get(); }

private:
    //! Write to the end of buffer.
    //! \param ptr - pointer to the buffer
    //! \param size of buffer
    /*!
      \return number of bytes which been really consumed 
    */
    uint32_t WriteEnd(const void* ptr, uint32_t size) noexcept;
    //! Write to the begin of buffer.
    //! \param ptr - pointer to the buffer
    //! \param size of buffer
    /*!
      \return number of bytes which been really consumed 
    */
    uint32_t WriteBegin(const void* ptr, uint32_t size) noexcept;

    uint8_t* m_tail = nullptr; /*!< pointer to tail of buffer */
    uint8_t* m_head = nullptr; /*!< pointer to head of buffer */

    uint32_t m_size; /*!< totally reserver buffer size*/

    std::unique_ptr<uint8_t[]> m_buffer; /*!< data buffer */
    State m_state;                       /*!< buffer state(full, empty, ...)*/
};

} // namespace sparrow