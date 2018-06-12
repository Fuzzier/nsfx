/**
 * @file
 *
 * @brief Zero-compressed packet buffer for Network Simulation Frameworks.
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

#ifndef ZC_BUFFER_H__C2DEDFE9_A0FA_405F_AE24_66679E3B3608
#define ZC_BUFFER_H__C2DEDFE9_A0FA_405F_AE24_66679E3B3608


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-storage.h>
#include <nsfx/network/buffer/zc-buffer-iterator.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


class ConstZcBuffer;


////////////////////////////////////////////////////////////////////////////////
// ZcBuffer.
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
 *   The copy-on-write operations are \c ZcBuffer::AddAtStart() and
 *   \c ZcBuffer::AddAtEnd().
 */
class ZcBuffer
{
public:
    typedef ZcBufferIterator      iterator;
    typedef ConstZcBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    ZcBuffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The initial apacity of the buffer.
     *
     * The zero-compressed data area is empty in this buffer.
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data toward the head of the storage.
     */
    explicit ZcBuffer(size_t capacity);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The initial apacity of the buffer.
     * @param[in] zeroSize The size of the zero-compressed data area.
     *
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data at the head of the storage.
     */
    ZcBuffer(size_t capacity, size_t zeroSize);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity   The initial apacity of the buffer.
     * @param[in] zeroStart  The start of the zero-compressed data area.
     *                       <p>
     *                       <code>zeroStart <= capacity</code>.
     * @param[in] zeroSize   The size of the zero-compressed data area.
     */
    ZcBuffer(size_t capacity, size_t zeroStart, size_t zeroSize);

private:
    /**
     * @brief Create a buffer.
     *
     * @param[in] storage  The reference count is taken by the buffer.
     *
     * @see \c InternalMakeRealCopy().
     * @internal
     */
    ZcBuffer(BufferStorage* storage, size_t start, size_t zeroStart,
           size_t zeroEnd, size_t end) BOOST_NOEXCEPT;

public:
    ~ZcBuffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ZcBuffer(const ZcBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ZcBuffer& operator=(const ZcBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ZcBuffer(ZcBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ZcBuffer& operator=(ZcBuffer&& rhs) BOOST_NOEXCEPT;

    // Acquire/release buffer storage.
private:
    void Acquire(void) BOOST_NOEXCEPT;

    void Release(void) BOOST_NOEXCEPT;

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

private:
    struct ContinuousTag {};
    struct SegmentedTag {};

    size_t InternalCopyTo(uint8_t* dst, size_t size, ContinuousTag) const BOOST_NOEXCEPT;
    size_t InternalCopyTo(uint8_t* dst, size_t size, SegmentedTag) const BOOST_NOEXCEPT;

    // Add/remove.
private:
    struct AdjustOffsetTag {};
    struct ReallocateTag {};
    struct MoveMemoryTag {};

public:
    /**
     * @brief Expand the buffer toward the start.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(size_t size);

    /**
     * @brief Expand the buffer and copy the specified contents.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(const uint8_t* src, size_t size);

    /**
     * @brief Expand the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(const ConstZcBuffer& src);

private:
    void InternalAddAtStart(size_t size, AdjustOffsetTag) BOOST_NOEXCEPT;
    void InternalAddAtStart(size_t size, size_t newCapacity,
                            size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtStart(size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT;

public:
    /**
     * @brief Expand the buffer toward the end.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(size_t size);

    /**
     * @brief Extend the buffer and copy the specified contents.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const uint8_t* src, size_t size);

    /**
     * @brief Extend the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const ConstZcBuffer& src);

private:
    void InternalAddAtEnd(size_t size, size_t dataSize, AdjustOffsetTag) BOOST_NOEXCEPT;
    void InternalAddAtEnd(size_t size, size_t newCapacity,
                          size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtEnd(size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT;

public:
    /**
     * @brief Shrink the buffer from the start.
     *
     * @param[in] size The number of bytes to remove.
     *                 <p>
     *                 If it is no less than the size of the buffer, the buffer
     *                 becomes empty.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void RemoveAtStart(size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Shrink the buffer from the end.
     *
     * @param[in] size The number of bytes to remove.
     *                 <p>
     *                 If it is no less than the size of the buffer, the buffer
     *                 becomes empty.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void RemoveAtEnd(size_t size) BOOST_NOEXCEPT;

    // Fragmentation.
public:
    /**
     * @brief Make a fragment of the buffer.
     *
     * @param[in] start The start of the fragment.
     * @param[in] size  The size of the fragment.
     */
    ZcBuffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    ZcBuffer MakeRealBuffer(void) const;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const;

private:
    ZcBuffer InternalGetRealBuffer(ReallocateTag) const;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    ZcBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ZcBufferIterator end(void) BOOST_NOEXCEPT;

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
    void swap(ZcBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The storage.
     */
    BufferStorage* storage_;

    /**
     * @brief The logical offset of the start of the header area.
     */
    size_t start_;

    /**
     * @brief The logical offset of the start of the zero-compressed data area.
     */
    size_t zeroStart_;

    /**
     * @brief The logical offset of the end of the zero-compressed data area (on-byte-beyond).
     */
    size_t zeroEnd_;

    /**
     * @brief The logical offset of the end of the trailer area (on-byte-beyond).
     */
    size_t end_;

};


NSFX_CLOSE_NAMESPACE


#include <nsfx/network/buffer/const-zc-buffer.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ZcBuffer.
inline ZcBuffer::ZcBuffer(void) BOOST_NOEXCEPT :
    storage_(nullptr),
    start_(0),
    zeroStart_(0),
    zeroEnd_(0),
    end_(0)
{
}

inline ZcBuffer::ZcBuffer(size_t capacity) :
    storage_(BufferStorage::Allocate(capacity)),
    start_(capacity),
    zeroStart_(capacity),
    zeroEnd_(capacity),
    end_(capacity)
{
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline ZcBuffer::ZcBuffer(size_t capacity, size_t zeroSize) :
    storage_(BufferStorage::Allocate(capacity)),
    start_(capacity),
    zeroStart_(capacity),
    zeroEnd_(capacity + zeroSize),
    end_(capacity + zeroSize)
{
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline ZcBuffer::ZcBuffer(size_t capacity, size_t zeroStart, size_t zeroSize)
{
    BOOST_ASSERT_MSG(zeroStart <= capacity,
                     "Cannot construct a ZcBuffer, since the start of "
                     "the zero-compressed data area is beyond the end of "
                     "the buffer storage.");
    storage_   = BufferStorage::Allocate(capacity);
    start_     = zeroStart;
    zeroStart_ = zeroStart;
    zeroEnd_   = zeroStart + zeroSize;
    end_       = zeroStart + zeroSize;
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline ZcBuffer::ZcBuffer(BufferStorage* storage, size_t start,
                          size_t zeroStart, size_t zeroEnd, size_t end) BOOST_NOEXCEPT :
    storage_(storage),
    start_(start),
    zeroStart_(zeroStart),
    zeroEnd_(zeroEnd),
    end_(end)
{
    BOOST_ASSERT(start     <= zeroStart);
    BOOST_ASSERT(zeroStart <= zeroEnd);
    BOOST_ASSERT(zeroEnd   <= end);
    BOOST_ASSERT(!storage_ ? true :
                 end - (zeroEnd - zeroStart) <= storage_->capacity_);
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline ZcBuffer::~ZcBuffer(void) BOOST_NOEXCEPT
{
    Release();
}

inline ZcBuffer::ZcBuffer(const ZcBuffer& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_),
    start_(rhs.start_),
    zeroStart_(rhs.zeroStart_),
    zeroEnd_(rhs.zeroEnd_),
    end_(rhs.end_)
{
    Acquire();
}

inline ZcBuffer& ZcBuffer::operator=(const ZcBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        BufferStorage* tmp = storage_;
        storage_   = rhs.storage_;
        start_     = rhs.start_;
        zeroStart_ = rhs.zeroStart_;
        zeroEnd_   = rhs.zeroEnd_;
        end_       = rhs.end_;
        Acquire();
        if (tmp)
        {
            BufferStorage::Release(tmp);
        }
    }
    return *this;
}

inline ZcBuffer::ZcBuffer(ZcBuffer&& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_),
    start_(rhs.start_),
    zeroStart_(rhs.zeroStart_),
    zeroEnd_(rhs.zeroEnd_),
    end_(rhs.end_)
{
    rhs.storage_   = nullptr;
    rhs.start_     = 0;
    rhs.zeroStart_ = 0;
    rhs.zeroEnd_   = 0;
    rhs.end_       = 0;
}

inline ZcBuffer& ZcBuffer::operator=(ZcBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        swap(rhs);
        rhs.Release();
    }
    return *this;
}

inline void ZcBuffer::Acquire(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        BufferStorage::AddRef(storage_);
    }
}

inline void ZcBuffer::Release(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        BufferStorage* tmp = storage_;
        storage_   = nullptr;
        start_     = 0;
        zeroStart_ = 0;
        zeroEnd_   = 0;
        end_       = 0;
        BufferStorage::Release(tmp);
    }
}

inline size_t ZcBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return end_ - start_;
}

inline size_t ZcBuffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return (zeroStart_ - start_) + (end_ - zeroEnd_);
}

inline size_t ZcBuffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return storage_ ? storage_->capacity_ : 0;
}

inline size_t ZcBuffer::GetStart(void) const BOOST_NOEXCEPT
{
    return start_;
}

inline size_t ZcBuffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return zeroStart_;
}

