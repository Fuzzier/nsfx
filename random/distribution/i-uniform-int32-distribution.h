/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
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

#ifndef I_UNIFORM_INT32_DISTRIBUTION_H__BE510F83_B556_4518_AD1B_BFEF642D011A
#define I_UNIFORM_INT32_DISTRIBUTION_H__BE510F83_B556_4518_AD1B_BFEF642D011A


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
class IUniformInt32Distribution :
    virtual public IObject
{
public:
    virtual ~IUniformInt32Distribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
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
     * @brief The minimum value (inclusive).
     *
     * The greatest lower bound of the range of values that can be potentially
     * generated.
     *
     * @return The parameter <i>a</i>.
     */
    virtual int32_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value (exclusive).
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return The parameter <i>b</i>.
     */
    virtual int32_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the uniform distribution.
     *
     * The lower bound of the range of values that can be potentially generated.
     */
    virtual int32_t GetLowerBound(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the uniform distribution.
     *
     * The upper bound of the range of values that can be potentially generated.
     */
    virtual int32_t GetUpperBound(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IUniformInt32Distribution,
                      "edu.uestc.nsfx.IUniformInt32Distribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IUniformInt32DistributionUser, "edu.uestc.nsfx.IUniformInt32DistributionUser",
    IUniformInt32Distribution);


NSFX_CLOSE_NAMESPACE


#endif // I_UNIFORM_INT32_DISTRIBUTION_H__BE510F83_B556_4518_AD1B_BFEF642D011A

