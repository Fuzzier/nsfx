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

#ifndef I_EXPONENTIAL_DISTRIBUTION_H__80B4A2B6_EE6D_4D4B_82A7_5EC09CC2ED7C
#define I_EXPONENTIAL_DISTRIBUTION_H__80B4A2B6_EE6D_4D4B_82A7_5EC09CC2ED7C


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief An exponential distribution.
 *
 * This distribution produces random numbers where each value represents the
 * interval between two random events that are independent but statistically
 * defined by a constant average rate of occurrence (<i>lambda</i>).
 *
 * Its analogous discrete distribution is the geometric distribution.
 */
class IExponentialDistribution :
    virtual public IObject
{
public:
    virtual ~IExponentialDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>lambda</i> associated with the exponential distribution.
     *
     * The average number of times the random events are observed in interval.
     */
    virtual double GetLambda(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IExponentialDistribution,
                      "edu.uestc.nsfx.IExponentialDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IExponentialDistributionUser, "edu.uestc.nsfx.IExponentialDistributionUser",
    IExponentialDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_EXPONENTIAL_DISTRIBUTION_H__80B4A2B6_EE6D_4D4B_82A7_5EC09CC2ED7C

