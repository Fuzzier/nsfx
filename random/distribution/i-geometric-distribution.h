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

#ifndef I_GEOMETRIC_DISTRIBUTION_H__20DCD271_4F50_4D9A_A169_280D405082F4
#define I_GEOMETRIC_DISTRIBUTION_H__20DCD271_4F50_4D9A_A169_280D405082F4


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A geometric discrete distribution.
 *
 * This distribution produces positive random integers where each value
 * represents the number of unsuccessful trials before a first success in
 * a sequence of trials, each with a probability of success equal to <i>p</i>.
 *
 * Its analogous continuous distribution is the exponential distribution.
 */
class IGeometricDistribution :
    virtual public IObject
{
public:
    virtual ~IGeometricDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual uint32_t Generate(void) = 0;

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
    virtual uint32_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>std::numeric_limits<uint32_t>::max()</code>.
     */
    virtual uint32_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>p</i> associated with the geometric distribution.
     *
     * The probability of success on each of the sequence of independent
     * Bernoulli-distributed experiments to simulate.
     */
    virtual double GetProbability(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IGeometricDistribution,
                      "edu.uestc.nsfx.IGeometricDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IGeometricDistributionUser, "edu.uestc.nsfx.IGeometricDistributionUser",
    IGeometricDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_GEOMETRIC_DISTRIBUTION_H__20DCD271_4F50_4D9A_A169_280D405082F4

