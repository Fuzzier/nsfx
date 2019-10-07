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

#ifndef XOROSHIRO_ENGINE_H__F9ACBF0B_52EC_412C_B70D_3397EF964427
#define XOROSHIRO_ENGINE_H__F9ACBF0B_52EC_412C_B70D_3397EF964427


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
// The state of xoroshiro (n >= 4).
template<class UIntType, size_t n>
struct xoroshiro_state
{
    static_assert(is_power_2<n>::value,
                  "Invalid state size, it must be a power of 2.");

    xoroshiro_state(void) : p_(n-1) {}
    UIntType s_[n];
    size_t   p_; // Indicates the last register.
};

// The state of xoroshiro (n == 2).
template<class UIntType>
struct xoroshiro_state<UIntType, 2>
{
    UIntType s_[2];
};

////////////////////////////////////////
// The + scrambler (n >= 4).
template<class UIntType, size_t n, size_t i, size_t j>
struct xoroshiro_plus_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static_assert(j < n, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, n>& state)
    {
        return state.s_[(state.p_ + ((i+1) & (n-1))) & (n-1)] +
               state.s_[(state.p_ + ((j+1) & (n-1))) & (n-1)];
    }
};

// The * scrambler (n >= 4).
template<class UIntType, size_t n, size_t i, UIntType s>
struct xoroshiro_star_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, n>& state)
    {
        return state.s_[(state.p_ + ((i+1) & (n-1))) & (n-1)] * s;
    }
};

// The ** scrambler (n >= 4).
template<class UIntType, size_t n, size_t i, UIntType s, size_t r, UIntType t>
struct xoroshiro_starstar_scrambler
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    static_assert(i < n, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, n>& state)
    {
        return rotl(state.s_[(state.p_ + ((i+1) & (n-1))) & (n-1)] * s, r) * t;
    }
};

////////////////////////////////////////
// The + scrambler (n == 2).
template<class UIntType, size_t i, size_t j>
struct xoroshiro_plus_scrambler<UIntType, 2, i, j>
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = 2);
    static_assert(i < 2, "Invalid parameter.");
    static_assert(j < 2, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, 2>& state)
    {
        return state.s_[i] + state.s_[j];
    }
};

// The * scrambler (n == 2).
template<class UIntType, size_t i, UIntType s>
struct xoroshiro_star_scrambler<UIntType, 2, i, s>
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = 2);
    static_assert(i < 2, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, 2>& state)
    {
        return state.s_[i] * s;
    }
};

// The ** scrambler (n == 2).
template<class UIntType, size_t i, UIntType s, size_t r, UIntType t>
struct xoroshiro_starstar_scrambler<UIntType, 2, i, s, r, t>
{
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(size_t, state_size = 2);
    static_assert(i < 2, "Invalid parameter.");
    static result_type scramble(xoroshiro_state<UIntType, 2>& state)
    {
        return rotl(state.s_[i] * s, r) * t;
    }
};


} // namespace aux
} // namespace random


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoroshiro pseudo-random number generator.
 */
template<class UIntType, size_t n,
         size_t a, size_t b, size_t c,
         class Scrambler>
