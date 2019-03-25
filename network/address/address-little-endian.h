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

#ifndef ADDRESS_LITTLE_ENDIAN_H__F787C62F_D1FF_41EF_9E2C_8DF94C98A995
#define ADDRESS_LITTLE_ENDIAN_H__F787C62F_D1FF_41EF_9E2C_8DF94C98A995


#include <nsfx/network/config.h>
#include <nsfx/utility/endian.h>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <type_traits> // conditional, is_integral, is_signed
#include <sstream>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
namespace address {
namespace aux {


////////////////////////////////////////
//                 Value[N]
//  1~32  bits: uint32_t[1]
// 33~64  bits: uint64_t[1]
// 65~96  bits: uint32_t[3]
// 96~128 bits: uint32_t[4]


////////////////////////////////////////
// N32: the number of uint32_t's required to hold the bits.
template<size_t bits, size_t N32 = (bits+31)/32>
struct LengthTraits
{
    // The type of operating value.
    typedef uint32_t  Value;
    typedef uint64_t  DoubleValue;

    // The number of bits of a value.
    BOOST_STATIC_CONSTANT(size_t, V_BITS = 8 * sizeof (Value));

    // The number of bytes to hold the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_BYTES = (bits + 7) / 8);
    // The mask of most significant byte.
    BOOST_STATIC_CONSTANT(uint8_t, MSB_MASK = 0xff >> (8 * NUM_BYTES - bits));

    // The number of limbs to holds the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_VALUES = (bits + 31) / 32);
    // The mask of value (with all bits set to 1).
    BOOST_STATIC_CONSTANT(Value, V_MASK = (Value)(-1));
    // The mask of most significant value.
    BOOST_STATIC_CONSTANT(Value,
        MSV_MASK = V_MASK >> (8 * sizeof (Value) * NUM_VALUES - bits));
};


template<size_t bits>
struct LengthTraits<bits, /* N32 = */1>
{
    // The type of operating value.
    typedef uint32_t  Value;
    typedef void      DoubleValue; // Unused, since a single limb is enough.

    // The number of bits of a value.
    BOOST_STATIC_CONSTANT(size_t, V_BITS = 8 * sizeof (Value));

    // The number of bytes to hold the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_BYTES = (bits + 7) / 8);
    // The mask of most significant byte.
    BOOST_STATIC_CONSTANT(uint8_t, MSB_MASK = 0xff >> (8 * NUM_BYTES - bits));

    // The number of values to holds the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_VALUES = (bits + 31) / 32);
    // The mask of value (with all bits set to 1).
    BOOST_STATIC_CONSTANT(Value, V_MASK = (Value)(-1));
    // The mask of most significant value.
    BOOST_STATIC_CONSTANT(Value,
        MSV_MASK = V_MASK >> (8 * sizeof (Value) * NUM_VALUES - bits));
};


template<size_t bits>
struct LengthTraits<bits, /* N32 = */2>
{
    // The type of operating value.
    typedef uint64_t  Value;
    typedef void      DoubleValue; // Unused, since a single limb is enough.

    // The number of bits of a value.
    BOOST_STATIC_CONSTANT(size_t, V_BITS = 8 * sizeof (Value));

    // The number of bytes to hold the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_BYTES = (bits + 7) / 8);
    // The mask of most significant byte.
    BOOST_STATIC_CONSTANT(uint8_t, MSB_MASK = 0xff >> (8 * NUM_BYTES - bits));

    // The number of values to holds the bits.
    BOOST_STATIC_CONSTANT(size_t, NUM_VALUES = (bits + 63) / 64);
    // The mask of value (with all bits set to 1).
    BOOST_STATIC_CONSTANT(Value, V_MASK = (Value)(-1));
    // The mask of most significant value.
    BOOST_STATIC_CONSTANT(Value,
        MSV_MASK = V_MASK >> (8 * sizeof (Value) * NUM_VALUES - bits));
};


} // namespace aux
} // namespace address


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief An address with a fixed bit length.
 *
 * @tparam bits  The number of bits.
 *
 * A network address is usually no larger than \c 128 bits.
 *
 * For efficiency, when the address is no larger than \c 32 bits,
 * a single \c uint32_t is used to implement an address.
 *
 * When the address is no larger than \c 64 bits,
 * a single \c uint64_t is used to implement an address.
 *
 * When the address is larger than \c 64 bits,
 * an array of \c uint32_t is used to implement an address.
 */
template<size_t bits>
class Address
{
private:
    static_assert(bits > 0,
                  "Invalid number of bits, which must be positive.");

    static_assert(endian_traits<native_endian_t>::is_little_endian,
                  "Cannot use the little-endian version of Address, "
                  "since the system is not in little-endian.");

    ////////////////////////////////////////
    typedef address::aux::LengthTraits<bits>   LengthTraits;
    typedef typename LengthTraits::Value       Value;
    typedef typename LengthTraits::DoubleValue DoubleValue;

    // The number of bits of a value.
    BOOST_STATIC_CONSTANT(size_t,  V_BITS   = LengthTraits::V_BITS);

    // The mask of value (with all bits set to 1).
    BOOST_STATIC_CONSTANT(Value,   V_MASK   = LengthTraits::V_MASK);
    // The mask of most significant value.
    BOOST_STATIC_CONSTANT(Value,   MSV_MASK = LengthTraits::MSV_MASK);
    // The number of values to holds the bits.
    BOOST_STATIC_CONSTANT(size_t,  NV       = LengthTraits::NUM_VALUES);

    // The mask of most significant byte.
    BOOST_STATIC_CONSTANT(uint8_t, MSB_MASK = LengthTraits::MSB_MASK);
    // The number of bytes to holds the bits.
    BOOST_STATIC_CONSTANT(size_t,  NB       = LengthTraits::NUM_BYTES);
    // The number of bytes of the data.
    BOOST_STATIC_CONSTANT(size_t,  DB       = NV * sizeof (Value));

    static_assert(NV >= 1, "Invalid number of values.");

    ////////////////////////////////////////
    struct single_value_t    {};
    struct multiple_values_t {};

    // value_multiplicity_t: single_value_t or multiple_values_t.
    typedef typename
        std::conditional<NV == 1, single_value_t, multiple_values_t>::type
            value_multiplicity_t;

