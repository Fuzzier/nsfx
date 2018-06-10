/**
 * @file
 *
 * @brief Fixed length address.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef FIXED_LENGTH_ADDRESS_H__932843EC_3203_40F9_B7F7_A40FCCF4B287
#define FIXED_LENGTH_ADDRESS_H__932843EC_3203_40F9_B7F7_A40FCCF4B287


#include <nsfx/utility/config.h>
#include <nsfx/utility/least-int.h>
#include <boost/type_traits/is_copy_constructible.hpp>
#include <boost/type_traits/is_copy_assignable.hpp>
#include <boost/type_traits/has_equal_to.hpp>
#include <boost/type_traits/has_not_equal_to.hpp>
#include <boost/type_traits/has_less.hpp>
#include <boost/type_traits/has_less_equal.hpp>
#include <boost/type_traits/has_greater.hpp>
#include <boost/type_traits/has_greater_equal.hpp>
#include <boost/type_traits/has_pre_increment.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_pre_decrement.hpp>
#include <boost/type_traits/has_post_decrement.hpp>
#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/type_traits/has_minus.hpp>
#include <boost/type_traits/has_minus_assign.hpp>
#include <boost/functional/hash.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
/**
 * @ingroup Network
 * @brief An address.
 * @tparam bits  Length in bits.
 *
 */
template<size_t bits>
class FixedLengthAddress
{
    static_assert(bits >= 1 && bits <= 64,
                  "Invalid number of bits for FixedLengthAddress.");

public:
    /**
     * @brief The type of underlying value.
     */
    typedef typename LeastInt<bits>::UintType   ValueType;

    /**
     * @brief The type of signed integer used in operators.
     */
    typedef typename LeastInt<bits>::IntOpType  IntOpType;

    /**
     * @brief The type of unsigned integer used in operators.
     */
    typedef typename LeastInt<bits>::UintOpType UintOpType;

private:
    static const ValueType MASK =
        static_cast<ValueType>(
            static_cast<ValueType>(-1) >>
                (sizeof (ValueType) * 8 - bits));

    // LengthTag
private:
    struct FullLengthTag {};
    struct PartialLengthTag {};

    template<bool full>
    struct MakeLengthTag
    {
        typedef FullLengthTag  Type;
    };
    template<>
    struct MakeLengthTag<false>
    {
        typedef PartialLengthTag  Type;
    };

    typedef typename MakeLengthTag<bits == sizeof (ValueType)>::Type
                        MyLengthTag;

    // Xtructors
public:
    /**
     * @brief Construct an uninitialized address.
     */
    FixedLengthAddress(void) BOOST_NOEXCEPT {}

    BOOST_CONSTEXPR FixedLengthAddress(ValueType value) BOOST_NOEXCEPT :
        value_(MakeValue(value))
    {}

private:
    BOOST_CONSTEXPR FixedLengthAddress(ValueType value, FullLengthTag) BOOST_NOEXCEPT :
        value_(value)
    {}

    // Copyable
public:
    BOOST_CONSTEXPR FixedLengthAddress(const FixedLengthAddress& rhs) BOOST_NOEXCEPT :
        value_(rhs.value_)
    {}

    BOOST_CONSTEXPR FixedLengthAddress& operator=(const FixedLengthAddress& rhs) BOOST_NOEXCEPT
    {
        value_ = rhs.value_;
        return *this;
    }

    // Operators.
public:
    bool operator==(const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ == rhs.value_;
    }

    bool operator!=(const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ != rhs.value_;
    }

    bool operator< (const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ < rhs.value_;
    }

    bool operator<=(const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ <= rhs.value_;
    }

    bool operator> (const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ > rhs.value_;
    }

    bool operator>=(const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ >= rhs.value_;
    }

    BOOST_CONSTEXPR FixedLengthAddress& operator++(void) BOOST_NOEXCEPT
    {
        Increment();
        return *this;
    }

    BOOST_CONSTEXPR FixedLengthAddress operator++(int) BOOST_NOEXCEPT
    {
        ValueType result = value_;
        Increment();
        return FixedLengthAddress(result, FullLengthTag());
    }

    BOOST_CONSTEXPR FixedLengthAddress& operator--(void) BOOST_NOEXCEPT
    {
        Decrement();
        return *this;
    }

    BOOST_CONSTEXPR FixedLengthAddress operator--(int) BOOST_NOEXCEPT
    {
        ValueType result = value_;
        Decrement();
        return FixedLengthAddress(result, FullLengthTag());
    }

    BOOST_CONSTEXPR FixedLengthAddress operator+(IntOpType n) const BOOST_NOEXCEPT
    {
        return FixedLengthAddress(MakeValue(value_ + n));
    }

    BOOST_CONSTEXPR FixedLengthAddress& operator+=(IntOpType n) BOOST_NOEXCEPT
    {
        value_ = MakeValue(value_ + n);
        return *this;
    }

    BOOST_CONSTEXPR FixedLengthAddress operator-(IntOpType n) const BOOST_NOEXCEPT
    {
        return FixedLengthAddress(MakeValue(value_ - n));
    }

    BOOST_CONSTEXPR FixedLengthAddress operator-=(IntOpType n) BOOST_NOEXCEPT
    {
        value_ = MakeValue(value_ - n);
        return *this;
    }

    BOOST_CONSTEXPR IntOpType operator-(const FixedLengthAddress& rhs) const BOOST_NOEXCEPT
    {
        return value_ - rhs.value_;
    }

public:
    // Hashable
    size_t hash_value(void) const BOOST_NOEXCEPT
    {
        return boost::hash_value<ValueType>(value_);
    }

public:
    // Swappable
    void swap(FixedLengthAddress& rhs) BOOST_NOEXCEPT
    {
        boost::swap(value_, rhs.value_);
    }

public:
    BOOST_CONSTEXPR ValueType GetValue(void) const BOOST_NOEXCEPT
    {
        return value_;
    }

