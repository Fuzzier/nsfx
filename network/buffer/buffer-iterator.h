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

#ifndef BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0
#define BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-storage.h>
#include <nsfx/utility/endian.h>
#include <boost/core/swap.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/type_identity.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BufferIterator.
/**
 * @ingroup Network
 * @brief The iterator for accessing the buffer data.
 *
 * A buffer iterator is not associated with a buffer, but associated with the
 * underlying storage that is shared among buffers and iterators.
 *
 * ### Supported methods:
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
 */
class BufferIterator
{
private:
    // Endian tag.
    struct ReverseEndianTag {};
    struct KeepEndianTag {};

    template<bool reverseEndian>
    struct MakeBoolEndianTag
    {
        typedef ReverseEndianTag  Type;
    };

    template<>
    struct MakeBoolEndianTag<false>
    {
        typedef KeepEndianTag  Type;
    };

    template<endian::Order order>
    struct MakeEndianTag
    {
        typedef typename MakeBoolEndianTag<order != endian::native>::Type  Type;
    };

    // Area tag.
    struct InSolidAreaTag {};

    // Read tag.
    template<size_t numBytes, class areaTag>
    struct ReadTag {};

    // Xtructors.
public:
    BufferIterator(uint8_t* bytes, size_t start,
                   size_t end, size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    BufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT;
    BufferIterator& operator=(const BufferIterator& rhs) BOOST_NOEXCEPT;

public:
    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;
    size_t GetCursor(void) const BOOST_NOEXCEPT;

    // Move cursor.
public:
    /**
     * @brief Move the iterator toward the end of the data area.
     */
    void MoveForward(size_t numBytes) BOOST_NOEXCEPT;

    /**
     * @brief Move the iterator toward the end of the data area.
     */
    void MoveBackward(size_t numBytes) BOOST_NOEXCEPT;

    // Write data.
public:
    /**
     * @brief Write data in native endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void Write(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT;

    /**
     * @brief Write data in little endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void WriteL(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT;

    /**
     * @brief Write data in big endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    void WriteB(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT;

private:
    /**
     * @brief Write data in specified endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T, endian::Order order>
    void WriteInOrder(T data) BOOST_NOEXCEPT;

    void InternalWrite(uint8_t  value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint16_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint32_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint64_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;

    void InternalWrite(uint8_t  value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint16_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint32_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint64_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;

    // Read data.
public:
    /**
     * @brief Read data in native endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T Read(void) BOOST_NOEXCEPT;

    /**
     * @brief Read data in little endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadL(void) BOOST_NOEXCEPT;

    /**
     * @brief Read data in big endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadB(void) BOOST_NOEXCEPT;

private:
    /**
     * @brief Read data in the specified endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T, endian::Order order>
    T ReadInOrder(void) BOOST_NOEXCEPT;

    uint8_t  InternalRead(size_t offset, ReadTag<1, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(size_t offset, ReadTag<2, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(size_t offset, ReadTag<4, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(size_t offset, ReadTag<8, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;

    uint8_t  InternalRead(size_t offset, ReadTag<1, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(size_t offset, ReadTag<2, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(size_t offset, ReadTag<4, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(size_t offset, ReadTag<8, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;

    // Boundary check.
private:
    bool CanMoveForward(size_t numBytes) const BOOST_NOEXCEPT;

    bool CanMoveBackward(size_t numBytes) const BOOST_NOEXCEPT;

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void ForwardCheck(size_t numBytes) const BOOST_NOEXCEPT;

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void BackwardCheck(size_t numBytes) const BOOST_NOEXCEPT;

    /**
     * @brief Can we write the number of bytes at the cursor.
     */
    void WritableCheck(size_t numBytes) const BOOST_NOEXCEPT;

    /**
     * @brief Can we read the number of bytes at the cursor.
     */
    void ReadableCheck(size_t numBytes) const BOOST_NOEXCEPT;

    // Operators.
public:
    BufferIterator& operator++(void) BOOST_NOEXCEPT;
    BufferIterator  operator++(int) BOOST_NOEXCEPT;
    BufferIterator& operator--(void) BOOST_NOEXCEPT;
    BufferIterator  operator--(int) BOOST_NOEXCEPT;
    BufferIterator& operator+=(size_t numBytes) BOOST_NOEXCEPT;
    BufferIterator& operator-=(size_t numBytes) BOOST_NOEXCEPT;

    friend bool operator> (const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator>=(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator==(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator< (const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator<=(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;

    friend BufferIterator operator+(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend BufferIterator operator-(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ptrdiff_t operator-(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The data.
     */
    uint8_t* bytes_;

    /**
     * @brief The logical offset of the start of the header area.
     */
    size_t start_;

    /**
     * @brief The logical offset of the end of the trailer area (on-byte-beyond).
     */
    size_t end_;

    /**
     * @brief The current position in the data area.
     */
    size_t cursor_;

};


////////////////////////////////////////////////////////////////////////////////
// BufferIterator.
inline BufferIterator::BufferIterator(uint8_t* bytes,
                                      size_t start,
                                      size_t end,
                                      size_t cursor) BOOST_NOEXCEPT :
    bytes_(bytes),
    start_(start),
    end_(end),
    cursor_(cursor)
{
}

inline BufferIterator::BufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT :
    bytes_(rhs.bytes_),
    start_(rhs.start_),
    end_(rhs.end_),
    cursor_(rhs.cursor_)
{
}

inline BufferIterator&
BufferIterator::operator=(const BufferIterator& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        bytes_  = rhs.bytes_;
        start_  = rhs.start_;
        end_    = rhs.end_;
        cursor_ = rhs.cursor_;
    }
    return *this;
}

inline size_t BufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return start_;
}

inline size_t BufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline size_t BufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return cursor_;
}

inline void BufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    ForwardCheck(numBytes);
    cursor_ += numBytes;
}

inline void BufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    BackwardCheck(numBytes);
    cursor_ -= numBytes;
}

template<class T>
inline void BufferIterator::Write(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::native>(data);
}

template<class T>
inline void BufferIterator::WriteL(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::little>(data);
}

template<class T>
inline void BufferIterator::WriteB(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::big>(data);
}

template<class T, endian::Order order>
inline void BufferIterator::WriteInOrder(T data) BOOST_NOEXCEPT
{
    static_assert(boost::is_integral<T>::value, "Invalid data type.");
    WritableCheck(sizeof (T));
    typedef typename MakeEndianTag<order>::Type  E;
    InternalWrite(static_cast<typename boost::make_unsigned<T>::type>(data),
                  cursor_,
                  E());
}

inline void
BufferIterator::InternalWrite(uint8_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    uint8_t* data = bytes_;
    data[offset++] = value;
    ++cursor_;
}

inline void
BufferIterator::InternalWrite(uint16_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[0];
    data[offset++] = b[1];
    cursor_ += 2;
}

inline void
BufferIterator::InternalWrite(uint32_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[0];
    data[offset++] = b[1];
    data[offset++] = b[2];
    data[offset++] = b[3];
    cursor_ += 4;
}

inline void
BufferIterator::InternalWrite(uint64_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[0];
    data[offset++] = b[1];
    data[offset++] = b[2];
    data[offset++] = b[3];
    data[offset++] = b[4];
    data[offset++] = b[5];
    data[offset++] = b[6];
    data[offset++] = b[7];
    cursor_ += 8;
}

inline void
BufferIterator::InternalWrite(uint8_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    uint8_t* data = bytes_;
    data[offset++] = value;
    ++cursor_;
}

inline void
BufferIterator::InternalWrite(uint16_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[1];
    data[offset++] = b[0];
    cursor_ += 2;
}

inline void
BufferIterator::InternalWrite(uint32_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[3];
    data[offset++] = b[2];
    data[offset++] = b[1];
    data[offset++] = b[0];
    cursor_ += 4;
}

inline void
BufferIterator::InternalWrite(uint64_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    v = value;
    uint8_t* data = bytes_;
    data[offset++] = b[7];
    data[offset++] = b[6];
    data[offset++] = b[5];
    data[offset++] = b[4];
    data[offset++] = b[3];
    data[offset++] = b[2];
    data[offset++] = b[1];
    data[offset++] = b[0];
    cursor_ += 8;
}

template<class T>
inline T BufferIterator::Read(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::native>();
}

template<class T>
inline T BufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::little>();
}

template<class T>
inline T BufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::big>();
}

template<class T, endian::Order order>
inline T BufferIterator::ReadInOrder(void) BOOST_NOEXCEPT
{
    static_assert(boost::is_integral<T>::value, "Invalid data type.");
    ReadableCheck(sizeof (T));
    typedef typename MakeEndianTag<order>::Type  E;
    typedef ReadTag<sizeof (T), InSolidAreaTag>  R;
    return InternalRead(cursor_, R(), E());
}

inline uint8_t
BufferIterator::InternalRead(size_t offset, ReadTag<1, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
BufferIterator::InternalRead(size_t offset, ReadTag<2, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    const uint8_t* data = bytes_;
    b[0] = data[offset++];
    b[1] = data[offset++];
    cursor_+= 2;
    return v;
}

inline uint32_t
BufferIterator::InternalRead(size_t offset, ReadTag<4, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    const uint8_t* data = bytes_;
    b[0] = data[offset++];
    b[1] = data[offset++];
    b[2] = data[offset++];
    b[3] = data[offset++];
    cursor_ += 4;
    return v;
}

inline uint64_t
BufferIterator::InternalRead(size_t offset, ReadTag<8, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    const uint8_t* data = bytes_;
    b[0] = data[offset++];
    b[1] = data[offset++];
    b[2] = data[offset++];
    b[3] = data[offset++];
    b[4] = data[offset++];
    b[5] = data[offset++];
    b[6] = data[offset++];
    b[7] = data[offset++];
    cursor_ += 8;
    return v;
}

inline uint8_t
BufferIterator::InternalRead(size_t offset, ReadTag<1, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
BufferIterator::InternalRead(size_t offset, ReadTag<2, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    const uint8_t* data = bytes_;
    b[1] = data[offset++];
    b[0] = data[offset++];
    cursor_ += 2;
    return v;
}

inline uint32_t
BufferIterator::InternalRead(size_t offset, ReadTag<4, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    const uint8_t* data = bytes_;
    b[3] = data[offset++];
    b[2] = data[offset++];
    b[1] = data[offset++];
    b[0] = data[offset++];
    cursor_ += 4;
    return v;
}

inline uint64_t
BufferIterator::InternalRead(size_t offset, ReadTag<8, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    const uint8_t* data = bytes_;
    b[7] = data[offset++];
    b[6] = data[offset++];
    b[5] = data[offset++];
    b[4] = data[offset++];
    b[3] = data[offset++];
    b[2] = data[offset++];
    b[1] = data[offset++];
    b[0] = data[offset++];
    cursor_ += 8;
    return v;
}

inline bool BufferIterator::CanMoveForward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ + numBytes <= end_;
}

inline bool BufferIterator::CanMoveBackward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ >= start_ + numBytes;
}

inline void BufferIterator::ForwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot move beyond the end of buffer.");
}

inline void BufferIterator::BackwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveBackward(numBytes),
                     "The buffer iterator cannot move beyond the start of buffer.");
}

