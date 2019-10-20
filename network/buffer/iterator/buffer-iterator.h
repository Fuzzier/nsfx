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

#ifndef BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0
#define BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/utility/endian.h>
#include <boost/type_traits/type_identity.hpp>
#include <type_traits> // is_integral, is_floating_point, make_unsigned


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BasicBufferIterator.
/**
 * @ingroup Network
 * @brief The iterator for accessing buffer data.
 *
 * A specialization of <code>BasicBufferIterator<></code>.
 *
 * This buffer iterator operates on a common buffer.
 * i.e., it does not support zero-compressed buffer.
 */
template<>
class BasicBufferIterator</*readOnly=*/false, /*zcAware=*/false>
{
public:
    typedef BasicBufferIterator<false, false>  BufferIterator;

private:
    // Endian tag.
    struct KeepEndianTag {};
    struct ReverseEndianTag {};

    template<class endian_t, bool = endian_traits<endian_t>::is_native>
    struct MakeEndianTag
    {
        typedef KeepEndianTag  type;
    };

    template<class endian_t>
    struct MakeEndianTag<endian_t, /* is_native = */false>
    {
        typedef ReverseEndianTag  type;
    };

    // Area tag.
    struct InSolidAreaTag {};

    // Read tag.
    template<class T, class areaTag>
    struct ReadTag {};

    // Type.
    template<class T, bool integral = std::is_integral<T>::value,
                      bool floating_point = std::is_floating_point<T>::value>
    struct MakeRegularType
    {
        static_assert(integral || floating_point, "Unsupported type.");
    };

    template<class T>
    struct MakeRegularType<T, true, false>
    {
        typedef typename std::make_unsigned<T>::type  type;
    };

    template<class T>
    struct MakeRegularType<T, false, true>
    {
        typedef T  type;
    };

    // Xtructors.
public:
    BasicBufferIterator(uint8_t* bytes, size_t start,
                        size_t end, size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    BasicBufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT;
    BasicBufferIterator& operator=(const BufferIterator& rhs) BOOST_NOEXCEPT;

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
     * @tparam endian_t  Use one of the pre-defined enumeration values to
     *                   automatically deduce this type.
     *                   * \c big_endian
     *                   * \c little_endian
     *                   * \c native_endian
     */
    template<class T, class endian_t>
    void WriteInOrder(T data, endian_t) BOOST_NOEXCEPT;

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
    template<class endian_t>
    void WriteInOrder(const uint8_t* bytes, size_t size, endian_t) BOOST_NOEXCEPT;

    void InternalWrite(const uint8_t* bytes, size_t size, size_t offset, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalWrite(const uint8_t* bytes, size_t size, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT;

    // Fill bytes.
public:
    /**
     * @brief Fill.
     *
     * @param[in] v    The value used to fill.
     * @param[in] size The number of bytes to fill.
     */
    void Fill(uint8_t v, size_t size) BOOST_NOEXCEPT;

private:
    void InternalFill(uint8_t v, size_t size, size_t offset) BOOST_NOEXCEPT;

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
     * @tparam endian_t  Use one of the pre-defined enumeration values to
     *                   automatically deduce this type.
     *                   * \c big_endian
     *                   * \c little_endian
     *                   * \c native_endian
     */
    template<class T, class endian_t>
    T ReadInOrder(endian_t) BOOST_NOEXCEPT;

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
    template<class endian_t>
    void ReadInOrder(uint8_t* bytes, size_t size, endian_t) BOOST_NOEXCEPT;

    void InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, KeepEndianTag) BOOST_NOEXCEPT;
    void InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, ReverseEndianTag) BOOST_NOEXCEPT;

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
// Typedef.
typedef BasicBufferIterator</*readOnly=*/false, /*zcAware=*/false>
        BufferIterator;


////////////////////////////////////////////////////////////////////////////////
// BufferIterator.
inline BufferIterator::BasicBufferIterator(uint8_t* bytes,
                                           size_t start,
                                           size_t end,
                                           size_t cursor) BOOST_NOEXCEPT :
    bytes_(bytes),
    start_(start),
    end_(end),
    cursor_(cursor)
{
}

inline BufferIterator::BasicBufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT :
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

inline size_t
BufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return start_;
}

inline size_t
BufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return end_;
}

