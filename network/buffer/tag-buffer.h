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
#include <nsfx/network/buffer/tag-buffer-storage.h>
#include <nsfx/network/buffer/buffer-iterator.h>
#include <boost/core/swap.hpp>
#include <cstring> // memcpy, memmove, memset


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
typedef BufferIterator       TagBufferIterator;
typedef ConstBufferIterator  ConstTagBufferIterator;


////////////////////////////////////////////////////////////////////////////////
// TagBuffer.
/**
 * @ingroup Network
 * @brief A reference-counted fixed-size buffer.
 */
class TagBuffer
{
public:
    typedef TagBufferIterator      iterator;
    typedef ConstTagBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    TagBuffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     */
    explicit TagBuffer(size_t size);

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     * @param[in] zero Initialize the buffer to zeros.
     */
    TagBuffer(size_t size, bool zeroInit);

    /**
     * @brief Create a buffer.
     *
     * @param[in] storage The tag storage.
     */
    explicit TagBuffer(TagStorage* storage) BOOST_NOEXCEPT;

public:
    ~TagBuffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    TagBuffer(const TagBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    TagBuffer& operator=(const TagBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    TagBuffer(TagBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    TagBuffer& operator=(TagBuffer&& rhs) BOOST_NOEXCEPT;

private:
    void TagBuffer::Acquire(void) BOOST_NOEXCEPT;
    void TagBuffer::Release(void) BOOST_NOEXCEPT;

    // Methods.
public:
    /**
     * @brief Get the size of the represented data.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    const TagBufferStorage* GetStorage(void) const BOOST_NOEXCEPT;

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
    TagBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    TagBufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstTagBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstTagBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(TagBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The storage.
     */
    TagBufferStorage* storage_;

};


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

inline Packet::TagBuffer(TagStorage* storage) BOOST_NOEXCEPT :
    storage_(storage)
{
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

