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

#ifndef STD_BINOMIAL_DISTRIBUTION_H__A737938C_551D_48C0_91BF_21D63033F275
#define STD_BINOMIAL_DISTRIBUTION_H__A737938C_551D_48C0_91BF_21D63033F275


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-binomial-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/binomial_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A binomial distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IBinomialDistribution.
 */
template<class StdRng>
class StdBinomialDistribution :
    public IBinomialDistribution
{
    typedef StdBinomialDistribution ThisClass;
    typedef boost::random::binomial_distribution<int32_t, double> DistributionType;
    typedef StdRng RngType;

public:
    StdBinomialDistribution(Ptr<RngType> rng, uint32_t numTrials, double prob) :
        rng_(rng),
        dist_(numTrials, prob)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdBinomialDistribution(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual uint32_t GetNumTrials(void) NSFX_OVERRIDE
    {
        return dist_.t();
    }

    virtual double GetProbability(void) NSFX_OVERRIDE
    {
        return dist_.p();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IBinomialDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_BINOMIAL_DISTRIBUTION_H__A737938C_551D_48C0_91BF_21D63033F275

