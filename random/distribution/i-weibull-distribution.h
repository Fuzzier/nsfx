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

#ifndef I_WEIBULL_DISTRIBUTION_H__91725FDB_5431_41E8_B37A_60C9F2501B12
#define I_WEIBULL_DISTRIBUTION_H__91725FDB_5431_41E8_B37A_60C9F2501B12


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A 2-parameter Weibull distribution.
 *
 * This distribution produces random numbers where each value can be
 * interpreted, in terms of population, as the lifetime for which the death
 * probability is proportional to the <i>a</i>-th power of time.
 * Parameter <i>b</i> scales the process.
 */
class IWeibullDistribution :
    virtual public IObject
{
public:
    virtual ~IWeibullDistribution(void) BOOST_NOEXCEPT {}

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
     * @return <code>numeric_limits<double>::max()</code> or
     *         <code>numeric_limits<double>::infinity()</code>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the Weibull distribution.
     *
     * The shape parameter of the Weibull distribution.
     */
    virtual double GetShape(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the Weibull distribution.
     *
     * The scale parameter of the Weibull distribution.
     */
    virtual double GetScale(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IWeibullDistribution,
                      "edu.uestc.nsfx.IWeibullDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IWeibullDistributionUser, "edu.uestc.nsfx.IWeibullDistributionUser",
    IWeibullDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_WEIBULL_DISTRIBUTION_H__91725FDB_5431_41E8_B37A_60C9F2501B12