class xoroshiro_engine :
    public random::aux::xoroshiro_state<UIntType, n>
{
public:
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(result_type, default_seed = 1u);
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    BOOST_STATIC_CONSTANT(size_t, rotate_a = a);
    BOOST_STATIC_CONSTANT(size_t, shift_b  = b);
    BOOST_STATIC_CONSTANT(size_t, rotate_c = c);

    static_assert(std::is_same<typename Scrambler::result_type,
                               result_type>::value,
                  "Incompatible scrambler type.");

    static_assert(Scrambler::state_size == state_size,
                  "Incompatible scrambler type.");

private:
    template<size_t n_>
    struct state_size_tag {};

    // For n > 2.
    template<size_t n_>
    void seed(result_type value, state_size_tag<n_>)
    {
        seed(value, state_size_tag<2>());
        this->p_ = n_ - 1;
    }

    template<size_t n_>
    void transform(state_size_tag<n_>)
    {
        size_t p = this->p_;
        size_t q = p;
        p = (p + 1) & (n_ - 1); // n_ must be 2^k, where k >= 1.
        const UIntType s0 = this->s_[p];
              UIntType s1 = this->s_[q];
        s1 ^= s0;
        this->s_[q] = random::aux::rotl(s0, a) ^ s1 ^ (s1 << b);
        this->s_[p] = random::aux::rotl(s1, c);
    }

    // For n == 2.
    void seed(result_type value, state_size_tag<2>)
    {
        splitmix64 gen(value);
        for (size_t i = 0; i < n; ++i)
        {
            this->s_[i] = static_cast<UIntType>(gen());
        }
    }

    void transform(state_size_tag<2>)
    {
        const UIntType s0 = this->s_[0];
              UIntType s1 = this->s_[1];
        s1 ^= s0;
        this->s_[0] = random::aux::rotl(s0, a) ^ s1 ^ (s1 << b);
        this->s_[1] = random::aux::rotl(s1, c);
    }

public:
    xoroshiro_engine(void)
    {
        seed(default_seed);
    }

    explicit xoroshiro_engine(result_type value)
    {
        seed(value);
    }

    void seed(result_type value)
    {
        seed(value, state_size_tag<n>());
    }

public:
    result_type operator()(void)
    {
        const UIntType result = Scrambler::scramble(*this);
        transform(state_size_tag<n>());
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
};

////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoroshiro64* pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2016.
 * See http://xoshiro.di.unimi.it/xoroshiro64star.c
 *
 * This is `xoroshiro64*` 1.0, the authors' best and fastest 32-bit
 * small-state generator for 32-bit floating-point numbers.
 *
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than `xoroshiro64**`.
 * It passes all tests the authors are aware of except for linearity tests,
 * as the lowest six bits have low linear complexity, so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 32-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * A `splitmix64` generator is seeded, and its output is used to fill the state.
 */
typedef xoroshiro_engine<uint32_t, 2, 26, 9, 13,
        random::aux::xoroshiro_star_scrambler<uint32_t, 2, 0, 0x9e3779bb>>
    xoroshiro64star;

/**
 * @ingroup Random
 * @brief A xoroshiro64** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoroshiro64starstar.c
 *
 * This is `xoroshiro64**` 1.0, the authors' 32-bit all-purpose, rock-solid,
 * small-state generator.
 * It is extremely fast and it passes all tests the authors are aware of, but
 * its state space is not large enough for any parallel application.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point
 * numbers, `xoroshiro64*` is even faster.
 *
 * A `splitmix64` generator is seeded, and its output is used to fill the state.
 */
typedef xoroshiro_engine<uint32_t, 2, 26, 9, 13,
        random::aux::xoroshiro_starstar_scrambler<uint32_t, 2, 0, 0x9e3779bb, 5, 5>>
    xoroshiro64starstar;

/**
 * @ingroup Random
 * @brief A xoroshiro128+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoroshiro128plus.c
 *
 * This is `xoroshiro128+` 1.0, the authors' best and fastest small-state
 * generator for floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than `xoroshiro128**`.
 * It passes all tests the authors are aware of except for the four lower bits,
 * which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too; moreover, this generator has a
 * very mild Hamming-weight dependency making our test
 * (http://prng.di.unimi.it/hwd.php) fail after 8 TB of output; the authors
 * believe this slight bias cannot affect any application.
 * If the users are concerned, use `xoroshiro128**` or `xoshiro256+`.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_engine<uint64_t, 2, 24, 16, 37,
        random::aux::xoroshiro_plus_scrambler<uint64_t, 2, 0, 1>>
    xoroshiro128plus;

/**
 * @ingroup Random
 * @brief A xoroshiro128** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * Ported from the code written by David Blackman and Sebastiano Vigna in 2018.
 * See http://xoshiro.di.unimi.it/xoroshiro128starstar.c
 *
 * This is `xoroshiro128**` 1.0, the authors' all-purpose, rock-solid,
 * small-state generator.
 * It is extremely (sub-ns) fast and it passes all tests the authors are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, `xoroshiro128+` is even
 * faster (but it has a very mild bias, see the comments).
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_engine<uint64_t, 2, 24, 16, 37,
        random::aux::xoroshiro_starstar_scrambler<uint64_t, 2, 0, 5, 7, 9>>
    xoroshiro128starstar;

/**
 * @ingroup Random
 * @brief A xoroshiro1024+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_engine<uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_plus_scrambler<uint64_t, 16, 15, 0>>
    xoroshiro1024plus;

/**
 * @ingroup Random
 * @brief A xoroshiro1024* pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_engine<uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_star_scrambler<uint64_t, 16, 0, 0x9e3779b97f4a7c13ULL>>
    xoroshiro1024star;

/**
 * @ingroup Random
 * @brief A xoroshiro1024** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_engine<uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_starstar_scrambler<uint64_t, 16, 0, 5, 7, 9>>
    xoroshiro1024starstar;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoroshiro pseudo-random number generator.
 */
template<class RealType, class UIntType, size_t n,
         size_t a, size_t b, size_t c,
         class Scrambler>
class xoroshiro_01_engine
{
private:
    typedef xoroshiro_engine<UIntType, n, a, b, c, Scrambler> rng_type;

public:
    typedef RealType result_type;
    typedef UIntType seed_type;
    BOOST_STATIC_CONSTANT(UIntType, default_seed = 1u);
    BOOST_STATIC_CONSTANT(size_t, state_size = n);
    BOOST_STATIC_CONSTANT(size_t, rotate_a = a);
    BOOST_STATIC_CONSTANT(size_t, shift_b  = b);
    BOOST_STATIC_CONSTANT(size_t, rotate_c = c);

    static_assert(sizeof (float)  == 4, "Unsupported float type.");
    static_assert(std::numeric_limits<float>::digits == 24,
                  "Unsupported float type.");

    static_assert(sizeof (double) == 8, "Unsupported double type.");
    static_assert(std::numeric_limits<double>::digits == 53,
                  "Unsupported double type.");

public:
    xoroshiro_01_engine(void) {}

    explicit xoroshiro_01_engine(seed_type value) :
        rng_(value)
    {}

    void seed(seed_type value)
    {
        rng_.seed(value);
    }

private:
    template<class RealType1, class UIntType1>
    struct uint_to_real
    {
        static RealType1 convert(uint32_t m)
        {
            return (m >> 8) * (1.0f / 16777216.0f); // 2^-24
        }
    };

    template<class RealType1>
    struct uint_to_real<RealType1, uint64_t>
    {
        static RealType1 convert(uint64_t m)
        {
            return (m >> 11) * (1.0 / 9007199254740992.0); // 2^-53
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
 * @brief A xoroshiro64* pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * This is `xoroshiro64*` 1.0, the authors' best and fastest 32-bit
 * small-state generator for 32-bit floating-point numbers.
 *
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than `xoroshiro64**`.
 * It passes all tests the authors are aware of except for linearity tests,
 * as the lowest six bits have low linear complexity, so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 32-bit outputs, too.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * A `splitmix64` generator is seeded, and its output is used to fill the state.
 */
typedef xoroshiro_01_engine<float, uint32_t, 2, 26, 9, 13,
        random::aux::xoroshiro_star_scrambler<uint32_t, 2, 0, 0x9e3779bb>>
    xoroshiro64star_01;

/**
 * @ingroup Random
 * @brief A xoroshiro64** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * This is `xoroshiro64**` 1.0, the authors' 32-bit all-purpose, rock-solid,
 * small-state generator.
 * It is extremely fast and it passes all tests the authors are aware of, but
 * its state space is not large enough for any parallel application.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point
 * numbers, `xoroshiro64*` is even faster.
 *
 * A `splitmix64` generator is seeded, and its output is used to fill the state.
 */
typedef xoroshiro_01_engine<float, uint32_t, 2, 26, 9, 13,
        random::aux::xoroshiro_starstar_scrambler<uint32_t, 2, 0, 0x9e3779bb, 5, 5>>
    xoroshiro64starstar_01;

/**
 * @ingroup Random
 * @brief A xoroshiro128+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * This is `xoroshiro128+` 1.0, the authors' best and fastest small-state
 * generator for floating-point numbers.
 * The authors suggest to use its upper bits for floating-point generation,
 * as it is slightly faster than `xoroshiro128**`.
 * It passes all tests the authors are aware of except for the four lower bits,
 * which might fail linearity tests (and just those), so if low linear
 * complexity is not considered an issue (as it is usually the case), it can
 * be used to generate 64-bit outputs, too; moreover, this generator has a
 * very mild Hamming-weight dependency making our test
 * (http://prng.di.unimi.it/hwd.php) fail after 8 TB of output; the authors
 * believe this slight bias cannot affect any application.
 * If the users are concerned, use `xoroshiro128**` or `xoshiro256+`.
 *
 * The authors suggest to use a sign test to extract a random Boolean value,
 * and right shifts to extract subsets of bits.
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_01_engine<double, uint64_t, 2, 24, 16, 37,
        random::aux::xoroshiro_plus_scrambler<uint64_t, 2, 0, 1>>
    xoroshiro128plus_01;

/**
 * @ingroup Random
 * @brief A xoroshiro128** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * This is `xoroshiro128**` 1.0, the authors' all-purpose, rock-solid,
 * small-state generator.
 * It is extremely (sub-ns) fast and it passes all tests the authors are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, `xoroshiro128+` is even
 * faster (but it has a very mild bias, see the comments).
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_01_engine<double, uint64_t, 2, 24, 16, 37,
        random::aux::xoroshiro_starstar_scrambler<uint64_t, 2, 0, 5, 7, 9>>
    xoroshiro128starstar_01;

/**
 * @ingroup Random
 * @brief A xoroshiro1024+ pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_01_engine<double, uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_plus_scrambler<uint64_t, 16, 15, 0>>
    xoroshiro1024plus_01;

/**
 * @ingroup Random
 * @brief A xoroshiro1024* pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_01_engine<double, uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_star_scrambler<uint64_t, 16, 0, 0x9e3779b97f4a7c13ULL>>
    xoroshiro1024star_01;

/**
 * @ingroup Random
 * @brief A xoroshiro1024** pseudo-random number generator.
 *
 * It is discovered by David Blackman and Sebastiano Vigna in 2018.
 * See http://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
 *
 * The authors suggest to seed a `splitmix64` generator and use its output to
 * fill the state.
 */
typedef xoroshiro_01_engine<double, uint64_t, 16, 25, 27, 36,
        random::aux::xoroshiro_starstar_scrambler<uint64_t, 16, 0, 5, 7, 9>>
    xoroshiro1024starstar_01;


NSFX_CLOSE_NAMESPACE


#endif // XOROSHIRO_ENGINE_H__F9ACBF0B_52EC_412C_B70D_3397EF964427

