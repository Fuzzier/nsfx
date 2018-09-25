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

#ifndef I_CAUCHY_DISTRIBUTION_H__BFB39A9C_9D0B_4E7D_9F06_C6CB13FD3F32
#define I_CAUCHY_DISTRIBUTION_H__BFB39A9C_9D0B_4E7D_9F06_C6CB13FD3F32


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Cauchy distribution.
 *
 * This distribution produces random numbers as the result of dividing two
 * independent standard normal random variables, like a Student-t distribution
 * with one degree of freedom.
 */
class ICauchyDistribution :
    virtual public IObject
{
public:
    virtual ~ICauchyDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>a</i> associated with the Cauchy distribution.
     *
     * The shift to either side of the distribution shape.
     */
    virtual double GetLocation(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the Cauchy distribution.
     *
     * The scale parameter, which is always positive.
     */
    virtual double GetScale(void) = 0;

};

NSFX_DEFINE_CLASS_UID(ICauchyDistribution,
                      "edu.uestc.nsfx.ICauchyDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    ICauchyDistributionUser, "edu.uestc.nsfx.ICauchyDistributionUser",
    ICauchyDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_CAUCHY_DISTRIBUTION_H__BFB39A9C_9D0B_4E7D_9F06_C6CB13FD3F32

