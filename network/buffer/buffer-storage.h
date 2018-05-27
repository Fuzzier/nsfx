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

#ifndef BUFFER_STORAGE_H__9898887C_61B1_4A9D_A5FB_A786F9FB19E1
#define BUFFER_STORAGE_H__9898887C_61B1_4A9D_A5FB_A786F9FB19E1


#include <nsfx/network/config.h>
#include <nsfx/network/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BufferStorage.
/**
 * @ingroup Network
 * @brief The storage of a buffer (POD).
 */
struct BufferStorage
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
     * @brief The start of the dirty area.
     *
     * The dirty area is union of used areas of all buffers that link to this
     * storage.
     */
    size_t dirtyStart_;

    /**
     * @brief The end of the dirty area.
     */
    size_t dirtyEnd_;

    /**
     * @brief The space for storing data (at least 1 byte).
     */
    uint8_t bytes_[1];


    ////////////////////////////////////////
    // Static methods.
    /**
     * @brief Create a buffer storage.
     *
     * @return If the capacity is \c 0, \c nullptr is returned.
     */
    static BufferStorage* Create(size_t capacity);

    static void AddRef(BufferStorage* storage) BOOST_NOEXCEPT;

    static void Release(BufferStorage* storage) BOOST_NOEXCEPT;

}; // struct BufferStorage


////////////////////////////////////////////////////////////////////////////////
inline BufferStorage* BufferStorage::Create(size_t capacity)
{
    BufferStorage* storage = nullptr;
    if (capacity > 0)
    {
        size_t storageSize = sizeof (BufferStorage) - 1 + capacity;
        uint8_t* bytes = new uint8_t[storageSize];
        storage = reinterpret_cast<BufferStorage*>(bytes);
        storage->capacity_   = capacity;
        storage->dirtyStart_ = 0;
        storage->dirtyEnd_   = 0;
        storage->refCount_   = 1;
    }
    return storage;
}

inline void BufferStorage::AddRef(BufferStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->refCount_ >= 0);
    ++storage->refCount_;
}

inline void BufferStorage::Release(BufferStorage* storage) BOOST_NOEXCEPT
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


#endif // BUFFER_STORAGE_H__9898887C_61B1_4A9D_A5FB_A786F9FB19E1