inline void BufferIterator::WritableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot write beyond the end of buffer.");
}

inline void BufferIterator::ReadableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot read beyond the end of buffer.");
}

inline BufferIterator& BufferIterator::operator++(void) BOOST_NOEXCEPT
{
    MoveForward(1);
    return *this;
}

inline BufferIterator BufferIterator::operator++(int) BOOST_NOEXCEPT
{
    BufferIterator it = *this;
    MoveForward(1);
    return it;
}

inline BufferIterator& BufferIterator::operator--(void) BOOST_NOEXCEPT
{
    MoveBackward(1);
    return *this;
}

inline BufferIterator BufferIterator::operator--(int) BOOST_NOEXCEPT
{
    BufferIterator it = *this;
    MoveBackward(1);
    return it;
}

inline BufferIterator& BufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveForward(numBytes);
    return *this;
}

inline BufferIterator& BufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveBackward(numBytes);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
// BufferIterator operators.
inline bool operator> (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ > rhs.cursor_;
}

inline bool operator>=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ >= rhs.cursor_;
}

inline bool operator==(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ == rhs.cursor_;
}

inline bool operator!=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ != rhs.cursor_;
}

inline bool operator< (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ < rhs.cursor_;
}

inline bool operator<=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ <= rhs.cursor_;
}