    ////////////////////////////////////////
    // endian_t: big_endian_t or little_endian_t.
    typedef typename endian_traits<native_endian_t>::endian_t  endian_t;

    ////////////////////////////////////////
    struct signed_t   {};
    struct unsigned_t {};

    ////////////////////////////////////////
    template<size_t n>
    struct num_elements_t {};

public:
    BOOST_STATIC_CONSTANT(size_t, NUM_BITS = bits);
    // The number of bytes to holds the address.
    BOOST_STATIC_CONSTANT(size_t, NUM_BYTES = NB);

public:
    BOOST_CONSTEXPR Address(void) BOOST_NOEXCEPT
    {
        Assign0();
    }

    ////////////////////////////////////////
    /**
     * @brief Construct an address via an array of bytes in little-endian.
     *
     * @param[in] a  An array of bytes.
     */
    template<size_t M>
    BOOST_CONSTEXPR Address(const uint8_t (&a)[M], little_endian_t) BOOST_NOEXCEPT
    {
        AssignBytes(a, little_endian_t());
    }

    /**
     * @brief Construct an address via an array of bytes in big-endian.
     *
     * @param[in] a  An array of bytes.
     */
    template<size_t M>
    BOOST_CONSTEXPR Address(const uint8_t (&a)[M], big_endian_t) BOOST_NOEXCEPT
    {
        AssignBytes(a, big_endian_t());
    }

    ////////////////////////////////////////
    /**
     * @brief Construct an address via a sequence of bytes in little-endian.
     *
     * @param[in] bytes  A sequence of bytes.
     *                   It <b>must not</b> be \c nullptr.
     * @param[in] size   The number of bytes in the sequence.
     */
    Address(const uint8_t* bytes, size_t size, little_endian_t) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(bytes);
        AssignBytes(bytes, size, little_endian_t());
    }

    /**
     * @brief Construct an address via a sequence of bytes in big-endian.
     *
     * @param[in] bytes  A sequence of bytes.
     *                   It <b>must not</b> be \c nullptr.
     * @param[in] size   The number of bytes in the sequence.
     */
    Address(const uint8_t* bytes, size_t size, big_endian_t) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(bytes);
        AssignBytes(bytes, size, big_endian_t());
    }

    ////////////////////////////////////////
    /**
     * @brief Construct an address via a value.
     *
     * @tparam T  The type of value.
     *            It <b>must</b> be an integral type.
     * @param[in] v  An integral value.
     *               For a \c signed \c negative value, the sign bit is
     *               extended til the most significant bit.
     */
    template<class T>
    BOOST_CONSTEXPR Address(const T& v) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        AssignValue(v);
    }

    ////////////////////////////////////////
    /**
     * @brief Construct an address via a sequence of values in little-endian.
     *
     * @tparam T  The type of values.
     *            It <b>must</b> be an integral type.
     *
     * @param[in] a  An array of values.
     *               All elements are treated as \c unsigned values.
     * @param[in] little_endian
     */
    template<class T, size_t M>
    BOOST_CONSTEXPR Address(const T (&a)[M], typename
        std::enable_if<std::is_integral<T>::value, little_endian_t>::type) BOOST_NOEXCEPT
    {
        AssignValues(a, little_endian_t());
    }

    /**
     * @brief Construct an address via a sequence of values in big-endian.
     *
     * @tparam T  The type of values.
     *            It <b>must</b> be an integral type.
     *
     * @param[in] a  An array of values.
     *               All elements are treated as \c unsigned values.
     * @param[in] big_endian
     */
    template<class T, size_t M>
    BOOST_CONSTEXPR Address(const T (&a)[M], typename
        std::enable_if<std::is_integral<T>::value, big_endian_t>::type) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        AssignValues(a, big_endian_t());
    }

    /**
     * @brief Construct an address via a sequence of values in little-endian.
     *
     * @tparam T  The type of values.
     *            It <b>must</b> be an integral type.
     *
     * @param[in] values  An array of values.
     *                    All elements are treated as \c unsigned values.
     *                    It <b>must not</b> be \c nullptr.
     * @param[in] count   The number of values in the sequence.
     * @param[in] little_endian
     */
    template<class T>
    BOOST_CONSTEXPR Address(const T* values, size_t count, typename
       std::enable_if<std::is_integral<T>::value, little_endian_t>::type) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        BOOST_ASSERT(values);
        AssignValues(values, count, little_endian_t());
    }

    /**
     * @brief Construct an address via a sequence of values in big-endian.
     *
     * @tparam T  The type of values.
     *            It <b>must</b> be an integral type.
     *
     * @param[in] values  An array of values.
     *                    All elements are treated as \c unsigned values.
     *                    It <b>must not</b> be \c nullptr.
     * @param[in] count   The number of values in the sequence.
     * @param[in] big_endian
     */
    template<class T>
    BOOST_CONSTEXPR Address(const T* values, size_t count, typename
       std::enable_if<std::is_integral<T>::value, big_endian_t>::type) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        BOOST_ASSERT(values);
        AssignValues(values, count, big_endian_t());
    }

    ////////////////////////////////////////
#if 0 && !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    /**
     * @brief Construct an address via a pack of values in little-endian.
     *
     * @tparam T The type of the values.
     *           It <b>must</b> be an integral type.
     *           All values are treated as \c unsigned values.
     * @tparam U The types of the remaining values.
     *           Each <b>must</b> be an integral type.
     *           Each <b>must</b> have the same size as \c T.
     *
     * @param[in] v0    The first value.
     * @param[in] v1... The remaining values (optional).
     * @param[in] little_endian
     */
    template<class T, class... U>
    BOOST_CONSTEXPR Address(T v0, U... v1, typename
       std::enable_if<std::is_integral<T>::value, little_endian_t>::type) BOOST_NOEXCEPT
    {
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must be integral types.");
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must have the same size as T.");
        AssignValues(v0, v1..., little_endian_t());
    }

    /**
     * @brief Construct an address via a pack of values in big-endian.
     *
     * @tparam T The type of the values.
     *           It <b>must</b> be an integral type.
     *           All values are treated as \c unsigned values.
     * @tparam U The types of the remaining values.
     *           Each <b>must</b> be an integral type.
     *           Each <b>must</b> have the same size as \c T.
     *
     * @param[in] v0    The first value.
     * @param[in] v1... The remaining values (optional).
     * @param[in] big_endian
     */
    template<class T, class... U>
    BOOST_CONSTEXPR Address(T v0, U... v1, typename
       std::enable_if<std::is_integral<T>::value, big_endian_t>::type) BOOST_NOEXCEPT
    {
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must be integral types.");
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must have the same size as T.");
        AssignValues(v0, v1..., big_endian_t());
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/network/address/address-little-endian.h>, 0))
# include BOOST_PP_ITERATE()
#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ////////////////////////////////////////
    // Copyable.