inline size_t
BufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return cursor_;
}

inline void
BufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    ForwardCheck(numBytes);
    cursor_ += numBytes;
}

inline void
BufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    BackwardCheck(numBytes);
    cursor_ -= numBytes;
}

template<class T>
inline void
BufferIterator::Write(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T>(data, native_endian);
}

template<class T>
inline void
BufferIterator::WriteL(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T>(data, little_endian);
}

template<class T>
inline void
BufferIterator::WriteB(typename boost::type_identity<T>::type  data) BOOST_NOEXCEPT
{
    WriteInOrder<T>(data, big_endian);
}

template<class T, class endian_t>
inline void
BufferIterator::WriteInOrder(T data, endian_t) BOOST_NOEXCEPT
{
    static_assert(std::is_integral<T>::value ||
                  std::is_floating_point<T>::value,
                  "Invalid data type.");
    WritableCheck(sizeof (T));
    typedef typename MakeEndianTag<endian_t>::type  E;
    typedef typename MakeRegularType<T>::type       V;
    InternalWrite(static_cast<V>(data), cursor_, E());
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
BufferIterator::InternalWrite(float value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalWrite(double value, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
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

inline void
BufferIterator::InternalWrite(float value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalWrite(double value, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::Write(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder(bytes, size, native_endian);
}

inline void
BufferIterator::WriteL(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder(bytes, size, little_endian);
}

inline void
BufferIterator::WriteB(const uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    WriteInOrder(bytes, size, big_endian);
}

template<class endian_t>
inline void
BufferIterator::WriteInOrder(const uint8_t* bytes, size_t size, endian_t) BOOST_NOEXCEPT
{
    BOOST_ASSERT(bytes);
    WritableCheck(size);
    typedef typename MakeEndianTag<endian_t>::type  E;
    InternalWrite(bytes, size, cursor_, E());
}

inline void
BufferIterator::InternalWrite(const uint8_t* bytes, size_t size, size_t offset, KeepEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes_[offset++] = *bytes++;
        ++cursor_;
    }
}

inline void
BufferIterator::InternalWrite(const uint8_t* bytes, size_t size, size_t offset, ReverseEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes_[offset++] = bytes[size];
        ++cursor_;
    }
}

inline void
BufferIterator::Fill(uint8_t v, size_t size) BOOST_NOEXCEPT
{
    WritableCheck(size);
    InternalFill(v, size, cursor_);
}

inline void
BufferIterator::InternalFill(uint8_t v, size_t size, size_t offset) BOOST_NOEXCEPT
{
    std::memset(bytes_ + offset, v, size);
    cursor_ += size;
}

template<class T>
inline T
BufferIterator::Read(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T>(native_endian);
}

template<class T>
inline T
BufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T>(little_endian);
}

template<class T>
inline T
BufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return ReadInOrder<T>(big_endian);
}

template<class T, class endian_t>
inline T
BufferIterator::ReadInOrder(endian_t) BOOST_NOEXCEPT
{
    static_assert(std::is_integral<T>::value ||
                  std::is_floating_point<T>::value,
                  "Invalid data type.");
    ReadableCheck(sizeof (T));
    typedef typename MakeEndianTag<endian_t>::type  E;
    typedef typename MakeRegularType<T>::type       V;
    typedef ReadTag<V, InSolidAreaTag>  R;
    return InternalRead(cursor_, R(), E());
}

inline uint8_t
BufferIterator::InternalRead(size_t offset, ReadTag<uint8_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
BufferIterator::InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<float, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<double, InSolidAreaTag>, KeepEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<uint8_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
{
    uint8_t b;
    const uint8_t* data = bytes_;
    b = data[offset++];
    ++cursor_;
    return b;
}

inline uint16_t
BufferIterator::InternalRead(size_t offset, ReadTag<uint16_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<uint32_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<uint64_t, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<float, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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
BufferIterator::InternalRead(size_t offset, ReadTag<double, InSolidAreaTag>, ReverseEndianTag) BOOST_NOEXCEPT
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

inline void
BufferIterator::Read(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder(bytes, size, native_endian);
}

inline void
BufferIterator::ReadL(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder(bytes, size, little_endian);
}

inline void
BufferIterator::ReadB(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    ReadInOrder(bytes, size, big_endian);
}

template<class endian_t>
inline void
BufferIterator::ReadInOrder(uint8_t* bytes, size_t size, endian_t) BOOST_NOEXCEPT
{
    BOOST_ASSERT(bytes);
    ReadableCheck(size);
    typedef typename MakeEndianTag<endian_t>::type  E;
    InternalRead(bytes, size, cursor_, InSolidAreaTag(), E());
}

inline void
BufferIterator::InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, KeepEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        *bytes++ = bytes_[offset++];
        ++cursor_;
    }
}

inline void
BufferIterator::InternalRead(uint8_t* bytes, size_t size, size_t offset, InSolidAreaTag, ReverseEndianTag) BOOST_NOEXCEPT
{
    while (size--)
    {
        bytes[size] = bytes_[offset++];
        ++cursor_;
    }
}

inline bool
BufferIterator::CanMoveForward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ + numBytes <= end_;
}

