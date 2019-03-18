/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-18
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_UNIFORM_FLOAT_DISTRIBUTION_H__97389153_5CFF_4744_92DF_0A16C638194C
#define I_UNIFORM_FLOAT_DISTRIBUTION_H__97389153_5CFF_4744_92DF_0A16C638194C


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
class IUniformFloatDistribution :
    virtual public IObject
{
public:
    virtual ~IUniformFloatDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual float Generate(void) = 0;

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
    virtual float GetMinValue(void) = 0;

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
    virtual float GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the uniform distribution.
     *
     * The lower bound of the range of values that can be potentially generated.
     */
    virtual float GetLowerBound(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the uniform distribution.
     *
     * The greatest upper bound of the range of values that can be potentially
     * generated.
     * The potential produced values are never equal to this value, but lower.
     */
    virtual float GetUpperBound(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IUniformFloatDistribution,
                      "edu.uestc.nsfx.IUniformFloatDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IUniformFloatDistributionUser, "edu.uestc.nsfx.IUniformFloatDistributionUser",
    IUniformFloatDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_UNIFORM_FLOAT_DISTRIBUTION_H__97389153_5CFF_4744_92DF_0A16C638194C

