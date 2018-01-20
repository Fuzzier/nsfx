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
#include <nsfx/utility/endian.h>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/type_identity.hpp>
#include <boost/core/swap.hpp>
#include <cstring>
#include <memory>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BufferStorage./*{{{*/
/**
 * @ingroup Network
 * @brief The storage of a buffer (POD).
 */
struct BufferStorage
{
    /**
     * @brief The size of \c bytes_ in bytes.
     */
    size_t size_;

    /**
     * @brief The reference count.
     *
     * A reference count is held by each buffer and buffer iterator.
     *
     * A reference count is provided so the storage can be shared without a
     * deep copy.<br/>
     * e.g., to extract the a chunk from the buffer, a new buffer is created
     * with a different data position, and shares the same storage with the
     * original buffer to avoid a deep copy.<br/>
     */
    ptrdiff_t refCount_;

    /**
     * @brief The number of locks on the data area.
     *
     * A data lock is held by each buffer iterator.
     *
     * When the data area is locked, the data area cannot be moved or reduced.<br/>
     * e.g., each buffer iterator holds a lock, while an iterator is accessing
     * the data area, the data area cannot be moved or reduced as the iterator
     * would become <i>semantically</i> invalidated and cause unexpected errors
     * that are hard to detect.<br/>
     * Runtime check is performed by \c Buffer objects to prevent such errors,
     * which encourages <b>scoped</b> usage of buffer iterators.<br/>
     * i.e., users shall always use buffer iterators in a local scope, where
     * users shall not invoke methods that <i>potentially</i> move or reduce
     * the data area.
     *
     * @see \c Buffer::AddAtStart(),
     *      \c Buffer::AddAtEnd(),
     *      \c Buffer::RemoveAtStart(),
     *      \c Buffer::RemoveAtEnd().
     */
    ptrdiff_t dataLockCount_;

    /**
     * @brief The space for storing data (at least 1 byte).
     */
    uint8_t bytes_[1];

    // Static methods.
    static BufferStorage* Create(size_t size)
    {
        if (size == 0)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("Cannot allocation 0-sized buffer."));
        }
        size_t storageSize = sizeof (BufferStorage) - 1 + size;
        uint8_t* bytes = new uint8_t[storageSize];
        BufferStorage* storage = reinterpret_cast<BufferStorage*>(bytes);
        storage->size_ = size;
        storage->dataLockCount_ = 0;
        storage->refCount_ = 0;
        return storage;
    }

    static void AddRef(BufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ >= 0);
        ++storage->refCount_;
    }

    /**
     * @brief Release the storage.
     *
     * ### Strong exception safety.
     * Exceptions are thrown after the storage is released to prevent memory
     * leak.
     */
    static void Release(BufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->refCount_ > 0);
        if (--storage->refCount_ == 0)
        {
#if !defined(NDEBUG)
            ptrdiff_t refCount = storage->refCount_;
            ptrdiff_t dataLockCount = storage->dataLockCount_;
            bool unbalanced = (storage->refCount_ < storage->dataLockCount_);
#endif // !defined(NDEBUG)
            uint8_t* bytes = reinterpret_cast<uint8_t*>(storage);
            delete[] bytes;
            BOOST_ASSERT_MSG(!unbalanced,
                        "A buffer iterator holds both a reference count and a "
                        "data lock count, and it must release the data lock "
                        "count before releasing the reference count. Thus, "
                        "the reference count cannot be less than the data lock "
                        "count.");
        }
    }

    static void AddDataLock(BufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->dataLockCount_ >= 0);
        ++storage->dataLockCount_;
    }

    static void ReleaseDataLock(BufferStorage* storage) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(storage);
        BOOST_ASSERT(storage->dataLockCount_ > 0);
        --storage->dataLockCount_;
    }

    static bool IsDataLocked(const BufferStorage* storage) BOOST_NOEXCEPT
    {
        return !!storage->dataLockCount_;
    }

}; // struct BufferStorage /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// BufferIterator./*{{{*/
/**
 * @ingroup Network
 * @brief The iterator for accessing the data.
 *
 * The iterator is limited to move alone the data area in the buffer.<br/>
 * An attempt to move outside of the data area would result in throwing
 * \c OutOfBounds.<br/>
 *
 * A buffer iterator is not associated with a buffer, but associated with the
 * underlying storage that is shared among buffers and iterators.<br/>
 *
 * ### Supported methods:<br/>
 * * copyable
 * * operator ++, --
 * * operator +, +=, -, -=
 * * operator >, >=, ==, !=, <=, <
 * * Write<T>(T data)
 * * WriteL<T>(T data)
 * * WriteB<T>(T data)
 * * T Read<T>()
 * * T ReadL<T>()
 * * T ReadB<T>()
 *
 * ### Strong exception safety.
 * If a method throws an exception, the iterator retains its original state.
 */
