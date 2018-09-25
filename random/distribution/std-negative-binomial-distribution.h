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

#ifndef STD_NEGATIVE_BINOMIAL_DISTRIBUTION_H__D21911DC_210B_409E_9F95_2ECE1D2E5359
#define STD_NEGATIVE_BINOMIAL_DISTRIBUTION_H__D21911DC_210B_409E_9F95_2ECE1D2E5359


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-negative-binomial-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/negative-binomial_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A negative binomial distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c INegativeBinomialDistribution.
 */
template<class StdRng>
class StdNegativeBinomialDistribution :
    public INegativeBinomialDistribution
{
    typedef StdNegativeBinomialDistribution ThisClass;
    typedef boost::random::negative_binomial_distribution<uint32_t, double> DistributionType;
    typedef StdRng RngType;

public:
    StdNegativeBinomialDistribution(Ptr<RngType> rng,
                                    uint32_t numFailures, double prob) :
        rng_(rng),
        dist_(numFailures, prob)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdNegativeBinomialDistribution(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(*rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return dist_.(min)();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return dist_.(max)();
    }

    virtual uint32_t GetNumFailures(void) NSFX_OVERRIDE
    {
        return dist_.k();
    }

    virtual double GetProbability(void) NSFX_OVERRIDE
    {
        return dist_.p();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(INegativeBinomialDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_NEGATIVE_BINOMIAL_DISTRIBUTION_H__D21911DC_210B_409E_9F95_2ECE1D2E5359

