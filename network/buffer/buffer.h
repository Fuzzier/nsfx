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
#include <nsfx/network/buffer/buffer-declare.h>
#include <nsfx/network/buffer/const-buffer-declare.h>
#include <nsfx/network/buffer/const-tag-buffer-declare.h>
#include <nsfx/network/buffer/const-zc-buffer-declare.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Buffer.
inline Buffer::Buffer(void) BOOST_NOEXCEPT :
    storage_(nullptr),
    start_(0),
    end_(0)
{
}

inline Buffer::Buffer(size_t capacity) :
    storage_(BufferStorage::Allocate(capacity)),
    start_(capacity),
    end_(capacity)
{
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_;
    }
}

inline Buffer::Buffer(size_t reserved, size_t zeroSize) :
    storage_(BufferStorage::Allocate(reserved + zeroSize)),
    start_(reserved),
    end_(reserved + zeroSize)
{
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_;
        std::memset(storage_->bytes_ + start_, 0, zeroSize);
    }
}

inline Buffer::Buffer(size_t reserved, size_t zeroStart, size_t zeroSize)
{
    BOOST_ASSERT_MSG(zeroStart <= reserved,
                     "Cannot construct a Buffer, since the start of "
                     "the zero data is beyond the end of the buffer storage.");
    storage_ = BufferStorage::Allocate(reserved + zeroSize);
    start_   = zeroStart;
    end_     = zeroStart + zeroSize;
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_;
        std::memset(storage_->bytes_ + zeroStart, 0, zeroSize);
    }
}

inline Buffer::Buffer(BufferStorage* storage, size_t start, size_t end) BOOST_NOEXCEPT :
    storage_(storage),
    start_(start),
    end_(end)
{
    BOOST_ASSERT(start <= end);
    BOOST_ASSERT(!storage_ ? true : end <= storage_->capacity_);
    if (storage_)
    {
        storage_->dirtyStart_ = start_;
        storage_->dirtyEnd_   = end_;
    }
}

inline Buffer::Buffer(const ConstBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline Buffer::Buffer(const ConstZcBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline Buffer::Buffer(const ConstTagBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline Buffer::~Buffer(void) BOOST_NOEXCEPT
{
    Release();
}

inline Buffer::Buffer(const Buffer& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_),
    start_(rhs.start_),
    end_(rhs.end_)
{
    Acquire();
}

inline Buffer& Buffer::operator=(const Buffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        BufferStorage* tmp = storage_;
        storage_ = rhs.storage_;
        start_   = rhs.start_;
        end_     = rhs.end_;
        Acquire();
        if (tmp)
        {
            BufferStorage::Release(tmp);
        }
    }
    return *this;
}

inline Buffer::Buffer(Buffer&& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_),
    start_(rhs.start_),
    end_(rhs.end_)
{
    rhs.storage_ = nullptr;
    rhs.start_   = 0;
    rhs.end_     = 0;
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
        storage_ = nullptr;
        start_   = 0;
        end_     = 0;
        BufferStorage::Release(tmp);
    }
}

inline size_t Buffer::GetSize(void) const BOOST_NOEXCEPT
{
    return GetInternalSize();
}

inline size_t Buffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return end_ - start_;
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
    return GetEnd();
}

inline size_t Buffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return GetEnd();
}

inline size_t Buffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline const BufferStorage* Buffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return storage_;
}

inline size_t Buffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    size_t copied = 0;
    if (storage_ && dst)
    {
        copied = InternalCopyTo(dst, size, ContinuousTag());
    }
    return copied;
}

inline size_t
Buffer::InternalCopyTo(uint8_t* dst, size_t size, ContinuousTag) const BOOST_NOEXCEPT
{
    size_t dataSize = GetInternalSize();
    size_t copied = (dataSize <= size ? dataSize : size);
    std::memmove(dst, storage_->bytes_ + start_, copied);
    return copied;
}

inline size_t
Buffer::InternalCopyTo(uint8_t* dst, size_t size, SegmentedTag) const BOOST_NOEXCEPT
{
    InternalCopyTo(dst, size, ContinuousTag());
}

inline void Buffer::AddAtStart(size_t size)
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

inline void Buffer::AddAtStart(const uint8_t* src, size_t size)
{
    BOOST_ASSERT_MSG(src, "Invalid pointer.");
    if (size)
    {
        AddAtStart(size);
        std::memmove(storage_->bytes_ + start_, src, size);
    }
}

