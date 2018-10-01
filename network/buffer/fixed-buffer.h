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

#ifndef FIXED_BUFFER_H__B0ECE3BE_2E92_48AF_AA5B_F570C78E63FC
#define FIXED_BUFFER_H__B0ECE3BE_2E92_48AF_AA5B_F570C78E63FC


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/basic-buffer.h>
#include <nsfx/network/buffer/storage/basic-buffer-storage.h>
#include <nsfx/network/buffer/iterator/buffer-iterator.h>
#include <nsfx/network/buffer/iterator/const-buffer-iterator.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// FixedBuffer.
/**
 * @ingroup Network
 * @brief A fixed-size buffer that does not support resizing.
 */
template<>
class BasicBuffer</*readOnly*/false, /*copyOnResize*/false, /*zeroArea*/false>
{
public:
    typedef BasicBuffer<false, false, false>              FixedBuffer;
    typedef BasicBufferStorage</*trackDirtyArea=*/false>  BufferStorage;
    typedef BufferIterator      iterator;
    typedef ConstBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    BasicBuffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     */
    explicit BasicBuffer(size_t size);

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     * @param[in] zero Initialize the buffer to zeros.
     */
    BasicBuffer(size_t size, bool zeroInit);

    // Conversions.
public:
    /**
     * @brief Deep copy from a read-only buffer.
     */
    template<bool readOnly, bool copyOnResize, bool zeroArea>
    explicit BasicBuffer(const BasicBuffer<readOnly, copyOnResize, zeroArea>& rhs);

public:
    ~BasicBuffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    BasicBuffer(const FixedBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    FixedBuffer& operator=(const FixedBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    BasicBuffer(FixedBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    FixedBuffer& operator=(FixedBuffer&& rhs) BOOST_NOEXCEPT;

private:
    void Acquire(void) BOOST_NOEXCEPT;
    void Release(void) BOOST_NOEXCEPT;

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
    BufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    BufferIterator end(void) BOOST_NOEXCEPT;

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
    void swap(FixedBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The storage.
     */
    BufferStorage* storage_;

};


////////////////////////////////////////////////////////////////////////////////
// Typedef.
typedef BasicBuffer</*readOnly*/false, /*copyOnResize*/false, /*zeroArea*/false>
        FixedBuffer;


////////////////////////////////////////////////////////////////////////////////
// FixedBuffer.
inline FixedBuffer::BasicBuffer(void) BOOST_NOEXCEPT :
    storage_(nullptr)
{
}

inline FixedBuffer::BasicBuffer(size_t size) :
    storage_(BufferStorage::Allocate(size))
{
}

inline FixedBuffer::BasicBuffer(size_t size, bool zeroInit) :
    storage_(BufferStorage::Allocate(size))
{
    if (zeroInit && storage_)
    {
        std::memset(storage_->bytes_, 0, storage_->capacity_);
    }
}

template<bool readOnly, bool copyOnResize, bool zeroArea>
inline FixedBuffer::BasicBuffer(const BasicBuffer<readOnly, copyOnResize, zeroArea>& rhs) :
    storage_(BufferStorage::Allocate(rhs.GetSize()))
{
    if (storage_)
    {
        rhs.CopyTo(storage_->bytes_, storage_->capacity_);
    }
}

inline FixedBuffer::~BasicBuffer(void) BOOST_NOEXCEPT
{
    Release();
}

inline FixedBuffer::BasicBuffer(const FixedBuffer& rhs) BOOST_NOEXCEPT :
    storage_(rhs.storage_)
{
    Acquire();
}

inline FixedBuffer& FixedBuffer::operator=(const FixedBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        BufferStorage* tmp = storage_;
        storage_ = rhs.storage_;
        Acquire();
        if (tmp)
        {
            BufferStorage::Release(tmp);
        }
    }
    return *this;
}

inline FixedBuffer::BasicBuffer(FixedBuffer&& rhs) BOOST_NOEXCEPT :
        storage_(rhs.storage_)
{
    rhs.storage_ = nullptr;
}

inline FixedBuffer& FixedBuffer::operator=(FixedBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        swap(rhs);
        rhs.Release();
    }
    return *this;
}

inline void FixedBuffer::Acquire(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        BufferStorage::AddRef(storage_);
    }
}

inline void FixedBuffer::Release(void) BOOST_NOEXCEPT
{
    if (storage_)
    {
        BufferStorage* tmp = storage_;
        storage_ = nullptr;
        BufferStorage::Release(tmp);
    }
}

inline size_t FixedBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return storage_ ? storage_->capacity_ : 0;
}

inline const FixedBuffer::BufferStorage*
FixedBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return storage_;
}

inline size_t FixedBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
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

inline BufferIterator FixedBuffer::begin(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = 0;
    return BufferIterator(bytes, 0, end, cursor);
}

inline BufferIterator FixedBuffer::end(void) BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = end;
    return BufferIterator(bytes, 0, end, cursor);
}

inline ConstBufferIterator FixedBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = 0;
    return ConstBufferIterator(bytes, 0, end, cursor);
}

inline ConstBufferIterator FixedBuffer::cend(void) const BOOST_NOEXCEPT
{
    uint8_t* bytes = storage_ ? storage_->bytes_ : nullptr;
    size_t end = storage_ ? storage_->capacity_ : 0;
    size_t cursor = end;
    return ConstBufferIterator(bytes, 0, end, cursor);
}

inline void FixedBuffer::swap(FixedBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(storage_, rhs.storage_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(FixedBuffer& lhs, FixedBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // FIXED_BUFFER_H__B0ECE3BE_2E92_48AF_AA5B_F570C78E63FC

