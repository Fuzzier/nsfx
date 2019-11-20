/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-26
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CIRCULAR_SEQUENCE_NUMBER_H__4D0E7B48_E09B_44E6_A896_D56EA9C74306
#define CIRCULAR_SEQUENCE_NUMBER_H__4D0E7B48_E09B_44E6_A896_D56EA9C74306


#include <nsfx/utility/config.h>
#include <boost/integer.hpp>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>
#include <type_traits> // conditional
#include <nsfx/utility/endian.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <iostream>


NSFX_OPEN_NAMESPACE


/////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief The traits of cyclic sequence number.
 *
 * Users **shall not** use this class directly.
 * Instead, use `CircularSequenceNumber::TraitsType` to access the traits.
 */
template<size_t bits>
class CircularSequenceNumberTraits/*{{{*/
{
    static_assert(1 <= bits && bits <= 64,
                  "Invalid number of bits for circular sequence number.");

public:
    typedef typename boost::uint_t<bits>::least  ValueType;

    // 2^bits - 1
    BOOST_STATIC_CONSTANT(ValueType, MAX_VALUE =
        static_cast<ValueType>(-1) >> (sizeof (ValueType) * 8 - bits)
    );

    // 2^(bits-1) - 1
    BOOST_STATIC_CONSTANT(ValueType, GAP_VALUE = MAX_VALUE / 2);

};/*}}}*/


/////////////////////////////////////////////////////////////////////////////////
namespace details/*{{{*/
{

template<size_t bits>
struct circular_sequence_number_bits_tag {};

////////////////////////////////////////
// Increment.
template<size_t bits>
inline typename CircularSequenceNumberTraits<bits>::ValueType
circular_sequence_number_inc(
    typename CircularSequenceNumberTraits<bits>::ValueType sn)
{
    if (sn < CircularSequenceNumberTraits<bits>::MAX_VALUE)
    {
        ++sn;
    }
    else
    {
        sn = 0;
    }
    return sn;
}

////////////////////////////////////////
// Less than.
template<size_t bits>
inline bool
circular_sequence_number_less_than(
    typename CircularSequenceNumberTraits<bits>::ValueType lhs,
    typename CircularSequenceNumberTraits<bits>::ValueType rhs,
    circular_sequence_number_bits_tag</*bits*/1>)
{
    return lhs != rhs;
}

template<size_t bits>
inline bool
circular_sequence_number_less_than(
    typename CircularSequenceNumberTraits<bits>::ValueType lhs,
    typename CircularSequenceNumberTraits<bits>::ValueType rhs,
    circular_sequence_number_bits_tag<bits/*>1*/>)
{
    bool result = false;
    if (lhs < rhs)
    {
        result = (rhs - lhs) <= CircularSequenceNumberTraits<bits>::GAP_VALUE;
    }
    else
    {
        result = (lhs - rhs) > CircularSequenceNumberTraits<bits>::GAP_VALUE;
    }
    return result;
}

////////////////////////////////////////
// Less equal.
template<size_t bits>
inline bool
circular_sequence_number_less_equal(
    typename CircularSequenceNumberTraits<bits>::ValueType lhs,
    typename CircularSequenceNumberTraits<bits>::ValueType rhs,
    circular_sequence_number_bits_tag</*bits*/1>)
{
    return true;
}

template<size_t bits>
inline bool
circular_sequence_number_less_equal(
    typename CircularSequenceNumberTraits<bits>::ValueType lhs,
    typename CircularSequenceNumberTraits<bits>::ValueType rhs,
    circular_sequence_number_bits_tag<bits/*>1*/>)
{
    bool result = false;
    if (lhs <= rhs)
    {
        result = (rhs - lhs) <= CircularSequenceNumberTraits<bits>::GAP_VALUE;
    }
    else
    {
        result = (lhs - rhs) > CircularSequenceNumberTraits<bits>::GAP_VALUE;
    }
    return result;
}

} // namespace details/*}}}*/


/////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief A circular sequence number.
 *
 * @tparam bits The number of bits. Must be within `[1, 64]`.
 *
 * The type of the underlying value is the smallest unsigned integral type
 * whose bit size is no less than `bits`.
 *
 * @remark When `bits == 1`, then the range of the circular sequence number is
 *         `[0, 1]`, and both `0 < 1` and `1 < 0` are `true`.
 */
template<size_t bits>
class CircularSequenceNumber/*{{{*/
{
public:
    typedef CircularSequenceNumber             ThisType;
    typedef CircularSequenceNumberTraits<bits> TraitsType;
    typedef typename TraitsType::ValueType     ValueType;

public:
    CircularSequenceNumber(void) BOOST_NOEXCEPT :
        value_(0)
    {
    }

    explicit CircularSequenceNumber(ValueType value) :
        value_(value <= TraitsType::MAX_VALUE ? value : TraitsType::MAX_VALUE)
    {
    }

    ValueType GetValue(void) const BOOST_NOEXCEPT
    {
        return value_;
    }