class BufferIterator
{
    // Function template dispatchers./*{{{*/
private:
    template<size_t byte_size, bool reverseEndian>
    struct Dispatcher {};

    template<class T, endian::Order order>
    struct DispatcherMaker
    {
        typedef Dispatcher<sizeof (T), order != endian::native>  Type;
    };

    /*}}}*/

public:
    typedef BufferIterator  ThisType;

    // Xtructors./*{{{*/
public:
    BufferIterator(void) BOOST_NOEXCEPT :
        storage_(nullptr),
        start_(0),
        end_(0),
        cursor_(0)
    {
    }

    BufferIterator(BufferStorage* storage,
                   size_t start,
                   size_t end,
                   size_t cursor) BOOST_NOEXCEPT :
        storage_(storage),
        start_(start),
        end_(end),
        cursor_(cursor)
    {
        Acquire();
    }

public:
    ~BufferIterator(void) BOOST_NOEXCEPT
    {
        Release();
    }

    /*}}}*/

    // Copyable./*{{{*/
    BufferIterator(const ThisType& rhs) BOOST_NOEXCEPT :
        storage_(rhs.storage_),
        start_(rhs.start_),
        end_(rhs.end_),
        cursor_(rhs.cursor_)
    {
        Acquire();
    }

    ThisType& operator=(const ThisType& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            Release();
            storage_ = rhs.storage_;
            start_ = rhs.start_;
            end_ = rhs.end_;
            cursor_ = rhs.cursor_;
            Acquire();
        }
        return *this;
    }
    /*}}}*/

    // Methods./*{{{*/
private:
    void Acquire(void) const BOOST_NOEXCEPT
    {
        if (storage_)
        {
            BufferStorage::AddRef(storage_);
            BufferStorage::AddDataLock(storage_);
        }
    }

    void Release(void) BOOST_NOEXCEPT
    {
        if (storage_)
        {
            BufferStorage* tmp = storage_;
            storage_ = nullptr;
            start_ = 0;
            end_ = 0;
            cursor_ = 0;
            BufferStorage::ReleaseDataLock(tmp);
            BufferStorage::Release(tmp);
        }
    }

#if !defined(NDEBUG)
public:
    size_t GetStart(void) const BOOST_NOEXCEPT
    {
        return start_;
    }

    size_t GetEnd(void) const BOOST_NOEXCEPT
    {
        return end_;
    }

    size_t GetCursor(void) const BOOST_NOEXCEPT
    {
        return cursor_;
    }

    BufferStorage* GetStorage(void) const BOOST_NOEXCEPT
    {
        return storage_;
    }
#endif // !defined(NDEBUG)

    // Move cursor./*{{{*/
public:
    /**
     * @brief Move the iterator toward the end of the data area.
     *
     * ### Strong exception safty.
     *
     * @throw OutOfBounds
     */
    void MoveForward(size_t numBytes)
    {
        ForwardCheck(numBytes);
        cursor_ += numBytes;
    }

    /**
     * @brief Move the iterator toward the end of the data area.
     *
     * ### Strong exception safty.
     *
     * @throw OutOfBounds
     */
    void MoveBackward(size_t numBytes)
    {
        BackwardCheck(numBytes);
        cursor_ -= numBytes;
    }

    /*}}}*/

    // Write data./*{{{*/
