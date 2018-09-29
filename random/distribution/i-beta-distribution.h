/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-25
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_BETA_DISTRIBUTION_H__DAA2CC24_5718_4793_B9B8_3286701389BC
#define I_BETA_DISTRIBUTION_H__DAA2CC24_5718_4793_B9B8_3286701389BC


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A beta distribution.
 *
 * This distribution produces random numbers within <i>[0, 1]</i> or
 * <i>(0, 1)</i>.
 */
class IBetaDistribution :
    virtual public IObject
{
public:
    virtual ~IBetaDistribution(void) BOOST_NOEXCEPT {}

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
     * @return \c 1.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>alpha</i> associated with the beta distribution.
     */
    virtual double GetAlpha(void) = 0;

    /**
     * @brief The parameter <i>beta</i> associated with the beta distribution.
     */
    virtual double GetBeta(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IBetaDistribution,
                      "edu.uestc.nsfx.IBetaDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IBetaDistributionUser, "edu.uestc.nsfx.IBetaDistributionUser",
    IBetaDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_BETA_DISTRIBUTION_H__DAA2CC24_5718_4793_B9B8_3286701389BC

