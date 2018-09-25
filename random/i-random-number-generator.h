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

#ifndef I_RANDOM_NUMBER_GENERATOR_H__E6D026FA_399E_402F_BA49_D9AD1E703A1B
#define I_RANDOM_NUMBER_GENERATOR_H__E6D026FA_399E_402F_BA49_D9AD1E703A1B


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>

#include <nsfx/random/distribution/i-uniform-int-distribution.h>
#include <nsfx/random/distribution/i-uniform-real-distribution.h>

#include <nsfx/random/distribution/i-bernoulli-distribution.h>
#include <nsfx/random/distribution/i-binomial-distribution.h>
#include <nsfx/random/distribution/i-negative-binomial-distribution.h>
#include <nsfx/random/distribution/i-geometric-distribution.h>

#include <nsfx/random/distribution/i-poisson-distribution.h>
#include <nsfx/random/distribution/i-exponential-distribution.h>
#include <nsfx/random/distribution/i-gamma-distribution.h>
#include <nsfx/random/distribution/i-weibull-distribution.h>
#include <nsfx/random/distribution/i-extreme-value-distribution.h>

#include <nsfx/random/distribution/i-normal-distribution.h>
#include <nsfx/random/distribution/i-lognormal-distribution.h>
#include <nsfx/random/distribution/i-chi-squared-distribution.h>
#include <nsfx/random/distribution/i-cauchy-distribution.h>
#include <nsfx/random/distribution/i-fisher-f-distribution.h>
#include <nsfx/random/distribution/i-student-t-distribution.h>

#include <nsfx/random/distribution/i-discrete-distribution.h>
#include <nsfx/random/distribution/i-piecewise-constant-distribution.h>
#include <nsfx/random/distribution/i-piecewise-linear-distribution.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A uniform random number generator.
 *
 * A uniform random number generator produces unsigned integer numbers with
 * a uniform distribution.
 *
 * For performance reasons, the interface of the random number generator is
 * designed to be able to create various distributions, in the hope that a
 * distribution can directly access the internal resource of the uniform
 * random number generator without invoking the virtual functions on this
 * interface.
 */