public:
    /**
     * @brief Write data in native endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void Write(typename boost::type_identity<T>::type  data)
    {
        WriteInOrder<T, endian::native>(data);
    }

    /**
     * @brief Write data in little endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void WriteL(typename boost::type_identity<T>::type  data)
    {
        WriteInOrder<T, endian::little>(data);
    }

    /**
     * @brief Write data in big endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void WriteB(typename boost::type_identity<T>::type  data)
    {
        WriteInOrder<T, endian::big>(data);
    }

private:
    /**
     * @brief Write data in specified endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T, endian::Order order>
    void WriteInOrder(T data)
    {
        static_assert(boost::is_integral<T>::value, "Invalid data type.");
        WritableCheck(sizeof (T));
        typedef typename DispatcherMaker<T, order>::Type  D;
        InternalWrite(
            static_cast<typename boost::make_unsigned<T>::type>(data), D());
    }

    void InternalWrite(uint8_t value, Dispatcher<1, false> /* not reverse */) BOOST_NOEXCEPT
    {
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = value;
    }

    void InternalWrite(uint16_t value, Dispatcher<2, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[2];
            uint16_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[0];
        data[cursor_++] = b[1];
    }

    void InternalWrite(uint32_t value, Dispatcher<4, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[4];
            uint32_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[0];
        data[cursor_++] = b[1];
        data[cursor_++] = b[2];
        data[cursor_++] = b[3];
    }

    void InternalWrite(uint64_t value, Dispatcher<8, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[8];
            uint64_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[0];
        data[cursor_++] = b[1];
        data[cursor_++] = b[2];
        data[cursor_++] = b[3];
        data[cursor_++] = b[4];
        data[cursor_++] = b[5];
        data[cursor_++] = b[6];
        data[cursor_++] = b[7];
    }

    void InternalWrite(uint8_t value, Dispatcher<1, true> /* reverse */) BOOST_NOEXCEPT
    {
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = value;
    }

    void InternalWrite(uint16_t value, Dispatcher<2, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[2];
            uint16_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[1];
        data[cursor_++] = b[0];
    }

    void InternalWrite(uint32_t value, Dispatcher<4, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[4];
            uint32_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[3];
        data[cursor_++] = b[2];
        data[cursor_++] = b[1];
        data[cursor_++] = b[0];
    }

    void InternalWrite(uint64_t value, Dispatcher<8, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[8];
            uint64_t v;
        };
        v = value;
        uint8_t* data = storage_->bytes_;
        data[cursor_++] = b[7];
        data[cursor_++] = b[6];
        data[cursor_++] = b[5];
        data[cursor_++] = b[4];
        data[cursor_++] = b[3];
        data[cursor_++] = b[2];
        data[cursor_++] = b[1];
        data[cursor_++] = b[0];
    }

    /*}}}*/

    // Read data./*{{{*/
public:
    /**
     * @brief Read data in native endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T Read(void)
    {
        return ReadInOrder<T, endian::native>();
    }

    /**
     * @brief Read data in little endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadL(void)
    {
        return ReadInOrder<T, endian::little>();
    }

    /**
     * @brief Read data in big endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadB(void)
    {
        return ReadInOrder<T, endian::big>();
    }

private:
    /**
     * @brief Read data in the specified endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T, endian::Order order>
    T ReadInOrder(void)
    {
        static_assert(boost::is_integral<T>::value, "Invalid data type.");
        ReadableCheck(sizeof (T));
        typedef typename DispatcherMaker<T, order>::Type  D;
        return static_cast<T>(InternalRead(D()));
    }

    uint8_t InternalRead(Dispatcher<1, false> /* not reverse */) BOOST_NOEXCEPT
    {
        const uint8_t* data = storage_->bytes_;
        return data[cursor_++];
    }

    uint16_t InternalRead(Dispatcher<2, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[2];
            uint16_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[0] = data[cursor_++];
        b[1] = data[cursor_++];
        return v;
    }

    uint32_t InternalRead(Dispatcher<4, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[4];
            uint32_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[0] = data[cursor_++];
        b[1] = data[cursor_++];
        b[2] = data[cursor_++];
        b[3] = data[cursor_++];
        return v;
    }

    uint64_t InternalRead(Dispatcher<8, false> /* not reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[8];
            uint64_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[0] = data[cursor_++];
        b[1] = data[cursor_++];
        b[2] = data[cursor_++];
        b[3] = data[cursor_++];
        b[4] = data[cursor_++];
        b[5] = data[cursor_++];
        b[6] = data[cursor_++];
        b[7] = data[cursor_++];
        return v;
    }

    uint8_t InternalRead(Dispatcher<1, true> /* reverse */) BOOST_NOEXCEPT
    {
        const uint8_t* data = storage_->bytes_;
        return data[cursor_++];
    }

    uint16_t InternalRead(Dispatcher<2, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[2];
            uint16_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[1] = data[cursor_++];
        b[0] = data[cursor_++];
        return v;
    }

    uint32_t InternalRead(Dispatcher<4, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[4];
            uint32_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[3] = data[cursor_++];
        b[2] = data[cursor_++];
        b[1] = data[cursor_++];
        b[0] = data[cursor_++];
        return v;
    }

    uint64_t InternalRead(Dispatcher<8, true> /* reverse */) BOOST_NOEXCEPT
    {
        union
        {
            uint8_t  b[8];
            uint64_t v;
        };
        const uint8_t* data = storage_->bytes_;
        b[7] = data[cursor_++];
        b[6] = data[cursor_++];
        b[5] = data[cursor_++];
        b[4] = data[cursor_++];
        b[3] = data[cursor_++];
        b[2] = data[cursor_++];
        b[1] = data[cursor_++];
        b[0] = data[cursor_++];
        return v;
    }

    /*}}}*/

    /*}}}*/

    // Boundary check./*{{{*/