public:
    BOOST_CONSTEXPR Address(const Address& rhs) BOOST_NOEXCEPT
    {
        CopyAssign(rhs);
    }

    BOOST_CONSTEXPR Address& operator=(const Address& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            CopyAssign(rhs);
        }
        return *this;
    }

    ////////////////////////////////////////
    // Comparison.
public:
    BOOST_CONSTEXPR bool operator==(const Address& rhs) const BOOST_NOEXCEPT
    {
        return Equal(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR bool operator!=(const Address& rhs) const BOOST_NOEXCEPT
    {
        return !Equal(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR bool operator<(const Address& rhs) const BOOST_NOEXCEPT
    {
        return LessThan(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR bool operator<=(const Address& rhs) const BOOST_NOEXCEPT
    {
        return LessEqual(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR bool operator>(const Address& rhs) const BOOST_NOEXCEPT
    {
        return !LessEqual(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR bool operator>=(const Address& rhs) const BOOST_NOEXCEPT
    {
        return !LessThan(rhs, value_multiplicity_t());
    }

    ////////////////////////////////////////
    // Arithmetics.
public:
    BOOST_CONSTEXPR Address& operator++(void) BOOST_NOEXCEPT
    {
        Increment(value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator++(int) BOOST_NOEXCEPT
    {
        Address result(*this);
        Increment(value_multiplicity_t());
        return result;
    }

    BOOST_CONSTEXPR Address& operator--(void) BOOST_NOEXCEPT
    {
        Decrement(value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator--(int) BOOST_NOEXCEPT
    {
        Address result(*this);
        Decrement(value_multiplicity_t());
        return result;
    }

    BOOST_CONSTEXPR Address& operator+(void) BOOST_NOEXCEPT
    {
        return *this;
    }

    BOOST_CONSTEXPR Address& operator-(void) BOOST_NOEXCEPT
    {
        Negate();
        return *this;
    }

    BOOST_CONSTEXPR Address& operator+=(int64_t n) BOOST_NOEXCEPT
    {
        Plus(n, value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator+(int64_t n) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.Plus(n, value_multiplicity_t());
        return result;
    }

    BOOST_CONSTEXPR Address& operator-=(int64_t n) BOOST_NOEXCEPT
    {
        Minus(n, value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator-(int64_t n) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.Minus(n, value_multiplicity_t());
        return result;
    }

    /**
     * @brief The difference from another address.
     *
     * @remarks The range of the difference is limitted within
     *          <code>[INT64_MIN, INT64_MAX]</code>.
     *          Thus, if the return value is \c INT64_MIN, the actual
     *          difference <b>may</b> be more negative than \c INT64_MIN.
     *          If the return value is \c INT64_MAX, the actual difference
     *          <b>may</b> be greater than \c INT64_MAX.
     */
    int64_t operator-(const Address& rhs) const BOOST_NOEXCEPT
    {
        return Difference(rhs, value_multiplicity_t());
    }

    BOOST_CONSTEXPR Address& operator*=(uint64_t n) BOOST_NOEXCEPT
    {
        MultiplyAssign(n, value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator*(uint64_t n) const BOOST_NOEXCEPT
    {
        return Multiply(n, value_multiplicity_t());
    }

    // BOOST_CONSTEXPR Address& operator/=(uint64_t n) BOOST_NOEXCEPT
    // {
    // }
    //
    // BOOST_CONSTEXPR Address operator/(uint64_t n) const BOOST_NOEXCEPT
    // {
    // }
    //
    // BOOST_CONSTEXPR uint64_t operator/(const Address& rhs) const BOOST_NOEXCEPT
    // {
    // }

    BOOST_CONSTEXPR Address& operator~(void) BOOST_NOEXCEPT
    {
        BitwiseNot();
        return *this;
    }

    BOOST_CONSTEXPR Address& operator&=(const Address& rhs) BOOST_NOEXCEPT
    {
        BitwiseAnd(rhs);
        return *this;
    }

    BOOST_CONSTEXPR Address operator&(const Address& rhs) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.BitwiseAnd(rhs);
        return result;
    }

    BOOST_CONSTEXPR Address& operator|=(const Address& rhs) BOOST_NOEXCEPT
    {
        BitwiseOr(rhs);
        return *this;
    }

    BOOST_CONSTEXPR Address operator|(const Address& rhs) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.BitwiseOr(rhs);
        return result;
    }

    BOOST_CONSTEXPR Address& operator^=(const Address& rhs) BOOST_NOEXCEPT
    {
        BitwiseXor(rhs);
        return *this;
    }

    BOOST_CONSTEXPR Address operator^(const Address& rhs) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.BitwiseXor(rhs);
        return result;
    }

    BOOST_CONSTEXPR Address& operator<<=(size_t n) BOOST_NOEXCEPT
    {
        LeftShift(n, value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator<<(size_t n) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.LeftShift(n, value_multiplicity_t());
        return result;
    }

    BOOST_CONSTEXPR Address& operator>>=(size_t n) BOOST_NOEXCEPT
    {
        RightShift(n, value_multiplicity_t());
        return *this;
    }

    BOOST_CONSTEXPR Address operator>>(size_t n) const BOOST_NOEXCEPT
    {
        Address result(*this);
        result.RightShift(n, value_multiplicity_t());
        return result;
    }

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
public:
    explicit operator bool() const BOOST_NOEXCEPT
    {
        return IsZero();
    }
#else // defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
private:
    bool ToBool(void) const BOOST_NOEXCEPT { return IsZero(); }
public:
    typedef bool (Address::* BoolType)(void) const BOOST_NOEXCEPT;
    operator BoolType() const BOOST_NOEXCEPT
    {
        return IsZero() ? &Address::ToBool : nullptr;
    }
#endif // !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

    bool operator!(void) const BOOST_NOEXCEPT
    {
        return IsZero();
    }

    ////////////////////////////////////////
public:
    void Assign0(void) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] = 0;
        }
    }

    ////////////////////////////////////////
public:
    template<size_t M>
    void AssignBytes(const uint8_t (&a)[M], little_endian_t) BOOST_NOEXCEPT
    {
        Assign0();
        BOOST_STATIC_CONSTANT(size_t, NUM_BYTES_TO_COPY = (M <= NB ? M : NB));
        for (size_t i = 0; i < NUM_BYTES_TO_COPY; ++i)
        {
            data_.b_[i] = a[i];
        }
        // Mask the MSB.
        data_.v_[NV - 1] &= MSV_MASK;
    }

    template<size_t M>
    void AssignBytes(const uint8_t (&a)[M], big_endian_t) BOOST_NOEXCEPT
    {
        Assign0();
        BOOST_STATIC_CONSTANT(size_t, NUM_BYTES_TO_COPY = (M <= NB ? M : NB));
        for (size_t i = 0; i < NUM_BYTES_TO_COPY; ++i)
        {
            data_.b_[i] = a[M-1-i];
        }
        // Mask the MSB.
        data_.v_[NV - 1] &= MSV_MASK;
    }

    ////////////////////////////////////////
public:
    void AssignBytes(const uint8_t* bytes, size_t size, little_endian_t) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(bytes);
        Assign0();
        size_t numBytesToCopy = (size <= NB ? size : NB);
        for (size_t i = 0; i < numBytesToCopy; ++i)
        {
            data_.b_[i] = bytes[i];
        }
        // Mask the MSB.
        data_.v_[NV - 1] &= MSV_MASK;
    }

    void AssignBytes(const uint8_t* bytes, size_t size, big_endian_t) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(bytes);
        Assign0();
        size_t numBytesToCopy = (size <= NB ? size : NB);
        for (size_t i = 0; i < numBytesToCopy; ++i)
        {
            data_.b_[i] = bytes[size-1-i];
        }
        // Mask the MSB.
        data_.v_[NV - 1] &= MSV_MASK;
    }

    ////////////////////////////////////////
public:
    template<class T>
    void AssignValue(const T& v) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        typedef typename std::conditional<std::is_signed<T>::value,
                         signed_t, unsigned_t>::type  sign_t;
        AssignValue(v, sign_t());
    }

private:
    template<class T>
    void AssignValue(const T& v, unsigned_t) BOOST_NOEXCEPT
    {
        BOOST_STATIC_CONSTANT(size_t, M = sizeof (T));
        typedef const uint8_t (&ByteArrayType) [M];
        AssignBytes((ByteArrayType)(v), endian_t());
    }

    template<class T>
    void AssignValue(const T& v, signed_t) BOOST_NOEXCEPT
    {
        BOOST_STATIC_CONSTANT(size_t, M = sizeof (T));
        typedef const uint8_t (&ByteArrayType) [M];
        AssignBytes((ByteArrayType)(v), endian_t());
        // If the value is negative, extend the most significant bit.
        if (M < NB && v < 0)
        {
            for (size_t i = M; i < NB; ++i)
            {
                data_.b_[i] = 0xff;
            }
            // Mask the MSB.
            data_.v_[NV - 1] &= MSV_MASK;
        }
    }

    ////////////////////////////////////////
public:
    template<class T, size_t M>
    void AssignValues(const T (&a)[M], little_endian_t) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        BOOST_STATIC_CONSTANT(size_t, N = M * sizeof (T));
        typedef const uint8_t (&ByteArrayType) [N];
        AssignBytes((ByteArrayType)(a), little_endian_t());
    }

    template<class T, size_t M>
    void AssignValues(const T (&a)[M], big_endian_t) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        // The number of T's to hold the bits.
        BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + (8 * sizeof (T)) - 1)  \
                                         / (8 * sizeof (T)));
        BOOST_STATIC_CONSTANT(size_t, K = M < NT ? M : NT);
        BOOST_STATIC_CONSTANT(size_t, N = K * sizeof (T));
        union
        {
            uint8_t b[N];
            T       t[K];
        } data;
        for (size_t i = 0; i < K; ++i)
        {
            data.t[i] = a[M-1-i];
        }
        AssignBytes(data.b, little_endian_t());
    }

    template<class T>
    void AssignValues(const T* values, size_t count, little_endian_t) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        AssignBytes((const uint8_t*)(values), count * sizeof (T), little_endian_t());
    }

    template<class T>
    void AssignValues(const T* values, size_t count, big_endian_t) BOOST_NOEXCEPT
    {
        static_assert(std::is_integral<T>::value,
                      "Invalid type T, which must be an integral type.");
        // The number of T's to hold the bits.
        BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + (8 * sizeof (T)) - 1)  \
                                         / (8 * sizeof (T)));
        // The number of bytes to hold the T's.
        BOOST_STATIC_CONSTANT(size_t, N = NT * sizeof (T));
        union
        {
            uint8_t b[N];
            T       t[NT];
        } data;
        size_t i = 0;
        while (i < NT && i < count)
        {
            data.t[i] = values[count-1-i];
            ++i;
        }
        while (i < NT)
        {
            data.t[i] = 0;
            ++i;
        }
        AssignBytes(data.b, little_endian_t());
    }

    ////////////////////////////////////////
#if 0 && !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
public:
    template<class T, class... U>
    typename std::enable_if<std::is_integral<T>::value, void>::type
    AssignValues(T v0, U... v1, little_endian_t) BOOST_NOEXCEPT
    {
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must be integral types.");
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must have the same size as T.");
        // The number of T's supplied.
        BOOST_STATIC_CONSTANT(size_t, M = 1 + sizeof...(U));
        // The number of T's to hold bits.
        BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + 8 * sizeof (T) - 1)  \
                                         / (8 * sizeof (T)));
        BOOST_STATIC_CONSTANT(size_t, K = M < NT ? M : NT);
        BOOST_STATIC_CONSTANT(size_t, N = K * sizeof (T));
        union
        {
            uint8_t b[N];
            T       t[K];
        } data;
        FillUp(data.t, v0, v1..., num_elements_t<K>);
        AssignBytes(data.b, little_endian_t());
    }

    template<class T, class... U>
    typename std::enable_if<std::is_integral<T>::value, void>::type
    AssignValues(T v0, U... v1, big_endian_t) BOOST_NOEXCEPT
    {
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must be integral types.");
        static_assert((std::is_integral<U>::value && ...),
                      "Invalid type pack U, which must have the same size as T.");
        // The number of T's supplied.
        BOOST_STATIC_CONSTANT(size_t, M = 1 + sizeof...(U));
        // The number of T's to hold bits.
        BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + 8 * sizeof (T) - 1)  \
                                         / (8 * sizeof (T)));
        BOOST_STATIC_CONSTANT(size_t, K = M < NT ? M : NT);
        BOOST_STATIC_CONSTANT(size_t, N = K * sizeof (T));
        union
        {
            uint8_t b[N];
            T       t[K];
        } data;
        FillDown(data.t+K-1, v0, v1..., num_elements_t<K>);
        AssignBytes(data.b, little_endian_t());
    }

