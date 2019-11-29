/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-16
 *
 * @copyright Copyright (c) 2019.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOLLIPOP_SEQUENCE_NUMBER_H__092CC9EE_D2A1_4037_AFB4_6E950BFDA676
#define LOLLIPOP_SEQUENCE_NUMBER_H__092CC9EE_D2A1_4037_AFB4_6E950BFDA676


#include <nsfx/utility/config.h>
#include <boost/integer.hpp>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>
#include <type_traits> // is_unsigned, is_integral, make_unsigned, conditional
#include <nsfx/utility/endian.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <iostream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief The traits of lollipop sequence number.
 *
 * Users **shall not** use this class directly.
 * Instead, use `LollipopSequenceNumber::TraitsType` to access the traits.
 */
template<size_t bits, typename boost::uint_t<bits>::least start>
class LollipopSequenceNumberTraits/*{{{*/
{
public:
    typedef typename boost::uint_t<bits>::least  ValueType;

    // Maximum number of the lollipop part, `2^bits - 1`.
    BOOST_STATIC_CONSTANT(ValueType, MAX_VALUE =
        static_cast<ValueType>(-1) >> (sizeof (ValueType) * 8 - bits));

    // First number of the lollipop part.
    BOOST_STATIC_CONSTANT(ValueType, START_VALUE =
                          static_cast<ValueType>(start));

    // Gap of the lollipop part, no more than half of the lollipop number space.
    BOOST_STATIC_CONSTANT(ValueType, GAP_VALUE = (MAX_VALUE - start) / 2);

    // 1: only one number.
    // 2: two numbers.
    // 3: three or more numbers.
    BOOST_STATIC_CONSTANT(size_t, CIRCULAR_SPACE = (
        MAX_VALUE == START_VALUE     ? 1 :
        MAX_VALUE == START_VALUE + 1 ? 2 : 3));
};/*}}}*/


////////////////////////////////////////////////////////////////////////////////
namespace detail/*{{{*/
{

template<size_t n> // n = CICURLAR_SPACE
struct lollipop_sequence_number_circular_space_tag {};


////////////////////////////////////////
// Increment.
template<size_t bits, typename boost::uint_t<bits>::least start>
inline typename LollipopSequenceNumberTraits<bits, start>::ValueType
lollipop_sequence_number_inc(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType sn)
{
    typedef LollipopSequenceNumberTraits<bits, start>  TraitsType;
    if (sn < TraitsType::MAX_VALUE)
    {
        ++sn;
    }
    else
    {
        sn = TraitsType::START_VALUE;
    }
    return sn;
}

////////////////////////////////////////
// Less than.
template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_than(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/1>)
{
    // The circular part has only one number, that is `start`.
    return lhs < rhs;
}

template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_than(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/2>)
{
    // The circular part has only two numbers, that is `start` and `start+1`.
    typedef LollipopSequenceNumberTraits<bits, start>  TraitsType;
    bool result = true;
    if (rhs < TraitsType::START_VALUE)
    {
        result = lhs < rhs;
    }
    return result;
}

template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_than(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/3>)
{
    // The circular part has three or more numbers.
    typedef LollipopSequenceNumberTraits<bits, start>  TraitsType;
    bool result = false;
    if (lhs < TraitsType::START_VALUE)
    {
        result = lhs < rhs;
    }
    else if (lhs < rhs)
    {
        result = (rhs - lhs) <= TraitsType::GAP_VALUE;
    }
    else if (rhs >= TraitsType::START_VALUE)
    {
        result = (lhs - rhs) > TraitsType::GAP_VALUE;
    }
    return result;
}

////////////////////////////////////////
// Less equal.
template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_equal(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/1>)
{
    // The circular part has only one number, that is `start`.
    return lhs <= rhs;
}

template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_equal(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/2>)
{
    // The circular part has only two numbers, that is `start` and `start+1`.
    typedef LollipopSequenceNumberTraits<bits, start>  TraitsType;
    bool result = true;
    if (rhs < TraitsType::START_VALUE)
    {
        result = lhs <= rhs;
    }
    return result;
}

template<size_t bits, typename boost::uint_t<bits>::least start>
inline bool
lollipop_sequence_number_less_equal(
    typename LollipopSequenceNumberTraits<bits, start>::ValueType lhs,
    typename LollipopSequenceNumberTraits<bits, start>::ValueType rhs,
    lollipop_sequence_number_circular_space_tag</*n*/3>)
{
    // The circular part has three or more numbers.
    typedef LollipopSequenceNumberTraits<bits, start>  TraitsType;
    bool result = false;
    if (lhs < TraitsType::START_VALUE)
    {
        result = lhs <= rhs;
    }
    else if (lhs <= rhs)
    {
        result = (rhs - lhs) <= TraitsType::GAP_VALUE;
    }
    else if (rhs >= TraitsType::START_VALUE)
    {
        result = (lhs - rhs) > TraitsType::GAP_VALUE;
    }
    return result;
}

} // namespace detail
/*}}}*/

