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

#ifndef I_BERNOULLI_DISTRIBUTION_H__C63F9C49_BCD5_4027_ACAF_AF004513F3EA
#define I_BERNOULLI_DISTRIBUTION_H__C63F9C49_BCD5_4027_ACAF_AF004513F3EA


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Bernoulli distribution.
 *
 * This distribution produces a random value of \c true or \c flase according
 * to a probability <i>p</i>.
 * It has a probability of <i>p</i> to generate \c true, and <i>1-p</i> to
 * generate \c false.
 *
 * e.g., the tossing of a coin is distributed according to a Bernoulli
 * distribution with a probability p of approximately \c 0.5.
 */
class IBernoulliDistribution :
    virtual public IObject
{
public:
    virtual ~IBernoulliDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random value.
     */
    virtual bool Generate(void) = 0;

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
     * @return \c false.
     */
    virtual bool GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return \c true.
     */
    virtual bool GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>p</i> associated with the Bernoulli distribution.
     *
     * The probability that the generated random value is \c true.
     */
    virtual double GetProbability(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IBernoulliDistribution,
                      "edu.uestc.nsfx.IBernoulliDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IBernoulliDistributionUser, "edu.uestc.nsfx.IBernoulliDistributionUser",
    IBernoulliDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_BERNOULLI_DISTRIBUTION_H__C63F9C49_BCD5_4027_ACAF_AF004513F3EA