private:
    bool CanMoveForward(size_t numBytes) const BOOST_NOEXCEPT
    {
        return cursor_ + numBytes <= end_;
    }

    bool CanMoveBackward(size_t numBytes) const BOOST_NOEXCEPT
    {
        return cursor_ >= start_ + numBytes;
    }

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void ForwardCheck(size_t numBytes) const
    {
        if (!CanMoveForward(numBytes))
        {
            BOOST_THROW_EXCEPTION(
                OutOfBounds() << ErrorMessage(
                    "The iterator cannot move beyond the end of buffer."));
        }
    }

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void BackwardCheck(size_t numBytes) const
    {
        if (!CanMoveBackward(numBytes))
        {
            BOOST_THROW_EXCEPTION(
                OutOfBounds() << ErrorMessage(
                    "The iterator cannot move beyond the start of buffer."));
        }
    }

    /**
     * @brief Can we write the number of bytes at the cursor.
     */
    void WritableCheck(size_t numBytes) const
    {
        if (!CanMoveForward(numBytes))
        {
            BOOST_THROW_EXCEPTION(
                OutOfBounds() << ErrorMessage(
                    "The iterator cannot write beyond the end of buffer."));
        }
    }

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void ReadableCheck(size_t numBytes) const
    {
        if (!CanMoveForward(numBytes))
        {
            BOOST_THROW_EXCEPTION(
                OutOfBounds() << ErrorMessage(
                    "The iterator cannot read beyond the end of buffer."));
        }
    }

    /*}}}*/

    // Operators/*{{{*/.
public:
    ThisType& operator++(void)
    {
        MoveForward(1);
        return *this;
    }

    ThisType operator++(int)
    {
        ThisType it = *this;
        MoveForward(1);
        return it;
    }

    ThisType& operator--(void)
    {
        MoveBackward(1);
        return *this;
    }

    ThisType operator--(int)
    {
        ThisType it = *this;
        MoveBackward(1);
        return it;
    }

    ThisType& operator+=(size_t numBytes)
    {
        MoveForward(numBytes);
        return *this;
    }

    ThisType& operator-=(size_t numBytes)
    {
        MoveBackward(numBytes);
        return *this;
    }

    friend bool operator> (const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;
    friend bool operator>=(const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;
    friend bool operator==(const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;
    friend bool operator< (const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;
    friend bool operator<=(const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;

    friend ThisType operator+(const ThisType& lhs, size_t numBytes);
    friend ThisType operator-(const ThisType& lhs, size_t numBytes);
    friend ptrdiff_t operator-(const ThisType& lhs, const ThisType& rhs) BOOST_NOEXCEPT;

    /*}}}*/

    // Properties./*{{{*/
private:
    BufferStorage* storage_;

    /**
     * @brief The start position in the data area (inclusive).
     */
    size_t start_;

    /**
     * @brief The end position in the data area (exclusive).
     */
    size_t end_;

    /**
     * @brief The current position in the data area.
     */
    size_t cursor_;

    /*}}}*/

}; // class Iterator


// BufferIterator operators./*{{{*/
inline bool operator> (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ > rhs.cursor_;
}

inline bool operator>=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ >= rhs.cursor_;
}

inline bool operator==(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ == rhs.cursor_;
}

inline bool operator!=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ != rhs.cursor_;
}

inline bool operator< (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ < rhs.cursor_;
}

inline bool operator<=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ <= rhs.cursor_;
}

inline BufferIterator
operator+(const BufferIterator& lhs, size_t numBytes)
{
    BufferIterator it = lhs;
    it.MoveForward(numBytes);
    return it;
}

inline BufferIterator
operator-(const BufferIterator& lhs, size_t numBytes)
{
    BufferIterator it = lhs;
    it.MoveBackward(numBytes);
    return it;
}

inline ptrdiff_t
operator-(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.storage_ == rhs.storage_,
                     "Cannot compare unrelated iterators.");
    return lhs.cursor_ - rhs.cursor_;
}

