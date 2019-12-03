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
#include <nsfx/random/i-random-double-generator.h>

#include <nsfx/random/i-pseudo-random-engine.h>

#include <nsfx/random/i-random.h>

#include <nsfx/random/distribution/std-uniform-uint32-distribution.h>
#include <nsfx/random/distribution/std-uniform-int32-distribution.h>
#include <nsfx/random/distribution/std-uniform-uint64-distribution.h>
#include <nsfx/random/distribution/std-uniform-int64-distribution.h>
#include <nsfx/random/distribution/std-uniform-double-distribution.h>
#include <nsfx/random/distribution/std-uniform-float-distribution.h>

#include <nsfx/random/distribution/std-bernoulli-distribution.h>
#include <nsfx/random/distribution/std-binomial-distribution.h>
#include <nsfx/random/distribution/std-geometric-distribution.h>
#include <nsfx/random/distribution/std-negative-binomial-distribution.h>

#include <nsfx/random/distribution/std-poisson-distribution.h>
#include <nsfx/random/distribution/std-exponential-distribution.h>
#include <nsfx/random/distribution/std-gamma-distribution.h>
#include <nsfx/random/distribution/std-weibull-distribution.h>
#include <nsfx/random/distribution/std-extreme-value-distribution.h>
#include <nsfx/random/distribution/std-beta-distribution.h>
#include <nsfx/random/distribution/std-laplace-distribution.h>

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


////////////////////////////////////////////////////////////////////////////////
namespace aux { /*{{{*/

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
                std::is_floating_point<StdRngResultType>::value && sizeof (StdRngResultType) <= sizeof (double),
                double,
                void
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
                std::is_same<ResultType, double>::value,
                IRandomDoubleGenerator,
                void
            >::type
        >::type
    >::type InterfaceType;

    static_assert(!std::is_same<InterfaceType, void>::value,
                  "The StdRng is unsupported.");
};


} // namespace aux /*}}}*/


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
 * * `IPseudoRandomEngine`
 * * `IRandomUInt32Generator` (if `StdRng::result_type` is 32-bit unsigned integer)
 * * `IRandomUInt64Generator` (if `StdRng::result_type` is 64-bit unsigned integer)
 * * `IRandomDoubleGenerator` (if `StdRng::result_type` is float or double)
 * * `IRandom`
 */
