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

#ifndef I_CHI_SQUARED_DISTRIBUTION_H__7F3E9319_909B_483C_813F_3F3D35CB0CF5
#define I_CHI_SQUARED_DISTRIBUTION_H__7F3E9319_909B_483C_813F_3F3D35CB0CF5


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A chi-squared distribution.
 *
 * This distribution produces random numbers as if the square of <i>n</i>
 * independent standard normal random variables, where <i>n</i> is the
 * distribution parameter, known as the <i>degrees of freedom</i>.
 */
class IChiSquaredDistribution :
    virtual public IObject
{
public:
    virtual ~IChiSquaredDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>n</i> associated with the chi-squared distribution.
     *
     * The degrees of freedom of the chi-square distribution.
     */
    virtual double GetDegreesOfFreedom(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IChiSquaredDistribution,
                      "edu.uestc.nsfx.IChiSquaredDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IChiSquaredDistributionUser, "edu.uestc.nsfx.IChiSquaredDistributionUser",
    IChiSquaredDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_CHI_SQUARED_DISTRIBUTION_H__7F3E9319_909B_483C_813F_3F3D35CB0CF5