inline void Buffer::AddAtStart(const ConstBuffer& src)
{
    size_t size = src.GetSize();
    if (size)
    {
        AddAtStart(src.GetSize());
        src.CopyTo(storage_->bytes_ + start_, size);
    }
}

inline void Buffer::InternalAddAtStart(size_t size, AdjustOffsetTag) BOOST_NOEXCEPT
{
    start_ -= size;
    storage_->dirtyStart_ = start_;
}

inline void Buffer::InternalAddAtStart(
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
    start_ = newStart;
    end_   += delta;
}

inline void Buffer::InternalAddAtStart(
    size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT
{
    std::memmove(storage_->bytes_ + size,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = 0;
    storage_->dirtyEnd_   = size + dataSize;

    ptrdiff_t delta = size - start_;
    start_ = 0;
    end_   += delta;
}

inline void Buffer::AddAtEnd(size_t size)
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

inline void Buffer::AddAtEnd(const uint8_t* src, size_t size)
{
    BOOST_ASSERT_MSG(src, "Invalid pointer.");
    if (size)
    {
        AddAtEnd(size);
        std::memmove(storage_->bytes_ + (end_ - size),
                     src, size);
    }
}

inline void Buffer::AddAtEnd(const ConstBuffer& src)
{
    size_t size = src.GetSize();
    if (size)
    {
        AddAtEnd(size);
        src.CopyTo(storage_->bytes_ + (end_ - size),
                   size);
    }
}

inline void Buffer::InternalAddAtEnd(
    size_t size, size_t dataSize, AdjustOffsetTag) BOOST_NOEXCEPT
{
    end_ += size;
    storage_->dirtyEnd_ = start_ + dataSize + size;
}

inline void Buffer::InternalAddAtEnd(
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
    start_ = newStart;
    end_   += delta + size;
}

inline void Buffer::InternalAddAtEnd(
    size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT
{
    size_t newStart = storage_->capacity_ - (dataSize + size);
    std::memmove(storage_->bytes_ + newStart,
                 storage_->bytes_ + start_,
                 dataSize);

    storage_->dirtyStart_ = newStart;
    storage_->dirtyEnd_   = storage_->capacity_;

    ptrdiff_t delta = newStart - start_;
    start_ = newStart;
    end_   += delta + size;
}

inline void Buffer::RemoveAtStart(size_t size) BOOST_NOEXCEPT
{
    if (size <= end_ - start_)
    {
        start_ += size;
    }
    else // if (size >= end_ - start_)
    {
        Release();
    }
}

inline void Buffer::RemoveAtEnd(size_t size) BOOST_NOEXCEPT
{
    if (size <= end_ - start_)
    {
        end_ -= size;
    }
    else // if (size > end_ - start_)
    {
        Release();
    }
}

inline Buffer Buffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(start <= GetSize(),
                     "Cannot create a fragment, since the start of "
                     "the fragment is beyond the end of the buffer.");
    BOOST_ASSERT_MSG(size <= GetSize() - start,
                     "Cannot create a fragment, since the end of "
                     "the fragment is beyond the end of the buffer.");
    if (size)
    {
        BufferStorage::AddRef(storage_);
        return Buffer(storage_, start_ + start, start_ + start + size);
    }
    else
    {
        return Buffer();
    }
}

inline Buffer Buffer::MakeRealBuffer(void) const BOOST_NOEXCEPT
{
    return Buffer(*this);
}

inline void Buffer::Realize(void) const BOOST_NOEXCEPT
{
    // This is already a real buffer.
}

inline Buffer Buffer::InternalGetRealBuffer(ReallocateTag) const BOOST_NOEXCEPT
{
    return Buffer(*this);
}

inline BufferIterator Buffer::begin(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = start_;
    return BufferIterator(bytes, start_, end_, cursor);
}

inline BufferIterator Buffer::end(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = end_;
    return BufferIterator(bytes, start_, end_, cursor);
}

inline ConstBufferIterator Buffer::cbegin(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = start_;
    return ConstBufferIterator(bytes, start_, end_, cursor);
}

inline ConstBufferIterator Buffer::cend(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t cursor = end_;
    return ConstBufferIterator(bytes, start_, end_, cursor);
}

inline void Buffer::swap(Buffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(storage_, rhs.storage_);
        boost::swap(start_,   rhs.start_);
        boost::swap(end_,     rhs.end_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(Buffer& lhs, Buffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_H__6C3B4527_139F_4C12_BE2D_89B97CF1ADDD

