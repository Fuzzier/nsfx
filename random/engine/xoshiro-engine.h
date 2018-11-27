/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2018-09-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef XOSHIRO_ENGINE_H__805A391A_924A_4BB0_88CD_9A674BEF2D41
#define XOSHIRO_ENGINE_H__805A391A_924A_4BB0_88CD_9A674BEF2D41


#include <nsfx/random/config.h>
#include <nsfx/random/engine/splitmix-engine.h>

#include <nsfx/random/detail/rot.h>
#include <nsfx/random/detail/is-power-2.h>

#include <type_traits> // is_same
#include <cstring> // memcpy


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
namespace random {
namespace aux {


////////////////////////////////////////
// The transformation matrix of xoshiro is denoted by T.
// T is a square matrix of the following operators:
// I: identity
// S(a): left shift a bits
// R(b): left rotate b bits
//
// The state s is a row vector.
// The transformation is as follows:
// s = s * T.
//
// A matrix based transformation is unified.
// However, the abstraction would introduce too much overhead.
// Therefore, tranformation classes are used.

template<class UIntType, size_t n, size_t a, size_t b>
struct xoshiro_transformer_traits
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    BOOST_STATIC_CONSTANT(size_t, shift_a  = a);
    BOOST_STATIC_CONSTANT(size_t, rotate_b = b);
};

// The transformation matrix of xoshiro128 and xoshiro256:
// | I  I  I     0    |
// | I  I  S(a)  R(b) |
// | 0  I  I     0    |
// | I  0  0     R(b) |
// xoshiro128: uint32_t, a:  9, b: 11
// xoshiro256: uint64_t, a: 17, b: 45
template<class UIntType, size_t a, size_t b>
struct xoshiro_n4_transformer :
    xoshiro_transformer_traits<UIntType, 4, a, b>
{
    static void transform(UIntType (&s)[state_size])
    {
        const UIntType t = s[1] << a;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], b);
    }
};

// The transformation matrix of xoshiro512:
// | I  I  I  0  0  0  0     0    |
// | 0  I  0  0  I  I  S(a)  0    |
// | 0  I  I  0  0  0  0     0    |
// | 0  0  0  I  0  0  I     R(b) |
// | 0  0  0  I  I  0  0     0    |
// | 0  0  0  0  I  I  0     0    |
// | I  0  0  0  0  0  I     0    |
// | 0  0  0  0  0  0  I     R(b) |
// xoshiro256: uint64_t, a: 11, b: 21
template<class UIntType, size_t a, size_t b>
struct xoshiro_n8_transformer :
    xoshiro_transformer_traits<UIntType, 8, a, b>
{
    typedef uint64_t UIntType;
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = 8);
    static void transform(UIntType (&s)[state_size])
    {
        const UIntType t = s[1] << a;
        s[2] ^= s[0];
        s[5] ^= s[1];
        s[1] ^= s[2];
        s[7] ^= s[3];
        s[3] ^= s[4];
        s[4] ^= s[5];
        s[0] ^= s[6];
        s[6] ^= s[7];
        s[6] ^= t;
        s[7] = rotl(s[7], b);
    }
};


////////////////////////////////////////
// The + scrambler.
template<class UIntType, size_t n, size_t i, size_t j>
struct xoshiro_plus_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static_assert(j < n, "Invalid parameter.");
    static result_type scramble(UIntType (&s)[n])
    {
        return s[i] + s[j];
    }
};

// The * scrambler.
template<class UIntType, size_t n, size_t i, UIntType s>
struct xoshiro_star_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static result_type scramble(UIntType (&s)[n])
    {
        return s[i] * s;
    }
};

// The ** scrambler.
template<class UIntType, size_t n, size_t i, UIntType s, size_t r, UIntType t>
struct xoshiro_starstar_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static result_type scramble(UIntType (&x)[n])
    {
        return rotl(x[i] * s, r) * t;
    }
};


} // namespace aux
} // namespace random


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro pseudo-random number generator.
 */