class IRandomNumberGenerator :
    virtual public IObject
{
public:
    virtual ~IRandomNumberGenerator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual uint32_t Generate(void) = 0;

    /**
     * @brief The minimum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual uint32_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual uint32_t GetMaxValue(void) = 0;

    /**
     * @brief The entropy estimate for the generated random numbers.
     *
     * The value is expressed on a base-2 scale, with a value between \c 0 and
     * <i>log2(max()+1)</i>
     * (which is equal to <code>std::numeric_limits<unsigned int>::digits</code>).
     *
     * @return For a pseudo-random (deterministic) number generator, the value
     *         returned is always \c 0.
     *         <p>
     *         For a non-deterministic random number generator, the value
     *         returned may be positive.
     */
    virtual double GetEntropy(void) = 0;

    // Common distributions.
    /**
     * @brief Create a discrete uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It <b>must</b> be greater than or equal to \c lb.
     *
     * The possible values the distribution can generate is within
     * <i>[lb, ub]</i>.
     */
    virtual Ptr<IUniformIntDistribution>
            CreateUniformIntDistribution(int32_t lb, int32_t ub) = 0;

    /**
     * @brief Create a continous uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It <b>must</b> be greater than or equal to \c lb.
     *
     * The possible values the distribution can generate is within
     * <i>[lb, ub)</i>.
     */
    virtual Ptr<IUniformRealDistribution>
            CreateUniformRealDistribution(double lb, double ub) = 0;

    /**
     * @brief Create a Bernoulli distribution.
     *
     * @param[in] prob Probability of producing a value of \c true.
     *                 It <b>must</b> be within <i>[0, 1]</i> .
     */
    virtual Ptr<IBernoulliDistribution>
            CreateBernoulliDistribution(double prob) = 0;

    /**
     * @brief Create a binomial distribution.
     *
     * @param[in] numTrials The number of independent Bernoulli-distributed
     *                      experiments.
     * @param[in] prob Probability of success of the independent
     *                 Bernoulli-distributed experiments.
     *                 It <b>must</b> be within <i>[0, 1]</i> .
     */
    virtual Ptr<IBinomialDistribution>
            CreateBinomialDistribution(uint32_t numTrials, double prob) = 0;

    /**
     * @brief Create a negative binomial distribution.
     *
     * @param[in] numFailures The number of unsuccessful trials that stops
     *                        the count of successful Bernoulli-distributed
     *                        experiments.
     * @param[in] prob Probability of success of the independent
     *                 Bernoulli-distributed experiments.
     *                 It <b>must</b> be within <i>[0, 1]</i> .
     */
    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numFailures, double prob) = 0;

    /**
     * @brief Create a geometric distribution.
     *
     * @param[in] prob Probability of success.
     *                 It <b>must</b> be within <i>[0, 1]</i> .
     */
    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) = 0;

    /**
     * @brief Create a Poisson distribution.
     *
     * @param[in] mean The expected number of events in interval.
     *                 It <b>must</b> be positive.
     */
    virtual Ptr<IPoissonDistribution>
            CreatePoissonDistribution(double mean) = 0;

    /**
     * @brief Create an exponential distribution.
     *
     * @param[in] lambda The average rate of occurrence.
     *                   It <b>must</b> be positive.
     */
    virtual Ptr<IExponentialDistribution>
            CreateExponentialDistribution(double lambda) = 0;

    /**
     * @brief Create a gamma distribution.
     *
     * @param[in] shape The shape of the distribution.
     *                  It <b>must</b> be positive.
     * @param[in] scale The scale of the distribution.
     *                  It <b>must</b> be positive.
     */
    virtual Ptr<IGammaDistribution>
            CreateGammaDistribution(double shape, double scale) = 0;

    /**
     * @brief Create a gamma distribution.
     *
     * @param[in] shape The shape of the distribution.
     *                  It <b>must</b> be positive.
     * @param[in] scale The scale of the distribution.
     *                  It <b>must</b> be positive.
     */
    virtual Ptr<IWeibullDistribution>
            CreateWeibullDistribution(double shape, double scale) = 0;

    /**
     * @brief Create an extreme value distribution.
     *
     * @param[in] location The location (shift) of the distribution.
     * @param[in] scale    The scale of the distribution.
     *                     It <b>must</b> be positive.
     */
    virtual Ptr<IExtremeValueDistribution>
            CreateExtremeValueDistribution(double location, double scale) = 0;

    /**
     * @brief Create a normal distribution.
     *
     * @param[in] mean   The mean of the distribution (its expected value).
     * @param[in] stddev The standard deviation of the distribution.
     *                   It <b>must</b> be positive.
     */
    virtual Ptr<INormalDistribution>
            CreateNormalDistribution(double mean, double stddev) = 0;

    /**
     * @brief Create a lognormal distribution.
     *
     * @param[in] mean   The mean of the underlying normal distribution.
     * @param[in] stddev The standard deviation of the underlying normal
     *                   distribution.
     *                   It <b>must</b> be positive.
     */
    virtual Ptr<ILognormalDistribution>
            CreateLognormalDistribution(double mean, double stddev) = 0;

    /**
     * @brief Create a chi-squared distribution.
     *
     * @param[in] degreesOfFreedom The degrees of freedom.
     *                             It <b>must</b> be positive.
     */
    virtual Ptr<IChiSquaredDistribution>
            CreatChiSquaredDistribution(double degreesOfFreedom) = 0;

    /**
     * @brief Create a Cauchy distribution.
     *
     * @param[in] location The location of the peak (its mode).
     * @param[in] scale    The scale of the distribution.
     *                     It <b>must</b> be positive.
     */
    virtual Ptr<ICauchyDistribution>
            CreateCauchyDistribution(double location, double scale) = 0;

    /**
     * @brief Create a Fisher F-distribution.
     *
     * @param[in] numerator   The numerator's degrees of freedom.
     *                        It <b>must</b> be positive.
     * @param[in] denominator The denominator's degrees of freedom.
     *                        It <b>must</b> be positive.
     */
    virtual Ptr<IFisherFDistribution>
            CreateFisherFDistribution(double numerator, double denominator) = 0;

    /**
     * @brief Create a Student t-distribution.
     *
     * @param[in] degreesOfFreedom The degrees of freedom.
     *                             It <b>must</b> be positive.
     */
    virtual Ptr<IStudentTDistribution>
            CreateStudentTDistribution(double degreesOfFreedom) = 0;

    /**
     * @brief Create an uninitialized discrete distribution.
     */
    virtual Ptr<IDiscreteDistribution>
            CreateDiscreteDistribution(
                const DiscreteDistributionParam& param) = 0;

    /**
     * @brief Create an uninitialized piecewise constant distribution.
     */
    virtual Ptr<IPiecewiseConstantDistribution>
            CreatePiecewiseConstantDistribution(
                const PiecewiseConstantDistributionParam& param) = 0;

    /**
     * @brief Create an uninitialized piecewise linear distribution.
     */
    virtual Ptr<IPiecewiseLinearDistribution>
            CreatePiecewiseLinearDistribution(
                const PiecewiseLinearDistributionParam& param) = 0;

};

NSFX_DEFINE_CLASS_UID(IRandomNumberGenerator,
                      "edu.uestc.nsfx.IRandomNumberGenerator");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IRandomGeneratorUser, "edu.uestc.nsfx.IRandomGeneratorUser",
    IRandomNumberGenerator);


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A pseudo-random number generator.
 *
 * It generates a sequence of unsigned integer numbers with a uniform
 * distribution.
 */
class IRandomNumberEngine :
    public IRandomNumberGenerator
{
public:
    virtual ~IRandomNumberEngine(void) BOOST_NOEXCEPT {}

    // IRandomNumberGenerator
    virtual uint32_t Generate(void)    NSFX_OVERRIDE = 0;
    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE = 0;
    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE = 0;
    virtual double   GetEntropy(void)  NSFX_OVERRIDE = 0;

    virtual Ptr<IUniformDistribution>
            CreateUniformDistribution(double a, double b) = 0;

    /**
     * @brief Re-initialize the pseudo-random number generator by a seed value.
     *
     * @param[in] seed The seeding value.
     */
    virtual void Seed(uint32_t seed) = 0;

    /**
     * @brief Advances the internal state by \c z notches.
     *
     * This function operations as if \c Generate() was called \c z times,
     * but without generating any numbers in the process.
     */
    virtual void Discard(uint64_t z) = 0;

};

NSFX_DEFINE_CLASS_UID(IRandomNumberEngine,
                      "edu.uestc.nsfx.IRandomNumberEngine");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IRandomEngineUser, "edu.uestc.nsfx.IRandomEngineUser",
    IRandomNumberEngine);


NSFX_CLOSE_NAMESPACE


#endif // I_RANDOM_NUMBER_GENERATOR_H__E6D026FA_399E_402F_BA49_D9AD1E703A1B

