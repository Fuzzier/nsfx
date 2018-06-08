/**
 * @file
 *
 * @brief Zero-compressed packet buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-08
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83
#define CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/zc-buffer.h>
#include <nsfx/network/buffer/zc-buffer-iterator.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstZcBuffer.
/**
 * @ingroup Network
 * @brief An zero-compressed, automatically resized and copy-on-write buffer.
 *
 * # Structure
 *   The buffer holds a storage that provides a memory space for the buffer.
 *   The storage is logically divided into three areas.
 *   The data area is located in middle of the storage, and the remaining space
 *   is naturally divided in to the pre-data (pre-header) area and the post-data
 *   (post-trailer) area.
 *
 *   To reduce memory consumption, the buffer models a <i>virtual data area</i>.
 *   The buffer is logically divided into three areas: the <i>header area</i>,
 *   the <i>zero-compressed data area</i>, and the <i>trailer area</i>.
 *   The zero-compressed data area does not consume any physical memory, which
 *   is usually used to represent the application layer payload.
 *
 *   When the zero-compressed data area is not empty, the buffer represents a
 *   <i>virtual data buffer</i>.
 *   When the zero-compressed data area is empty, the buffer represents a
 *   <i>real buffer</i>.
 *
 * # Reallocation on expansion
 *   Several buffers can link to the same buffer storage, and each buffer has
 *   its own view of the start and end positions of the data area.
 *
 *   When a buffer expands, its buffer storage may be reallocated.
 *   If the buffer storage is shared by other buffers, and the buffer expands
 *   to an area that has already been occupied by other buffers, the buffer
 *   storage is automatically reallocated and duplicated for the buffer.
 *   The copy-on-write operations are \c ConstZcBuffer::AddAtStart() and
 *   \c ConstZcBuffer::AddAtEnd().
 */
class ConstZcBuffer
{
public:
    typedef ConstZcBufferIterator iterator;
    typedef ConstZcBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     *
     * A \c ZcBuffer can be converted to \c ConstZcBuffer implicitly.
     */
    ConstZcBuffer(const ZcBuffer& buffer) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstZcBuffer(const ConstZcBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstZcBuffer& operator=(const ConstZcBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ConstZcBuffer(ConstZcBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ConstZcBuffer& operator=(ConstZcBuffer&& rhs) BOOST_NOEXCEPT;

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
    ConstZcBuffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    ConstZcBuffer MakeRealBuffer(void) const;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    ConstZcBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ConstZcBufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstZcBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstZcBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(ConstZcBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The buffer.
     */
    ZcBuffer buffer_;

};


////////////////////////////////////////////////////////////////////////////////
// ConstZcBuffer.
inline ConstZcBuffer::ConstZcBuffer(const ZcBuffer& buffer) BOOST_NOEXCEPT :
    buffer_(buffer)
{
}

inline ConstZcBuffer::ConstZcBuffer(const ConstZcBuffer& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstZcBuffer& ConstZcBuffer::operator=(const ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstZcBuffer::ConstZcBuffer(ConstZcBuffer&& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstZcBuffer& ConstZcBuffer::operator=(ConstZcBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstZcBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline size_t ConstZcBuffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetInternalSize();
}

inline size_t ConstZcBuffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return buffer_.GetCapacity();
}

inline size_t ConstZcBuffer::GetStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStart();
}

inline size_t ConstZcBuffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroStart();
}

inline size_t ConstZcBuffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroEnd();
}

inline size_t ConstZcBuffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetEnd();
}

inline const BufferStorage* ConstZcBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstZcBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstZcBuffer ConstZcBuffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.MakeFragment(start, size);
}

inline ConstZcBuffer ConstZcBuffer::MakeRealBuffer(void) const
{
    return buffer_.MakeRealBuffer();
}

inline void ConstZcBuffer::Realize(void) const
{
    buffer_.Realize();
}

inline ConstZcBufferIterator ConstZcBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstZcBufferIterator ConstZcBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstZcBufferIterator ConstZcBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstZcBufferIterator ConstZcBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstZcBuffer::swap(ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    boost::swap(buffer_, rhs.buffer_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstZcBuffer& lhs, ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83