    /**
     * @brief The number bytes that is just enough to hold `bits`.
     *
     * Buffer I/O will read/write just such number of bytes.
     */
    static size_t GetSize(void) BOOST_NOEXCEPT
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        return B::size;
    }

    static size_t GetBitSize(void) BOOST_NOEXCEPT
    {
        return bits;
    }

    // Increment.
public:
    ThisType& operator++(void) BOOST_NOEXCEPT
    {
        value_ = details::circular_sequence_number_inc<bits>(value_);
        return *this;
    }

    ThisType operator++(int) BOOST_NOEXCEPT
    {
        ValueType old = value_;
        value_ = details::circular_sequence_number_inc<bits>(value_);
        return ThisType(old);
    }

    // Comparison.
public:
    bool operator< (const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return details::circular_sequence_number_less_than<bits>(
            value_, rhs.value_,
            details::circular_sequence_number_bits_tag<bits>());
    }

    bool operator<=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return details::circular_sequence_number_less_equal<bits>(
            value_, rhs.value_,
            details::circular_sequence_number_bits_tag<bits>());
    }

    bool operator==(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return value_ == rhs.value_;
    }

    bool operator!=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return value_ != rhs.value_;
    }

    bool operator> (const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return details::circular_sequence_number_less_than<bits>(
            rhs.value_, value_,
            details::circular_sequence_number_bits_tag<bits>());
    }

    bool operator>=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return details::circular_sequence_number_less_equal<bits>(
            rhs.value_, value_,
            details::circular_sequence_number_bits_tag<bits>());
    }

    void swap(ThisType& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            boost::swap(value_, rhs.value_);
        }
    }

    ////////////////////////////////////////
    // Buffer I/O.
public:
    template<bool zcAware>
    void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.Write(bytes_ + B::offset, B::size);
    }

    template<bool zcAware>
    void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.WriteL(bytes_ + B::offset, B::size);
    }

    template<bool zcAware>
    void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.WriteB(bytes_ + B::offset, B::size);
    }

    template<bool readOnly, bool zcAware>
    void Read(BasicBufferIterator<readOnly, zcAware>& it)
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.Read(bytes_ + B::offset, B::size);
    }

    template<bool readOnly, bool zcAware>
    void ReadL(BasicBufferIterator<readOnly, zcAware>& it)
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.ReadL(bytes_ + B::offset, B::size);
    }

    template<bool readOnly, bool zcAware>
    void ReadB(BasicBufferIterator<readOnly, zcAware>& it)
    {
        typedef bits_endian_traits<ValueType, bits>  B;
        it.ReadB(bytes_ + B::offset, B::size);
    }

private:
    union
    {
        ValueType value_;
        uint8_t   bytes_[sizeof (ValueType)];
    };

};/*}}}*/


/////////////////////////////////////////////////////////////////////////////////
// Free functions./*{{{*/
template<size_t bits>
inline size_t
hash_value(const CircularSequenceNumber<bits>& sn) BOOST_NOEXCEPT
{
    typedef typename CircularSequenceNumber<bits>::ValueType  ValueType;
    return boost::hash<ValueType>()(sn.GetValue());
}

template<size_t bits>
inline void
swap(CircularSequenceNumber<bits>& lhs,
     CircularSequenceNumber<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.swap(rhs);
}

template<class Char, class Traits, size_t bits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const CircularSequenceNumber<bits>& sn)
{
    typedef typename CircularSequenceNumber<bits>::ValueType  ValueType;
    typedef typename std::conditional<
                        sizeof (ValueType) <= sizeof (unsigned int),
                        unsigned int, ValueType>::type  U;
    return os << static_cast<U>(sn.GetValue());
}

////////////////////////////////////////
// Buffer I/O.
template<bool zcAware, size_t bits>
void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const CircularSequenceNumber<bits>& sn)
{
    sn.Write(it);
}

template<bool zcAware, size_t bits>
void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const CircularSequenceNumber<bits>& sn)
{
    sn.WriteL(it);
}

template<bool zcAware, size_t bits>
void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const CircularSequenceNumber<bits>& sn)
{
    sn.WriteB(it);
}

template<bool readOnly, bool zcAware, size_t bits>
void Read(BasicBufferIterator<readOnly, zcAware>& it,
           CircularSequenceNumber<bits>* sn)
{
    sn->Read(it);
}

template<bool readOnly, bool zcAware, size_t bits>
void ReadL(BasicBufferIterator<readOnly, zcAware>& it,
           CircularSequenceNumber<bits>* sn)
{
    sn->ReadL(it);
}

template<bool readOnly, bool zcAware, size_t bits>
void ReadB(BasicBufferIterator<readOnly, zcAware>& it,
           CircularSequenceNumber<bits>* sn)
{
    sn->ReadB(it);
}

/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CIRCULAR_SEQUENCE_NUMBER_H__4D0E7B48_E09B_44E6_A896_D56EA9C74306

