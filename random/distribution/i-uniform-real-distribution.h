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

#ifndef I_UNIFORM_REAL_DISTRIBUTION_H__1031C6AD_933F_4283_BCC6_B4ADA060D156
#define I_UNIFORM_REAL_DISTRIBUTION_H__1031C6AD_933F_4283_BCC6_B4ADA060D156


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A continuous uniform distribution.
 *
 * This distribution is also know as rectangular distribution.
 *
 * This distribution produces random numbers in a range <i>[a,b)</i> where
 * all intervals of the same length within it are equally probable.
 */
class IUniformRealDistribution :
    virtual public IObject
{
public:
    virtual ~IUniformRealDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The minimum value (inclusive).
     *
     * The greatest lower bound of the range of values that can be potentially
     * generated.
     *
     * @return The parameter <i>a</i>.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value (exclusive).
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @remarks The potentially produced values are never equal to this value,
     *          but lower.
     *
     * @return The parameter <i>b</i>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the uniform distribution.
     *
     * The lower bound of the range of values that can be potentially generated.
     */
    virtual double GetLowerBound(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the uniform distribution.
     *
     * The greatest upper bound of the range of values that can be potentially
     * generated.
     * The potential produced values are never equal to this value, but lower.
     */
    virtual double GetUpperBound(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IUniformRealDistribution,
                      "edu.uestc.nsfx.IUniformRealDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IUniformRealDistributionUser, "edu.uestc.nsfx.IUniformRealDistributionUser",
    IUniformRealDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_UNIFORM_REAL_DISTRIBUTION_H__1031C6AD_933F_4283_BCC6_B4ADA060D156
