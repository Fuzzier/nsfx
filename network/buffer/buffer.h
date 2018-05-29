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

#ifndef BUFFER_H__6C3B4527_139F_4C12_BE2D_89B97CF1ADDD
#define BUFFER_H__6C3B4527_139F_4C12_BE2D_89B97CF1ADDD


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-storage.h>
#include <nsfx/network/buffer/buffer-iterator.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Buffer.
/**
 * @ingroup Network
 * @brief An automatically resized and copy-on-write buffer.
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
 *   The copy-on-write operations are \c Buffer::AddAtStart() and
 *   \c Buffer::AddAtEnd().
 */
class Buffer
{
    // Xstructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    Buffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The capacity of the buffer storage.
     *
     * The zero-compressed data area is empty in this buffer.
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data toward the head of the storage.
     */
    explicit Buffer(size_t capacity);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity  The capacity of the buffer storage.
     * @param[in] zeroSize  The size of the zero-compressed data area.
     *
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data at the head of the storage.
     */
    Buffer(size_t capacity, size_t zeroSize);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity   The capacity of the buffer storage.
     * @param[in] zeroStart  The offset of the start of the zero-compressed data area.
     *                       <code>zeroStart <= capacity</code>.
     * @param[in] zeroSize   The size of the zero-compressed data area.
     */
    Buffer(size_t capacity, size_t zeroStart, size_t zeroSize);

private:
    /**
     * @brief Create a buffer.
     *
     * @param[in] storage  The reference count is taken by the buffer.
     *
     * @see \c InternalMakeRealCopy().
     * @internal
     */
    Buffer(BufferStorage* storage, size_t start, size_t zeroStart,
           size_t zeroEnd, size_t end) BOOST_NOEXCEPT;

public:
    ~Buffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    Buffer(const Buffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    Buffer& operator=(const Buffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    Buffer(Buffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    Buffer& operator=(Buffer&& rhs) BOOST_NOEXCEPT;

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
     * @brief Copy data to buffer.
     * @return The number of bytes copied.
     */
    size_t CopyTo(uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    // Add/remove.
private:
    struct AdjustOffsetTag {};
    struct ReallocateTag {};
    struct MoveMemoryTag {};

public:
    /**
     * @brief Expand the data area toward the start of the buffer storage.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     */
    void AddAtStart(size_t numBytes);

private:
    void InternalAddAtStart(size_t numBytes, AdjustOffsetTag);
    void InternalAddAtStart(size_t numBytes, size_t newCapacity,
                            size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtStart(size_t numBytes, size_t dataSize, MoveMemoryTag);

public:
    /**
     * @brief Expand the data area toward the end of the buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     */
    void AddAtEnd(size_t numBytes);

private:
    void InternalAddAtEnd(size_t numBytes, size_t dataSize, AdjustOffsetTag);
    void InternalAddAtEnd(size_t numBytes, size_t newCapacity,
                          size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtEnd(size_t numBytes, size_t dataSize, MoveMemoryTag);

public:
    /**
     * @brief Reduce the data area toward the end of the buffer.
     *
     * @param[in] numBytes If the number of bytes is too large, the data area
     *                     becomes empty.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     */
    void RemoveAtStart(size_t numBytes) BOOST_NOEXCEPT;

    /**
     * @brief Reduce the data area and leave more post-data area.
     *
     * @param[in] numBytes If the number of bytes is too large, the data area
     *                     becomes empty.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     */
    void RemoveAtEnd(size_t numBytes) BOOST_NOEXCEPT;

    // Fragmentation.
public:
    /**
     * @brief Make a shallow copy of a portion of the buffer.
     *
     * @param[in] start Offset from the start of the buffer.
     * @param[in] size  The size of the fragment.
     */
    Buffer GetFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    Buffer GetRealBuffer(void) const;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const;

private:
    Buffer InternalGetRealBuffer(ReallocateTag) const;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    BufferIterator begin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    BufferIterator end(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(Buffer& rhs) BOOST_NOEXCEPT;

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


////////////////////////////////////////////////////////////////////////////////
// Buffer.
inline Buffer::Buffer(void) BOOST_NOEXCEPT :
    storage_(nullptr),
    start_(0),
    zeroStart_(0),
    zeroEnd_(0),
    end_(0)
{
}

inline Buffer::Buffer(size_t capacity) :
    storage_(BufferStorage::Create(capacity)),
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

inline Buffer::Buffer(size_t capacity, size_t zeroSize) :
    storage_(BufferStorage::Create(capacity)),
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

inline Buffer::Buffer(size_t capacity, size_t zeroStart, size_t zeroSize) :
    storage_(BufferStorage::Create(capacity)),
    start_(zeroStart),
    zeroStart_(zeroStart),
    zeroEnd_(zeroStart + zeroSize),
    end_(zeroStart + zeroSize)
{
    BOOST_ASSERT_MSG(zeroStart <= capacity,
                     "The offset of the start of zero-compressed data area "
                     "must be within the buffer storage.");
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline Buffer::Buffer(BufferStorage* storage, size_t start, size_t zeroStart,
                      size_t zeroEnd, size_t end) BOOST_NOEXCEPT :
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
                 (end - start) - (zeroEnd - zeroStart) <= storage_->capacity_);
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_ - (zeroEnd_ - zeroStart_);
    }
}

inline Buffer::~Buffer(void) BOOST_NOEXCEPT
{
    Release();
}

inline Buffer::Buffer(const Buffer& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_),
    start_(rhs.start_),
    zeroStart_(rhs.zeroStart_),
    zeroEnd_(rhs.zeroEnd_),
    end_(rhs.end_)
{
    Acquire();
}

inline Buffer& Buffer::operator=(const Buffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        if (storage_)
        {
            BufferStorage::Release(storage_);
        }
        storage_   = rhs.storage_;
        start_     = rhs.start_;
        zeroStart_ = rhs.zeroStart_;
        zeroEnd_   = rhs.zeroEnd_;
        end_       = rhs.end_;
        Acquire();
    }
    return *this;
}

inline Buffer::Buffer(Buffer&& rhs) BOOST_NOEXCEPT :
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

inline Buffer& Buffer::operator=(Buffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        swap(rhs);
        rhs.Release();
    }
    return *this;
}

inline void Buffer::Acquire(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        BufferStorage::AddRef(storage_);
    }
}

inline void Buffer::Release(void) BOOST_NOEXCEPT
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

inline size_t Buffer::GetSize(void) const BOOST_NOEXCEPT
{
    return end_ - start_;
}

inline size_t Buffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return (zeroStart_ - start_) + (end_ - zeroEnd_);
}

inline size_t Buffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return storage_ ? storage_->capacity_ : 0;
}

inline size_t Buffer::GetStart(void) const BOOST_NOEXCEPT
{
    return start_;
}

inline size_t Buffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return zeroStart_;
}

