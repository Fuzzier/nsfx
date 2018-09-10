/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONST_FIXED_BUFFER_H__3937C17D_74A4_4302_B738_102FC0CD9DDF
#define CONST_FIXED_BUFFER_H__3937C17D_74A4_4302_B738_102FC0CD9DDF


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/basic-buffer.h>
#include <nsfx/network/buffer/const-fixed-buffer.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstFixedBuffer.
/**
 * @ingroup Network
 * @brief A fixed-size and read-only buffer.
 */
template<>
class BasicBuffer</*readOnly*/true, /*copyOnResize*/false, /*zeroArea*/false>
{
public:
    typedef BasicBuffer<true, false, false>               ConstFixedBuffer;
    typedef BasicBufferStorage</*trackDirtyArea=*/false>  BufferStorage;
    typedef ConstBufferIterator iterator;
    typedef ConstBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create a buffer.
     *
     * @param[in] buffer A tag buffer.
     *
     * A \c FixedBuffer can be converted to \c ConstFixedBuffer implicitly.
     */
    BasicBuffer(const FixedBuffer& buffer);

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    BasicBuffer(const ConstFixedBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    BasicBuffer& operator=(const ConstFixedBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    BasicBuffer(ConstFixedBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ConstFixedBuffer& operator=(ConstFixedBuffer&& rhs) BOOST_NOEXCEPT;

    // Methods.
public:
    /**
     * @brief Get the size of the represented data.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    const BufferStorage* GetStorage(void) const BOOST_NOEXCEPT;

    /**
     * @brief Copy data to a memory block.
     * @return The number of bytes copied.
     */
    size_t CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    ConstBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ConstBufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(ConstFixedBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The fixed buffer.
     */
    FixedBuffer buffer_;

};


////////////////////////////////////////////////////////////////////////////////
// Typedef.
typedef BasicBuffer</*readOnly*/true, /*copyOnResize*/false, /*zeroArea*/false>
        ConstFixedBuffer;


////////////////////////////////////////////////////////////////////////////////
// ConstFixedBuffer.
inline ConstFixedBuffer::BasicBuffer(const FixedBuffer& buffer) :
    buffer_(buffer)
{
}

inline ConstFixedBuffer::BasicBuffer(const ConstFixedBuffer& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstFixedBuffer& ConstFixedBuffer::operator=(const ConstFixedBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstFixedBuffer::BasicBuffer(ConstFixedBuffer&& rhs) BOOST_NOEXCEPT :
    buffer_(std::move(rhs.buffer_))
{
}

inline ConstFixedBuffer& ConstFixedBuffer::operator=(ConstFixedBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstFixedBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline const ConstFixedBuffer::BufferStorage*
ConstFixedBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstFixedBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstBufferIterator ConstFixedBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstFixedBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstBufferIterator ConstFixedBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstFixedBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstFixedBuffer::swap(ConstFixedBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(buffer_, rhs.buffer_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstFixedBuffer& lhs, ConstFixedBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_FIXED_BUFFER_H__3937C17D_74A4_4302_B738_102FC0CD9DDF