inline size_t ZcBuffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return zeroEnd_;
}

inline size_t ZcBuffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline const BufferStorage* ZcBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return storage_;
}

inline size_t ZcBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    size_t copied = 0;
    if (storage_ && dst)
    {
        if (zeroStart_ == zeroEnd_)
        {
            copied = InternalCopyTo(dst, size, ContinuousTag());
        }
        else
        {
            copied = InternalCopyTo(dst, size, SegmentedTag());
        }
    }
    return copied;
}

inline size_t
ZcBuffer::InternalCopyTo(uint8_t* dst, size_t size, ContinuousTag) const BOOST_NOEXCEPT
{
    size_t dataSize = GetInternalSize();
    size_t copied = (dataSize <= size ? dataSize : size);
    std::memmove(dst, storage_->bytes_ + start_, copied);
    return copied;
}

inline size_t
ZcBuffer::InternalCopyTo(uint8_t* dst, size_t size, SegmentedTag) const BOOST_NOEXCEPT
{
    size_t copied = 0;
    do
    {
        size_t headerSize = zeroStart_ - start_;
        if (size <= headerSize)
        {
            std::memmove(dst, storage_->bytes_ + start_, size);
            copied += size;
            break;
        }
        std::memmove(dst, storage_->bytes_ + start_, headerSize);
        size   -= headerSize;
        copied += headerSize;

        size_t zeroSize = zeroEnd_ - zeroStart_;
        if (size <= zeroSize)
        {
            std::memset(dst + copied, 0, size);
            copied += size;
            break;
        }
        std::memset(dst + copied, 0, zeroSize);
        size   -= zeroSize;
        copied += zeroSize;

        size_t trailerSize = end_ - zeroEnd_;
        if (size <= trailerSize)
        {
            std::memmove(dst + copied,
                         storage_->bytes_ + zeroStart_,
                         size);
            copied += size;
            break;
        }
        std::memmove(dst + copied,
                     storage_->bytes_ + zeroStart_,
                     trailerSize);
        copied += trailerSize;
    }
    while (false);
    return copied;
}

