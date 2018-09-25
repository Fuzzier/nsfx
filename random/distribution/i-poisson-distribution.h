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

#ifndef I_POISSON_DISTRIBUTION_H__6DB032C8_2345_4453_A075_94AE5CC09A23
#define I_POISSON_DISTRIBUTION_H__6DB032C8_2345_4453_A075_94AE5CC09A23


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Poisson distribution.
 *
 * This distribution produces random integers where each value represents
 * a specific count of independent events occurring within a fixed interval,
 * based on the observed mean rate (<i>mu</i>) at which they appear to happen.
 */
class IPoissonDistribution :
    virtual public IObject
{
public:
    virtual ~IPoissonDistribution(void) BOOST_NOEXCEPT {}

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
     * @brief The parameter <i>mu</i> associated with the Poisson distribution.
     *
     * The average number of times the random events are observed in interval.
     */
    virtual double GetMean(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IPoissonDistribution,
                      "edu.uestc.nsfx.IPoissonDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IPoissonDistributionUser, "edu.uestc.nsfx.IPoissonDistributionUser",
    IPoissonDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_POISSON_DISTRIBUTION_H__6DB032C8_2345_4453_A075_94AE5CC09A23

