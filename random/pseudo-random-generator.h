/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PSEUDO_RANDOM_GENERATOR_H__F0D2AE39_EDCE_4DD2_ABCE_1BE52AF367B2
#define PSEUDO_RANDOM_GENERATOR_H__F0D2AE39_EDCE_4DD2_ABCE_1BE52AF367B2


#include <nsfx/random/config.h>

#include <nsfx/random/i-random-uint32-generator.h>
#include <nsfx/random/i-random-uint64-generator.h>
#include <nsfx/random/i-random-float-generator.h>
#include <nsfx/random/i-random-double-generator.h>

#include <nsfx/random/i-pseudo-random-engine.h>

#include <nsfx/random/i-random-distribution-generator.h>

#include <nsfx/random/distribution/std-uniform-int-distribution.h>
#include <nsfx/random/distribution/std-uniform-real-distribution.h>

#include <nsfx/random/distribution/std-bernoulli-distribution.h>
#include <nsfx/random/distribution/std-binomial-distribution.h>
#include <nsfx/random/distribution/std-geometric-distribution.h>
#include <nsfx/random/distribution/std-negative-binomial-distribution.h>

#include <nsfx/random/distribution/std-poisson-distribution.h>
#include <nsfx/random/distribution/std-exponential-distribution.h>
#include <nsfx/random/distribution/std-gamma-distribution.h>
#include <nsfx/random/distribution/std-weibull-distribution.h>
#include <nsfx/random/distribution/std-extreme-value-distribution.h>

#include <nsfx/random/distribution/std-normal-distribution.h>
#include <nsfx/random/distribution/std-lognormal-distribution.h>
#include <nsfx/random/distribution/std-chi-squared-distribution.h>
#include <nsfx/random/distribution/std-cauchy-distribution.h>
#include <nsfx/random/distribution/std-fisher-f-distribution.h>
#include <nsfx/random/distribution/std-student-t-distribution.h>

#include <nsfx/random/distribution/std-discrete-distribution.h>
#include <nsfx/random/distribution/std-piecewise-constant-distribution.h>
#include <nsfx/random/distribution/std-piecewise-linear-distribution.h>

#include <nsfx/random/distribution/std-triangle-distribution.h>

#include <nsfx/random/engine/xoroshiro-engine.h>
#include <nsfx/random/engine/xoshiro-engine.h>
#include <boost/random/mersenne_twister.hpp>

#include <type_traits> // conditional, is_integral, is_floating_point, is_same


NSFX_OPEN_NAMESPACE


namespace random {
namespace aux {


template<class StdRng>
struct RandomNumberGeneratorTraits
{
    typedef typename StdRng::result_type  StdRngResultType;

    typedef
    typename std::conditional<
        std::is_integral<StdRngResultType>::value && sizeof (StdRngResultType) == sizeof (uint32_t),
        uint32_t,
        typename std::conditional<
            std::is_integral<StdRngResultType>::value && sizeof (StdRngResultType) == sizeof (uint64_t),
            uint64_t,
            typename std::conditional<
                std::is_floating_point<StdRngResultType>::value && sizeof (StdRngResultType) == sizeof (float),
                float,
                typename std::conditional<
                    std::is_floating_point<StdRngResultType>::value && sizeof (StdRngResultType) == sizeof (double),
                    double,
                    void
                >::type
            >::type
        >::type
    >::type ResultType;

    static_assert(!std::is_same<ResultType, void>::value,
                  "The StdRng is unsupported.");

    typedef
    typename std::conditional<
        std::is_same<ResultType, uint32_t>::value,
        IRandomUInt32Generator,
        typename std::conditional<
            std::is_same<ResultType, uint64_t>::value,
            IRandomUInt64Generator,
            typename std::conditional<
                std::is_same<ResultType, float>::value,
                IRandomFloatGenerator,
                typename std::conditional<
                    std::is_same<ResultType, double>::value,
                    IRandomDoubleGenerator,
                    void
                >::type
            >::type
        >::type
    >::type InterfaceType;

