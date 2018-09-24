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

#ifndef I_GAMMA_DISTRIBUTION_H__5C454176_3664_427E_B94D_58B579DFBE5C
#define I_GAMMA_DISTRIBUTION_H__5C454176_3664_427E_B94D_58B579DFBE5C


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A gamma distribution.
 *
 * This distribution can be interpreted as the aggregation of <i>alpha</i>
 * exponential distributions, each with <i>beta</i> as parameter.
 *
 * It is often used to model waiting times.
 */
class IGammaDistribution :
    virtual public IObject
{
public:
    virtual ~IGammaDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>alpha</i> associated with the gamma distribution.
     *
     * The shape parameter of the gamma distribution.
     */
    virtual double GetShape(void) = 0;

    /**
     * @brief The parameter <i>beta</i> associated with the gamma distribution.
     *
     * The scale parameter of the gamma distribution.
     */
    virtual double GetScale(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IGammaDistribution,
                      "edu.uestc.nsfx.IGammaDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IGammaDistributionUser, "edu.uestc.nsfx.IGammaDistributionUser",
    IGammaDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_GAMMA_DISTRIBUTION_H__5C454176_3664_427E_B94D_58B579DFBE5C