private:
    template<class T, class... U, size_t n>
    static void FillUp(T *t, T v0, U... v1, num_elements_t<n>) BOOST_NOEXCEPT
    {
        *(t++) = v0;
        FillUp(t, v1..., num_elements_t<n-1>);
    }

    // Terminate when all elements have been filled.
    template<class T, class... U>
    static void FillUp(T *t, T v0, U... v1, num_elements_t<1>) BOOST_NOEXCEPT
    {
        *t = v0;
    }

    // Terminate when all arguments have been consumed.
    template<class T, size_t n>
    static void FillUp(T *t, num_elements_t<n>) BOOST_NOEXCEPT
    {
    }

    template<class T, class... U, size_t n>
    static void FillDown(T *t, U... v1, T v0, num_elements_t<n>) BOOST_NOEXCEPT
    {
        *(t--) = v0;
        FillDown(t, v1..., num_elements_t<n-1>);
    }

    // Terminate when all elements have been filled.
    template<class T, class... U>
    static void FillDown(T *t, U... v1, T v0, num_elements_t<1>) BOOST_NOEXCEPT
    {
        *t = v0;
    }

    // Terminate when all arguments have been consumed.
    template<class T, size_t n>
    static void FillDown(T *t, num_elements_t<n>) BOOST_NOEXCEPT
    {
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
public:
# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/network/address/address-little-endian.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ////////////////////////////////////////
private:
    void CopyAssign(const Address& rhs) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(this != &rhs);
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] = rhs.data_.v_[i];
        }
    }

    ////////////////////////////////////////
