/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
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

#ifndef I_BINOMIAL_DISTRIBUTION_H__23289A6D_1D5A_4EDB_84AD_0DE6CA208EDB
#define I_BINOMIAL_DISTRIBUTION_H__23289A6D_1D5A_4EDB_84AD_0DE6CA208EDB


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A binomial discrete distribution.
 *
 * This distribution produces random integers in the range <i>[0,t]</i>, where
 * each value represents the number of successes in a sequence of <i>t</i>
 * trials (each with a probability of success equal to <i>p</i>).
 */
class IBinomialDistribution :
    virtual public IObject
{
public:
    virtual ~IBinomialDistribution(void) BOOST_NOEXCEPT {}

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
     * @return The parameter <i>t</i>.
     */
    virtual uint32_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>t</i> associated with the binomial distribution.
     *
     * The number of independent Bernoulli-distributed experiments to simulate.
     */
    virtual uint32_t GetNumTrials(void) = 0;

    /**
     * @brief The parameter <i>p</i> associated with the binomial distribution.
     *
     * The probabily that a trial generating \c true.
     */
    virtual double GetProbability(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IBinomialDistribution,
                      "edu.uestc.nsfx.IBinomialDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IBinomialDistributionUser, "edu.uestc.nsfx.IBinomialDistributionUser",
    IBinomialDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_BINOMIAL_DISTRIBUTION_H__23289A6D_1D5A_4EDB_84AD_0DE6CA208EDB

