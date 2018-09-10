/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
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

#ifndef ZC_BUFFER_ITERATOR_H__2ADF99D3_1A66_4A7F_A549_81CA18243133
#define ZC_BUFFER_ITERATOR_H__2ADF99D3_1A66_4A7F_A549_81CA18243133


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/utility/endian.h>
#include <boost/type_traits/type_identity.hpp>
#include <type_traits> // is_integral, is_floating_point, make_unsigned


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ZcBufferIterator.
/**
 * @ingroup Network
 * @brief The iterator for accessing zero-compressed buffer data.
 *
 * A specialization of <code>BasicBufferIterator<></code>.
 *
 * This buffer iterator operates on a zero-compressed buffer.
 * i.e., it does not support common buffer.
 */
template<>
class BasicBufferIterator</*readOnly=*/false, /*zcAware=*/true>
{
public:
    typedef BasicBufferIterator<false, true>  ZcBufferIterator;

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
    struct InSolidAreaTag   {};
    struct CrossZeroAreaTag {};
    struct CheckAreaTag     {};

    // Read tag.
    template<class T, class areaTag>
    struct ReadTag {};

    // Xtructors.
public:
    BasicBufferIterator(uint8_t* bytes,
                        size_t start,
                        size_t zeroStart,
                        size_t zeroEnd,
                        size_t end,
                        size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    BasicBufferIterator(const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    BasicBufferIterator& operator=(const ZcBufferIterator& rhs) BOOST_NOEXCEPT;

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

private:
    size_t CursorToOffset(void) const BOOST_NOEXCEPT;

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
    void InternalWrite(float    value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(double   value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;

    void InternalWrite(uint8_t  value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint16_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint32_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(uint64_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(float    value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(double   value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;

    // Write bytes.
public:
    /**
     * @brief Write bytes in native endian order.
     */
    void Write(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Write bytes in little endian order.
     */
    void WriteL(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Write bytes in big endian order.
     */
    void WriteB(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

private:
    template<endian::Order order>
    void WriteInOrder(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    void InternalWrite(const uint8_t* bytes, size_t size, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(const uint8_t* bytes, size_t size, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;

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

    uint8_t  InternalRead(size_t offset, ReadTag<uint8_t,  InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    float    InternalRead(size_t offset, ReadTag<float,    InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    double   InternalRead(size_t offset, ReadTag<double,   InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;

    uint8_t  InternalRead(size_t offset, ReadTag<uint8_t,  InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    float    InternalRead(size_t offset, ReadTag<float,    InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    double   InternalRead(size_t offset, ReadTag<double,   InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;

    uint8_t  InternalRead(ReadTag<uint8_t, CheckAreaTag>) BOOST_NOEXCEPT;

    uint8_t  InternalRead(ReadTag<uint8_t,  CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(ReadTag<uint16_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(ReadTag<uint32_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(ReadTag<uint64_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    float    InternalRead(ReadTag<float,    CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;
    double   InternalRead(ReadTag<double,   CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT;

    uint8_t  InternalRead(ReadTag<uint8_t,  CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint16_t InternalRead(ReadTag<uint16_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint32_t InternalRead(ReadTag<uint32_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    uint64_t InternalRead(ReadTag<uint64_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    float    InternalRead(ReadTag<float,    CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;
    double   InternalRead(ReadTag<double,   CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT;

    // Read bytes.
public:
    /**
     * @brief Read bytes in native endian order.
     */
    void Read(uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Read bytes in little endian order.
     */
    void ReadL(uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Read bytes in big endian order.
     */
    void ReadB(uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

private:
    template<endian::Order order>
    void ReadInOrder(uint8_t* bytes, size_t size) BOOST_NOEXCEPT;

    void InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, ReverseEndianTag) BOOST_NOEXCEPT;

    void InternalRead(uint8_t* bytes, size_t size, CrossZeroAreaTag, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalRead(uint8_t* bytes, size_t size, CrossZeroAreaTag, ReverseEndianTag) BOOST_NOEXCEPT;

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

    /**
     * @brief The cursor is within the zero-compressed area?
     */
    bool InZeroArea(void) const BOOST_NOEXCEPT;

    /**
     * @brief Moving the cursor crosses the zero-compressed area?
     */
    bool CrossZeroArea(size_t numBytes) const BOOST_NOEXCEPT;

    // Operators.
public:
    ZcBufferIterator& operator++(void) BOOST_NOEXCEPT;
    ZcBufferIterator  operator++(int) BOOST_NOEXCEPT;
    ZcBufferIterator& operator--(void) BOOST_NOEXCEPT;
    ZcBufferIterator  operator--(int) BOOST_NOEXCEPT;
    ZcBufferIterator& operator+=(size_t numBytes) BOOST_NOEXCEPT;
    ZcBufferIterator& operator-=(size_t numBytes) BOOST_NOEXCEPT;

    friend bool operator> (const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator>=(const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator==(const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator< (const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator<=(const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;

    friend ZcBufferIterator operator+(const ZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ZcBufferIterator operator-(const ZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ptrdiff_t operator-(const ZcBufferIterator& lhs, const ZcBufferIterator& rhs) BOOST_NOEXCEPT;

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
     * @brief The logical offset of the start of the zero-compressed data area.
     */
    size_t zeroStart_;

    /**
     * @brief The logical offset of the end of the zero-compressed data area (on-byte-beyond).
     */
    size_t zeroEnd_;

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
// Typedef.
typedef BasicBufferIterator</*readOnly=*/false, /*zcAware=*/true>
        ZcBufferIterator;


////////////////////////////////////////////////////////////////////////////////
// ZcBufferIterator.
inline ZcBufferIterator::BasicBufferIterator(uint8_t* bytes,
                                             size_t start,
                                             size_t zeroStart,
                                             size_t zeroEnd,
                                             size_t end,
                                             size_t cursor) BOOST_NOEXCEPT :
    bytes_(bytes),
    start_(start),
    zeroStart_(zeroStart),
    zeroEnd_(zeroEnd),
    end_(end),
    cursor_(cursor)
{
}

inline ZcBufferIterator::BasicBufferIterator(const ZcBufferIterator& rhs) BOOST_NOEXCEPT :
    bytes_(rhs.bytes_),
    start_(rhs.start_),
    zeroStart_(rhs.zeroStart_),
    zeroEnd_(rhs.zeroEnd_),
    end_(rhs.end_),
    cursor_(rhs.cursor_)
{
}

inline ZcBufferIterator&
ZcBufferIterator::operator=(const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        bytes_     = rhs.bytes_;
        start_     = rhs.start_;
        zeroStart_ = rhs.zeroStart_;
        zeroEnd_   = rhs.zeroEnd_;
        end_       = rhs.end_;
        cursor_    = rhs.cursor_;
    }
    return *this;
}

inline size_t
ZcBufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return start_;
}

inline size_t
ZcBufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline size_t
ZcBufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return cursor_;
}

inline void
ZcBufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    ForwardCheck(numBytes);
    cursor_ += numBytes;
}

inline void
ZcBufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    BackwardCheck(numBytes);
    cursor_ -= numBytes;
}

inline size_t
ZcBufferIterator::CursorToOffset(void) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(!InZeroArea(),
                     "Cannot convert the current cursor to offset since "
                     "the current cursor is in the zero-compressed area.");
    size_t offset;
    if (cursor_ < zeroStart_)
    {
        offset = cursor_;
    }
    else
    {
        offset = cursor_ - (zeroEnd_ - zeroStart_);
    }
    return offset;
}

template<class T>
inline void
ZcBufferIterator::Write(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::native>(data);
}

template<class T>
inline void
ZcBufferIterator::WriteL(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::little>(data);
}

template<class T>
inline void
ZcBufferIterator::WriteB(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T, endian::big>(data);
}

template<class T, endian::Order order>
inline void
ZcBufferIterator::WriteInOrder(T data) BOOST_NOEXCEPT
{
    static_assert(std::is_integral<T>::value ||
                  std::is_floating_point<T>::value,
                  "Invalid data type.");
    WritableCheck(sizeof (T));
    typedef typename MakeEndianTag<order>::Type  E;
    typedef typename std::conditional<
        std::is_floating_point<T>::value, T,
        typename std::make_unsigned<T>::type>::type  V;
    InternalWrite(static_cast<V>(data), CursorToOffset(), E());
}

inline void
ZcBufferIterator::InternalWrite(uint8_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    uint8_t* data = bytes_;
    data[offset++] = value;
    ++cursor_;
}

inline void
ZcBufferIterator::InternalWrite(uint16_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalWrite(uint32_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalWrite(uint64_t value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalWrite(float value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (float) == 4, "Unsupported size of float.");
    union
    {
        uint8_t  b[4];
        float    v;
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
ZcBufferIterator::InternalWrite(double value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (double) == 8, "Unsupported size of double.");
    union
    {
        uint8_t  b[8];
        double   v;
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
ZcBufferIterator::InternalWrite(uint8_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    uint8_t* data = bytes_;
    data[offset++] = value;
    ++cursor_;
}

inline void
ZcBufferIterator::InternalWrite(uint16_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalWrite(uint32_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalWrite(uint64_t value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
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

inline void
ZcBufferIterator::InternalWrite(float value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (float) == 4, "Unsupported size of float.");
    union
    {
        uint8_t  b[4];
        float    v;
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
ZcBufferIterator::InternalWrite(double value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (double) == 8, "Unsupported size of double.");
    union
    {
        uint8_t  b[8];
        double   v;
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

inline void
ZcBufferIterator::Write(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder<endian::native>(bytes, size);
}

inline void
ZcBufferIterator::WriteL(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder<endian::little>(bytes, size);
}

inline void
ZcBufferIterator::WriteB(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder<endian::big>(bytes, size);
}

template<endian::Order order>
inline void
ZcBufferIterator::WriteInOrder(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT(bytes);
    WritableCheck(size);
    typedef typename MakeEndianTag<order>::Type  E;
    InternalWrite(bytes, size, CursorToOffset(), E());
}

inline void
ZcBufferIterator::InternalWrite(const uint8_t* bytes, size_t size, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes_[offset++] = *bytes++;
        ++cursor_;
    }
}

inline void
ZcBufferIterator::InternalWrite(const uint8_t* bytes, size_t size, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes_[offset++] = bytes[size];
        ++cursor_;
    }
}

template<class T>
inline T
ZcBufferIterator::Read(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::native>();
}

template<class T>
inline T
ZcBufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::little>();
}

template<class T>
inline T
ZcBufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T, endian::big>();
}

template<class T, endian::Order order>
inline T
ZcBufferIterator::ReadInOrder(void) BOOST_NOEXCEPT
{
    static_assert(std::is_integral<T>::value ||
                  std::is_floating_point<T>::value,
                  "Invalid data type.");
    ReadableCheck(sizeof (T));
    typedef typename MakeEndianTag<order>::Type  E;
    typedef typename std::conditional<
        std::is_floating_point<T>::value, T,
        typename std::make_unsigned<T>::type>::type  V;
    // Read in header area.
    if (cursor_ + sizeof (T) <= zeroStart_)
    {
        typedef ReadTag<V, InSolidAreaTag>  R;
        return InternalRead(cursor_, R(), E());
    }
    // Read in trailer area.
    else if (zeroEnd_ <= cursor_)
    {
        typedef ReadTag<V, InSolidAreaTag>  R;
        return InternalRead(cursor_ - (zeroEnd_ - zeroStart_), R(), E());
    }
    // Read in zero-compressed area.
    else if (zeroStart_ <= cursor_ && cursor_ + sizeof (T) <= zeroEnd_)
    {
        cursor_ += sizeof (T);
        return 0;
    }
    // Read across zero-compressed area.
    else
    {
        typedef ReadTag<V, CrossZeroAreaTag>  R;
        return InternalRead(R(), E());
    }
}

inline uint8_t
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint8_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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

inline float
ZcBufferIterator::InternalRead(size_t offset, ReadTag<float, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (float) == 4, "Unsupported size of float.");
    union
    {
        uint8_t  b[4];
        float    v;
    };
    const uint8_t* data = bytes_;
    b[0] = data[offset++];
    b[1] = data[offset++];
    b[2] = data[offset++];
    b[3] = data[offset++];
    cursor_ += 4;
    return v;
}

inline double
ZcBufferIterator::InternalRead(size_t offset, ReadTag<double, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (double) == 8, "Unsupported size of double.");
    union
    {
        uint8_t  b[8];
        double   v;
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
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint8_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
ZcBufferIterator::InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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

inline float
ZcBufferIterator::InternalRead(size_t offset, ReadTag<float, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (float) == 4, "Unsupported size of float.");
    union
    {
        uint8_t  b[4];
        float    v;
    };
    const uint8_t* data = bytes_;
    b[3] = data[offset++];
    b[2] = data[offset++];
    b[1] = data[offset++];
    b[0] = data[offset++];
    cursor_ += 4;
    return v;
}

inline double
ZcBufferIterator::InternalRead(size_t offset, ReadTag<double, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (double) == 8, "Unsupported size of double.");
    union
    {
        uint8_t  b[8];
        double   v;
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

inline uint8_t
ZcBufferIterator::InternalRead(ReadTag<uint8_t, CheckAreaTag>) BOOST_NOEXCEPT
{
    uint8_t b = 0;
    if (!InZeroArea())
    {
        const uint8_t* data = bytes_;
        b = data[CursorToOffset()];
    }
    ++cursor_;
    return b;
}

inline uint8_t
ZcBufferIterator::InternalRead(ReadTag<uint8_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    ++cursor_;
    return 0;
}

inline uint16_t
ZcBufferIterator::InternalRead(ReadTag<uint16_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline uint32_t
ZcBufferIterator::InternalRead(ReadTag<uint32_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline uint64_t
ZcBufferIterator::InternalRead(ReadTag<uint64_t, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[4] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[5] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[6] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[7] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline float
ZcBufferIterator::InternalRead(ReadTag<float, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (float) == 4, "Unsupported size of float.");
    union
    {
        uint8_t  b[4];
        float    v;
    };
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline double
ZcBufferIterator::InternalRead(ReadTag<double, CrossZeroAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    static_assert(sizeof (double) == 8, "Unsupported size of double.");
    union
    {
        uint8_t  b[8];
        double   v;
    };
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[4] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[5] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[6] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[7] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline uint8_t
ZcBufferIterator::InternalRead(ReadTag<uint8_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    return 0;
}

inline uint16_t
ZcBufferIterator::InternalRead(ReadTag<uint16_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[2];
        uint16_t v;
    };
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline uint32_t
ZcBufferIterator::InternalRead(ReadTag<uint32_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        uint32_t v;
    };
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline uint64_t
ZcBufferIterator::InternalRead(ReadTag<uint64_t, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        uint64_t v;
    };
    b[7] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[6] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[5] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[4] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline float
ZcBufferIterator::InternalRead(ReadTag<float, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[4];
        float    v;
    };
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline double
ZcBufferIterator::InternalRead(ReadTag<double, CrossZeroAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    union
    {
        uint8_t  b[8];
        double   v;
    };
    b[7] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[6] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[5] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[4] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[3] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[2] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[1] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    b[0] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    return v;
}

inline void
ZcBufferIterator::Read(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder<endian::native>(bytes, size);
}

inline void
ZcBufferIterator::ReadL(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder<endian::little>(bytes, size);
}

inline void
ZcBufferIterator::ReadB(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder<endian::big>(bytes, size);
}

template<endian::Order order>
inline void
ZcBufferIterator::ReadInOrder(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadableCheck(size);
    typedef typename MakeEndianTag<order>::Type  E;
    // Read in header area.
    if (cursor_ + size <= zeroStart_)
    {
        return InternalRead(bytes, size, cursor_, InSolidAreaTag(), E());
    }
    // Read in trailer area.
    else if (zeroEnd_ <= cursor_)
    {
        return InternalRead(
            bytes, size, cursor_ - (zeroEnd_ - zeroStart_), InSolidAreaTag(), E());
    }
    // Read in zero-compressed area.
    else if (zeroStart_ <= cursor_ && cursor_ + size <= zeroEnd_)
    {
        std::memset(bytes, 0, size);
        cursor_ += size;
    }
    // Read across zero-compressed area.
    else
    {
        return InternalRead(bytes, size, CrossZeroAreaTag(), E());
    }
}

inline void
ZcBufferIterator::InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, KeepEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        *bytes++ = bytes_[offset++];
        ++cursor_;
    }
}

inline void
ZcBufferIterator::InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, ReverseEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes[size] = bytes_[offset++];
        ++cursor_;
    }
}

inline void
ZcBufferIterator::InternalRead(uint8_t* bytes, size_t size, CrossZeroAreaTag, KeepEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        *bytes++ = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    }
}

inline void
ZcBufferIterator::InternalRead(uint8_t* bytes, size_t size, CrossZeroAreaTag, ReverseEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes[size] = InternalRead(ReadTag<uint8_t, CheckAreaTag>());
    }
}

inline bool
ZcBufferIterator::CanMoveForward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ + numBytes <= end_;
}

inline bool
ZcBufferIterator::CanMoveBackward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ >= start_ + numBytes;
}

inline void
ZcBufferIterator::ForwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot move beyond the end of buffer.");
}

inline void
ZcBufferIterator::BackwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveBackward(numBytes),
                     "The buffer iterator cannot move beyond the start of buffer.");
}

inline void
ZcBufferIterator::WritableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot write beyond the end of buffer.");
    BOOST_ASSERT_MSG(!CrossZeroArea(numBytes),
                     "The buffer iterator cannot write in the zero-compressed area.");
}

inline void
ZcBufferIterator::ReadableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot read beyond the end of buffer.");
}

inline bool
ZcBufferIterator::InZeroArea(void) const BOOST_NOEXCEPT
{
    return (zeroStart_ <= cursor_) && (cursor_ < zeroEnd_);
}

inline bool
ZcBufferIterator::CrossZeroArea(size_t numBytes) const BOOST_NOEXCEPT
{
    return (cursor_ < zeroStart_ && zeroStart_ < cursor_ + numBytes) ||
           (cursor_ < zeroEnd_   && zeroEnd_   < cursor_ + numBytes);
}

inline ZcBufferIterator&
ZcBufferIterator::operator++(void) BOOST_NOEXCEPT
{
    MoveForward(1);
    return *this;
}

inline ZcBufferIterator
ZcBufferIterator::operator++(int) BOOST_NOEXCEPT
{
    ZcBufferIterator it = *this;
    MoveForward(1);
    return it;
}

inline ZcBufferIterator&
ZcBufferIterator::operator--(void) BOOST_NOEXCEPT
{
    MoveBackward(1);
    return *this;
}

inline ZcBufferIterator
ZcBufferIterator::operator--(int) BOOST_NOEXCEPT
{
    ZcBufferIterator it = *this;
    MoveBackward(1);
    return it;
}

inline ZcBufferIterator&
ZcBufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveForward(numBytes);
    return *this;
}

inline ZcBufferIterator&
ZcBufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveBackward(numBytes);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
// ZcBufferIterator operators.
inline bool
operator> (const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ > rhs.cursor_;
}

inline bool
operator>=(const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ >= rhs.cursor_;
}

inline bool
operator==(const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ == rhs.cursor_;
}

inline bool
operator!=(const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ != rhs.cursor_;
}

inline bool
operator< (const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ < rhs.cursor_;
}

inline bool
operator<=(const ZcBufferIterator& lhs,
           const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ <= rhs.cursor_;
}

inline ZcBufferIterator
operator+(const ZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    ZcBufferIterator it = lhs;
    it.MoveForward(numBytes);
    return it;
}

inline ZcBufferIterator
operator-(const ZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    ZcBufferIterator it = lhs;
    it.MoveBackward(numBytes);
    return it;
}

inline ptrdiff_t
operator-(const ZcBufferIterator& lhs,
          const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ - rhs.cursor_;
}


NSFX_CLOSE_NAMESPACE


#endif // ZC_BUFFER_ITERATOR_H__2ADF99D3_1A66_4A7F_A549_81CA18243133