private:
    bool Equal(const Address& rhs, single_value_t) const BOOST_NOEXCEPT
    {
        return data_.v_[0] == rhs.data_.v_[0];
    }

    bool Equal(const Address& rhs, multiple_values_t) const BOOST_NOEXCEPT
    {
        bool result = true;
        for (size_t i = 0; i < NV; ++i)
        {
            if (data_.v_[i] != rhs.data_.v_[i])
            {
                result = false;
                break;
            }
        }
        return result;
    }

    bool LessThan(const Address& rhs, single_value_t) const BOOST_NOEXCEPT
    {
        return data_.v_[0] < rhs.data_.v_[0];
    }

    bool LessThan(const Address& rhs, multiple_values_t) const BOOST_NOEXCEPT
    {
        bool result = false;
        size_t i = NV;
        while (i > 0)
        {
            --i;
            if (data_.v_[i] > rhs.data_.v_[i])
            {
                break;
            }
            else if (data_.v_[i] < rhs.data_.v_[i])
            {
                result = true;
                break;
            }
        }
        return result;
    }

    bool LessEqual(const Address& rhs, single_value_t) const BOOST_NOEXCEPT
    {
        return data_.v_[0] <= rhs.data_.v_[0];
    }

    bool LessEqual(const Address& rhs, multiple_values_t) const BOOST_NOEXCEPT
    {
        bool result = true;
        size_t i = NV;
        while (i > 0)
        {
            --i;
            if (data_.v_[i] < rhs.data_.v_[i])
            {
                break;
            }
            else if (data_.v_[i] > rhs.data_.v_[i])
            {
                result = false;
                break;
            }
        }
        return result;
    }

    ////////////////////////////////////////
