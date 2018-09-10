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

#ifndef CONST_ZC_BUFFER_ITERATOR_H__25E734E5_5CA4_4C8B_91D8_BAE4C2037F52
#define CONST_ZC_BUFFER_ITERATOR_H__25E734E5_5CA4_4C8B_91D8_BAE4C2037F52


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/network/buffer/iterator/zc-buffer-iterator.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstZcBufferIterator.
/**
 * @ingroup Network
 * @brief The iterator for reading zero-compressed buffer data.
 */
template<>
class BasicBufferIterator</*readOnly=*/true, /*zcAware=*/true>
{
public:
    typedef BasicBufferIterator<true, true>  ConstZcBufferIterator;

    // Xtructors.
public:
    BasicBufferIterator(uint8_t* bytes,
                        size_t start,
                        size_t zeroStart_,
                        size_t zeroEnd_,
                        size_t end,
                        size_t cursor) BOOST_NOEXCEPT;

    // Copyable.
public:
    BasicBufferIterator(const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    BasicBufferIterator& operator=(const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;

    // Implicit conversion from ZcBufferIterator<true>.
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
    ConstZcBufferIterator& operator++(void) BOOST_NOEXCEPT;
    ConstZcBufferIterator  operator++(int) BOOST_NOEXCEPT;
    ConstZcBufferIterator& operator--(void) BOOST_NOEXCEPT;
    ConstZcBufferIterator  operator--(int) BOOST_NOEXCEPT;
    ConstZcBufferIterator& operator+=(size_t numBytes) BOOST_NOEXCEPT;
    ConstZcBufferIterator& operator-=(size_t numBytes) BOOST_NOEXCEPT;

    friend bool operator> (const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator>=(const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator==(const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator< (const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;
    friend bool operator<=(const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;

    friend ConstZcBufferIterator operator+(const ConstZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ConstZcBufferIterator operator-(const ConstZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT;
    friend ptrdiff_t operator-(const ConstZcBufferIterator& lhs, const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    ZcBufferIterator it_;

};


////////////////////////////////////////////////////////////////////////////////
// Typedef.
typedef BasicBufferIterator</*readOnly=*/true, /*zcAware=*/true>
        ConstZcBufferIterator;


////////////////////////////////////////////////////////////////////////////////
// ConstZcBufferIterator.
inline ConstZcBufferIterator::BasicBufferIterator(
        uint8_t* bytes, size_t start, size_t zeroStart,
        size_t zeroEnd, size_t end, size_t cursor) BOOST_NOEXCEPT :
    it_(bytes, start, zeroStart, zeroEnd, end, cursor)
{
}

inline ConstZcBufferIterator::BasicBufferIterator(const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT :
    it_(rhs.it_)
{
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator=(const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        it_ = rhs.it_;
    }
    return *this;
}

inline ConstZcBufferIterator::BasicBufferIterator(const ZcBufferIterator& rhs) BOOST_NOEXCEPT :
    it_(rhs)
{
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator=(const ZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    it_ = rhs;
    return *this;
}

inline size_t
ConstZcBufferIterator::GetStart(void) const BOOST_NOEXCEPT
{
    return it_.GetStart();
}

inline size_t
ConstZcBufferIterator::GetEnd(void) const BOOST_NOEXCEPT
{
    return it_.GetEnd();
}

inline size_t
ConstZcBufferIterator::GetCursor(void) const BOOST_NOEXCEPT
{
    return it_.GetCursor();
}

inline void
ConstZcBufferIterator::MoveForward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveForward(numBytes);
}

inline void
ConstZcBufferIterator::MoveBackward(size_t numBytes) BOOST_NOEXCEPT
{
    it_.MoveBackward(numBytes);
}

template<class T>
inline T
ConstZcBufferIterator::Read(void) BOOST_NOEXCEPT
{
    return it_.Read<T>();
}

template<class T>
inline T
ConstZcBufferIterator::ReadL(void) BOOST_NOEXCEPT
{
    return it_.ReadL<T>();
}

template<class T>
inline T
ConstZcBufferIterator::ReadB(void) BOOST_NOEXCEPT
{
    return it_.ReadB<T>();
}

inline void
ConstZcBufferIterator::Read(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.Read(bytes, size);
}

inline void
ConstZcBufferIterator::ReadL(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.ReadL(bytes, size);
}

inline void
ConstZcBufferIterator::ReadB(uint8_t* bytes, size_t size) BOOST_NOEXCEPT
{
    it_.ReadB(bytes, size);
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator++(void) BOOST_NOEXCEPT
{
    ++it_;
    return *this;
}

inline ConstZcBufferIterator
ConstZcBufferIterator::operator++(int) BOOST_NOEXCEPT
{
    return ConstZcBufferIterator(it_++);
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator--(void) BOOST_NOEXCEPT
{
    --it_;
    return *this;
}

inline ConstZcBufferIterator
ConstZcBufferIterator::operator--(int) BOOST_NOEXCEPT
{
    return ConstZcBufferIterator(it_--);
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator+=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ += numBytes;
    return *this;
}

inline ConstZcBufferIterator&
ConstZcBufferIterator::operator-=(size_t numBytes) BOOST_NOEXCEPT
{
    it_ -= numBytes;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
// ConstZcBufferIterator operators.
inline bool
operator> (const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ > rhs.it_;
}

inline bool
operator>=(const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ >= rhs.it_;
}

inline bool
operator==(const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ == rhs.it_;
}

inline bool
operator!=(const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ != rhs.it_;
}

inline bool
operator< (const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ < rhs.it_;
}

inline bool
operator<=(const ConstZcBufferIterator& lhs,
           const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ <= rhs.it_;
}

inline ConstZcBufferIterator
operator+(const ConstZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    return ConstZcBufferIterator(lhs.it_ + numBytes);
}

inline ConstZcBufferIterator
operator-(const ConstZcBufferIterator& lhs, size_t numBytes) BOOST_NOEXCEPT
{
    return ConstZcBufferIterator(lhs.it_ - numBytes);
}

inline ptrdiff_t
operator-(const ConstZcBufferIterator& lhs,
          const ConstZcBufferIterator& rhs) BOOST_NOEXCEPT
{
    return lhs.it_ - rhs.it_;
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_ZC_BUFFER_ITERATOR_H__25E734E5_5CA4_4C8B_91D8_BAE4C2037F52

