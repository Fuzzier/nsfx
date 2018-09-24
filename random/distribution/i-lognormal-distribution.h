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

#ifndef I_LOGNORMAL_DISTRIBUTION_H__D602CF24_CC18_4944_B310_C2BBEED24160
#define I_LOGNORMAL_DISTRIBUTION_H__D602CF24_CC18_4944_B310_C2BBEED24160


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A log-normal distribution.
 *
 * This distribution produces random numbers whose logarithms are normally
 * distributed.
 */
class ILognormalDistribution :
    virtual public IObject
{
public:
    virtual ~ILognormalDistribution(void) BOOST_NOEXCEPT {}

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
     * @return \c 0.
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
     * @brief The parameter <i>mu</i> associated with the lognormal distribution.
     *
     * The mean of the underlying normal distribution formed by the logarithm
     * transformations of the possible values in this distribution.
     */
    virtual double GetMean(void) = 0;

    /**
     * @brief The parameter <i>sigma</i> associated with the lognormal distribution.
     *
     * The standard deviation of the underlying normal distribution formed by
     * the logarithm transformations of the possible values in this distribution.
     */
    virtual double GetStddev(void) = 0;

};

NSFX_DEFINE_CLASS_UID(ILognormalDistribution,
                      "edu.uestc.nsfx.ILognormalDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    ILognormalDistributionUser, "edu.uestc.nsfx.ILognormalDistributionUser",
    ILognormalDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_LOGNORMAL_DISTRIBUTION_H__D602CF24_CC18_4944_B310_C2BBEED24160