inline size_t Buffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return zeroEnd_;
}

inline size_t Buffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline const BufferStorage* Buffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return storage_;
}

size_t Buffer::CopyTo(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    size_t copyied = 0;
    if (storage_ && bytes)
    {
        do
        {
            size_t headerSize = zeroStart_ - start_;
            if (size <= headerSize)
            {
                std::memmove(bytes_, storage_->bytes_ + start_, size);
                copied += size;
                break;
            }
            std::memmove(bytes_, storage_->bytes_ + start_, headerSize);
            size   -= headerSize;
            copied += headerSize;

            size_t zeroSize = zeroEnd_ - zeroStart_;
            if (size <= zeroSize)
            {
                std::memset(bytes + copied, 0, size);
                copied += size;
                break;
            }
            std::memset(bytes + copied, 0, zeroSize);
            size   -= zeroSize;
            copied += zeroSize;

            size_t trailerSize = end_ - zeroEnd_;
            if (size <= trailerSize)
            {
                std::memmove(bytes + copied,
                             storage_->bytes_ + zeroStart_,
                             size);
                copied += size;
                break;
            }
            std::memmove(bytes + copied,
                         storage_->bytes_ + zeroStart_,
                         trailerSize);
            copied += trailerSize;
        }
        while (false);
    }
    return copied;
}