/*}}}*/

// BufferIterator /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// BufferCopyPolicy /*{{{*/
/**
 * @ingroup Network
 * @brief Buffer copy policy.
 *
 * A buffer holds a memory space, and may have a sequence of meaningful bytes
 * within the space.<br/>
 * The area before the meaningful bytes is called the pre-data area.<br/>
 * The area after the meaningful bytes is call the post-data area.<br/>
 * The area occupied by the meaningful bytes is call the data area.<br/>
 * When cloning from a source buffer, the destination buffer may have some
 * memory space already.<br/>
 * Copy policies specify whether the destination buffer should be reallocated.
 */
struct BufferCopyPolicy {};

/**
 * @ingroup Network
 * @brief Buffer copy policy.
 *
 * This policy ensures that the destination buffer must have enough space to
 * accommodate the data in the source buffer, even if the destination has
 * a smaller space than the source buffer.<br/>
 * If possible, the destination buffer reserves the same post-data area as the
 * source buffer, leaving a smaller pre-data size than the source buffer.<br/>
 * Otherwise, the pre-data area becomes empty, and the post-data area is also
 * compressed.<br/>
 *
 * This is the policy currently in use.
 */
struct BufferCopyPolicyAccommodateData : BufferCopyPolicy {};

/**
 * @ingroup Network
 * @brief Buffer copy policy.
 *
 * This policy ensures that the destination buffer must have no less space than
 * the source buffer, even if the destination buffer is large enough to
 * accommodate the data in the source buffer.<br/>
 * The destination buffer reserves the same post-data size is the source buffer.
 */
struct BufferCopyPolicyAccommodateSize : BufferCopyPolicy {};

// BufferCopyPolicy /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Buffer./*{{{*/
/**
 * @ingroup Network
 * @brief A resizeable buffer.
 *
 * The buffer holds a storage that provides a memory space for the buffer.<br/>
 * The storage is logically divided into three areas.<br/>
 * The data area is located in middle of the storage, and the remaining space
 * is naturally divied in to the pre-data area and the post-data area.<br/>
 *
 * ### Basic exception safety for 'Copy' methods.
 * The source buffer retains its original state.<br/>
 * The destination buffer is put in an empty state.
 *
 * ### Strong exception safety for all other methods.
 * The buffer retains its original state.
 */
class Buffer /*{{{*/
{
    // Xstructors./*{{{*/
public:
    /**
     * @brief Create an empty buffer.
     */
    Buffer(void) BOOST_NOEXCEPT:
        storage_(nullptr),
        start_(0),
        end_(0)
    {
    }

    /**
     * @brief Create a buffer.
     *
     * @param[in] size  The size of the storage.
     *
     * The data area is positioned at the end of the storage, optimizing for
     * adding data at the head of the storage.
     */
    Buffer(size_t size) :
        storage_(BufferStorage::Create(size)),
        start_(size),
        end_(size)
    {
        Acquire();
    }

    /**
     * @brief Create a buffer.
     *
     * @param[in] size  The size of the storage.
     * @param[in] start The start position of the data area.
     *
     * @throw InvalidArgument Thrown if \c start is beyond \c size.
     */
    Buffer(size_t size, size_t start) :
        storage_(BufferStorage::Create(size)),
        start_(0),
        end_(0)
    {
        Acquire();
        if (start > size)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("The data area is outside of the storage space."));
        }
        start_ = start;
        end_ = start;
    }