inline BufferIterator operator+(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    BufferIterator it = lhs;
    it.MoveForward(numBytes);
    return it;
}

inline BufferIterator operator-(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    BufferIterator it = lhs;
    it.MoveBackward(numBytes);
    return it;
}

inline ptrdiff_t
operator-(const BufferIterator& lhs, const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ - rhs.cursor_;
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief Read-only buffer iterator.
 */
class ConstBufferIterator
{
    // Xtructors.
public:
    ConstBufferIterator(uint8_t* bytes, size_t start,
                        size_t end, size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    ConstBufferIterator(const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    ConstBufferIterator& operator=(const ConstBufferIterator& rhs) BOOST_NOEXCEPT;

    // Emplicit conversion from BufferIterator.
public:
    ConstBufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT;
    ConstBufferIterator& operator=(const BufferIterator& rhs) BOOST_NOEXCEPT;

public:
    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;
    size_t GetCursor(void) const BOOST_NOEXCEPT;

    // Move cursor.
public:
    /**
     * @brief Move the iterator toward the end of the data area.
     */
    void MoveForward(size_t numBytes) BOOST_NOEXCEPT;

    /**
     * @brief Move the iterator toward the end of the data area.
     */
    void MoveBackward(size_t numBytes) BOOST_NOEXCEPT;

    // Read data.
public:
    /**
     * @brief Read data in native endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T Read(void) BOOST_NOEXCEPT;

    /**
     * @brief Read data in little endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadL(void) BOOST_NOEXCEPT;

    /**
     * @brief Read data in big endian order.
     *
     * @tparam T Must be an integral type.
     */
    template<class T>
    T ReadB(void) BOOST_NOEXCEPT;

    // Operators.
public:
    ConstBufferIterator& operator++(void) BOOST_NOEXCEPT;
    ConstBufferIterator  operator++(int) BOOST_NOEXCEPT;
    ConstBufferIterator& operator--(void) BOOST_NOEXCEPT;
    ConstBufferIterator  operator--(int) BOOST_NOEXCEPT;
    ConstBufferIterator& operator+=(size_t numBytes) BOOST_NOEXCEPT;
    ConstBufferIterator& operator-=(size_t numBytes) BOOST_NOEXCEPT;

    friend bool operator> (const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator>=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator==(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator< (const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator<=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;

    friend ConstBufferIterator operator+(const ConstBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ConstBufferIterator operator-(const ConstBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ptrdiff_t operator-(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    BufferIterator it_;

};

////////////////////////////////////////////////////////////////////////////////
inline ConstBufferIterator::ConstBufferIterator(
    uint8_t* bytes, size_t start, size_t end, size_t cursor) BOOST_NOEXCEPT :
    it_(bytes, start, end, cursor)
{
}

inline ConstBufferIterator::ConstBufferIterator(const ConstBufferIterator& rhs) BOOST_NOEXCEPT :
    it_(rhs.it_)
{
}

inline ConstBufferIterator&
ConstBufferIterator::operator=(const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        it_ = rhs.it_;
    }
    return *this;
}

inline ConstBufferIterator::ConstBufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT :
    it_(rhs)
{
}

inline ConstBufferIterator&
ConstBufferIterator::operator=(const BufferIterator& rhs) BOOST_NOEXCEPT
{
    it_ = rhs;
    return *this;
}

inline size_t ConstBufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return it_.GetStart();
}

inline size_t ConstBufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return it_.GetEnd();
}

inline size_t ConstBufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return it_.GetCursor();
}

inline void ConstBufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveForward(numBytes);
}

inline void ConstBufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveBackward(numBytes);
}

template<class T>
inline T ConstBufferIterator::Read(void) BOOST_NOEXCEPT
{
    return it_.Read<T>();
}

template<class T>
inline T ConstBufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return it_.ReadL<T>();
}

template<class T>
inline T ConstBufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return it_.ReadB<T>();
}

inline ConstBufferIterator& ConstBufferIterator::operator++(void) BOOST_NOEXCEPT
{
    ++it_;
    return *this;
}

inline ConstBufferIterator ConstBufferIterator::operator++(int) BOOST_NOEXCEPT
{
    return ConstBufferIterator(it_++);
}

inline ConstBufferIterator& ConstBufferIterator::operator--(void) BOOST_NOEXCEPT
{
    --it_;
    return *this;
}

inline ConstBufferIterator ConstBufferIterator::operator--(int) BOOST_NOEXCEPT
{
    return ConstBufferIterator(it_--);
}

inline ConstBufferIterator& ConstBufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ += numBytes;
    return *this;
}

inline ConstBufferIterator& ConstBufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ -= numBytes;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ > rhs.it_;
}

inline bool
operator>=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ >= rhs.it_;
}

inline bool
operator==(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ == rhs.it_;
}

inline bool
operator!=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ != rhs.it_;
}

inline bool
operator< (const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ < rhs.it_;
}

inline bool
operator<=(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ <= rhs.it_;
}

inline ConstBufferIterator
operator+(const ConstBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    return ConstBufferIterator(lhs.it_ + numBytes);
}

inline ConstBufferIterator
operator-(const ConstBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    return ConstBufferIterator(lhs.it_ - numBytes);
}

inline ptrdiff_t
operator-(const ConstBufferIterator& lhs, const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ - rhs.it_;
}


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0