private:
    void Increment(single_value_t) BOOST_NOEXCEPT
    {
        ++data_.v_[0];
        // Mask the MSB.
        data_.v_[0] &= MSV_MASK;
    }

    void Increment(multiple_values_t) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] += 1;
            // If there is no carry.
            if (!(data_.v_[i] < 1))
            {
                break;
            }
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    void Decrement(single_value_t) BOOST_NOEXCEPT
    {
        --data_.v_[0];
        // Mask the MSB.
        data_.v_[0] &= MSV_MASK;
    }

    void Decrement(multiple_values_t) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] += V_MASK;
            // If there is a carry (no borrow).
            if (data_.v_[i] < V_MASK)
            {
                break;
            }
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    ////////////////////////////////////////
    void Plus(int64_t n, single_value_t) BOOST_NOEXCEPT
    {
        data_.v_[0] += (Value)(n);
        // Mask the MSB.
        data_.v_[0] &= MSV_MASK;
    }

    void Plus(int64_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        // Plus n (extends to NUM_BITS).
        // The case that n is negative:
        //                             -----------------
        //                             |       n       |
        // ---------------------------------------------
        // | 0xff..ff | 0xff..ff | ... |  hi   |  lo   |
        // ---------------------------------------------
        // | v_{NV-1} | v_{NV-2} | ... | v_{1} | v_{0} |
        // ---------------------------------------------
        // +lo
        DoubleValue carry = (n & V_MASK);
        carry += data_.v_[0];
        data_.v_[0] = carry & V_MASK;
        carry >>= V_BITS;
        // +hi
        carry += (n >> V_BITS) & V_MASK;
        carry += data_.v_[1];
        data_.v_[1] = carry & V_MASK;
        // +ex
        Value ex = (n >= 0 ? 0 : V_MASK);
        for (size_t i = 2; i < NV; ++i)
        {
            carry >>= V_BITS;
            carry += ex;
            carry += data_.v_[i];
            data_.v_[i] = carry & V_MASK;
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    void Minus(int64_t n, single_value_t) BOOST_NOEXCEPT
    {
        data_.v_[0] += (Value)(-n);
        // Mask the MSB.
        data_.v_[0] &= MSV_MASK;
    }

    void Minus(int64_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        // Plus -n (extends to NUM_BITS).
        // The case that -n is negative:
        //                             -----------------
        //                             |      -n       |
        // ---------------------------------------------
        // | 0xff..ff | 0xff..ff | ... |  hi   |  lo   |
        // ---------------------------------------------
        // | v_{NV-1} | v_{NV-2} | ... | v_{1} | v_{0} |
        // ---------------------------------------------
        DoubleValue neg = (DoubleValue)(-n);
        // +lo
        DoubleValue carry = (neg & V_MASK);
        carry += data_.v_[0];
        data_.v_[0] = carry & V_MASK;
        carry >>= V_BITS;
        // +hi
        carry += (neg >> V_BITS) & V_MASK;
        carry += data_.v_[1];
        data_.v_[1] = carry & V_MASK;
        // +ex
        Value ex = (n <= 0 ? 0 : V_MASK);
        for (size_t i = 2; i < NV; ++i)
        {
            carry >>= V_BITS;
            carry += ex;
            carry += data_.v_[i];
            data_.v_[i] = carry & V_MASK;
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    ////////////////////////////////////////
private:
    void MultiplyAssign(uint64_t n, single_value_t) BOOST_NOEXCEPT
    {
        data_.v_[0] *= (Value)(n);
        data_.v_[0] &= MSV_MASK;
    }

    void MultiplyAssign(uint64_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        if (!n)
        {
            Assign0();
        }
        else if (n > 1)
        {
            Address m(*this);
            MultiplyPlus(m, n-1, multiple_values_t());
        }
    }

    BOOST_CONSTEXPR Address Multiply(uint64_t n, single_value_t) const BOOST_NOEXCEPT
    {
        Address result;
        result.data_.v_[0] = (Value)(data_.v_[0] * n);
        result.data_.v_[0] &= MSV_MASK;
        return result;
    }

    BOOST_CONSTEXPR Address Multiply(uint64_t n, multiple_values_t) const BOOST_NOEXCEPT
    {
        Address result;
        result.MultiplyPlus(*this, n, multiple_values_t());
        return result;
    }

    // *this += m * n.
    // *this must not be the same as 'm'.
    void MultiplyPlus(const Address& m, uint64_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(this != &m);
        for (size_t i = 0; i < 2; ++i)
        {
            DoubleValue x = (n & V_MASK);
            DoubleValue carry = 0;
            for (size_t j = 0; j < NV; ++j)
            {
                carry += x * m.data_.v_[j];
                carry += data_.v_[i+j];
                data_.v_[i+j] = carry & V_MASK;
                carry >>= V_BITS;
            }
            n >>= V_BITS;
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    ////////////////////////////////////////
private:
    void Negate(void) BOOST_NOEXCEPT
    {
        BitwiseNot();
        Increment(value_multiplicity_t());
    }

    void BitwiseNot(void) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] = ~data_.v_[i];
        }
        // Mask the MSB.
        data_.v_[NV-1] &= MSV_MASK;
    }

    void BitwiseAnd(const Address& rhs) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] &= rhs.data_.v_[i];
        }
    }

    void BitwiseOr(const Address& rhs) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] |= rhs.data_.v_[i];
        }
    }

    void BitwiseXor(const Address& rhs) BOOST_NOEXCEPT
    {
        for (size_t i = 0; i < NV; ++i)
        {
            data_.v_[i] ^= rhs.data_.v_[i];
        }
    }

    void LeftShift(size_t n, single_value_t) BOOST_NOEXCEPT
    {
        if (n < V_BITS)
        {
            data_.v_[0] <<= n;
            // Mask the MSB.
            data_.v_[NV-1] &= MSV_MASK;
        }
        else
        {
            data_.v_[0] = 0;
        }
    }

    void LeftShift(size_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        if (n > 0)
        {
            size_t z = n / V_BITS;
            if (z >= NV)
            {
                Assign0();
            }
            else
            {
                //                     | c ; s |    z values    |
                //     ------------------------------------------
                //     |   ;   |  ...  |   ;   |   ...  |       |
                //     ------------------------------------------
                //       NV-1              z                0
                // -------------------------
                // |   ;   |  ...  |   ;   | <--- left shift ---|
                // -------------------------
                //  NV-1-z             0
                // | s ; c |
                size_t s = n % V_BITS;
                size_t c = V_BITS - s;
                size_t i = NV - 1;
                while (i > z)
                {
                    data_.v_[i] = ((data_.v_[i-z]   << s) & V_MASK)
                                | ((data_.v_[i-z-1] >> c) & V_MASK);
                    --i;
                }
                // i == z
                data_.v_[i] = (data_.v_[0] << s) & V_MASK;
                while (i > 0)
                {
                    --i;
                    data_.v_[i] = 0;
                }
                // Mask the MSB.
                data_.v_[NV-1] &= MSV_MASK;
            }
        }
    }

    void RightShift(size_t n, single_value_t) BOOST_NOEXCEPT
    {
        if (n >= V_BITS)
        {
            data_.v_[0] = 0;
        }
        else
        {
            data_.v_[0] >>= n;
        }
    }

    void RightShift(size_t n, multiple_values_t) BOOST_NOEXCEPT
    {
        if (n > 0)
        {
            size_t z = n / V_BITS;
            if (z >= NV)
            {
                Assign0();
            }
            else
            {
                // |   z values    | s ; c |
                // -----------------------------------------
                // |       |  ...  |   ;   |  ...  |   ;   |
                // -----------------------------------------
                //   NV-1           NV-1-z             0
                //                     -------------------------
                // |-- right shift --> |   ;   |  ...  |   ;   |
                //                     -------------------------
                //                        NV-1             z
                //                                     | c ; s |
                size_t s = n % V_BITS;
                size_t c = V_BITS - s;
                size_t i = 0;
                while (i < NV-1-z)
                {
                    data_.v_[i] = ((data_.v_[i+z]   >> s) & V_MASK)
                                | ((data_.v_[i+z+1] << c) & V_MASK);
                    ++i;
                }
                // i == NV-1-z
                data_.v_[i] = (data_.v_[NV-1] >> s) & V_MASK;
                while (++i < NV)
                {
                    data_.v_[i] = 0;
                }
            }
        }
    }

    ////////////////////////////////////////
public:
    bool IsZero(void) const BOOST_NOEXCEPT
    {
        return IsZero(value_multiplicity_t());
    }