template<class UIntType, size_t n,
         class Transformer, class Scrambler>
class xoshiro_engine
{
public:
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(result_type, default_seed = 1u);
    BOOST_STATIC_CONSTANT(size_t, state_size = n);

    static_assert(std::is_same<typename Transformer::result_type,
                               result_type>::value,
                  "Incompatible transformer type.");

    static_assert(Transformer::state_size == state_size,
                  "Incompatible transformer type.");

    static_assert(std::is_same<typename Scrambler::result_type,
                               result_type>::value,
                  "Incompatible scrambler type.");

    static_assert(Scrambler::state_size == state_size,
                  "Incompatible scrambler type.");

    BOOST_STATIC_CONSTANT(size_t, shift_a  = Transformer::shift_a);
    BOOST_STATIC_CONSTANT(size_t, rotate_b = Transformer::rotate_b);

public:
    xoshiro_engine(void)
    {
        seed(default_seed);
    }

    explicit xoshiro_engine(result_type value) :
    {
        seed(value);
    }

    void seed(result_type value)
    {
        splitmix64 gen(value);
        for (size_t i = 0; i < n; ++i)
        {
            s_[i] = static_cast<UIntType>(gen());
        }
    }

public:
    result_type operator()(void)
    {
        const UIntType result = Scrambler::scramble(s_);
        Transformer::transform(s_);
        return result;
    }

    void discard(uintmax_t z)
    {
        for (uintmax_t i = 0; i < z; ++i)
        {
            (*this)();
        }
    }

    static result_type (min)(void)
    {
        return 0;
    }

    static result_type (max)(void)
    {
        return (std::numeric_limits<result_type>::max)();
    }

private:
    UIntType s_[n];
};

////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro128+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro128plus.c
 *
 * This is \c xoshiro128+ 1.0, the authors' best and fastest 32-bit generator
 * for 32-bit floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro128**.
 * It passes all tests the authors are aware of except for linearity tests,
 * as the lowest four bits have low linear complexity, so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 32-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * A \c splitmix64 generator is seeded, and its output is used to fill the state.
 */
typedef xoshiro_engine<uint32_t, 4,
        random::aux::xoshiro_n4_transformer<uint32_t, 9, 11>,
        random::aux::xoshiro_plus_scrambler<uint32_t, 4, 0, 3>>
    xoshiro128plus;

/**
 * @ingroup Random
 * @brief A xoshiro128** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro128starstar.c
 *
 * This is \c xoshiro128** 1.0, a 32-bit all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed, a state size (128 bits) that is large
 * enough for mild parallelism, and it passes all tests the authors are aware of.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * \c xoshiro128+ is even faster.
 *
 * A \c splitmix64 generator is seeded, and its output is used to fill the state.
 */
typedef xoshiro_engine<uint32_t, 4,
        random::aux::xoshiro_n4_transformer<uint32_t, 9, 11>,
        random::aux::xoshiro_starstar_scrambler<uint32_t, 4, 0, 5, 7, 9>>
    xoshiro128starstar;

