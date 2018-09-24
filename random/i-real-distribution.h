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

#ifndef I_REAL_DISTRIBUTION_H__19BAB8EF_DA23_488B_BEE4_3A76378B6E54
#define I_REAL_DISTRIBUTION_H__19BAB8EF_DA23_488B_BEE4_3A76378B6E54


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A continuous distribution.
 */
class IRealDistribution :
    virtual public IObject
{
public:
    virtual ~IRealDistribution(void) BOOST_NOEXCEPT {}

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
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     */
    virtual double GetMaxValue(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IRealDistribution, "edu.uestc.nsfx.IRealDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IRealDistributionUser, "edu.uestc.nsfx.IRealDistributionUser",
    IRealDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_REAL_DISTRIBUTION_H__19BAB8EF_DA23_488B_BEE4_3A76378B6E54

