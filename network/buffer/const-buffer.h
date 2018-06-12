/**
 * @file
 *
 * @brief Packet buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-05
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer.h>
#include <boost/core/swap.hpp>


#ifndef CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32
#define CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstBuffer.
/**
 * @ingroup Network
 * @brief A read-only buffer.
 */
class ConstBuffer
{
public:
    typedef ConstBufferIterator iterator;
    typedef ConstBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create a buffer.
     *
     * @param[in] buffer A buffer.
     *
     * A \c Buffer can be converted implicitly to a \c ConstBuffer.
     */
    ConstBuffer(const Buffer& buffer);

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstBuffer(const ConstBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstBuffer& operator=(const ConstBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ConstBuffer(ConstBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ConstBuffer& operator=(ConstBuffer&& rhs) BOOST_NOEXCEPT;

    // Methods.
public:
    /**
     * @brief Get the size of the represented data.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the size of the actually used memory.
     */
    size_t GetInternalSize(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the capacity of the storage.
     */
    size_t GetCapacity(void) const BOOST_NOEXCEPT;

    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetZeroStart(void) const BOOST_NOEXCEPT;
    size_t GetZeroEnd(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;
    const BufferStorage* GetStorage(void) const BOOST_NOEXCEPT;

    /**
     * @brief Copy data to a memory block.
     * @return The number of bytes copied.
     */
    size_t CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT;

    // Fragmentation.
public:
    /**
     * @brief Make a fragment of the buffer.
     *
     * @param[in] start The start of the fragment.
     * @param[in] size  The size of the fragment.
     */
    ConstBuffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    ConstBuffer MakeRealBuffer(void) const BOOST_NOEXCEPT;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const BOOST_NOEXCEPT;

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
    void swap(ConstBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The buffer.
     */
    Buffer buffer_;

};


////////////////////////////////////////////////////////////////////////////////
// ConstBuffer.
inline ConstBuffer::ConstBuffer(const Buffer& buffer) :
    buffer_(buffer)
{
}

inline ConstBuffer::ConstBuffer(const ConstBuffer& rhs) BOOST_NOEXCEPT :
   buffer_(rhs.buffer_)
{
}

inline ConstBuffer& ConstBuffer::operator=(const ConstBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstBuffer::ConstBuffer(ConstBuffer&& rhs) BOOST_NOEXCEPT :
        buffer_(std::move(rhs.buffer_))
{
}

inline ConstBuffer& ConstBuffer::operator=(ConstBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline size_t ConstBuffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetInternalSize();
}

inline size_t ConstBuffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return buffer_.GetCapacity();
}

inline size_t ConstBuffer::GetStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStart();
}

inline size_t ConstBuffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroStart();
}

inline size_t ConstBuffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroEnd();
}

inline size_t ConstBuffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetEnd();
}

inline const BufferStorage* ConstBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstBuffer ConstBuffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.MakeFragment(start, size);
}

inline ConstBuffer ConstBuffer::MakeRealBuffer(void) const BOOST_NOEXCEPT
{
    return buffer_.MakeRealBuffer();
}

inline void ConstBuffer::Realize(void) const BOOST_NOEXCEPT
{
    buffer_.Realize();
}

inline ConstBufferIterator ConstBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstBufferIterator ConstBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstBuffer::swap(ConstBuffer& rhs) BOOST_NOEXCEPT
{
    boost::swap(buffer_, rhs.buffer_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstBuffer& lhs, ConstBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32