private:
    /**
     * @brief Construct a buffer.
     *
     * This constructor is used internally.
     *
     * @see \c GetChunk().
     */
    Buffer(BufferStorage* storage, size_t start, size_t end) BOOST_NOEXCEPT :
        storage_(storage),
        start_(start),
        end_(end)
    {
        Acquire();
    }

public:
    ~Buffer(void) BOOST_NOEXCEPT
    {
        Release();
    }

    /*}}}*/

    // Copyable/*{{{*/.
public:
    /**
     * @brief Make a shallow copy.
     */
    Buffer(const Buffer& rhs) BOOST_NOEXCEPT :
        storage_(rhs.storage_),
        start_(rhs.start_),
        end_(rhs.end_)
    {
        Acquire();
    }

    /**
     * @brief Release storage, and make a shallow copy.
     */
    Buffer& operator=(const Buffer& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            Release();
            storage_ = rhs.storage_;
            start_ = rhs.start_;
            end_ = rhs.end_;
            Acquire();
        }
        return *this;
    }

    /*}}}*/

    // Movable./*{{{*/
public:
    /**
     * @brief Move a buffer.
     */
    Buffer(Buffer&& rhs) BOOST_NOEXCEPT :
        storage_(rhs.storage_),
        start_(rhs.start_),
        end_(rhs.end_)
    {
        rhs.storage_ = nullptr;
        rhs.start_ = 0;
        rhs.end_ = 0;
    }

    /**
     * @brief Release self, and move a buffer.
     */
    Buffer& operator=(Buffer&& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            swap(rhs);
            rhs.Release();
        }
        return *this;
    }

    /*}}}*/

    // Private./*{{{*/
private:
    void Acquire(void) BOOST_NOEXCEPT
    {
        if (storage_)
        {
            BufferStorage::AddRef(storage_);
        }
    }

    void Release(void) BOOST_NOEXCEPT
    {
        if (storage_)
        {
            BufferStorage* tmp = storage_;
            storage_ = nullptr;
            start_ = 0;
            end_ = 0;
            BufferStorage::Release(tmp);
        }
    }

    /*}}}*/

    // Methods./*{{{*/
public:
    /**
     * @brief Get the size of storage.
     */
    size_t GetCapacity(void) const BOOST_NOEXCEPT
    {
        size_t result = 0;
        if (storage_)
        {
            result = storage_->size_;
        }
        return result;
    }

    /**
     * @brief Get the size of data area.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT
    {
        return end_ - start_;
    }

#if !defined(NDEBUG)
    /**
     * @brief Get the start position of data area.
     *
     * Only avaiable if \c NDEBUG is not defined.
     */
    size_t GetDataStart(void) const BOOST_NOEXCEPT
    {
        return start_;
    }

    /**
     * @brief Get the start position of data area.
     *
     * Only avaiable if \c NDEBUG is not defined.
     */
    size_t GetDataEnd(void) const BOOST_NOEXCEPT
    {
        return end_;
    }

    /**
     * @brief Get the start position of data area.
     *
     * Only avaiable if \c NDEBUG is not defined.
     */
    BufferStorage* GetStorage(void) const BOOST_NOEXCEPT
    {
        return storage_;
    }