    static BOOST_CONSTEXPR FixedLengthAddress Zero(void) BOOST_NOEXCEPT
    {
        return FixedLengthAddress(0);
    }

    static BOOST_CONSTEXPR FixedLengthAddress Bcast(void) BOOST_NOEXCEPT
    {
        return FixedLengthAddress(MASK);
    }

private:
    static BOOST_CONSTEXPR ValueType
    MakeValue(ValueType value) BOOST_NOEXCEPT
    {
        return MakeValue(value, MyLengthTag());
    }

    static BOOST_CONSTEXPR ValueType
    MakeValue(ValueType value, FullLengthTag) BOOST_NOEXCEPT
    {
        return value;
    }

    static BOOST_CONSTEXPR ValueType
    MakeValue(ValueType value, PartialLengthTag) BOOST_NOEXCEPT
    {
        return value & MASK;
    }

    // operator++()
    void Increment(void) BOOST_NOEXCEPT
    {
        Increment(MyLengthTag());
    }

    void Increment(FullLengthTag) BOOST_NOEXCEPT
    {
        ++value_;
    }

    void Increment(PartialLengthTag) BOOST_NOEXCEPT
    {
        if (++value_ > MASK)
        {
            value_ = 0;
        }
    }

    // operator--()
    void Decrement(void) BOOST_NOEXCEPT
    {
        Decrement(MyLengthTag());
    }

    void Decrement(FullLengthTag) BOOST_NOEXCEPT
    {
        --value_;
    }

    void Decrement(PartialLengthTag) BOOST_NOEXCEPT
    {
        if (--value_ > MASK)
        {
            value_ = MASK;
        }
    }

private:
    ValueType  value_;
};


////////////////////////////////////////////////////////////////////////////////
template<class FixedLengthAddress>
class FixedLengthAddressConcept;

template<size_t bits>
class FixedLengthAddressConcept<FixedLengthAddress<bits> >
{
    typedef FixedLengthAddress<bits>  Type;

    static_assert(boost::is_copy_constructible<Type>::value, "");
    static_assert(boost::is_copy_assignable<Type>::value, "");
    static_assert(boost::has_equal_to<Type>::value, "");
    static_assert(boost::has_not_equal_to<Type>::value, "");
    static_assert(boost::has_less<Type>::value, "");
    static_assert(boost::has_less_equal<Type>::value, "");
    static_assert(boost::has_greater<Type>::value, "");
    static_assert(boost::has_greater_equal<Type>::value, "");

public:
    BOOST_CONCEPT_USAGE(FixedLengthAddressConcept)
    {
        IsDefaultConstructible();
        Algorithms();
        Hashable();
        Swappable();
    }

    void IsDefaultConstructible(void)
    {
        Type address;
    }

    void Algorithms(void)
    {
        Type* address = nullptr;

        ++(*address);
        (*address)++;

        --(*address);
        (*address)--;

        *address += 1;
        *address -= 1;

        *address = *address + 1;
        *address = 1 + *address;
        *address = *address - 1;

        ptrdiff_t n = *address - *address;
    }

    void Hashable(void)
    {
        const Type* address = nullptr;
        size_t hash = boost::hash<Type>()(*address);
    }

    void Swappable(void)
    {
        Type* a = nullptr;
        Type* b = nullptr;
        boost::swap(*a, *b);
    }

};


////////////////////////////////////////////////////////////////////////////////
template<size_t bits>
inline bool operator==(const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.oprator==(rhs);
}

template<size_t bits>
inline bool operator!=(const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator!=(rhs);
}

template<size_t bits>
inline bool operator< (const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator<(rhs);
}

template<size_t bits>
inline bool operator<=(const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator<=(rhs);
}

template<size_t bits>
inline bool operator> (const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator>(rhs);
}

template<size_t bits>
inline bool operator>=(const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator>=(rhs);
}

template<size_t bits>
inline FixedLengthAddress<bits> operator+(
    const FixedLengthAddress<bits>& lhs,
    typename FixedLengthAddress<bits>::IntOpType n) BOOST_NOEXCEPT
{
    return lhs.operator+(n);
}

template<size_t bits>
inline FixedLengthAddress<bits> operator+(
    typename FixedLengthAddress<bits>::IntOpType n,
    const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return rhs.operator+(n);
}

template<size_t bits>
inline FixedLengthAddress<bits> operator-(
    const FixedLengthAddress<bits>& lhs,
    typename FixedLengthAddress<bits>::IntOpType n) BOOST_NOEXCEPT
{
    return lhs.operator-(n);
}

template<size_t bits>
inline typename FixedLengthAddress<bits>::IntOpType
operator-(const FixedLengthAddress<bits>& lhs, const FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    return lhs.operator-(rhs);
}

template<size_t bits>
inline size_t hash_value(const FixedLengthAddress<bits>& address) BOOST_NOEXCEPT
{
    return address.hash_value();
}

template<size_t bits>
inline void swap(FixedLengthAddress<bits>& lhs, FixedLengthAddress<bits>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


////////////////////////////////////////////////////////////////////////////////
template<size_t bits, class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, const FixedLengthAddress<bits>& address)
{
    typedef typename FixedLengthAddress<bits>::UintOpType  UintOpType;
    return os << static_cast<UintOpType>(address.GetValue());
}


NSFX_CLOSE_NAMESPACE


#endif // FIXED_LENGTH_ADDRESS_H__932843EC_3203_40F9_B7F7_A40FCCF4B287

