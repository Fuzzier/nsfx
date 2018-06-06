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

#ifndef TAG_BUFFER_STORAGE_H__CF3A7480_501E_46A5_8637_86204902716F
#define TAG_BUFFER_STORAGE_H__CF3A7480_501E_46A5_8637_86204902716F


#include <nsfx/network/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// TagBufferStorage.
/**
 * @ingroup Network
 * @brief The storage of a tag buffer (POD).
 */
struct TagBufferStorage
{
    /**
     * @brief The capacity of \c bytes_ in bytes.
     */
    size_t capacity_;

    /**
     * @brief The reference count.
     *
     * A reference count is held by each buffer and buffer iterator.
     *
     * A reference count is provided so the storage can be shared without a
     * deep copy.
     * e.g., to extract the a chunk from the buffer, a new buffer is created
     * with a different data position, and shares the same storage with the
     * original buffer to avoid a deep copy.
     */
    refcount_t refCount_;

    /**
     * @brief The space for storing data (at least 1 byte).
     */
    uint8_t bytes_[1];


    ////////////////////////////////////////
    // Static methods.
    /**
     * @brief Create a tag buffer storage.
     *
     * @return If the capacity is \c 0, \c nullptr is returned.
     */
    static TagBufferStorage* Create(size_t capacity);

    static void AddRef(TagBufferStorage* storage) BOOST_NOEXCEPT;

    static void Release(TagBufferStorage* storage) BOOST_NOEXCEPT;

};


////////////////////////////////////////////////////////////////////////////////
inline TagBufferStorage* TagBufferStorage::Create(size_t capacity)
{
    TagBufferStorage* storage = nullptr;
    if (capacity > 0)
    {
        size_t storageSize = sizeof (TagBufferStorage) - 1 + capacity;
        uint8_t* bytes = new uint8_t[storageSize];
        storage = reinterpret_cast<TagBufferStorage*>(bytes);
        storage->capacity_ = capacity;
        storage->refCount_ = 1;
    }
    return storage;
}

inline void TagBufferStorage::AddRef(TagBufferStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->refCount_ >= 0);
    ++storage->refCount_;
}

inline void TagBufferStorage::Release(TagBufferStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->refCount_ > 0);
    if (--storage->refCount_ == 0)
    {
        uint8_t* bytes = reinterpret_cast<uint8_t*>(storage);
        delete[] bytes;
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_BUFFER_STORAGE_H__CF3A7480_501E_46A5_8637_86204902716F

