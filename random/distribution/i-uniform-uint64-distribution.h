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

#ifndef I_UNIFORM_UINT64_DISTRIBUTION_H__41F64A6B_C940_44B5_8AA8_CA1B1F2D7A8A
#define I_UNIFORM_UINT64_DISTRIBUTION_H__41F64A6B_C940_44B5_8AA8_CA1B1F2D7A8A


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete uniform distribution.
 *
 * This distribution is also know as rectangular distribution.
 *
 * This distribution produces random numbers in a range <i>[a,b]</i> where
 * each value within it are equally probable.
 */
class IUniformUint64Distribution :
    virtual public IObject
{
public:
    virtual ~IUniformUint64Distribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual uint64_t Generate(void) = 0;

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
    virtual uint64_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value (exclusive).
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return The parameter <i>b</i>.
     */
    virtual uint64_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the uniform distribution.
     *
     * The lower bound of the range of values that can be potentially generated.
     */
    virtual uint64_t GetLowerBound(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the uniform distribution.
     *
     * The upper bound of the range of values that can be potentially generated.
     */
    virtual uint64_t GetUpperBound(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IUniformUint64Distribution,
                      "edu.uestc.nsfx.IUniformUint64Distribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IUniformUint64DistributionUser, "edu.uestc.nsfx.IUniformUint64DistributionUser",
    IUniformUint64Distribution);


NSFX_CLOSE_NAMESPACE


#endif // I_UNIFORM_UINT64_DISTRIBUTION_H__41F64A6B_C940_44B5_8AA8_CA1B1F2D7A8A