private:
    bool IsZero(single_value_t) const BOOST_NOEXCEPT
    {
        return !data_.v_[0];
    }

    bool IsZero(multiple_values_t) const BOOST_NOEXCEPT
    {
        bool result = true;
        for (size_t i = 0; i < NV; ++i)
        {
            if (data_.v_[i] != 0)
            {
                result = false;
                break;
            }
        }
        return result;
    }

    ////////////////////////////////////////
private:
    int64_t Difference(const Address& rhs, single_value_t) const BOOST_NOEXCEPT
    {
        return (data_.v_[0] - rhs.data_.v_[0]);
    }

    int64_t Difference(const Address& rhs, multiple_values_t) const BOOST_NOEXCEPT
    {
        Address d;
        DoubleValue borrow = 0;
        for (size_t i = 0; i < NV; ++i)
        {
            borrow += rhs.data_.v_[i];
            DoubleValue x = data_.v_[i] - borrow;
            borrow = (data_.v_[i] < borrow);
            d.data_.v_[i] = (x & V_MASK);
        }
        int64_t result = d.ToInt64(multiple_values_t());
        return result;
    }

    int64_t ToInt64(multiple_values_t) const BOOST_NOEXCEPT
    {
        union
        {
            Value   v[2];
            int64_t d;
        } r;
        r.v[0] = data_.v_[0];
        r.v[1] = data_.v_[1];
        // If the most significant bit is 0, the difference is non-negative.
        if (data_.v_[NV-1] <= V_MASK/2)
        {
            if (r.d < 0)
            {
                r.d = INT64_MAX;
            }
            else
            {
                size_t i = NV;
                while (--i > 2)
                {
                    if (data_.v_[i])
                    {
                        r.d = INT64_MAX;
                        break;
                    }
                }
            }
        }
        // If the most significant bit is 1, the difference is negative.
        else // if (data_.v_[NV-1] > V_MASK/2)
        {
            if (r.d >= 0)
            {
                r.d = INT64_MIN;
            }
            else
            {
                size_t i = NV;
                while (--i > 2)
                {
                    if (data_.v_[i] != V_MASK)
                    {
                        r.d = INT64_MIN;
                        break;
                    }
                }
            }
        }
        return r.d;
    }

    ////////////////////////////////////////
public:
    size_t hash_value(void) const BOOST_NOEXCEPT
    {
        return hash_value(value_multiplicity_t());
    }

private:
    size_t hash_value(single_value_t) const BOOST_NOEXCEPT
    {
        return boost::hash_value(data_.v_[0]);
    }

    size_t hash_value(multiple_values_t) const BOOST_NOEXCEPT
    {
        return boost::hash_value(data_.v_);
    }

    ////////////////////////////////////////
public:
    void swap(Address& rhs) BOOST_NOEXCEPT
    {
        boost::swap(data_.v_, rhs.data_.v_);
    }

    ////////////////////////////////////////
public:
    static BOOST_CONSTEXPR size_t GetBitSize(void) BOOST_NOEXCEPT
    {
        return NUM_BITS;
    }

    static BOOST_CONSTEXPR size_t GetSize(void) BOOST_NOEXCEPT
    {
        return NB;
    }

    BOOST_CONSTEXPR const uint8_t* GetData(void) const BOOST_NOEXCEPT
    {
        return data_.b_;
    }

    ////////////////////////////////////////
public:
    static BOOST_CONSTEXPR const Address& Zero(void) BOOST_NOEXCEPT
    {
        static Address zero;
        return zero;
    }

    static BOOST_CONSTEXPR const Address& Mask(void) BOOST_NOEXCEPT
    {
        static Address mask(-1L);
        return mask;
    }

    ////////////////////////////////////////
public:
    std::string ToString(void) const
    {
        std::stringstream oss;
        size_t i = NB;
        while (i > 0)
        {
            --i;
            oss << std::hex << std::setw(2) << std::setfill('0')
                << (int)(data_.b_[i]);
        }
        return oss.str();
    }

    ////////////////////////////////////////
public:
    template<bool zcAware>
    void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        it.Write(data_.b_, NB);
    }

    template<bool zcAware>
    void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        it.WriteL(data_.b_, NB);
    }

    template<bool zcAware>
    void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it) const
    {
        it.WriteB(data_.b_, NB);
    }

    template<bool readOnly, bool zcAware>
    void Read(BasicBufferIterator<readOnly, zcAware>& it)
    {
        it.Read(data_.b_, NB);
        data_.v_[NV-1] &= MSV_MASK;
    }

    template<bool readOnly, bool zcAware>
    void ReadL(BasicBufferIterator<readOnly, zcAware>& it)
    {
        it.ReadL(data_.b_, NB);
        data_.v_[NV-1] &= MSV_MASK;
    }

    template<bool readOnly, bool zcAware>
    void ReadB(BasicBufferIterator<readOnly, zcAware>& it)
    {
        it.ReadB(data_.b_, NB);
        data_.v_[NV-1] &= MSV_MASK;
    }

private:
    // DB: number of bytes of data_.
    // NB: number of bytes of Address.
    // NV: number of values of Address.
    // ---------------------------------------------------------------
    // |    v_{0}    | v_{2} | ... |             v{NV-1}             |
    // ---------------------------------------------------------------
    // | b_{0} ; ... |  ...  | ... | ... ; b_{NB-1} ; ... ; b_{DB-1} |
    // ---------------------------------------------------------------
    //                                   | MSB_MASK |
    //                             -----------------------------------
    //                             |            MSV_MASK             |
    // ---------------------------------------------------------------
    // |                   Address                  |
    // ----------------------------------------------
    union
    {
        uint8_t b_[DB];
        Value   v_[NV];
    } data_;

};


////////////////////////////////////////////////////////////////////////////////
template<size_t bits>
inline BOOST_CONSTEXPR Address<bits>
operator+(int64_t n, const Address<bits>& rhs) BOOST_NOEXCEPT
{
    return (rhs + n);
}

template<size_t bits>
inline BOOST_CONSTEXPR Address<bits>
operator*(int64_t n, const Address<bits>& rhs) BOOST_NOEXCEPT
{
    return (rhs * n);
}

template<size_t bits>
inline size_t hash_value(const Address<bits>& addr) BOOST_NOEXCEPT
{
    return addr.hash_value();
}