inline void ZcBuffer::AddAtStart(size_t size)
{
    if (!size)
    {
        return;
    }

    size_t dataSize = GetInternalSize();
    // The storage is not yet allocated.
    if (!storage_)
    {
        size_t newCapacity = size;
        size_t newStart = 0;
        InternalAddAtStart(size, newCapacity, newStart,
                           dataSize, ReallocateTag());
        return;
    }

    // Shall we reallocate the storage?
    bool dirty = false;
    // Shall we move the storage if it is not reallocated?
    bool mmove = false;
    // The storage is linked to this buffer alone.
    if (storage_->refCount_ == 1)
    {
        // The pre-header area has enough space.
        if (size <= start_)
        {
            InternalAddAtStart(size, AdjustOffsetTag());
        }
        // The pre-header area does not have enough space.
        else // if (size > start_)
        {
            // The storage has enough space to accommodate the requested size.
            if (storage_->capacity_ >= dataSize + size)
            {
                mmove = true;
                InternalAddAtStart(size, dataSize, MoveMemoryTag());
            }
            // The storage does not have enough space to accommodate
            // the requested size.
            else // if (storage_->capacity_ < dataSize + size)
            {
                dirty = true;
                size_t newCapacity = size + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(size, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
        }
    }
    // The storage is linked to other buffers.
    // We cannot move the data within the storage in this case.
    else // if (storage_->refCount_ > 1)
    {
        // The pre-header area is not used by other buffers.
        if (storage_->dirtyStart_ == start_)
        {
            // The pre-header area has enough space.
            if (size <= start_)
            {
                InternalAddAtStart(size, AdjustOffsetTag());
            }
            // The pre-header area does not have enough space.
            else // if (size > start_)
            {
                dirty = true;
                size_t newCapacity = size + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(size, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
        }
        // The pre-header area is used by other buffers.
        else // if (storage_->dirtyStart_ < start_)
        {
            dirty = true;
            // The pre-header area has enough space.
            if (size <= start_)
            {
                size_t newCapacity = storage_->capacity_;
                size_t newStart = start_ - size;
                InternalAddAtStart(size, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
            // The pre-header area does not have enough space.
            else // if (numBytes_ > start_)
            {
                size_t newCapacity = size + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(size, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
        }
    }
}

inline void ZcBuffer::AddAtStart(const uint8_t* src, size_t size)
{
    BOOST_ASSERT_MSG(src, "Invalid pointer.");
    if (size)
    {
        AddAtStart(size);
        std::memmove(storage_->bytes_ + start_, src, size);
    }
}

inline void ZcBuffer::AddAtStart(const ConstZcBuffer& src)
{
    size_t size = src.GetSize();
    if (size)
    {
        AddAtStart(src.GetSize());
        src.CopyTo(storage_->bytes_ + start_, size);
    }
}

inline void ZcBuffer::InternalAddAtStart(size_t size, AdjustOffsetTag) BOOST_NOEXCEPT
{
    start_ -= size;
    storage_->dirtyStart_ = start_;
}

inline void ZcBuffer::InternalAddAtStart(
    size_t size, size_t newCapacity,
    size_t newStart, size_t dataSize, ReallocateTag)
{
    BufferStorage* newStorage = BufferStorage::Allocate(newCapacity);
    std::memcpy(newStorage->bytes_ + newStart + size,
                  storage_->bytes_ + start_,
                dataSize);
    if (storage_)
    {
        BufferStorage::Release(storage_);
    }
    storage_  = newStorage;
    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = newStart + size + dataSize;

    ptrdiff_t delta = newStart + size - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta;
}

inline void ZcBuffer::InternalAddAtStart(
    size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT
{
    std::memmove(storage_->bytes_ + size,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = 0;
    storage_->dirtyEnd_   = size + dataSize;

    ptrdiff_t delta = size - start_;
    start_      = 0;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta;
}

inline void ZcBuffer::AddAtEnd(size_t size)
{
    if (!size)
    {
        return;
    }

    size_t dataSize = GetInternalSize();
    // The storage is not yet allocated.
    if (!storage_)
    {
        size_t newCapacity = size;
        size_t newStart = 0;
        InternalAddAtEnd(size, newCapacity, newStart,
                         dataSize, ReallocateTag());
        return;
    }

    // Shall we reallocate the storage?
    bool dirty = false;
    // Shall we move the storage if it is not reallocated?
    bool mmove = false;
    size_t postSize = storage_->capacity_ - (start_ + dataSize);
    // The storage is linked to this buffer alone.
    if (storage_->refCount_ == 1)
    {
        // The post-trailer area has enough space.
        if (size <= postSize)
        {
            InternalAddAtEnd(size, dataSize, AdjustOffsetTag());
        }
        // The post-trailer area does not have enough space.
        else // if (size > postSize)
        {
            // The storage has enough space to accommodate the requested size.
            if (storage_->capacity_ >= dataSize + size)
            {
                mmove = true;
                InternalAddAtEnd(size, dataSize, MoveMemoryTag());
            }
            // The storage does not have enough space to accommodate
            // the requested size.
            else // if (storage_->capacity_ < dataSize + size)
            {
                dirty = true;
                size_t newCapacity = dataSize + size;
                size_t newStart = 0;
                InternalAddAtEnd(size, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
        }
    }
    // The storage is linked to other buffers.
    // We cannot move the data within the storage in this case.
    else // if (storage_->refCount_ > 1)
    {
        // The post-trailer area is not by other buffers.
        if (storage_->dirtyEnd_ == start_ + dataSize)
        {
            // The post-trailer area has enough space.
            if (size <= postSize)
            {
                InternalAddAtEnd(size, dataSize, AdjustOffsetTag());
            }
            // The post-trailer area does not have enough space.
            else // if (size > postSize)
            {
                dirty = true;
                size_t newCapacity = dataSize + size;
                size_t newStart = 0;
                InternalAddAtEnd(size, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
        }
        // The post-tailer area is used by other buffers.
        else // if (storage_->dirtyEnd_ < start_ + dataSize)
        {
            dirty = true;
            // The post-tailer area has enough space.
            if (size <= postSize)
            {
                size_t newCapacity = storage_->capacity_;
                size_t newStart = start_;
                InternalAddAtEnd(size, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
            // The post-tailer area does not have enough space.
            else // if (numBytes_ > postSize)
            {
                size_t newCapacity = size + dataSize;
                size_t newStart = 0;
                InternalAddAtEnd(size, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
        }
    }
}

inline void ZcBuffer::AddAtEnd(const uint8_t* src, size_t size)
{
    BOOST_ASSERT_MSG(src, "Invalid pointer.");
    if (size)
    {
        AddAtEnd(size);
        std::memmove(storage_->bytes_ + (end_ - (zeroEnd_ - zeroStart_) - size),
                     src, size);
    }
}

inline void ZcBuffer::AddAtEnd(const ConstZcBuffer& src)
{
    size_t size = src.GetSize();
    if (size)
    {
        AddAtEnd(size);
        src.CopyTo(storage_->bytes_ + (end_ - (zeroEnd_ - zeroStart_) - size),
                   size);
    }
}

inline void ZcBuffer::InternalAddAtEnd(
    size_t size, size_t dataSize, AdjustOffsetTag) BOOST_NOEXCEPT
{
    end_ += size;
    storage_->dirtyEnd_ = start_ + dataSize + size;
}

inline void ZcBuffer::InternalAddAtEnd(
    size_t size, size_t newCapacity,
    size_t newStart, size_t dataSize, ReallocateTag)
{
    BufferStorage* newStorage = BufferStorage::Allocate(newCapacity);
    std::memcpy(newStorage->bytes_ + newStart,
                  storage_->bytes_ + start_,
                dataSize);
    if (storage_)
    {
        BufferStorage::Release(storage_);
    }
    storage_  = newStorage;
    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = newStart + dataSize + size;

    ptrdiff_t delta = newStart - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta + size;
}

inline void ZcBuffer::InternalAddAtEnd(
    size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT
{
    size_t newStart = storage_->capacity_ - (dataSize + size);
    std::memmove(storage_->bytes_ + newStart,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = storage_->capacity_;

    ptrdiff_t delta = newStart - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta + size;
}

inline void ZcBuffer::RemoveAtStart(size_t size) BOOST_NOEXCEPT
{
    size_t newStart = start_ + size;
    if (newStart <= zeroStart_)
    {
        start_ = newStart;
    }
    else if (newStart <= zeroEnd_)
    {
        size_t delta = newStart - zeroStart_;
        start_     = zeroStart_;
        zeroStart_ = start_;
        zeroEnd_  -= delta;
        end_      -= delta;
    }
    else if (newStart <= end_)
    {
        size_t delta = newStart - zeroEnd_;
        size_t gamma = zeroEnd_ - zeroStart_;
        start_     = zeroStart_ + delta;
        zeroStart_ = start_;
        zeroEnd_   = start_;
        end_      -= gamma;
    }
    else // if (newStart > end_)
    {
        Release();
    }
}

inline void ZcBuffer::RemoveAtEnd(size_t size) BOOST_NOEXCEPT
{
    if (size <= end_ - zeroEnd_)
    {
        end_ -= size;
    }
    else if (size <= end_ - zeroStart_)
    {
        size_t delta = size - (end_ - zeroEnd_);
        zeroEnd_ -= delta;
        end_      = zeroEnd_;
    }
    else if (size <= end_ - start_)
    {
        size_t delta = size - (end_ - zeroStart_);
        zeroStart_ -= delta;
        zeroEnd_    = zeroStart_;
        end_        = zeroStart_;
    }
    else // if (size > end_ - start_)
    {
        Release();
    }
}

inline ZcBuffer ZcBuffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(start <= GetSize(),
                     "Cannot create a fragment, since the start of "
                     "the fragment is beyond the end of the buffer.");
    BOOST_ASSERT_MSG(size <= GetSize() - start,
                     "Cannot create a fragment, since the end of "
                     "the fragment is beyond the end of the buffer.");
    ZcBuffer fragment(*this);
    fragment.RemoveAtStart(start);
    fragment.RemoveAtEnd(GetSize() - (start + size));
    return fragment;
}

inline ZcBuffer ZcBuffer::MakeRealBuffer(void) const
{
    return (zeroEnd_ == zeroStart_) ?
        ZcBuffer(*this) : InternalGetRealBuffer(ReallocateTag());
}

inline void ZcBuffer::Realize(void) const
{
    *const_cast<ZcBuffer*>(this) = MakeRealBuffer();
}

inline ZcBuffer ZcBuffer::InternalGetRealBuffer(ReallocateTag) const
{
    size_t header  = zeroStart_ - start_;
    size_t gamma   = zeroEnd_   - zeroStart_;
    size_t trailer = end_       - zeroEnd_;
    size_t newCapacity = end_ - start_;
    BufferStorage* newStorage = BufferStorage::Allocate(newCapacity);
    std::memcpy(newStorage->bytes_,
                  storage_->bytes_ + start_,
                header);
    std::memset(newStorage->bytes_ + header, 0, gamma);
    std::memcpy(newStorage->bytes_ + header + gamma,
                  storage_->bytes_ + zeroStart_,
                trailer);
    return ZcBuffer(newStorage, 0, header + gamma, header + gamma, newCapacity);
}

inline ZcBufferIterator ZcBuffer::begin(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = start_;
    return ZcBufferIterator(bytes, start_, zeroStart_, zeroEnd_, end_, cursor);
}

inline ZcBufferIterator ZcBuffer::end(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = end_;
    return ZcBufferIterator(bytes, start_, zeroStart_, zeroEnd_, end_, cursor);
}

inline ConstZcBufferIterator ZcBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = start_;
    return ConstZcBufferIterator(bytes, start_, zeroStart_, zeroEnd_, end_, cursor);
}

inline ConstZcBufferIterator ZcBuffer::cend(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = end_;
    return ConstZcBufferIterator(bytes, start_, zeroStart_, zeroEnd_, end_, cursor);
}

inline void ZcBuffer::swap(ZcBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(storage_,   rhs.storage_);
        boost::swap(start_,     rhs.start_);
        boost::swap(zeroStart_, rhs.zeroStart_);
        boost::swap(zeroEnd_,   rhs.zeroEnd_);
        boost::swap(end_,       rhs.end_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ZcBuffer& lhs, ZcBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // ZC_BUFFER_H__C2DEDFE9_A0FA_405F_AE24_66679E3B3608

