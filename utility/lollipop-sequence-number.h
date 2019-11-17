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

    // Maximum number of the circular part, `2^bits - 1`.
    BOOST_STATIC_CONSTANT(ValueType, MAX_VALUE =
        static_cast<ValueType>(-1) >> (sizeof (ValueType) * 8 - bits));

    // First number of the circular part.
    BOOST_STATIC_CONSTANT(ValueType, START_VALUE =
                          static_cast<ValueType>(start));

    // Gap of the circular part, no more than half of the circular number space.
    BOOST_STATIC_CONSTANT(ValueType, GAP_VALUE = (MAX_VALUE - start) / 2);

};/*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief A lollipop sequence number.
 *
 * @tparam bits The number of bits. Must be within `[1, 64]`.
 * @tparam start The first number of the circular part.
 *               It **must** be within `[0, N-1]`, where `N = 2^bits`.
 *
 * The range of lollipop sequence number is `[0, N-1]`.
 * The linear part is `[0, start-1]`, and the circular part is `[start, N-1]`.
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
private:
    void InternalIncrement(void) BOOST_NOEXCEPT
    {
        if (value_ == TraitsType::MAX_VALUE)
        {
            value_ = TraitsType::START_VALUE;
        }
        else
        {
            ++value_;
        }
    }

public:
    ThisType& operator++(void) BOOST_NOEXCEPT
    {
        InternalIncrement();
        return *this;
    }

    ThisType operator++(int) BOOST_NOEXCEPT
    {
        ValueType old = value_;
        InternalIncrement();
        return ThisType(old);
    }

    // Comparison.
private:
    // Space of circular part.
    struct OneNumberTag  {};
    struct TwoNumbersTag   {};
    struct MoreNumbersTag {};
    template<ValueType space> struct SpaceTag : MoreNumbersTag {};
    template<> struct SpaceTag<1> : TwoNumbersTag {};
    template<> struct SpaceTag<0> : OneNumberTag {};
    typedef SpaceTag<TraitsType::MAX_VALUE - TraitsType::START_VALUE>
            MySpaceTag;

    // The circular part has only one number, that is `start`.
    static bool InternalLessThan(ValueType lhs, ValueType rhs, OneNumberTag) BOOST_NOEXCEPT
    {
        return lhs < rhs;
    }

    static bool InternalLessEqual(ValueType lhs, ValueType rhs, OneNumberTag) BOOST_NOEXCEPT
    {
        return lhs <= rhs;
    }

    // The circular part has only two numbers, that is `start` and `start+1`.
    static bool InternalLessThan(ValueType lhs, ValueType rhs, TwoNumbersTag) BOOST_NOEXCEPT
    {
        bool result = true;
        if (rhs < TraitsType::START_VALUE)
        {
            result = lhs < rhs;
        }
        return result;
    }

    static bool InternalLessEqual(ValueType lhs, ValueType rhs, TwoNumbersTag) BOOST_NOEXCEPT
    {
        bool result = true;
        if (rhs < TraitsType::START_VALUE)
        {
            result = lhs <= rhs;
        }
        return result;
    }

    // The circular part has more than two numbers.
    static bool InternalLessThan(ValueType lhs, ValueType rhs, MoreNumbersTag) BOOST_NOEXCEPT
    {
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

    static bool InternalLessEqual(ValueType lhs, ValueType rhs, MoreNumbersTag) BOOST_NOEXCEPT
    {
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

public:
    bool operator< (const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return InternalLessThan(value_, rhs.value_, MySpaceTag());
    }

    bool operator<=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return InternalLessEqual(value_, rhs.value_, MySpaceTag());
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
        return InternalLessThan(rhs.value_, value_, MySpaceTag());
    }

    bool operator>=(const ThisType& rhs) const BOOST_NOEXCEPT
    {
        return InternalLessEqual(rhs.value_, value_, MySpaceTag());
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

template<class Char, class Traits, size_t bits, typename boost::uint_t<bits>::least start>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const LollipopSequenceNumber<bits, start>& sn)
{
    typedef LollipopSequenceNumber<bits, start>::ValueType  ValueType;
    typedef std::conditional<sizeof (ValueType) <= sizeof (unsigned int),
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

