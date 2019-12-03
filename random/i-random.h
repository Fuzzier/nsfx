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

#ifndef I_RANDOM_H__9373C92B_7102_42B6_871A_7B6675C33E2C
#define I_RANDOM_H__9373C92B_7102_42B6_871A_7B6675C33E2C


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>

#include <nsfx/random/distribution/i-uniform-uint32-distribution.h>
#include <nsfx/random/distribution/i-uniform-int32-distribution.h>
#include <nsfx/random/distribution/i-uniform-uint64-distribution.h>
#include <nsfx/random/distribution/i-uniform-int64-distribution.h>
#include <nsfx/random/distribution/i-uniform-double-distribution.h>
#include <nsfx/random/distribution/i-uniform-float-distribution.h>

#include <nsfx/random/distribution/i-bernoulli-distribution.h>
#include <nsfx/random/distribution/i-binomial-distribution.h>
#include <nsfx/random/distribution/i-negative-binomial-distribution.h>
#include <nsfx/random/distribution/i-geometric-distribution.h>

#include <nsfx/random/distribution/i-poisson-distribution.h>
#include <nsfx/random/distribution/i-exponential-distribution.h>
#include <nsfx/random/distribution/i-gamma-distribution.h>
#include <nsfx/random/distribution/i-weibull-distribution.h>
#include <nsfx/random/distribution/i-extreme-value-distribution.h>
#include <nsfx/random/distribution/i-beta-distribution.h>
#include <nsfx/random/distribution/i-laplace-distribution.h>

#include <nsfx/random/distribution/i-normal-distribution.h>
#include <nsfx/random/distribution/i-lognormal-distribution.h>
#include <nsfx/random/distribution/i-chi-squared-distribution.h>
#include <nsfx/random/distribution/i-cauchy-distribution.h>
#include <nsfx/random/distribution/i-fisher-f-distribution.h>
#include <nsfx/random/distribution/i-student-t-distribution.h>

#include <nsfx/random/distribution/i-discrete-distribution.h>
#include <nsfx/random/distribution/i-piecewise-constant-distribution.h>
#include <nsfx/random/distribution/i-piecewise-linear-distribution.h>

#include <nsfx/random/distribution/i-triangle-distribution.h>

/**
 * @ingroup Random
 * @{
 */

NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief A statistical distribution generator.
 *
 * # Supported statistical distributions
 * ## Uniform distributions
 * * `IUniformUint32Distribution`
 * * `IUniformInt32Distribution`
 * * `IUniformUint64Distribution`
 * * `IUniformInt64Distribution`
 * * `IUniformDoubleDistribution`
 * * `IUniformFloatDistribution`
 * ## Bernoulli distributions
 * * `IBernoulliDistribution`
 * * `IBinomialDistribution`
 * * `INegativeBinomialDistribution`
 * * `IGeometricDistribution`
 * ## Poisson distributions
 * * `IPoissonDistribution`
 * * `IExponentialDistribution`
 * * `IGammaDistribution`
 * * `IWeibullDistribution`
 * * `IExtremeValueDistribution`
 * * `IBetaDistribution`
 * * `ILaplaceDistribution`
 * ## Normal distributions
 * * `INormalDistribution`
 * * `ILognormalDistribution`
 * * `IChiSquaredDistribution`
 * * `ICauchyDistribution`
 * * `IFisherFDistribution`
 * * `IStudentTDistribution`
 * ## Sampling distributions
 * * `IDiscreteDistribution`
 * * `IPiecewiseConstantDistribution`
 * * `IPiecewiseLinearDistribution`
 * ## Miscellaneous Distributions
 * * `ITriangleDistribution`
 *
 * This class provides two means of generating random numbers.
 * * `CreateXxxDistribution()`: Create a random distribution with fixed
 *   parameters, and use it to generate random numbers.
 * * `GenerateXxx()`: Generate random numbers directly.
 *   It would be convenient when random numbers are not generated repeatedly,
 *   or it is undesired to create and keep a random distribution.
 *   However, sampling distributions are not supported.
 *
 * It is recommended to use `GenerateUniform01()` to generate uniform double
 * random numbers within `[0, 1)`, since it is generally *faster* than other means
 * provided by `IRandom`.
 */
