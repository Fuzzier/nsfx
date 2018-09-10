/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef BASIC_BUFFER_STORAGE_H__CA395E02_E1AA_412F_BC51_26DABB16A07A
#define BASIC_BUFFER_STORAGE_H__CA395E02_E1AA_412F_BC51_26DABB16A07A


#include <nsfx/network/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BasicBufferStorage.
/**
 * @ingroup Network
 * @brief The storage of a buffer.
 *
 * This type is POD.
 *
 * @tparam trackDirtyArea The storage maintains the range of dirty area to
 *                        better support copy-on-resize.
 */
template<bool trackDirtyArea>
struct BasicBufferStorage;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The shared storage that supports copy-on-resize buffers.
 *
 * This is a specialization of <code>BasicBufferStorage<></code>.
 *
 * The storage provides a reference counter to support shared ownership.
 *
 * This storage maintains the range of dirty area to better support
 * copy-on-resize.
 * The dirty area is used to track the union of used data areas of all buffers
 * that share the storage.
 */
template<>
struct BasicBufferStorage</*trackDirtyArea=*/true>
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
     * @brief Allocate a buffer storage.
     *
     * @return If the capacity is \c 0, \c nullptr is returned.
     */
    static BasicBufferStorage* Allocate(size_t capacity)
    {
        BasicBufferStorage* storage = nullptr;
        if (capacity > 0)
        {
            size_t storageSize = sizeof (BasicBufferStorage) - 1 + capacity;
            uint8_t* bytes = new uint8_t[storageSize];
            storage = reinterpret_cast<BasicBufferStorage*>(bytes);
            storage->capacity_   = capacity;
            storage->dirtyStart_ = 0;
            storage->dirtyEnd_   = 0;
            storage->refCount_   = 1;
        }
        return storage;
    }

    static void AddRef(BasicBufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ >= 0);
        ++storage->refCount_;
    }

    static void Release(BasicBufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ > 0);
        if (--storage->refCount_ == 0)
        {
            uint8_t* bytes = reinterpret_cast<uint8_t*>(storage);
            delete[] bytes;
        }
    }

};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A shared storage that supports fixed-size buffers.
 *
 * This is a specialization of <code>BasicBufferStorage<></code>.
 *
 * The storage provides a reference counter to support shared ownership.
 */
template<>
struct BasicBufferStorage</*trackDirtyArea=*/false>
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
     * @brief Allocate a buffer storage.
     *
     * @return If the capacity is \c 0, \c nullptr is returned.
     */
    static BasicBufferStorage* Allocate(size_t capacity)
    {
        BasicBufferStorage* storage = nullptr;
        if (capacity > 0)
        {
            size_t storageSize = sizeof (BasicBufferStorage) - 1 + capacity;
            uint8_t* bytes = new uint8_t[storageSize];
            storage = reinterpret_cast<BasicBufferStorage*>(bytes);
            storage->capacity_   = capacity;
            storage->refCount_   = 1;
        }
        return storage;
    }

    static void AddRef(BasicBufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ >= 0);
        ++storage->refCount_;
    }

    static void Release(BasicBufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ > 0);
        if (--storage->refCount_ == 0)
        {
            uint8_t* bytes = reinterpret_cast<uint8_t*>(storage);
            delete[] bytes;
        }
    }

};


NSFX_CLOSE_NAMESPACE


#endif // BASIC_BUFFER_STORAGE_H__CA395E02_E1AA_412F_BC51_26DABB16A07A

