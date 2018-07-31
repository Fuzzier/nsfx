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
#include <nsfx/network/buffer/zc-buffer-declare.h>
#include <nsfx/network/buffer/const-buffer-declare.h>
#include <nsfx/network/buffer/const-zc-buffer-declare.h>
#include <nsfx/network/buffer/const-tag-buffer-declare.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


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

inline ZcBuffer::ZcBuffer(const ConstBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    zeroStart_(rhs.GetSize()),
    zeroEnd_(rhs.GetSize()),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline ZcBuffer::ZcBuffer(const ConstZcBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    zeroStart_(rhs.GetSize()),
    zeroEnd_(rhs.GetSize()),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline ZcBuffer::ZcBuffer(const ConstTagBuffer& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize())),
    start_(0),
    zeroStart_(rhs.GetSize()),
    zeroEnd_(rhs.GetSize()),
    end_(rhs.GetSize())
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
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