/**
 * @ingroup Random
 * @brief A xoshiro256+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro256plus.c
 *
 * This is xoshiro256+ 1.0, the authors' best and fastest generator for
 * floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro256**.
 * It passes all tests the authors are aware of except for the lowest three
 * bits, which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value, and
 * right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_engine<uint64_t, 4,
        random::aux::xoshiro_n4_transformer<uint64_t, 17, 45>,
        random::aux::xoshiro_plus_scrambler<uint64_t, 4, 0, 3>>
    xoshiro256plus;

/**
 * @ingroup Random
 * @brief A xoshiro256** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro256starstar.c
 *
 * This is xoshiro256** 1.0, the authors' all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed, a state (256 bits) that is large enough for
 * any parallel application, and it passes all tests the authors are aware of.
 *
 * For generating just floating-point numbers, \c xoshiro256+ is even faster.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_engine<uint64_t, 4,
        random::aux::xoshiro_n4_transformer<uint64_t, 17, 45>,
        random::aux::xoshiro_starstar_scrambler<uint64_t, 4, 1, 5, 7, 9>>
    xoshiro256starstar;

/**
 * @ingroup Random
 * @brief A xoshiro512+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro512plus.c
 *
 * This is \c xoshiro512+ 1.0, the authers' generator for floating-point
 * numbers with increased state size.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro512**.
 * It passes all tests the authors are aware of except for the lowest three
 * bits, which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_engine<uint64_t, 8,
        random::aux::xoshiro_n8_transformer<uint64_t, 11, 21>,
        random::aux::xoshiro_plus_scrambler<uint64_t, 8, 0, 2>>
    xoshiro512plus;

/**
 * @ingroup Random
 * @brief A xoshiro512** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro512starstar.c
 *
 * This is \c xoshiro512** 1.0, an all-purpose, rock-solid generator.
 * It has excellent (about 1ns) speed, an increased state (512 bits) that
 * is large enough for any parallel application, and it passes all tests
 * the authors are aware of.
 *
 * For generating just floating-point numbers, \c xoshiro512+ is even faster.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_engine<uint64_t, 8,
        random::aux::xoshiro_n8_transformer<uint64_t, 11, 21>,
        random::aux::xoshiro_starstar_scrambler<uint64_t, 8, 1, 5, 7, 9>>
    xoshiro512starstar;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro pseudo-random number generator.
 */
template<class RealType, class UIntType, size_t n,
         class Transformer, class Scrambler>
class xoshiro_01_engine
{
private:
    typedef xoshiro_engine<UIntType, n, Transformer, Scrambler> rng_type;

public:
    typedef RealType result_type;
    typedef UIntType seed_type;
    BOOST_STATIC_CONSTANT(UIntType, default_seed = 1u);
    BOOST_STATIC_CONSTANT(size_t, state_size = n);

    static_assert(sizeof (float)  == 4, "Unsupported float type.");
    static_assert(std::numeric_limits<float>::digits == 24,
                  "Unsupported float type.");

    static_assert(sizeof (double) == 8, "Unsupported double type.");
    static_assert(std::numeric_limits<double>::digits == 53,
                  "Unsupported double type.");

    BOOST_STATIC_CONSTANT(size_t, shift_a  = Transformer::shift_a);
    BOOST_STATIC_CONSTANT(size_t, rotate_b = Transformer::rotate_b);

public:
    xoshiro_01_engine(void)
    {
    }

    explicit xoshiro_01_engine(seed_type value) :
        rng_(value)
    {
    }

    void seed(seed_type value)
    {
        rng_.seed(value);
    }

private:
    template<class RealType, class UIntType>
    struct uint_to_real
    {
        static RealType convert(uint32_t n)
        {
            return (n >> 8) * (1.0f / 16777216.0f); // 2^-24
        }
    };

    template<class RealType>
    struct uint_to_real<RealType, uint64_t>
    {
        static RealType convert(uint64_t n)
        {
            return (n >> 11) * (1.0 / 9007199254740992.0); // 2^-53
        }
    };

public:
    result_type operator()(void)
    {
        return uint_to_real<RealType, UIntType>::convert(rng_());
    }

    void discard(uintmax_t z)
    {
        rng_.discard(z);
    }

    static BOOST_CONSTEXPR result_type (min)(void)
    {
        return 0;
    }

    static BOOST_CONSTEXPR result_type (max)(void)
    {
        return 1;
    }

private:
    rng_type rng_;
};

////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro128+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro128plus.c
 *
 * This is \c xoshiro128+ 1.0, the authors' best and fastest 32-bit generator
 * for 32-bit floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro128**.
 * It passes all tests the authors are aware of except for linearity tests,
 * as the lowest four bits have low linear complexity, so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 32-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * A \c splitmix64 generator is seeded, and its output is used to fill the state.
 */
