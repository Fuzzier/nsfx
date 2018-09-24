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

#ifndef I_FISHER_F_DISTRIBUTION_H__F67790B4_451F_4D8A_A64C_0B8C5946C0E4
#define I_FISHER_F_DISTRIBUTION_H__F67790B4_451F_4D8A_A64C_0B8C5946C0E4


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Fisher F-distribution.
 *
 * This distribution produces random numbers as the result of dividing
 * two independent chi-squared distributions of <i>m</i> and <i>n</i>
 * degrees of freedom.
 */
class IFisherFDistribution :
    virtual public IObject
{
public:
    virtual ~IFisherFDistribution(void) BOOST_NOEXCEPT {}

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
     * @return <code>numeric_limits<double>::max()</code> or
     *         <code>numeric_limits<double>::infinity()</code>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>m</i> associated with the Fisher F-distribution.
     *
     * The numerator's degrees of freedom.
     */
    virtual double GetNumerator(void) = 0;

    /**
     * @brief The parameter <i>n</i> associated with the Fisher F-distribution.
     *
     * The denominator's degrees of freedom.
     */
    virtual double GetDenominator(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IFisherFDistribution,
                      "edu.uestc.nsfx.IFisherFDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IFisherFDistributionUser, "edu.uestc.nsfx.IFisherFDistributionUser",
    IFisherFDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_FISHER_F_DISTRIBUTION_H__F67790B4_451F_4D8A_A64C_0B8C5946C0E4