class IRandom :
    virtual public IObject
{
public:
    virtual ~IRandom(void) BOOST_NOEXCEPT {}

    /**
     * @brief Create a discrete uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It **must** be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub]`.
     */
    virtual Ptr<IUniformUint32Distribution>
            CreateUniformUint32Distribution(uint32_t lb, uint32_t ub) = 0;

    virtual uint32_t GenerateUniformUint32(uint32_t lb, uint32_t ub) = 0;

    /**
     * @brief Create a discrete uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It **must** be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub]`.
     */
    virtual Ptr<IUniformInt32Distribution>
            CreateUniformInt32Distribution(int32_t lb, int32_t ub) = 0;

    virtual int32_t GenerateUniformInt32(int32_t lb, int32_t ub) = 0;

    /**
     * @brief Create a discrete uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It **must** be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub]`.
     */
    virtual Ptr<IUniformUint64Distribution>
            CreateUniformUint64Distribution(uint64_t lb, uint64_t ub) = 0;

    virtual uint64_t GenerateUniformUint64(uint64_t lb, uint64_t ub) = 0;

    /**
     * @brief Create a discrete uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It **must** be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub]`.
     */
    virtual Ptr<IUniformInt64Distribution>
            CreateUniformInt64Distribution(int64_t lb, int64_t ub) = 0;

    virtual int64_t GenerateUniformInt64(int64_t lb, int64_t ub) = 0;

    /**
     * @brief Create a continous uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It **must** be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub)`.
     */
    virtual Ptr<IUniformDoubleDistribution>
            CreateUniformDoubleDistribution(double lb, double ub) = 0;

    virtual double GenerateUniformDouble(double lb, double ub) = 0;

    virtual double GenerateUniform01(void) = 0;

    /**
     * @brief Create a continous uniform distribution.
     *
     * @param[in] lb The lower bound of the range.
     * @param[in] ub The upper bound of the range.
     *            It <b>must</b> be greater than or equal to `lb`.
     *
     * The possible values the distribution can generate is within `[lb, ub)`.
     */
    virtual Ptr<IUniformFloatDistribution>
            CreateUniformFloatDistribution(float lb, float ub) = 0;

    virtual float GenerateUniformFloat(float lb, float ub) = 0;

    /**
     * @brief Create a Bernoulli distribution.
     *
     * @param[in] prob Probability of producing a value of `true`.
     *                 It <b>must</b> be within `[0, 1]` .
     */
    virtual Ptr<IBernoulliDistribution>
            CreateBernoulliDistribution(double prob) = 0;

    virtual bool GenerateBernoulli(double prob) = 0;

    /**
     * @brief Create a binomial distribution.
     *
     * @param[in] numTrials The number of independent Bernoulli-distributed
     *                      experiments.
     * @param[in] prob Probability of success of the independent
     *                 Bernoulli-distributed experiments.
     *                 It <b>must</b> be within `[0, 1]` .
     */
    virtual Ptr<IBinomialDistribution>
            CreateBinomialDistribution(uint32_t numTrials, double prob) = 0;

    virtual uint32_t GenerateBinomial(uint32_t numTrials, double prob) = 0;

    /**
     * @brief Create a negative binomial distribution.
     *
     * @param[in] numTrials The number of successful trials that stops
     *                      the count of unsuccessful Bernoulli-distributed
     *                      experiments.
     * @param[in] prob Probability of success of the independent
     *                 Bernoulli-distributed experiments.
     *                 It <b>must</b> be within `[0, 1]` .
     */
    virtual Ptr<INegativeBinomialDistribution>
            CreateNegativeBinomialDistribution(
                uint32_t numTrials, double prob) = 0;

    virtual uint32_t GenerateNegativeBinomial(
                uint32_t numTrials, double prob) = 0;

    /**
     * @brief Create a geometric distribution.
     *
     * @param[in] prob Probability of success.
     *                 It <b>must</b> be within `[0, 1]` .
     */
    virtual Ptr<IGeometricDistribution>
            CreateGeometricDistribution(double prob) = 0;

    virtual uint32_t GenerateGeometric(double prob) = 0;

    /**
     * @brief Create a Poisson distribution.
     *
     * @param[in] mean The expected number of events in interval.
     *                 It <b>must</b> be positive.
     */
    virtual Ptr<IPoissonDistribution>
            CreatePoissonDistribution(double mean) = 0;

    virtual uint32_t GeneratePoisson(double mean) = 0;

    /**
     * @brief Create an exponential distribution.
     *
     * @param[in] lambda The average rate of occurrence.
     *                   It <b>must</b> be positive.
     */
    virtual Ptr<IExponentialDistribution>
            CreateExponentialDistribution(double lambda) = 0;

    virtual double GenerateExponential(double lambda) = 0;

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

    virtual double GenerateGamma(double shape, double scale) = 0;

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

    virtual double GenerateWeibull(double shape, double scale) = 0;

    /**
     * @brief Create an extreme value distribution.
     *
     * @param[in] location The location (shift) of the distribution.
     * @param[in] scale    The scale of the distribution.
     *                     It <b>must</b> be positive.
     */
    virtual Ptr<IExtremeValueDistribution>
            CreateExtremeValueDistribution(double location, double scale) = 0;

    virtual double GenerateExtremeValue(double location, double scale) = 0;

    /**
     * @brief Create a beta distribution.
     *
     * @param[in] alpha The first shape of the distribution.
     *                  It <b>must</b> be positive.
     * @param[in] beta  The second shape of the distribution.
     *                  It <b>must</b> be positive.
     */
    virtual Ptr<IBetaDistribution>
            CreateBetaDistribution(double alpha, double beta) = 0;

    virtual double GenerateBeta(double alpha, double beta) = 0;

    /**
     * @brief Create a laplace distribution.
     *
     * @param[in] mean  The mean (location) of the distribution.
     * @param[in] scale The shape of the distribution.
     *                  It <b>must</b> be positive.
     */
    virtual Ptr<ILaplaceDistribution>
            CreateLaplaceDistribution(double mean, double scale) = 0;

    virtual double GenerateLaplace(double mean, double scale) = 0;

    /**
     * @brief Create a normal distribution.
     *
     * @param[in] mean   The mean of the distribution (its expected value).
     * @param[in] stddev The standard deviation of the distribution.
     *                   It <b>must</b> be positive.
     */
    virtual Ptr<INormalDistribution>
            CreateNormalDistribution(double mean, double stddev) = 0;

    virtual double GenerateNormal(double mean, double stddev) = 0;

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

    virtual double GenerateLognormal(double mean, double stddev) = 0;

    /**
     * @brief Create a chi-squared distribution.
     *
     * @param[in] degreesOfFreedom The degrees of freedom.
     *                             It <b>must</b> be positive.
     */
    virtual Ptr<IChiSquaredDistribution>
            CreateChiSquaredDistribution(double degreesOfFreedom) = 0;

    virtual double GenerateChiSquared(double degreesOfFreedom) = 0;

    /**
     * @brief Create a Cauchy distribution.
     *
     * @param[in] location The location of the peak (its mode).
     * @param[in] scale    The scale of the distribution.
     *                     It <b>must</b> be positive.
     */
    virtual Ptr<ICauchyDistribution>
            CreateCauchyDistribution(double location, double scale) = 0;

    virtual double GenerateCauchy(double location, double scale) = 0;

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

    virtual double GenerateFisherF(double numerator, double denominator) = 0;

    /**
     * @brief Create a Student t-distribution.
     *
     * @param[in] degreesOfFreedom The degrees of freedom.
     *                             It <b>must</b> be positive.
     */
    virtual Ptr<IStudentTDistribution>
            CreateStudentTDistribution(double degreesOfFreedom) = 0;

    virtual double GenerateStudentT(double degreesOfFreedom) = 0;

    /**
     * @brief Create a discrete distribution.
     */
    virtual Ptr<IDiscreteDistribution>
            CreateDiscreteDistribution(
                Ptr<IDiscreteDistributionParam> param) = 0;

    /**
     * @brief Create a piecewise constant distribution.
     */
    virtual Ptr<IPiecewiseConstantDistribution>
            CreatePiecewiseConstantDistribution(
                Ptr<IPiecewiseConstantDistributionParam> param) = 0;

    /**
     * @brief Create a piecewise linear distribution.
     */
    virtual Ptr<IPiecewiseLinearDistribution>
            CreatePiecewiseLinearDistribution(
                Ptr<IPiecewiseLinearDistributionParam> param) = 0;

    /**
     * @brief Create a triangle distribution.
     *
     * @param[in] a The smallest value.
     * @param[in] b The most probable value.
     * @param[in] c The largest value.
     */
    virtual Ptr<ITriangleDistribution>
            CreateTriangleDistribution(double a, double b, double c) = 0;

    virtual double GenerateTriangle(double a, double b, double c) = 0;

};

NSFX_DEFINE_CLASS_UID(IRandom, "edu.uestc.nsfx.IRandom");


////////////////////////////////////////
/**
 * @class IRandomUser
 * @brief The user interface of `IRandom`.
 *
 * # Prototype
 * @code{.cpp}
 * void Use(Ptr<IRandom> random);
 * @endcode
 *
 * @see `IRandom`
 */
NSFX_DEFINE_USER_INTERFACE(
    IRandomUser, "edu.uestc.nsfx.IRandomUser",
    IRandom);


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // I_RANDOM_H__9373C92B_7102_42B6_871A_7B6675C33E2C

