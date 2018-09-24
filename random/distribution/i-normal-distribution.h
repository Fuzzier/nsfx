/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-24
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_NORMAL_DISTRIBUTION_H__2970862B_1918_4DDF_B47E_F2200EC75376
#define I_NORMAL_DISTRIBUTION_H__2970862B_1918_4DDF_B47E_F2200EC75376


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A normal distribution.
 *
 * This distribution produces random numbers around the distribution mean with
 * a specific standard deviation.
 *
 * The normal distribution is a common distribution used for many kind of
 * processes, since it is the distribution that the aggregation of a large
 * number of independent random variables approximates to, when all follow
 * the same distribution (no matter which distribution).
 */
class INormalDistribution :
    virtual public IObject
{
public:
    virtual ~INormalDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual double Generate(void) = 0;

    /**
     * @brief Reset the distribution.
     *
     * Reset the distribution, so that the subsequently generated values do not
     * depend on values already produced by it.
     *
     * This function may have no effect if the implementation for this
     * distribution class produces independent values.
     */
    virtual void Reset(void) = 0;

    /**
     * @brief The minimum value that can be potentially generated.
     *
     * The greatest lower bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>numeric_limits<result_type>::lowest()</code> or
     *         <code>-numeric_limits<result_type>::infinity()</code>.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>numeric_limits<result_type>::max()</code> or
     *         <code>numeric_limits<result_type>::infinity()</code>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>mu</i> associated with the normal distribution.
     *
     * The expected value of the normal distribution.
     * Its value coincides with the location of the distribution peak.
     */
    virtual double GetMean(void) = 0;

    /**
     * @brief The parameter <i>sigma</i> associated with the normal distribution.
     *
     * the root square of the distribution variance, which expresses
     * the dispersion of parameters from its mean.
     */
    virtual double GetStddev(void) = 0;

};

NSFX_DEFINE_CLASS_UID(INormalDistribution,
                      "edu.uestc.nsfx.INormalDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    INormalDistributionUser, "edu.uestc.nsfx.INormalDistributionUser",
    INormalDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_NORMAL_DISTRIBUTION_H__2970862B_1918_4DDF_B47E_F2200EC75376