template<size_t bits>
inline void swap(Address<bits>& lhs, Address<bits>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


////////////////////////////////////////////////////////////////////////////////
template<size_t bits, class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, const Address<bits>& addr)
{
    return os << addr.ToString();
}


NSFX_CLOSE_NAMESPACE


#endif // ADDRESS_LITTLE_ENDIAN_H__F787C62F_D1FF_41EF_9E2C_8DF94C98A995


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

#  define NSFX_PP_IS_INTEGRAL(z, n, d)                                     \
            static_assert(                                                 \
                std::is_integral<T ## n>::value,                           \
                "Invalid type T" #n ", which must be an integral type.");  \

#  define NSFX_PP_IS_SAME_SIZE(z, n, d)                                         \
            static_assert(                                                      \
                sizeof (T0) == sizeof (T ## n),                                 \
                "Invalid type T" #n ", which must have the same size as T0.");  \

#  define NSFX_PP_FILL_UP(z, n, d)    if (n < K)     { t[n]     = v ## n; }
#  define NSFX_PP_FILL_DOWN(z, n, d)  if (M-1-n < K) { t[M-1-n] = v ## n; }

# if BOOST_PP_ITERATION_FLAGS() == 0/*{{{*/

// template<class T0, class T1, ...>
// Address(T0 v0, T1 v1, ...,
//         typename std::enable_if<std::is_integral<T0>::value,
//         little_endian_t>::type)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class T)>
BOOST_CONSTEXPR
Address(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), T, v),
        typename std::enable_if<std::is_integral<T0>::value,
        little_endian_t>::type) BOOST_NOEXCEPT
{
    // static_assert(std::is_integral<Tn>::value,
    //               "Invalid type Tn, which must be an integral type.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_INTEGRAL, );
    // static_assert(sizeof (T0) == sizeof (Tn),
    //               "Invalid type Tn, which must have the same size as T0.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_SAME_SIZE, );
    // AssignValues(v0, v1, ..., little_endian_t());
    AssignValues(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), v),
                 little_endian_t());
}

// template<class T0, class T1, ...>
// Address(T0 v0, T1 v1, ...,
//         typename std::enable_if<std::is_integral<T0>::value,
//         big_endian_t>::type)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class T)>
BOOST_CONSTEXPR
Address(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), T, v),
        typename std::enable_if<std::is_integral<T0>::value,
        big_endian_t>::type) BOOST_NOEXCEPT
{
    // static_assert(std::is_integral<Tn>::value,
    //               "Invalid type Tn, which must be an integral type.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_INTEGRAL, );
    // static_assert(sizeof (T0) == sizeof (Tn),
    //               "Invalid type Tn, which must have the same size as T0.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_SAME_SIZE, );
    // AssignValues(v0, v1, ..., big_endian_t());
    AssignValues(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), v),
                 big_endian_t());
}

/*}}}*/

# elif BOOST_PP_ITERATION_FLAGS() == 1/*{{{*/

// template<class T0, class T1, ...>
// typename std::enable_if<std::is_integral<T0>::value, void>::type
// AssignValues(T0 v0, T1 v1, ..., little_endian_t)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class T)>
typename std::enable_if<std::is_integral<T0>::value, void>::type
AssignValues(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), T, v),
             little_endian_t) BOOST_NOEXCEPT
{
    // static_assert(std::is_integral<Tn>::value,
    //               "Invalid type Tn, which must be an integral type.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_INTEGRAL, );
    // static_assert(sizeof (T0) == sizeof (Tn),
    //               "Invalid type Tn, which must have the same size as T0.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_SAME_SIZE, );
    // The number of T's supplied.
    BOOST_STATIC_CONSTANT(size_t, M = BOOST_PP_ITERATION());
    // The number of T's to hold bits.
    BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + 8 * sizeof (T0) - 1)  \
                                     / (8 * sizeof (T0)));
    BOOST_STATIC_CONSTANT(size_t, K = M < NT ? M : NT);
    BOOST_STATIC_CONSTANT(size_t, N = K * sizeof (T0));
    union
    {
        uint8_t b[N];
        T0      t[K];
    } data;
    // if (0 < K) { data.t[0] = v_{0}; }
    // if (1 < K) { data.t[1] = v_{1}; }
    // ...
    // if (M-1 < K) { data.t[M-1] = v_{M-1}; }
    T0* t = data.t;
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_FILL_UP, );
    AssignBytes(data.b, little_endian_t());
}

// template<class T0, class T1, ...>
// typename std::enable_if<std::is_integral<T0>::value, void>::type
// AssignValues(T0 v0, T1 v1, ..., little_endian_t)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class T)>
typename std::enable_if<std::is_integral<T0>::value, void>::type
AssignValues(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), T, v),
             big_endian_t) BOOST_NOEXCEPT
{
    // static_assert(std::is_integral<Tn>::value,
    //               "Invalid type Tn, which must be an integral type.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_INTEGRAL, );
    // static_assert(sizeof (T0) == sizeof (Tn),
    //               "Invalid type Tn, which must have the same size as T0.");
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_IS_SAME_SIZE, );
    // The number of T's supplied.
    BOOST_STATIC_CONSTANT(size_t, M = BOOST_PP_ITERATION());
    // The number of T's to hold bits.
    BOOST_STATIC_CONSTANT(size_t, NT = (NUM_BITS + 8 * sizeof (T0) - 1)  \
                                     / (8 * sizeof (T0)));
    BOOST_STATIC_CONSTANT(size_t, K = M < NT ? M : NT);
    BOOST_STATIC_CONSTANT(size_t, N = K * sizeof (T0));
    union
    {
        uint8_t b[N];
        T0      t[K];
    } data;
    // if (M-1 < K) { data.t[M-1] = v_{0}; }
    // ...
    // if (1 < K) { data.t[1] = v_{M-2}; }
    // if (0 < K) { data.t[0] = v_{M-1}; }
    T0* t = data.t;
    BOOST_PP_REPEAT(BOOST_PP_ITERATION(), NSFX_PP_FILL_DOWN, );
    AssignBytes(data.b, little_endian_t());
}

/*}}}*/

# endif // BOOST_PP_ITERATION_FLAGS() == x

#  undef NSFX_PP_IS_INTEGRAL
#  undef NSFX_PP_IS_SAME_SIZE
#  undef NSFX_PP_FILL_UP
#  undef NSFX_PP_FILL_DOWN

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

