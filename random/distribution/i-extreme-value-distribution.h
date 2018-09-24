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

#ifndef I_EXTREME_VALUE_DISTRIBUTION_H__993969EC_1224_4DA6_8D19_1A062AEE9153
#define I_EXTREME_VALUE_DISTRIBUTION_H__993969EC_1224_4DA6_8D19_1A062AEE9153


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A type-1 extreme value distribution.
 *
 * This distribution produces random numbers where each value can be
 * interpreted as the extreme (maximum or minimum) of a number of samples of
 * a random variable.
 */
class IExtremeValueDistribution :
    virtual public IObject
{
public:
    virtual ~IExtremeValueDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>a</i> associated with the extreme value distribution.
     *
     * The location parameter of the extreme value distribution, which
     * determines the shift to either side of the distribution shape.
     */
    virtual double GetLocation(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the extreme value distribution.
     *
     * The scale parameter of the extreme value distribution, which is always
     * positive.
     */
    virtual double GetScale(double index) = 0;

};

NSFX_DEFINE_CLASS_UID(IExtremeValueDistribution,
                      "edu.uestc.nsfx.IExtremeValueDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IExtremeValueDistributionUser, "edu.uestc.nsfx.IExtremeValueDistributionUser",
    IExtremeValueDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_EXTREME_VALUE_DISTRIBUTION_H__993969EC_1224_4DA6_8D19_1A062AEE9153

