/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-29
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_LAPLACE_DISTRIBUTION_H__253F3168_AC88_4656_92BA_935FE8382229
#define I_LAPLACE_DISTRIBUTION_H__253F3168_AC88_4656_92BA_935FE8382229


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Paplace distribution.
 *
 * This distribution produces random numbers as the result of dividing two
 * independent standard normal random variables, like a Student-t distribution
 * with one degree of freedom.
 */
class ILaplaceDistribution :
    virtual public IObject
{
public:
    virtual ~ILaplaceDistribution(void) BOOST_NOEXCEPT {}

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
     * @return <code>std::numeric_limits<result_type>::lowest()</code> or
     *         <code>-std::numeric_limits<result_type>::infinity()</code>.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>std::numeric_limits<result_type>::max()</code> or
     *         <code>std::numeric_limits<result_type>::infinity()</code>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>mean</i> associated with the Laplace distribution.
     */
    virtual double GetMean(void) = 0;

    /**
     * @brief The parameter <i>scale</i> associated with the Laplace distribution.
     *
     * The scale parameter, which is always positive.
     */
    virtual double GetScale(void) = 0;

};

NSFX_DEFINE_CLASS_UID(ILaplaceDistribution,
                      "edu.uestc.nsfx.ILaplaceDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    ILaplaceDistributionUser, "edu.uestc.nsfx.ILaplaceDistributionUser",
    ILaplaceDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_LAPLACE_DISTRIBUTION_H__253F3168_AC88_4656_92BA_935FE8382229