inline void Buffer::AddAtStart(size_t numBytes)
{
    if (!numBytes)
    {
        return;
    }

    size_t dataSize = GetInternalSize();
    // The storage is not yet allocated.
    if (!storage_)
    {
        size_t newCapacity = numBytes;
        size_t newStart = 0;
        InternalAddAtStart(numBytes, newCapacity, newStart,
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
        if (numBytes <= start_)
        {
            InternalAddAtStart(numBytes, AdjustOffsetTag());
        }
        // The pre-header area does not have enough space.
        else // if (numBytes > start_)
        {
            // The storage has enough space to accommodate the requested size.
            if (storage_->capacity_ >= dataSize + numBytes)
            {
                mmove = true;
                InternalAddAtStart(numBytes, dataSize, MoveMemoryTag());
            }
            // The storage does not have enough space to accommodate
            // the requested size.
            else // if (storage_->capacity_ < dataSize + numBytes)
            {
                dirty = true;
                size_t newCapacity = numBytes + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(numBytes, newCapacity, newStart,
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
            if (numBytes <= start_)
            {
                InternalAddAtStart(numBytes, AdjustOffsetTag());
            }
            // The pre-header area does not have enough space.
            else // if (numBytes > start_)
            {
                dirty = true;
                size_t newCapacity = numBytes + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(numBytes, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
        }
        // The pre-header area is used by other buffers.
        else // if (storage_->dirtyStart_ < start_)
        {
            dirty = true;
            // The pre-header area has enough space.
            if (numBytes <= start_)
            {
                size_t newCapacity = storage_->capacity_;
                size_t newStart = start_ - numBytes;
                InternalAddAtStart(numBytes, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
            // The pre-header area does not have enough space.
            else // if (numBytes_ > start_)
            {
                size_t newCapacity = numBytes + dataSize;
                size_t newStart = 0;
                InternalAddAtStart(numBytes, newCapacity, newStart,
                                   dataSize, ReallocateTag());
            }
        }
    }
}

inline void Buffer::InternalAddAtStart(size_t numBytes, AdjustOffsetTag)
{
    start_ -= numBytes;
    storage_->dirtyStart_ = start_;
}

inline void Buffer::InternalAddAtStart(
    size_t numBytes, size_t newCapacity,
    size_t newStart, size_t dataSize, ReallocateTag)
{
    BufferStorage* newStorage = BufferStorage::Create(newCapacity);
    std::memcpy(newStorage->bytes_ + newStart + numBytes,
                  storage_->bytes_ + start_,
                dataSize);
    if (storage_)
    {
        BufferStorage::Release(storage_);
    }
    storage_  = newStorage;
    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = newStart + numBytes + dataSize;

    ptrdiff_t delta = newStart + numBytes - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta;
}

inline void Buffer::InternalAddAtStart(
    size_t numBytes, size_t dataSize, MoveMemoryTag)
{
    std::memmove(storage_->bytes_ + numBytes,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = 0;
    storage_->dirtyEnd_   = numBytes + dataSize;

    ptrdiff_t delta = numBytes - start_;
    start_      = 0;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta;
}

inline void Buffer::AddAtEnd(size_t numBytes)
{
    if (!numBytes)
    {
        return;
    }

    size_t dataSize = GetInternalSize();
    // The storage is not yet allocated.
    if (!storage_)
    {
        size_t newCapacity = numBytes;
        size_t newStart = 0;
        InternalAddAtEnd(numBytes, newCapacity, newStart,
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
        if (numBytes <= postSize)
        {
            InternalAddAtEnd(numBytes, dataSize, AdjustOffsetTag());
        }
        // The post-trailer area does not have enough space.
        else // if (numBytes > postSize)
        {
            // The storage has enough space to accommodate the requested size.
            if (storage_->capacity_ >= dataSize + numBytes)
            {
                mmove = true;
                InternalAddAtEnd(numBytes, dataSize, MoveMemoryTag());
            }
            // The storage does not have enough space to accommodate
            // the requested size.
            else // if (storage_->capacity_ < dataSize + numBytes)
            {
                dirty = true;
                size_t newCapacity = dataSize + numBytes;
                size_t newStart = 0;
                InternalAddAtEnd(numBytes, newCapacity, newStart,
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
            if (numBytes <= postSize)
            {
                InternalAddAtEnd(numBytes, dataSize, AdjustOffsetTag());
            }
            // The post-trailer area does not have enough space.
            else // if (numBytes > postSize)
            {
                dirty = true;
                size_t newCapacity = dataSize + numBytes;
                size_t newStart = 0;
                InternalAddAtEnd(numBytes, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
        }
        // The post-tailer area is used by other buffers.
        else // if (storage_->dirtyEnd_ < start_ + dataSize)
        {
            dirty = true;
            // The post-tailer area has enough space.
            if (numBytes <= postSize)
            {
                size_t newCapacity = storage_->capacity_;
                size_t newStart = start_;
                InternalAddAtEnd(numBytes, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
            // The post-tailer area does not have enough space.
            else // if (numBytes_ > postSize)
            {
                size_t newCapacity = numBytes + dataSize;
                size_t newStart = 0;
                InternalAddAtEnd(numBytes, newCapacity, newStart,
                                 dataSize, ReallocateTag());
            }
        }
    }
}

inline void Buffer::InternalAddAtEnd(
    size_t numBytes, size_t dataSize, AdjustOffsetTag)
{
    end_ += numBytes;
    storage_->dirtyEnd_ = start_ + dataSize + numBytes;
}

inline void Buffer::InternalAddAtEnd(
    size_t numBytes, size_t newCapacity,
    size_t newStart, size_t dataSize, ReallocateTag)
{
    BufferStorage* newStorage = BufferStorage::Create(newCapacity);
    std::memcpy(newStorage->bytes_ + newStart,
                  storage_->bytes_ + start_,
                dataSize);
    if (storage_)
    {
        BufferStorage::Release(storage_);
    }
    storage_  = newStorage;
    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = newStart + dataSize + numBytes;

    ptrdiff_t delta = newStart - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta + numBytes;
}

inline void Buffer::InternalAddAtEnd(
    size_t numBytes, size_t dataSize, MoveMemoryTag)
{
    size_t newStart = storage_->capacity_ - (dataSize + numBytes);
    std::memmove(storage_->bytes_ + newStart,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = storage_->capacity_;

    ptrdiff_t delta = newStart - start_;
    start_      = newStart;
    zeroStart_ += delta;
    zeroEnd_   += delta;
    end_       += delta + numBytes;
}

inline void Buffer::RemoveAtStart(size_t numBytes) BOOST_NOEXCEPT
{
    size_t newStart = start_ + numBytes;
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
        size_t gamma = zeroEnd_ - zeroStart_;
        end_      -= gamma;
        start_     = end_;
        zeroStart_ = end_;
        zeroEnd_   = end_;
    }
}

inline void Buffer::RemoveAtEnd(size_t numBytes) BOOST_NOEXCEPT
{
    if (numBytes <= end_ - zeroEnd_)
    {
        end_ -= numBytes;
    }
    else if (numBytes <= end_ - zeroStart_)
    {
        size_t delta = numBytes - (end_ - zeroEnd_);
        zeroEnd_ -= delta;
        end_      = zeroEnd_;
    }
    else if (numBytes <= end_ - start_)
    {
        size_t delta = numBytes - (end_ - zeroStart_);
        zeroStart_ -= delta;
        zeroEnd_    = zeroStart_;
        end_        = zeroStart_;
    }
    else // if (numBytes > end_ - start_)
    {
        zeroStart_ = start_;
        zeroEnd_   = start_;
        end_       = start_;
    }
}

inline Buffer Buffer::GetFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    Buffer result(*this);
    result.RemoveAtStart(start);
    result.RemoveAtEnd(GetSize() - (start + size));
    return result;
}

inline Buffer Buffer::GetRealBuffer(void) const
{
    return (zeroEnd_ == zeroStart_) ?
        Buffer(*this) : InternalGetRealBuffer(ReallocateTag());
}

inline void Buffer::Realize(void) const
{
    *const_cast<Buffer*>(this) = GetRealBuffer();
}

inline Buffer Buffer::InternalGetRealBuffer(ReallocateTag) const
{
    size_t header  = zeroStart_ - start_;
    size_t gamma   = zeroEnd_   - zeroStart_;
    size_t trailer = end_       - zeroEnd_;
    size_t newCapacity = end_ - start_;
    BufferStorage* newStorage = BufferStorage::Create(newCapacity);
    std::memcpy(newStorage->bytes_,
                  storage_->bytes_ + start_,
                header);
    std::memset(newStorage->bytes_ + header, 0, gamma);
    std::memcpy(newStorage->bytes_ + header + gamma,
                  storage_->bytes_ + zeroStart_,
                trailer);
    return Buffer(newStorage, 0, header + gamma, header + gamma, newCapacity);
}

inline void Buffer::swap(Buffer& rhs) BOOST_NOEXCEPT
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

inline BufferIterator Buffer::begin(void) const BOOST_NOEXCEPT
{
    size_t cursor = start_;
    return BufferIterator(storage_, start_, zeroStart_, zeroEnd_, end_, cursor);
}

inline BufferIterator Buffer::end(void) const BOOST_NOEXCEPT
{
    size_t cursor = end_;
    return BufferIterator(storage_, start_, zeroStart_, zeroEnd_, end_, cursor);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(Buffer& lhs, Buffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_H__6C3B4527_139F_4C12_BE2D_89B97CF1ADDD