typedef xoshiro_01_engine<float, uint32_t, 4,
        random::aux::xoshiro_n4_transformer<uint32_t, 9, 11>,
        random::aux::xoshiro_plus_scrambler<uint32_t, 4, 0, 3>>
    xoshiro128plus_01;

/**
 * @ingroup Random
 * @brief A xoshiro128** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro128starstar.c
 *
 * This is \c xoshiro128** 1.0, a 32-bit all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed, a state size (128 bits) that is large
 * enough for mild parallelism, and it passes all tests the authors are aware of.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * \c xoshiro128+ is even faster.
 *
 * A \c splitmix64 generator is seeded, and its output is used to fill the state.
 */
typedef xoshiro_01_engine<float, uint32_t, 4,
        random::aux::xoshiro_n4_transformer<uint32_t, 9, 11>,
        random::aux::xoshiro_starstar_scrambler<uint32_t, 4, 0, 5, 7, 9>>
    xoshiro128starstar_01;

/**
 * @ingroup Random
 * @brief A xoshiro256+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro256plus.c
 *
 * This is xoshiro256+ 1.0, the authors' best and fastest generator for
 * floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro256**.
 * It passes all tests the authors are aware of except for the lowest three
 * bits, which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value, and
 * right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_01_engine<double, uint64_t, 4,
        random::aux::xoshiro_n4_transformer<uint64_t, 17, 45>,
        random::aux::xoshiro_plus_scrambler<uint64_t, 4, 0, 3>>
    xoshiro256plus_01;

/**
 * @ingroup Random
 * @brief A xoshiro256** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro256starstar.c
 *
 * This is xoshiro256** 1.0, the authors' all-purpose, rock-solid generator.
 * It has excellent (sub-ns) speed, a state (256 bits) that is large enough for
 * any parallel application, and it passes all tests the authors are aware of.
 *
 * For generating just floating-point numbers, \c xoshiro256+ is even faster.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_01_engine<double, uint64_t, 4,
        random::aux::xoshiro_n4_transformer<uint64_t, 17, 45>,
        random::aux::xoshiro_starstar_scrambler<uint64_t, 4, 1, 5, 7, 9>>
    xoshiro256starstar_01;

/**
 * @ingroup Random
 * @brief A xoshiro512+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro512plus.c
 *
 * This is \c xoshiro512+ 1.0, the authers' generator for floating-point
 * numbers with increased state size.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than \c xoshiro512**.
 * It passes all tests the authors are aware of except for the lowest three
 * bits, which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_01_engine<double, uint64_t, 8,
        random::aux::xoshiro_n8_transformer<uint64_t, 11, 21>,
        random::aux::xoshiro_plus_scrambler<uint64_t, 8, 0, 2>>
    xoshiro512plus_01;

/**
 * @ingroup Random
 * @brief A xoshiro512** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoshiro512starstar.c
 *
 * This is \c xoshiro512** 1.0, an all-purpose, rock-solid generator.
 * It has excellent (about 1ns) speed, an increased state (512 bits) that
 * is large enough for any parallel application, and it passes all tests
 * the authors are aware of.
 *
 * For generating just floating-point numbers, \c xoshiro512+ is even faster.
 *
 * The authors suggest to seed a \c splitmix64 generator and use its output to
 * fill the state.
 */
typedef xoshiro_01_engine<double, uint64_t, 8,
        random::aux::xoshiro_n8_transformer<uint64_t, 11, 21>,
        random::aux::xoshiro_starstar_scrambler<uint64_t, 8, 1, 5, 7, 9>>
    xoshiro512starstar_01;


NSFX_CLOSE_NAMESPACE


#endif // XOSHIRO_ENGINE_H__805A391A_924A_4BB0_88CD_9A674BEF2D41

