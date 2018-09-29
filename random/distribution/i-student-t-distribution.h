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

#ifndef I_STUDENT_T_DISTRIBUTION_H__6E1B7F63_2567_490B_AA08_68F843E1E45B
#define I_STUDENT_T_DISTRIBUTION_H__6E1B7F63_2567_490B_AA08_68F843E1E45B


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Student t-distribution.
 *
 * This distribution produces random numbers as the result of normalizing
 * the values of a relatively small sample (<i>n+1</i> values) of independent
 * normally-distributed values.
 *
 * As the sample size increases, the distribution approaches a standard normal
 * distribution.
 */
class IStudentTDistribution :
    virtual public IObject
{
public:
    virtual ~IStudentTDistribution(void) BOOST_NOEXCEPT {}

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
     * @return <code>std::numeric_limits<double>::lowest()</code> or
     *         <code>-std::numeric_limits<double>::infinity()</code>.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>std::numeric_limits<double>::max()</code> or
     *         <code>std::numeric_limits<double>::infinity()</code>.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>n</i> associated with the Student t-distribution.
     *
     * The theoretical number of samples that originating the distribution is
     * <i>n + 1</i>.
     */
    virtual double GetDegreesOfFreedom(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IStudentTDistribution,
                      "edu.uestc.nsfx.IStudentTDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IStudentTDistributionUser, "edu.uestc.nsfx.IStudentTDistributionUser",
    IStudentTDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_STUDENT_T_DISTRIBUTION_H__6E1B7F63_2567_490B_AA08_68F843E1E45B

