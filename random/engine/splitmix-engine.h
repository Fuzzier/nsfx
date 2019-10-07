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

#ifndef SPLITMIX_ENGINE_H__6A895005_C2EE_43A4_AE87_EAFD3532454F
#define SPLITMIX_ENGINE_H__6A895005_C2EE_43A4_AE87_EAFD3532454F


#include <nsfx/random/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A splitmix pseudo-random number generator.
 *
 * Ported from the code written by Sebastiano Vigna, 2015.
 * See http://xoshiro.di.unimi.it/splitmix64.c
 */
template<class UIntType,
         UIntType a, UIntType b, UIntType c,
         UIntType x, UIntType y, UIntType z>
class splitmix_engine
{
public:
    typedef UIntType result_type;
    BOOST_STATIC_CONSTANT(UIntType, default_seed = 1u);

    splitmix_engine(void)
    {
        seed(default_seed);
    }

    explicit splitmix_engine(result_type value)
    {
        seed(value);
    }

    splitmix_engine(const splitmix_engine& rhs) :
        s_(rhs.s_)
    {
    }

    void seed(result_type value)
    {
        s_ = value;
    }

    result_type operator()(void)
    {
        UIntType t = (s_ += x);
        t = (t ^ (t >> a)) * y;
        t = (t ^ (t >> b)) * z;
        return t ^ (t >> c);
    }

    void discard(uintmax_t n)
    {
        for (uintmax_t i = 0; i < n; ++i)
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
    UIntType s_;
};


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A splitmix64 pseudo-random number generator.
 *
 * Ported from the code written by Sebastiano Vigna, 2015.
 * See http://xoshiro.di.unimi.it/splitmix64.c
 *
 * This is a fixed-increment version of Java 8's SplittableRandom generator
 * See http://dx.doi.org/10.1145/2714064.2660195 and
 * http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html
 *
 * It is a very fast generator passing BigCrush, and it can be useful if
 * for some reason you absolutely want 64 bits of state; otherwise, the
 * author suggests to use a `xoroshiro128+` (for moderately parallel
 * computations) or a `xorshift1024*` (for massively parallel computations)
 * generator.
 *
 * This is used to seed `xoshiro_engine` and `xoroshiro_engine`.
 */
typedef splitmix_engine<uint64_t,
            30, 27, 31,
            0x9e3779b97f4a7c15ULL, 0xbf58476d1ce4e5b9ULL, 0x94d049bb133111ebULL>
        splitmix64;


NSFX_CLOSE_NAMESPACE


#endif // SPLITMIX_ENGINE_H__6A895005_C2EE_43A4_AE87_EAFD3532454F