////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief A lollipop sequence number.
 *
 * @tparam bits The number of bits. Must be within `[1, 64]`.
 * @tparam start The first number of the lollipop part.
 *               It **must** be within `[0, N-1]`, where `N = 2^bits`.
 *
 * The range of lollipop sequence number is `[0, N-1]`.
 * The linear part is `[0, start-1]`, and the lollipop part is `[start, N-1]`.
 *
 * The default value of `start` is `N/2 = 2^(bits-1)`.
 *
 * A lollipop sequence number is represented as an *unsigned* integral number.
 */
template<size_t bits, typename boost::uint_t<bits>::least start =
        (static_cast<typename boost::uint_t<bits>::least>(-1) >>
           (sizeof (typename boost::uint_t<bits>::least)*8-bits))/2 + 1>
class LollipopSequenceNumber/*{{{*/
{
public:
    typedef LollipopSequenceNumber                    ThisType;
    typedef LollipopSequenceNumberTraits<bits, start> TraitsType;
    typedef typename TraitsType::ValueType            ValueType;

public:
    LollipopSequenceNumber(void) BOOST_NOEXCEPT :
        value_(0)
    {
    }

    explicit LollipopSequenceNumber(ValueType value) :
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
        value_ = detail::lollipop_sequence_number_inc<bits, start>(value_);
        return *this;
    }

    ThisType operator++(int) BOOST_NOEXCEPT
    {
        ValueType old = value_;
        value_ = detail::lollipop_sequence_number_inc<bits, start>(value_);
        return ThisType(old);
    }

    // Comparison.
public:
    bool operator< (const ThisType& rhs) const BOOST_NOEXCEPT
    {
        typedef detail::lollipop_sequence_number_circular_space_tag<
                    TraitsType::CIRCULAR_SPACE> tag;
        return detail::lollipop_sequence_number_less_than<bits, start>(
                value_, rhs.value_, tag());
    }

    bool operator<=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        typedef detail::lollipop_sequence_number_circular_space_tag<
                    TraitsType::CIRCULAR_SPACE> tag;
        return detail::lollipop_sequence_number_less_equal<bits, start>(
                value_, rhs.value_, tag());
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
        typedef detail::lollipop_sequence_number_circular_space_tag<
                    TraitsType::CIRCULAR_SPACE> tag;
        return detail::lollipop_sequence_number_less_than<bits, start>(
                rhs.value_, value_, tag());
    }

    bool operator>=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        typedef detail::lollipop_sequence_number_circular_space_tag<
                    TraitsType::CIRCULAR_SPACE> tag;
        return detail::lollipop_sequence_number_less_equal<bits, start>(
                rhs.value_, value_, tag());
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


////////////////////////////////////////////////////////////////////////////////
// Free functions./*{{{*/
template<size_t bits, typename boost::uint_t<bits>::least start>
inline size_t
hash_value(const LollipopSequenceNumber<bits, start>& sn) BOOST_NOEXCEPT
{
    typedef typename LollipopSequenceNumber<bits, start>::ValueType  ValueType;
    return boost::hash<ValueType>()(sn.GetValue());
}

template<size_t bits, typename boost::uint_t<bits>::least start>
inline void
swap(LollipopSequenceNumber<bits, start>& lhs,
     LollipopSequenceNumber<bits, start>& rhs) BOOST_NOEXCEPT
{
    return lhs.swap(rhs);
}

template<class Char, class Traits,
         size_t bits, typename boost::uint_t<bits>::least start>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const LollipopSequenceNumber<bits, start>& sn)
{
    typedef typename LollipopSequenceNumber<bits, start>::ValueType  ValueType;
    typedef typename std::conditional<
                sizeof (ValueType) <= sizeof (unsigned int),
                unsigned int, ValueType>::type  U;
    return os << static_cast<U>(sn.GetValue());
}

////////////////////////////////////////
// Buffer I/O.
template<bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const LollipopSequenceNumber<bits, start>& sn)
{
    sn.Write(it);
}

template<bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const LollipopSequenceNumber<bits, start>& sn)
{
    sn.WriteL(it);
}

template<bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const LollipopSequenceNumber<bits, start>& sn)
{
    sn.WriteB(it);
}

template<bool readOnly, bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void Read(BasicBufferIterator<readOnly, zcAware>& it,
           LollipopSequenceNumber<bits, start>* sn)
{
    sn->Read(it);
}

template<bool readOnly, bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void ReadL(BasicBufferIterator<readOnly, zcAware>& it,
           LollipopSequenceNumber<bits, start>* sn)
{
    sn->ReadL(it);
}

template<bool readOnly, bool zcAware, size_t bits, typename boost::uint_t<bits>::least start>
void ReadB(BasicBufferIterator<readOnly, zcAware>& it,
           LollipopSequenceNumber<bits, start>* sn)
{
    sn->ReadB(it);
}

/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // LOLLIPOP_SEQUENCE_NUMBER_H__092CC9EE_D2A1_4037_AFB4_6E950BFDA676

