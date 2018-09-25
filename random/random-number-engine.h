/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-23
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef RANDOM_NUMBER_ENGINE_H__09477B58_B350_4BE7_A807_55C4E816CEE4
#define RANDOM_NUMBER_ENGINE_H__09477B58_B350_4BE7_A807_55C4E816CEE4


#include <nsfx/random/config.h>
#include <nsfx/random/i-random-number-generator.h>

#include <nsfx/random/distribution/std-uniform-int-distribution.h>
#include <nsfx/random/distribution/std-uniform-real-distribution.h>

#include <nsfx/random/distribution/std-bernoulli-distribution.h>
#include <nsfx/random/distribution/std-binomial-distribution.h>
#include <nsfx/random/distribution/std-negative-binomial-distribution.h>
#include <nsfx/random/distribution/std-geometric-distribution.h>

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

#include <boost/random/mersenne_twister.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A pseudo-random number generator.
 *
 * @tparam Engine A standard pseudo-random number generator.
 *                e.g., the random number engine in STL, the pseudo-random
 *                number generator in BOOST random number library.
 *
 * Provides \c IRandomNumberGenerator and \c IRandomNumberEngine.
 */
template<class Engine>
class StdRandomNumberEngine :
    public IRandomNumberEngine
{
    typedef StdRandomNumberEngine ThisClass;

public:
    typedef Engine  EngineType;

    /**
     * @brief Construct the engine with the default seeding value.
     */
    StdRandomNumberEngine(void) {}

    /**
     * @brief Construct the engine with a seeding value.
     */
    StdRandomNumberEngine(uint32_t seed) :
        engine_(seed)
    {}

    virtual ~StdRandomNumberEngine(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return engine_();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return EngineType::min();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return EngineType::max();
    }

    virtual double GetEntropy(void) NSFX_OVERRIDE
    {
        return 0;
    }

    virtual void Seed(uint32_t seed) NSFX_OVERRIDE
    {
        engine_.seed(seed);
    }

    virtual void Discard(uint64_t z) NSFX_OVERRIDE
    {
        engine_.discard(z);
    }

public:
    /**
     * @brief Expose the internal random number generator.
     *
     * Called internaly by the distributions created by this class, so
     * they do not have to call virtual functions to generate random numbers.
     */
    EngineType* GetRng(void)
    {
        return &engine_;
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

    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numFailures, double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<INegativeBinomialDistribution>(
            new Object<StdNegativeBinomialDistribution<ThisClass> >(
                Ptr<ThisClass>(this), numFailures, prob));
    }

    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) NSFX_OVERRIDE
    {
        BOOST_ASSERT(0 <= prob && prob <= 1);
        return Ptr<IGeometricDistribution>(
            new Object<StdGeometricDistribution<ThisClass> >(
                Ptr<ThisClass>(this), prob));
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
                Ptr<ThisClass>(this), shape, scale));
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
            CreatChiSquaredDistribution(double degreesOfFreedom) NSFX_OVERRIDE
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

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IRandomNumberGenerator)
        NSFX_INTERFACE_ENTRY(IRandomNumberEngine)
    NSFX_INTERFACE_MAP_END()

private:
    EngineType engine_;
};


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Mersenne Twister pseudo-random generator of a state size of 19937 bits.
 *
 * This is a 32-bit Mersenne Twister pseudo-random generator discovered in 1998
 * by Matsumoto and Nishimura.
 *
 * The default seed value is \c 5489.
 */
typedef StdRandomNumberEngine<boost::random::mt19937>  Mt19937Engine;

NSFX_REGISTER_CLASS(Mt19937Engine, "edu.uestc.nsfx.Mt19937Engine");


NSFX_CLOSE_NAMESPACE


#endif // RANDOM_NUMBER_ENGINE_H__09477B58_B350_4BE7_A807_55C4E816CEE4