template<class StdRng>
class PseudoRandomEngine :
    public IPseudoRandomEngine,
    public aux::RandomNumberGeneratorTraits<StdRng>::InterfaceType,
    public IRandom
{
    typedef PseudoRandomEngine ThisClass;

public:
    typedef StdRng RngType;
    typedef typename aux::RandomNumberGeneratorTraits<StdRng>::ResultType    ResultType;
    typedef typename aux::RandomNumberGeneratorTraits<StdRng>::InterfaceType InterfaceType;

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
    virtual void Seed(uint32_t seed) NSFX_OVERRIDE
    {
        rng_.seed(seed);
    }

    virtual void Discard(uint64_t z) NSFX_OVERRIDE
    {
        rng_.discard(z);
    }

    // IRandom
public:
    /**
     * @brief Expose the internal random number generator.
     *
     * Called internaly by the distributions created by this class, so
     * they do not have to call virtual functions to generate random numbers.
     */
    RngType& GetRng(void) BOOST_NOEXCEPT
    {
        return rng_;
    }

public:
    virtual Ptr<IUniformUint32Distribution>
            CreateUniformUint32Distribution(uint32_t lb, uint32_t ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformUint32Distribution>(
            new Object<StdUniformUint32Distribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual uint32_t GenerateUniformUint32(uint32_t lb, uint32_t ub) NSFX_OVERRIDE
    {
        boost::random::uniform_int_distribution<uint32_t> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual Ptr<IUniformInt32Distribution>
            CreateUniformInt32Distribution(int32_t lb, int32_t ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformInt32Distribution>(
            new Object<StdUniformInt32Distribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual int32_t GenerateUniformInt32(int32_t lb, int32_t ub) NSFX_OVERRIDE
    {
        boost::random::uniform_int_distribution<int32_t> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual Ptr<IUniformUint64Distribution>
            CreateUniformUint64Distribution(uint64_t lb, uint64_t ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformUint64Distribution>(
            new Object<StdUniformUint64Distribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual uint64_t GenerateUniformUint64(uint64_t lb, uint64_t ub) NSFX_OVERRIDE
    {
        boost::random::uniform_int_distribution<uint64_t> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual Ptr<IUniformInt64Distribution>
            CreateUniformInt64Distribution(int64_t lb, int64_t ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformInt64Distribution>(
            new Object<StdUniformInt64Distribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual int64_t GenerateUniformInt64(int64_t lb, int64_t ub) NSFX_OVERRIDE
    {
        boost::random::uniform_int_distribution<int64_t> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual Ptr<IUniformDoubleDistribution>
            CreateUniformDoubleDistribution(double lb, double ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformDoubleDistribution>(
            new Object<StdUniformDoubleDistribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual double GenerateUniformDouble(double lb, double ub) NSFX_OVERRIDE
    {
        boost::random::uniform_real_distribution<double> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual double GenerateUniform01(void) NSFX_OVERRIDE
    {
        boost::random::uniform_01<double> dist_;
        return dist_(rng_);
    }

    virtual Ptr<IUniformFloatDistribution>
            CreateUniformFloatDistribution(float lb, float ub) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lb <= ub);
        return Ptr<IUniformFloatDistribution>(
            new Object<StdUniformFloatDistribution<ThisClass>>(
                Ptr<ThisClass>(this), lb, ub));
    }

    virtual float GenerateUniformFloat(float lb, float ub) NSFX_OVERRIDE
    {
        boost::random::uniform_real_distribution<float> dist_(lb, ub);
        return dist_(rng_);
    }

    virtual Ptr<IBernoulliDistribution>
            CreateBernoulliDistribution(double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IBernoulliDistribution>(
            new Object<StdBernoulliDistribution<ThisClass>>(
                Ptr<ThisClass>(this), prob));
    }

    virtual bool GenerateBernoulli(double prob) NSFX_OVERRIDE
    {
        boost::random::bernoulli_distribution<double> dist_(prob);
        return dist_(rng_);
    }

    virtual Ptr<IBinomialDistribution>
            CreateBinomialDistribution(uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IBinomialDistribution>(
            new Object<StdBinomialDistribution<ThisClass>>(
                Ptr<ThisClass>(this), numTrials, prob));
    }

    virtual uint32_t GenerateBinomial(uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        boost::random::binomial_distribution<int32_t, double>
               dist_(numTrials, prob);
        return dist_(rng_);
    }

    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<INegativeBinomialDistribution>(
            new Object<StdNegativeBinomialDistribution<ThisClass>>(
                Ptr<ThisClass>(this), numTrials, prob));
    }

    virtual uint32_t GenerateNegativeBinomial(uint32_t numTrials, double prob) NSFX_OVERRIDE
    {
        boost::random::negative_binomial_distribution<uint32_t, double>
               dist_(numTrials, prob);
        return dist_(rng_);
    }

    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IGeometricDistribution>(
            new Object<StdGeometricDistribution<ThisClass>>(
                Ptr<ThisClass>(this), prob));
    }

    virtual uint32_t GenerateGeometric(double prob) NSFX_OVERRIDE
    {
        boost::random::geometric_distribution<uint32_t, double> dist_(prob);
        return dist_(rng_);
    }

    virtual Ptr<IPoissonDistribution>
            CreatePoissonDistribution(double mean) NSFX_OVERRIDE
    {
        BOOST_ASSERT(mean > 0);
        return Ptr<IPoissonDistribution>(
            new Object<StdPoissonDistribution<ThisClass>>(
                Ptr<ThisClass>(this), mean));
    }

    virtual uint32_t GeneratePoisson(double mean) NSFX_OVERRIDE
    {
        boost::random::poisson_distribution<uint32_t, double> dist_(mean);
        return dist_(rng_);
    }

    virtual Ptr<IExponentialDistribution>
            CreateExponentialDistribution(double lambda) NSFX_OVERRIDE
    {
        BOOST_ASSERT(lambda > 0);
        return Ptr<IExponentialDistribution>(
            new Object<StdExponentialDistribution<ThisClass>>(
                Ptr<ThisClass>(this), lambda));
    }

    virtual double GenerateExponential(double lambda) NSFX_OVERRIDE
    {
        boost::random::exponential_distribution<double> dist_(lambda);
        return dist_(rng_);
    }

    virtual Ptr<IGammaDistribution>
            CreateGammaDistribution(double shape, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(shape > 0);
        BOOST_ASSERT(scale > 0);
        return Ptr<IGammaDistribution>(
            new Object<StdGammaDistribution<ThisClass>>(
                Ptr<ThisClass>(this), shape, scale));
    }

    virtual double GenerateGamma(double shape, double scale) NSFX_OVERRIDE
    {
        boost::random::gamma_distribution<double> dist_(shape, scale);
        return dist_(rng_);
    }

    virtual Ptr<IWeibullDistribution>
            CreateWeibullDistribution(double shape, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(shape > 0);
        BOOST_ASSERT(scale > 0);
        return Ptr<IWeibullDistribution>(
            new Object<StdWeibullDistribution<ThisClass>>(
                Ptr<ThisClass>(this), shape, scale));
    }

    virtual double GenerateWeibull(double shape, double scale) NSFX_OVERRIDE
    {
        boost::random::weibull_distribution<double> dist_(shape, scale);
        return dist_(rng_);
    }

    virtual Ptr<IExtremeValueDistribution>
            CreateExtremeValueDistribution(double location, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(scale > 0);
        return Ptr<IExtremeValueDistribution>(
            new Object<StdExtremeValueDistribution<ThisClass>>(
                Ptr<ThisClass>(this), location, scale));
    }

    virtual double GenerateExtremeValue(double location, double scale) NSFX_OVERRIDE
    {
        boost::random::extreme_value_distribution<double> dist_(location, scale);
        return dist_(rng_);
    }

    virtual Ptr<IBetaDistribution>
            CreateBetaDistribution(double alpha, double beta) NSFX_OVERRIDE
    {
        BOOST_ASSERT(alpha > 0);
        BOOST_ASSERT(beta > 0);
        return Ptr<IBetaDistribution>(
            new Object<StdBetaDistribution<ThisClass>>(
                Ptr<ThisClass>(this), alpha, beta));
    }

    virtual double GenerateBeta(double alpha, double beta) NSFX_OVERRIDE
    {
        boost::random::beta_distribution<double> dist_(alpha, beta);
        return dist_(rng_);
    }

    virtual Ptr<ILaplaceDistribution>
            CreateLaplaceDistribution(double mean, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(mean > 0);
        BOOST_ASSERT(scale > 0);
        return Ptr<ILaplaceDistribution>(
            new Object<StdLaplaceDistribution<ThisClass>>(
                Ptr<ThisClass>(this), mean, scale));
    }

    virtual double GenerateLaplace(double mean, double scale) NSFX_OVERRIDE
    {
        boost::random::laplace_distribution<double> dist_(mean, scale);
        return dist_(rng_);
    }

    virtual Ptr<INormalDistribution>
            CreateNormalDistribution(double mean, double stddev) NSFX_OVERRIDE
    {
        BOOST_ASSERT(stddev > 0);
        return Ptr<INormalDistribution>(
            new Object<StdNormalDistribution<ThisClass>>(
                Ptr<ThisClass>(this), mean, stddev));
    }

    virtual double GenerateNormal(double mean, double stddev) NSFX_OVERRIDE
    {
        boost::random::normal_distribution<double> dist_(mean, stddev);
        return dist_(rng_);
    }

    virtual Ptr<ILognormalDistribution>
            CreateLognormalDistribution(double mean, double stddev) NSFX_OVERRIDE
    {
        BOOST_ASSERT(stddev > 0);
        return Ptr<ILognormalDistribution>(
            new Object<StdLognormalDistribution<ThisClass>>(
                Ptr<ThisClass>(this), mean, stddev));
    }

    virtual double GenerateLognormal(double mean, double stddev) NSFX_OVERRIDE
    {
        boost::random::lognormal_distribution<double> dist_(mean, stddev);
        return dist_(rng_);
    }

    virtual Ptr<IChiSquaredDistribution>
            CreateChiSquaredDistribution(double degreesOfFreedom) NSFX_OVERRIDE
    {
        BOOST_ASSERT(degreesOfFreedom > 0);
        return Ptr<IChiSquaredDistribution>(
            new Object<StdChiSquaredDistribution<ThisClass>>(
                Ptr<ThisClass>(this), degreesOfFreedom));
    }

    virtual double GenerateChiSquared(double degreesOfFreedom) NSFX_OVERRIDE
    {
        boost::random::chi_squared_distribution<double> dist_(degreesOfFreedom);
        return dist_(rng_);
    }

    virtual Ptr<ICauchyDistribution>
            CreateCauchyDistribution(double location, double scale) NSFX_OVERRIDE
    {
        BOOST_ASSERT(scale > 0);
        return Ptr<ICauchyDistribution>(
            new Object<StdCauchyDistribution<ThisClass>>(
                Ptr<ThisClass>(this), location, scale));
    }

    virtual double GenerateCauchy(double location, double scale) NSFX_OVERRIDE
    {
        boost::random::cauchy_distribution<double> dist_(location, scale);
        return dist_(rng_);
    }

    virtual Ptr<IFisherFDistribution>
            CreateFisherFDistribution(double numerator, double denominator) NSFX_OVERRIDE
    {
        BOOST_ASSERT(numerator > 0);
        BOOST_ASSERT(denominator > 0);
        return Ptr<IFisherFDistribution>(
            new Object<StdFisherFDistribution<ThisClass>>(
                Ptr<ThisClass>(this), numerator, denominator));
    }

    virtual double GenerateFisherF(double numerator, double denominator) NSFX_OVERRIDE
    {
        boost::random::fisher_f_distribution<double> dist_(numerator, denominator);
        return dist_(rng_);
    }

    virtual Ptr<IStudentTDistribution>
            CreateStudentTDistribution(double degreesOfFreedom) NSFX_OVERRIDE
    {
        BOOST_ASSERT(degreesOfFreedom > 0);
        return Ptr<IStudentTDistribution>(
            new Object<StdStudentTDistribution<ThisClass>>(
                Ptr<ThisClass>(this), degreesOfFreedom));
    }

    virtual double GenerateStudentT(double degreesOfFreedom) NSFX_OVERRIDE
    {
        boost::random::student_t_distribution<double> dist_(degreesOfFreedom);
        return dist_(rng_);
    }

    virtual Ptr<IDiscreteDistribution>
            CreateDiscreteDistribution(
                Ptr<IDiscreteDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IDiscreteDistribution>(
            new Object<StdDiscreteDistribution<ThisClass>>(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<IPiecewiseConstantDistribution>
            CreatePiecewiseConstantDistribution(
                Ptr<IPiecewiseConstantDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IPiecewiseConstantDistribution>(
            new Object<StdPiecewiseConstantDistribution<ThisClass>>(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<IPiecewiseLinearDistribution>
            CreatePiecewiseLinearDistribution(
                Ptr<IPiecewiseLinearDistributionParam> param) NSFX_OVERRIDE
    {
        return Ptr<IPiecewiseLinearDistribution>(
            new Object<StdPiecewiseLinearDistribution<ThisClass>>(
                Ptr<ThisClass>(this), param));
    }

    virtual Ptr<ITriangleDistribution>
            CreateTriangleDistribution(double a, double b, double c) NSFX_OVERRIDE
    {
        return Ptr<ITriangleDistribution>(
            new Object<StdTriangleDistribution<ThisClass>>(
                Ptr<ThisClass>(this), a, b, c));
    }

    virtual double GenerateTriangle(double a, double b, double c) NSFX_OVERRIDE
    {
        boost::random::triangle_distribution<double> dist_(a, b, c);
        return dist_(rng_);
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPseudoRandomEngine)
        NSFX_INTERFACE_ENTRY(InterfaceType)
        NSFX_INTERFACE_ENTRY(IRandom)
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
 * It generates double precision floating-point numbers in the range of `[0, 1)`.
 *
 * It has a state size of `256` bits, and a period of `2^256 - 1`.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is `1u`.
 *
 * Provided interfaces:
 * * `IPseudoRandomEngine`
 * * `IRandomDoubleGenerator`
 * * `IRandom`
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
 * It has a state size of `256` bits, and a period of `2^256 - 1`.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is `1u`.
 *
 * Provided interfaces:
 * * `IPseudoRandomEngine`
 * * `IRandomUInt64Generator`
 * * `IRandom`
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
 * It generates single precision floating-point numbers in the range of `[0, 1)`.
 *
 * It has a state size of `128` bits, and a period of `2^128 - 1`.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is `1u`.
 *
 * Provided interfaces:
 * * `IPseudoRandomEngine`
 * * `IRandomFloatGenerator`
 * * `IRandom`
 */
typedef PseudoRandomEngine<nsfx::xoshiro128plus_01>  Xoshiro128Plus01Engine;

NSFX_REGISTER_CLASS(Xoshiro128Plus01Engine,
					"edu.uestc.nsfx.Xoshiro128Plus01Engine");


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
 * It has a state size of `128` bits, and a period of `2^128 - 1`.
 *
 * The seed value is a 64-bit unsigned integer.
 * The default seed value is `1u`.
 *
 * Provided interfaces:
 * * `IPseudoRandomEngine`
 * * `IRandomUInt32Generator`
 * * `IRandom`
 */
typedef PseudoRandomEngine<nsfx::xoshiro128starstar>  Xoshiro128StarstarEngine;

NSFX_REGISTER_CLASS(Xoshiro128StarstarEngine,
					"edu.uestc.nsfx.Xoshiro128StarstarEngine");


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
 * It has a state size of `19937` bits, and a period of `2^19937 - 1`.
 *
 * The seed value is a 32-bit unsigned integer.
 * The default seed value is `5489u`.
 *
 * Provided interfaces:
 * * `IPseudoRandomEngine`
 * * `IRandomUInt32Generator`
 * * `IRandom`
 */
typedef PseudoRandomEngine<boost::random::mt19937>  Mt19937Engine;

NSFX_REGISTER_CLASS(Mt19937Engine, "edu.uestc.nsfx.Mt19937Engine");


NSFX_CLOSE_NAMESPACE


#endif // PSEUDO_RANDOM_GENERATOR_H__F0D2AE39_EDCE_4DD2_ABCE_1BE52AF367B2