inline bool
BufferIterator::CanMoveBackward(size_t numBytes) const BOOST_NOEXCEPT
{
    return cursor_ >= start_ + numBytes;
}

inline void
BufferIterator::ForwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot move beyond the end of buffer.");
}

inline void
BufferIterator::BackwardCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveBackward(numBytes),
                     "The buffer iterator cannot move beyond the start of buffer.");
}

inline void
BufferIterator::WritableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot write beyond the end of buffer.");
}

inline void
BufferIterator::ReadableCheck(size_t numBytes) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(CanMoveForward(numBytes),
                     "The buffer iterator cannot read beyond the end of buffer.");
}

inline BufferIterator&
BufferIterator::operator++(void) BOOST_NOEXCEPT
{
    MoveForward(1);
    return *this;
}

inline BufferIterator
BufferIterator::operator++(int) BOOST_NOEXCEPT
{
    BasicBufferIterator it = *this;
    MoveForward(1);
    return it;
}

inline BufferIterator&
BufferIterator::operator--(void) BOOST_NOEXCEPT
{
    MoveBackward(1);
    return *this;
}

inline BufferIterator
BufferIterator::operator--(int) BOOST_NOEXCEPT
{
    BasicBufferIterator it = *this;
    MoveBackward(1);
    return it;
}

inline BufferIterator&
BufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveForward(numBytes);
    return *this;
}

inline BufferIterator&
BufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    MoveBackward(numBytes);
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
// BufferIterator operators.
inline bool
operator> (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ > rhs.cursor_;
}

inline bool
operator>=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ >= rhs.cursor_;
}

inline bool
operator==(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ == rhs.cursor_;
}

inline bool
operator!=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ != rhs.cursor_;
}

inline bool
operator< (const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ < rhs.cursor_;
}

inline bool
operator<=(const BufferIterator& lhs,
                       const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ <= rhs.cursor_;
}

inline BufferIterator
operator+(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    BufferIterator it = lhs;
    it.MoveForward(numBytes);
    return it;
}

inline BufferIterator
operator-(const BufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    BufferIterator it = lhs;
    it.MoveBackward(numBytes);
    return it;
}

inline ptrdiff_t
operator-(const BufferIterator& lhs,
          const BufferIterator& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(lhs.bytes_ == rhs.bytes_,
                     "Cannot compare unrelated buffer iterators.");
    return lhs.cursor_ - rhs.cursor_;
}


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_ITERATOR_H__C8C5F46F_FC42_42E4_8791_5A68CB98D9C0

