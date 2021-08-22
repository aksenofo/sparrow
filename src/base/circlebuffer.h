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

    //! Set data to the circular buffer with shifting reference to the begining of buffer
    /*!
      \param lateSize is shifting reference 
      \param ptr point to the data buffer.
      \return size of consumed data 
    */
    uint32_t SetLatecomer(uint32_t lateSize, const void* ptr, uint32_t size) noexcept;

    //! Set data to the circular buffer with shifting reference to the begining of buffer
    /*!
      \param lateSize is shifting reference 
      \param ptr point to the string buffer.
      \return size of consumed data 
    */
    uint32_t SetLatecomer(uint32_t lateSize, const char* ptr) noexcept { return SetLatecomer(lateSize, ptr, strlen(ptr)); }

    //! Get single byte from buffer and free one byte
    /*!
      \return single byte 
    */
    uint8_t Get();

    //! Get bytes from buffer
    /*!
      \param ptr is pointer to the buffer
      \param size of buffer 
      \return actual number of bytes 
    */
    uint32_t Get(void* ptr, uint32_t size) noexcept;

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

    //! Return how many bytes can be consumed from continuous data block
    /*!
      \return size of contunuous data block 
    */
    uint32_t ConsumeSize() noexcept;

    //! Consume some data from continuous block
    //! Att. if data block is less thab consume size than assert.
    /*!
      \param size of data block
    */
    void Consume(uint32_t size) noexcept;

    //! Return how many bytes can be populated to continuous data block
    /*!
      \return size of contunuous data block 
    */
    uint32_t PopulateSize() noexcept;


    //! Populate some data from continuous block
    //! Att. if data block is less thab populate size than assert.
    /*!
      \param size of data block
    */
    void Populate(uint32_t size) noexcept;


    //! Return const. pointer to the actulal buffer position
    /*!
      \return size of contunuous data block 
    */
    const uint8_t* Ptr() const noexcept { return m_tail; }

    //! Return pointer to the actulal buffer position
    /*!
      \return size of contunuous data block 
    */
    uint8_t* Ptr() noexcept { return m_tail; }

    uint32_t AvailableSize() const noexcept { return m_size - FilledSize(); }

    uint32_t FilledSize() const noexcept;

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

/*! \brief Consumer aux class.
 *
 *  Consumer allow simplify process reading data from circular buffer.
 */
class Consumer
{
public:
    Consumer() = default;
    COPYBLE_DEFAULT(Consumer);
    MOVEBLE_DEFAULT(Consumer);

    template<typename Func>
    uint32_t operator()(CircularBuffer& circularBuffer, Func fn)
    {
        uint32_t totallyConsumed = 0;
        while (true) {
            uint32_t size = circularBuffer.ConsumeSize();
            if (size) {
                uint32_t consSize = fn(size);
                assert(consSize <= size);
                circularBuffer.Consume(consSize);
                totallyConsumed += consSize;
                if (consSize < size)
                    break;
            } else
                break;
        }
        return totallyConsumed;
    }
};

/*! \brief Populator aux class.
 *
 *  Populator allow simplify process writing data to circular buffer.
 */
class Populator
{
public:
    Populator() = default;
    COPYBLE_DEFAULT(Populator);
    MOVEBLE_DEFAULT(Populator);

    template<typename Func>
    uint32_t operator()(CircularBuffer& circularBuffer, Func fn)
    {
        uint32_t totallyPopulated = 0;
        while (true) {
            uint32_t size = circularBuffer.PopulateSize();
            if (size) {
                uint32_t popSize = fn(size);
                assert(popSize <= size);
                circularBuffer.Populate(popSize);
                totallyPopulated += popSize;
                if (popSize < size)
                    break;
            } else
                break;
        }
        return totallyPopulated;
    }
};

} // namespace sparrow