#endif // !defined(NDEBUG)

    /**
     * @brief Expand the data area toward the start of the buffer.
     *
     * Compress the pre-data area to accommodate the requested size.<br/>
     * If the pre-data area is not enough, the data is moved toward the end of
     * the storage, and the post-data area is compressed.<br/>
     * If the post-data area is still not enough, reallocate the buffer.<br/>
     *
     * ### Strong exception safety.
     * The buffer is kept intact if an exception is thrown.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void AddAtStart(size_t numBytes)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        if (start_ >= numBytes)
        {
            start_ -= numBytes;
        }
        else
        {
            size_t dataSize = GetSize();
            if (storage_ && storage_->size_ >= numBytes + dataSize)
            {
                std::memmove(storage_->bytes_ + numBytes,
                             storage_->bytes_ + start_,
                             dataSize);
            }
            else
            {
                size_t newSize = numBytes + dataSize;
                BufferStorage* newStorage = BufferStorage::Create(newSize);
                std::memcpy(newStorage->bytes_ + numBytes,
                              storage_->bytes_ + start_,
                            dataSize);
                Release();
                storage_ = newStorage;
                Acquire();
            }
            start_ = 0;
            end_ = numBytes + dataSize;
        }
    }

    /**
     * @brief Expand the data area toward the end of the buffer.
     *
     * Compress the post-data area to accommodate the requested size.<br/>
     * If the post-data area is not enough, the data is moved toward the start
     * of the storage, and the pre-data area is compressed.<br/>
     * If the pre-data area is still not enough, reallocate the buffer.<br/>
     *
     * ### Strong exception safety.
     * The buffer is kept intact if an exception is thrown.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void AddAtEnd(size_t numBytes)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        if (storage_ && storage_->size_ >= end_ + numBytes)
        {
            end_ += numBytes;
        }
        else
        {
            size_t dataSize = GetSize();
            if (storage_ && storage_->size_ >= dataSize + numBytes)
            {
                size_t delta = numBytes - (storage_->size_ - end_);
                std::memmove(storage_->bytes_ + start_ - delta,
                             storage_->bytes_ + start_,
                             dataSize);
                start_ -= delta;
                end_ = start_ + dataSize + numBytes;
            }
            else
            {
                size_t newSize = dataSize + numBytes;
                BufferStorage* newStorage = BufferStorage::Create(newSize);
                std::memcpy(newStorage->bytes_,
                              storage_->bytes_ + start_,
                            dataSize);
                Release();
                storage_ = newStorage;
                Acquire();
                start_ = 0;
                end_ = dataSize + numBytes;
            }
        }
    }

    /**
     * @brief Reduce the data area and leave more pre-data area.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @param[in] numBytes If the number of bytes is too large, the data area
     *                     becomes empty.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void RemoveAtStart(size_t numBytes)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        size_t newStart = start_ + numBytes;
        if (newStart <= end_)
        {
            // Reduce pre-data area.
            start_ = newStart;
        }
        else
        {
            // Empty the data area, and leave the post-data area unchanged.
            start_ = end_;
        }
    }

    /**
     * @brief Reduce the data area and leave more post-data area.
     *
     * @param[in] numBytes If the number of bytes is too large, the data area
     *                     becomes empty.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void RemoveAtEnd(size_t numBytes)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        if (end_ - start_ >= numBytes)
        {
            // Reduce the post-data area.
            end_ -= numBytes;
        }
        else
        {
            // Empty the data area, and leave the pre-data area unchanged.
            end_ = start_;
        }
    }

    /**
     * @brief Deep copy the data area from a buffer.
     *
     * The copy policy is \c BufferCopyPolicyAccommodateData.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyFrom(const Buffer& src)
    {
        CopyFrom(src, BufferCopyPolicyAccommodateData());
    }

    /**
     * @brief Deep copy data area from a buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyFrom(const Buffer& src, BufferCopyPolicyAccommodateData)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        if (this != &src)
        {
            if (!src.storage_)
            {
                Release();
            }
            else
            {
                // If existing storage doesn't have enough space to accommodate the
                // data in the source buffer, reallocate a storage as large as the
                // source buffer.
                if (!storage_ || (storage_->size_ < src.end_ - src.start_))
                {
                    Release();
                    // Allocate enough space.
                    // If exception is thrown, the buffer is in a safe state, since
                    // it's been released already.
                    storage_ = BufferStorage::Create(src.storage_->size_);
                    Acquire();
                }
                // If existing storage has a smaller space.
                if (storage_->size_ < src.storage_->size_)
                {
                    // Reserve the same post-data area as the source buffer
                    // if possible.
                    if (storage_->size_ >= src.storage_->size_ - src.start_)
                    {
                        start_ = src.start_ - (src.storage_->size_ - storage_->size_);
                        end_   = src.end_   - (src.storage_->size_ - storage_->size_);
                    }
                    else
                    {
                        start_ = 0;
                        end_   = src.end_ - src.start_;
                    }
                }
                // If existing storage has a space no smaller than the source buffer.
                else // if (storage_->size_ >= src.storage_->size_)
                {
                    // Reserve the same post-data area as the source buffer.
                    start_ = src.start_ + (storage_->size_ - src.storage_->size_);
                    end_   = src.end_   + (storage_->size_ - src.storage_->size_);
                }
                // Copy data.
                std::memcpy(    storage_->bytes_ +     start_,
                                src.storage_->bytes_ + src.start_,
                                src.end_ - src.start_);
            }
        }
    }

    /**
     * @brief Deep copy data area from a buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyFrom(const Buffer& src, BufferCopyPolicyAccommodateSize)
    {
        if (storage_ && BufferStorage::IsDataLocked(storage_))
        {
            BOOST_THROW_EXCEPTION(
                Unexpected() <<
                ErrorMessage("There are dangling buffer iterators while "
                             "trying to resize the data area."));
        }
        if (this != &src)
        {
            if (!src.storage_)
            {
                Release();
            }
            else
            {
                // If existing storage doesn't have a space as large as the source
                // buffer, reallocate a storage as large as the source buffer.
                if (!storage_ || (storage_->size_ < src.storage_->size_))
                {
                    Release();
                    // Allocate enough space.
                    // If exception is thrown, the buffer is in a safe state, since
                    // it's been reset already.
                    storage_ = BufferStorage::Create(src.storage_->size_);
                    Acquire();
                }
                // Reserve the same post-data area as the source buffer.
                start_ = src.start_ + (storage_->size_ - src.storage_->size_);
                end_   = src.end_   + (storage_->size_ - src.storage_->size_);
                // Copy data.
                std::memcpy(    storage_->bytes_ +     start_,
                                src.storage_->bytes_ + src.start_,
                                src.end_ - src.start_);
            }
        }
    }

    /**
     * @brief Make a deep copy of the buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage of the destination buffer.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyTo(Buffer& dest) const
    {
        dest.CopyFrom(*this);
    }

    /**
     * @brief Make a deep copy of the buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage of the destination buffer.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyTo(Buffer& dest, BufferCopyPolicyAccommodateData) const
    {
        dest.CopyFrom(*this, BufferCopyPolicyAccommodateData());
    }

    /**
     * @brief Make a deep copy of the buffer.
     *
     * @pre There are no dangling buffer iterators holding the underlying
     *      storage of the destination buffer.
     *
     * @throw Unexpected  There are dangling buffer iterators.
     */
    void CopyTo(Buffer& dest, BufferCopyPolicyAccommodateSize) const
    {
        dest.CopyFrom(*this, BufferCopyPolicyAccommodateSize());
    }

    /**
     * @brief Make a deap copy (clone) of the buffer.
     *
     */
    Buffer Copy(void) const
    {
        Buffer result;
        CopyTo(result);
        return result;
    }

    /**
     * @brief Make a shallow copy of a portion of data area.
     *
     * @param[in] start Relative to the start of <b>data area</b>.
     * @param[in] size  The size of the chunk.
     *
     * @throw OutOfBounds Thrown if the chunk exceeds the data area.
     */
    Buffer GetChunk(size_t start, size_t size) const
    {
        if (start_ + start + size > end_)
        {
            BOOST_THROW_EXCEPTION(
                OutOfBounds() <<
                ErrorMessage("Cannot get a chunk that exceeds the data area."));
        }
        return Buffer(storage_, start_ + start, start_ + start + size);
    }

    /*}}}*/

    // Iterator./*{{{*/
