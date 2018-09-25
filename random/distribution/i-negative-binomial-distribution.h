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

#ifndef I_NEGATIVE_BINOMIAL_DISTRIBUTION_H__0603C9F3_264D_465B_941A_647D9CF7A67C
#define I_NEGATIVE_BINOMIAL_DISTRIBUTION_H__0603C9F3_264D_465B_941A_647D9CF7A67C


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A negative binomial discrete distribution.
 *
 * This distribution is also known as Pascal distribution.
 *
 * This distribution produces random integers where each value represents
 * the number of successful trials before <i>k</i> unsuccessful trials happen
 * in a sequence of trials, each with a probability of success equal to <i>p</i>.
 */
class INegativeBinomialDistribution :
    virtual public IObject
{
public:
    virtual ~INegativeBinomialDistribution(void) BOOST_NOEXCEPT {}

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
     * @return <code>numeric_limits<uint32_t>::max()</code>.
     */
    virtual uint32_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>k</i> associated with the negative binomial distribution.
     *
     * The number of unsuccessful trials that stops the count of successful
     * Bernoulli-distributed experiments to simulate.
     */
    virtual double GetNumFailures(void) = 0;

    /**
     * @brief The parameter <i>p</i> associated with the negative binomial distribution.
     *
     * The probability of success on each of the sequence of independent
     * Bernoulli-distributed experiments to simulate.
     */
    virtual double GetProbability(void) = 0;

};

NSFX_DEFINE_CLASS_UID(INegativeBinomialDistribution,
                      "edu.uestc.nsfx.INegativeBinomialDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    INegativeBinomialDistributionUser, "edu.uestc.nsfx.INegativeBinomialDistributionUser",
    INegativeBinomialDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_NEGATIVE_BINOMIAL_DISTRIBUTION_H__0603C9F3_264D_465B_941A_647D9CF7A67C

