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

#ifndef RANDOM_NUMBER_DEVICE_H__AC01075E_AD8C_460E_AB22_0D03F32FE6CF
#define RANDOM_NUMBER_DEVICE_H__AC01075E_AD8C_460E_AB22_0D03F32FE6CF


#include <nsfx/random/config.h>
#include <nsfx/random/i-random-number-generator.h>
#include <random>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A random number device.
 *
 * This class employs \c std::random_device instead of
 * \c boost::random::random_device.
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
    NSFX_INTERFACE_MAP_END()

private:
    DeviceType device_;
};

NSFX_REGISTER_CLASS(RandomDevice, "edu.uestc.nsfx.RandomDevice");


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


#endif // RANDOM_NUMBER_DEVICE_H__AC01075E_AD8C_460E_AB22_0D03F32FE6CF