public:
    /**
     * @brief Get a buffer iterator at the first byte of the data area.
     */
    BufferIterator begin(void) const BOOST_NOEXCEPT
    {
        size_t cursor = start_;
        return BufferIterator(storage_, start_, end_, cursor);
    }

    /**
     * @brief Get a buffer iterator that is one byte after the last byte of the data area.
     */
    BufferIterator end(void) const BOOST_NOEXCEPT
    {
        size_t cursor = end_;
        return BufferIterator(storage_, start_, end_, cursor);
    }

    /*}}}*/

    // Swap/*{{{*/
public:
    void swap(Buffer& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            boost::swap(storage_, rhs.storage_);
            boost::swap(start_, rhs.start_);
            boost::swap(end_, rhs.end_);
        }
    }

    /*}}}*/

    // Properties./*{{{*/
private:
    /**
     * @brief The storage.
     */
    BufferStorage* storage_;

    /**
     * @brief Start position of the data space (inclusive).
     */
    size_t start_;

    /**
     * @brief End position of the data space (exclusive).
     */
    size_t end_;

    /*}}}*/

}; // class Buffer /*}}}*/


inline void swap(Buffer& lhs, Buffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

// Buffer /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_H__6C3B4527_139F_4C12_BE2D_89B97CF1ADDD

