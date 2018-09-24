/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
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

#ifndef RANDOM_NUMBER_GENERATORS_H__624C2BF3_AEEE_49EA_9390_32229FD8CCED
#define RANDOM_NUMBER_GENERATORS_H__624C2BF3_AEEE_49EA_9390_32229FD8CCED


#include <nsfx/random/config.h>
#include <nsfx/random/i-random-number-generator.h>
#include <random>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A random number device.
 *
 * Provides \c IRandomNumberGenerator.
 */
class RandomDevice :
    public IRandomNumberGenerator
{
    typedef RandomDevice ThisClass;

public:
    typedef std::random_device  DeviceType;

    RandomDevice(void) {}

    virtual ~RandomDevice(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return device_();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return DeviceType::min();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return DeviceType::max();
    }

    virtual double GetEntropy(void) NSFX_OVERRIDE
    {
        return device_.entropy();
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IRandomNumberGenerator)
    NSFX_INTERFACE_MAP_END()

private:
    DeviceType device_;
};

NSFX_REGISTER_CLASS(RandomDevice, "edu.uestc.nsfx.RandomDevice");


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The class template for encapsulating standard random number engines.
 *
 * @tparam Engine A standard random number engine.
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
        return engine_.discard(z);
    }

    virtual Ptr<IUniformIntDistribution>
            CreateUniformIntDistribution(int32_t lb, int32_t ub) NSFX_OVERRIDE;

    virtual Ptr<IUniformRealDistribution>
            CreateUniformRealDistribution(double lb, double ub) NSFX_OVERRIDE;

    virtual Ptr<IBernoulliDistribution>
            CreateBernoulliDistribution(double prob) NSFX_OVERRIDE;

    virtual Ptr<IBinomialDistribution>
            CreateBinomialDistribution(uint32_t numTrials, double prob) NSFX_OVERRIDE;

    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numFailures, double prob) NSFX_OVERRIDE;

    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) NSFX_OVERRIDE;

    virtual Ptr<IPoissonDistribution>
            CreatePoissonDistribution(double mean) NSFX_OVERRIDE;

    virtual Ptr<IExponentialDistribution>
            CreateExponentialDistribution(double lambda) NSFX_OVERRIDE;

    virtual Ptr<IGammaDistribution>
            CreateGammaDistribution(double shape, double scale) NSFX_OVERRIDE;

    virtual Ptr<IWeibullDistribution>
            CreateWeibullDistribution(double p) NSFX_OVERRIDE;

    virtual Ptr<IExtremeValueDistribution>
            CreateExtremeValueDistribution(double location, double scale) NSFX_OVERRIDE;

    virtual Ptr<INormalDistribution>
            CreateNormalDistribution(double mean, double stddev) NSFX_OVERRIDE;

    virtual Ptr<ILognormalDistribution>
            CreateLognormalDistribution(double p) NSFX_OVERRIDE;

    virtual Ptr<IChiSquaredDistribution>
            CreatChiSquaredDistributione(double degreesOfFreedom) NSFX_OVERRIDE;

    virtual Ptr<ICauchyDistribution>
            CreateCauchyDistribution(double location, double scale) NSFX_OVERRIDE;

    virtual Ptr<IFisherFDistribution>
            CreateFisherFDistribution(double numerator, double denominator) NSFX_OVERRIDE;

    virtual Ptr<IStudentTDistribution>
            CreateStudentTDistribution(double degreesOfFreedom) NSFX_OVERRIDE;

    virtual Ptr<IDiscreteDistribution>
            CreateDiscreteDistribution(void) NSFX_OVERRIDE;

    virtual Ptr<IPiecewiseConstantDistribution>
            CreatePiecewiseConstantDistribution(void) NSFX_OVERRIDE;

    virtual Ptr<IPiecewiseLinearDistribution>
            CreatePiecewiseLinearDistribution(void) NSFX_OVERRIDE;

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
 * @brief A minimal standard linear congruential pseudo-random generator.
 *
 * This linear congruential pseudo-random number generator is discovered
 * in 1969 by Lewis, Goodman and Miller, which was adopted as the
 * "Minimal standard" in 1988 by Park and Miller.
 *
 * The default seed value is \c 1.
 */
typedef StdRandomNumberEngine<std::minstd_rand0>  Minstd0Engine;

NSFX_REGISTER_CLASS(Minstd0Engine, "edu.uestc.nsfx.Minstd0Engine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A minimal standard linear congruential pseudo-random generator.
 *
 * This linear congruential pseudo-random number generator is a newer
 * "Minimum standard", which was recommended by Park, Miller and Stockmeyer
 * in 1993.
 *
 * The default seed value is \c 1.
 */
typedef StdRandomNumberEngine<std::minstd_rand>  MinstdEngine;

NSFX_REGISTER_CLASS(MinstdEngine, "edu.uestc.nsfx.MinstdEngine");


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
typedef StdRandomNumberEngine<std::mt19937>  Mt19937Engine;

NSFX_REGISTER_CLASS(Mt19937Engine, "edu.uestc.nsfx.Mt19937Engine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A 24-bit RANLUX pseudo-random generator.
 *
 * This is a 24-bit RANLUX pseudo-random generator discovered in 1994 by
 * Martin Lüscher and Fred James.
 *
 * It is a subtract-with-carry pseudo-random generator of 24-bit numbers with
 * accelerated advancement.
 *
 * The default seed value is \c 19780503.
 */
typedef StdRandomNumberEngine<std::ranlux24>  Ranlux24Engine;

NSFX_REGISTER_CLASS(Ranlux24Engine, "edu.uestc.nsfx.Ranlux24Engine");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Knuth-B pseudo-random generator.
 *
 * It returns shuffled sequences generated with the simple pseudo-random number
 * generator engine \c std::minstd_rand0.
 *
 * The default seed value is \c 1.
 */
typedef StdRandomNumberEngine<std::knuth_b>  KnuthBEngine;

NSFX_REGISTER_CLASS(KnuthBEngine, "edu.uestc.nsfx.KnuthBEngine");


NSFX_CLOSE_NAMESPACE


#endif // RANDOM_NUMBER_GENERATORS_H__624C2BF3_AEEE_49EA_9390_32229FD8CCED

