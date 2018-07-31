/**
 * @file
 *
 * @brief Tag buffer for Network Simulation Frameworks.
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

#ifndef TAG_BUFFER_H__E8B509D8_1719_41B2_B9E7_08733BB7473B
#define TAG_BUFFER_H__E8B509D8_1719_41B2_B9E7_08733BB7473B


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/tag-buffer-declare.h>
#include <nsfx/network/buffer/const-buffer-declare.h>
#include <nsfx/network/buffer/const-zc-buffer-declare.h>
#include <nsfx/network/buffer/const-tag-buffer-declare.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// TagBuffer.
inline TagBuffer::TagBuffer(void) BOOST_NOEXCEPT :
    storage_(nullptr)
{
}

inline TagBuffer::TagBuffer(size_t size) :
    storage_(TagBufferStorage::Allocate(size))
{
}

inline TagBuffer::TagBuffer(size_t size, bool zeroInit) :
    storage_(TagBufferStorage::Allocate(size))
{
    if (zeroInit && storage_)
    {
        std::memset(storage_->bytes_, 0, storage_->capacity_);
    }
}

inline TagBuffer::TagBuffer(const ConstBuffer& rhs) :
    storage_(TagBufferStorage::Allocate(rhs.GetSize()))
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline TagBuffer::TagBuffer(const ConstZcBuffer& rhs) :
    storage_(TagBufferStorage::Allocate(rhs.GetSize()))
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline TagBuffer::TagBuffer(const ConstTagBuffer& rhs) :
    storage_(TagBufferStorage::Allocate(rhs.GetSize()))
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline TagBuffer::~TagBuffer(void) BOOST_NOEXCEPT
{
    Release();
}

inline TagBuffer::TagBuffer(const TagBuffer& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_)
{
    Acquire();
}

inline TagBuffer& TagBuffer::operator=(const TagBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        TagBufferStorage* tmp = storage_;
        storage_ = rhs.storage_;
        Acquire();
        if (tmp)
        {
            TagBufferStorage::Release(tmp);
        }
    }
    return *this;
}

inline TagBuffer::TagBuffer(TagBuffer&& rhs) BOOST_NOEXCEPT :
        storage_(rhs.storage_)
{
    rhs.storage_ = nullptr;
}

inline TagBuffer& TagBuffer::operator=(TagBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        swap(rhs);
        rhs.Release();
    }
    return *this;
}

inline void TagBuffer::Acquire(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        TagBufferStorage::AddRef(storage_);
    }
}

inline void TagBuffer::Release(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        TagBufferStorage* tmp = storage_;
        storage_ = nullptr;
        TagBufferStorage::Release(tmp);
    }
}

inline size_t TagBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return storage_ ? storage_->capacity_ : 0;
}

inline const TagBufferStorage* TagBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return storage_;
}

inline size_t TagBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    size_t copied = 0;
    if (storage_ && dst)
    {
        size_t dataSize = GetSize();
        copied = (dataSize <= size ? dataSize : size);
        std::memmove(dst, storage_->bytes_, copied);
    }
    return copied;
}

inline TagBufferIterator TagBuffer::begin(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = 0;
    return TagBufferIterator(bytes, 0, end, cursor);
}

inline TagBufferIterator TagBuffer::end(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = end;
    return TagBufferIterator(bytes, 0, end, cursor);
}

inline ConstTagBufferIterator TagBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = 0;
    return ConstTagBufferIterator(bytes, 0, end, cursor);
}

inline ConstTagBufferIterator TagBuffer::cend(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = end;
    return ConstTagBufferIterator(bytes, 0, end, cursor);
}

inline void TagBuffer::swap(TagBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(storage_, rhs.storage_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(TagBuffer& lhs, TagBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_BUFFER_H__E8B509D8_1719_41B2_B9E7_08733BB7473B

