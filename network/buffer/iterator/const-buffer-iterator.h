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

#ifndef CONST_BUFFER_ITERATOR_H__AB21FDF7_C94A_480A_A9BB_E131520BADB7
#define CONST_BUFFER_ITERATOR_H__AB21FDF7_C94A_480A_A9BB_E131520BADB7


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/network/buffer/iterator/buffer-iterator.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstBufferIterator
/**
 * @ingroup Network
 * @brief The iterator for reading buffer data.
 *
 * A specialization of <code>BasicBufferIterator<></code>.
 *
 * This buffer iterator operates on a common buffer.
 * i.e., it does not support zero-compressed buffer.
 */
template<>
class BasicBufferIterator</*readOnly=*/true, /*zcAware=*/false>
{
public:
    typedef BasicBufferIterator<true, false>  ConstBufferIterator;

    // Xtructors.
public:
    BasicBufferIterator(uint8_t* bytes, size_t start,
                        size_t end, size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    BasicBufferIterator(const ConstBufferIterator& rhs) BOOST_NOEXCEPT;
    BasicBufferIterator& operator=(const ConstBufferIterator& rhs) BOOST_NOEXCEPT;

    // Implicit conversion from BufferIterator.
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
// Typedef.
typedef BasicBufferIterator</*readOnly=*/true, /*zcAware=*/false>
        ConstBufferIterator;


////////////////////////////////////////////////////////////////////////////////
// ConstBufferIterator.
inline ConstBufferIterator::BasicBufferIterator(uint8_t* bytes,
                                                size_t start,
                                                size_t end,
                                                size_t cursor) BOOST_NOEXCEPT :
    it_(bytes, start, end, cursor)
{
}

inline ConstBufferIterator::BasicBufferIterator(const ConstBufferIterator& rhs) BOOST_NOEXCEPT :
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

inline ConstBufferIterator::BasicBufferIterator(const BufferIterator& rhs) BOOST_NOEXCEPT :
    it_(rhs)
{
}

inline ConstBufferIterator&
ConstBufferIterator::operator=(const BufferIterator& rhs) BOOST_NOEXCEPT
{
    it_ = rhs;
    return *this;
}

inline size_t
ConstBufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return it_.GetStart();
}

inline size_t
ConstBufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return it_.GetEnd();
}

inline size_t
ConstBufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return it_.GetCursor();
}

inline void
ConstBufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveForward(numBytes);
}

inline void
ConstBufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveBackward(numBytes);
}

template<class T>
inline T
ConstBufferIterator::Read(void) BOOST_NOEXCEPT
{
    return it_.Read<T>();
}

template<class T>
inline T
ConstBufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return it_.ReadL<T>();
}

template<class T>
inline T
ConstBufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return it_.ReadB<T>();
}

inline void
ConstBufferIterator::Read(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.Read(bytes, size);
}

inline void
ConstBufferIterator::ReadL(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.ReadL(bytes, size);
}

inline void
ConstBufferIterator::ReadB(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.ReadB(bytes, size);
}

inline ConstBufferIterator&
ConstBufferIterator::operator++(void) BOOST_NOEXCEPT
{
    ++it_;
    return *this;
}

inline ConstBufferIterator
ConstBufferIterator::operator++(int) BOOST_NOEXCEPT
{
    return ConstBufferIterator(it_++);
}

inline ConstBufferIterator&
ConstBufferIterator::operator--(void) BOOST_NOEXCEPT
{
    --it_;
    return *this;
}

inline ConstBufferIterator
ConstBufferIterator::operator--(int) BOOST_NOEXCEPT
{
    return ConstBufferIterator(it_--);
}

inline ConstBufferIterator&
ConstBufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ += numBytes;
    return *this;
}

inline ConstBufferIterator&
ConstBufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ -= numBytes;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
// ConstBufferIterator operators.
inline bool
operator> (const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ > rhs.it_;
}

inline bool
operator>=(const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ >= rhs.it_;
}

inline bool
operator==(const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ == rhs.it_;
}

inline bool
operator!=(const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ != rhs.it_;
}

inline bool
operator< (const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ < rhs.it_;
}

inline bool
operator<=(const ConstBufferIterator& lhs,
           const ConstBufferIterator& rhs) BOOST_NOEXCEPT
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
operator-(const ConstBufferIterator& lhs,
          const ConstBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ - rhs.it_;
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_BUFFER_ITERATOR_H__AB21FDF7_C94A_480A_A9BB_E131520BADB7