    static_assert(!std::is_same<InterfaceType, void>::value,
                  "The StdRng is unsupported.");
};


} // namespace aux
} // namespace random


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete pseudo-random number generator.
 *
 * @tparam StdRng A standard pseudo-random number generator.
 *                e.g., the random number engine in STL, the pseudo-random
 *                number generator in BOOST Random Number library.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomUInt32Generator (if \c StdRng::result_type is 32-bit unsigned integer)
 * * \c IRandomUInt64Generator (if \c StdRng::result_type is 64-bit unsigned integer)
 * * \c IRandomFloatGenerator  (if \c StdRng::result_type is float)
 * * \c IRandomDoubleGenerator (if \c StdRng::result_type is double)
 * * \c IRandomDistributionGenerator
 */
template<class StdRng>
class PseudoRandomEngine :
    public IPseudoRandomEngine,
    public random::aux::RandomNumberGeneratorTraits<StdRng>::InterfaceType,
    public IRandomDistributionGenerator
{
    typedef PseudoRandomEngine ThisClass;

public:
    typedef StdRng RngType;
    typedef typename random::aux::RandomNumberGeneratorTraits<StdRng>::ResultType    ResultType;
    typedef typename random::aux::RandomNumberGeneratorTraits<StdRng>::InterfaceType InterfaceType;

    /**
     * @brief Construct the engine with the default seeding value.
     */
    PseudoRandomEngine(void) {}

    /**
     * @brief Construct the engine with a seeding value.
     */
    PseudoRandomEngine(uint64_t value) :
        rng_(value)
    {}

    virtual ~PseudoRandomEngine(void) {}

    // IRandomUInt32Generator or
    // IRandomUInt64Generator or
    // IRandomFloatGenerator  or
    // IRandomDoubleGenerator
    virtual ResultType Generate(void) NSFX_OVERRIDE
    {
        return rng_();
    }

    virtual ResultType GetMinValue(void) NSFX_OVERRIDE
    {
        return (RngType::min)();
    }

    virtual ResultType GetMaxValue(void) NSFX_OVERRIDE
    {
        return (RngType::max)();
    }

    virtual double GetEntropy(void) NSFX_OVERRIDE
    {
        return 0;
    }

    // IPseudoRandomEngine
    virtual void Seed(uint64_t seed) NSFX_OVERRIDE
    {
        rng_.seed(seed);
    }

    virtual void Discard(uint64_t z) NSFX_OVERRIDE
    {
        rng_.discard(z);
    }

    // IRandomDistributionGenerator
public:
    /**
     * @brief Expose the internal random number generator.
     *
     * Called internaly by the distributions created by this class, so
     * they do not have to call virtual functions to generate random numbers.
     */
    RngType& GetRng(void)
    {
        return rng_;
    }

public:
    virtual Ptr<IUniformIntDistribution>
            CreateUniformIntDistribution(int32_t lb, int32_t ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformIntDistribution>(
            new Object<StdUniformIntDistribution<ThisClass> >(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual Ptr<IUniformRealDistribution>
            CreateUniformRealDistribution(double lb, double ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformRealDistribution>(
            new Object<StdUniformRealDistribution<ThisClass> >(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual Ptr<IBernoulliDistribution>
            CreateBernoulliDistribution(double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IBernoulliDistribution>(
            new Object<StdBernoulliDistribution<ThisClass> >(
                Ptr<ThisClass>(this), prob));
    }

    virtual Ptr<IBinomialDistribution>
            CreateBinomialDistribution(uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IBinomialDistribution>(
            new Object<StdBinomialDistribution<ThisClass> >(
                Ptr<ThisClass>(this), numTrials, prob));
    }

    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IGeometricDistribution>(
            new Object<StdGeometricDistribution<ThisClass> >(
                Ptr<ThisClass>(this), prob));
    }

    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<INegativeBinomialDistribution>(
            new Object<StdNegativeBinomialDistribution<ThisClass> >(
                Ptr<ThisClass>(this), numTrials, prob));
    }

    virtual Ptr<IPoissonDistribution>
            CreatePoissonDistribution(double mean) NSFX_OVERRIDE
    {
        BOOST_ASSERT(mean > 0);
        return Ptr<IPoissonDistribution>(
            new Object<StdPoissonDistribution<ThisClass> >(
                Ptr<ThisClass>(this), mean));
    }

    virtual Ptr<IExponentialDistribution>
            CreateExponentialDistribution(double lambda) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lambda > 0);
        return Ptr<IExponentialDistribution>(
            new Object<StdExponentialDistribution<ThisClass> >(
                Ptr<ThisClass>(this), lambda));
    }

    virtual Ptr<IGammaDistribution>
            CreateGammaDistribution(double shape, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(shape > 0);
        BOOST_ASSERT(scale > 0);
        return Ptr<IGammaDistribution>(
            new Object<StdGammaDistribution<ThisClass> >(
                Ptr<ThisClass>(this), shape, scale));
    }

    virtual Ptr<IWeibullDistribution>
            CreateWeibullDistribution(double shape, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(shape > 0);
        BOOST_ASSERT(scale > 0);
        return Ptr<IWeibullDistribution>(
            new Object<StdWeibullDistribution<ThisClass> >(
                Ptr<ThisClass>(this), shape, scale));
    }

    virtual Ptr<IExtremeValueDistribution>
            CreateExtremeValueDistribution(double location, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(scale > 0);
        return Ptr<IExtremeValueDistribution>(
            new Object<StdExtremeValueDistribution<ThisClass> >(
                Ptr<ThisClass>(this), location, scale));
    }

    virtual Ptr<INormalDistribution>
            CreateNormalDistribution(double mean, double stddev) NSFX_OVERRIDE
    {
        BOOST_ASSERT(stddev > 0);
        return Ptr<INormalDistribution>(
            new Object<StdNormalDistribution<ThisClass> >(
                Ptr<ThisClass>(this), mean, stddev));
    }

    virtual Ptr<ILognormalDistribution>
            CreateLognormalDistribution(double mean, double stddev) NSFX_OVERRIDE
    {
        BOOST_ASSERT(stddev > 0);
        return Ptr<ILognormalDistribution>(
            new Object<StdLognormalDistribution<ThisClass> >(
                Ptr<ThisClass>(this), mean, stddev));
    }

    virtual Ptr<IChiSquaredDistribution>
            CreateChiSquaredDistribution(double degreesOfFreedom) NSFX_OVERRIDE
    {
        BOOST_ASSERT(degreesOfFreedom > 0);
        return Ptr<IChiSquaredDistribution>(
            new Object<StdChiSquaredDistribution<ThisClass> >(
                Ptr<ThisClass>(this), degreesOfFreedom));
    }

    virtual Ptr<ICauchyDistribution>
            CreateCauchyDistribution(double location, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(scale > 0);
        return Ptr<ICauchyDistribution>(
            new Object<StdCauchyDistribution<ThisClass> >(
                Ptr<ThisClass>(this), location, scale));
    }

    virtual Ptr<IFisherFDistribution>
            CreateFisherFDistribution(double numerator, double denominator) NSFX_OVERRIDE
    {
        BOOST_ASSERT(numerator > 0);
        BOOST_ASSERT(denominator > 0);
        return Ptr<IFisherFDistribution>(
            new Object<StdFisherFDistribution<ThisClass> >(
                Ptr<ThisClass>(this), numerator, denominator));
    }

    virtual Ptr<IStudentTDistribution>
            CreateStudentTDistribution(double degreesOfFreedom) NSFX_OVERRIDE
    {
        BOOST_ASSERT(degreesOfFreedom > 0);
        return Ptr<IStudentTDistribution>(
            new Object<StdStudentTDistribution<ThisClass> >(
                Ptr<ThisClass>(this), degreesOfFreedom));
    }

    virtual Ptr<IDiscreteDistribution>
            CreateDiscreteDistribution(
                Ptr<IDiscreteDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IDiscreteDistribution>(
            new Object<StdDiscreteDistribution<ThisClass> >(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<IPiecewiseConstantDistribution>
            CreatePiecewiseConstantDistribution(
                Ptr<IPiecewiseConstantDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IPiecewiseConstantDistribution>(
            new Object<StdPiecewiseConstantDistribution<ThisClass> >(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<IPiecewiseLinearDistribution>
            CreatePiecewiseLinearDistribution(
                Ptr<IPiecewiseLinearDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IPiecewiseLinearDistribution>(
            new Object<StdPiecewiseLinearDistribution<ThisClass> >(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<ITriangleDistribution>
            CreateTriangleDistribution(double a, double b, double c) NSFX_OVERRIDE
    {
        return Ptr<ITriangleDistribution>(
            new Object<StdTriangleDistribution<ThisClass> >(
                Ptr<ThisClass>(this), a, b, c));
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPseudoRandomEngine)
        NSFX_INTERFACE_ENTRY(InterfaceType)
        NSFX_INTERFACE_ENTRY(IRandomDistributionGenerator)
    NSFX_INTERFACE_MAP_END()

private:
    RngType rng_;
};


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro256+ pseudo-random floating-point number generator.
 *
 * This is a pseudo-random generator discovered by David Blackman and
 * Sebastiano Vigna in 2018.
 *
 * It generates double precision floating-point numbers in the range of
 * <i>[0, 1)</i> .
 *
 * It has a state size of \c 256 bits, and a period of <code>2^256 - 1</code>.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is \c 1u.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomDoubleGenerator
 * * \c IRandomDistributionGenerator
 */
typedef PseudoRandomEngine<nsfx::xoshiro256plus_01>  Xoshiro256Plus01Engine;

NSFX_REGISTER_CLASS(Xoshiro256Plus01Engine, "edu.uestc.nsfx.Xoshiro256Plus01Engine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro256** pseudo-random 64-bit integer number generator.
 *
 * This is a pseudo-random generator discovered by David Blackman and
 * Sebastiano Vigna in 2018.
 *
 * It generates 64-bit unsigned integer numbers.
 *
 * It has a state size of \c 256 bits, and a period of <code>2^256 - 1</code>.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is \c 1u.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomUInt64Generator
 * * \c IRandomDistributionGenerator
 */
typedef PseudoRandomEngine<nsfx::xoshiro256starstar>  Xoshiro256StarstarEngine;

NSFX_REGISTER_CLASS(Xoshiro256StarstarEngine, "edu.uestc.nsfx.Xoshiro256StarstarEngine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro128+ pseudo-random floating-point number generator.
 *
 * This is a pseudo-random generator discovered by David Blackman and
 * Sebastiano Vigna in 2018.
 *
 * It generates single precision floating-point numbers in the range of
 * <i>[0, 1)</i>.
 *
 * It has a state size of \c 128 bits, and a period of <code>2^128 - 1</code>.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is \c 1u.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomFloatGenerator
 * * \c IRandomDistributionGenerator
 */
typedef PseudoRandomEngine<nsfx::xoshiro128plus_01>  Xoshiro128Plus01Engine;

NSFX_REGISTER_CLASS(Xoshiro128Plus01Engine, "edu.uestc.nsfx.Xoshiro128Plus01Engine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A xoshiro128** pseudo-random 32-bit integer number generator.
 *
 * This is a pseudo-random generator discovered by David Blackman and
 * Sebastiano Vigna in 2018.
 *
 * It generates 32-bit unsigned integer numbers.
 *
 * It has a state size of \c 128 bits, and a period of <code>2^128 - 1</code>.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is \c 1u.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomUInt32Generator
 * * \c IRandomDistributionGenerator
 */
typedef PseudoRandomEngine<nsfx::xoshiro128starstar>  Xoshiro128StarstarEngine;

NSFX_REGISTER_CLASS(Xoshiro128StarstarEngine, "edu.uestc.nsfx.Xoshiro128StarstarEngine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Mersenne Twister pseudo-random integer number generator.
 *
 * This is a 32-bit Mersenne Twister pseudo-random generator discovered in 1998
 * by Matsumoto and Nishimura.
 *
 * It generates 32-bit unsigned integer numbers.
 *
 * It has a state size of \c 19937 bits, and a period of <code>2^19937 - 1</code>.
 *
 * The seed value is a 32-bit unsigned integer.
 * The default seed value is \c 5489u.
 *
 * Provided interfaces:
 * * \c IPseudoRandomEngine
 * * \c IRandomUInt32Generator
 * * \c IRandomDistributionGenerator
 */
typedef PseudoRandomEngine<boost::random::mt19937>  Mt19937Engine;

NSFX_REGISTER_CLASS(Mt19937Engine, "edu.uestc.nsfx.Mt19937Engine");


NSFX_CLOSE_NAMESPACE


#endif // PSEUDO_RANDOM_GENERATOR_H__F0D2AE39_EDCE_4DD2_ABCE_1BE52AF367B2

