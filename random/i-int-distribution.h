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

#ifndef I_INT_DISTRIBUTION_H__3CBDC278_01AE_42A6_A26D_F1605B89096C
#define I_INT_DISTRIBUTION_H__3CBDC278_01AE_42A6_A26D_F1605B89096C


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete distribution.
 */
class IIntDistribution :
    virtual public IObject
{
public:
    virtual ~IIntDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random value.
     */
    virtual int32_t Generate(void) = 0;

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
    virtual int32_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     */
    virtual int32_t GetMaxValue(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IIntDistribution, "edu.uestc.nsfx.IIntDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IIntDistributionUser, "edu.uestc.nsfx.IIntDistributionUser",
    IIntDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_INT_DISTRIBUTION_H__3CBDC278_01AE_42A6_A26D_F1605B89096C

