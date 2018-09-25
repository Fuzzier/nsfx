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

#ifndef STD_POISSON_DISTRIBUTION_H__B8471F7D_AEF9_4699_BDD6_0EAA8AE1D5F9
#define STD_POISSON_DISTRIBUTION_H__B8471F7D_AEF9_4699_BDD6_0EAA8AE1D5F9


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-poisson-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/poisson_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Poisson distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IPoissonDistribution.
 */
template<class StdRng>
class StdPoissonDistribution :
    public IPoissonDistribution
{
    typedef StdPoissonDistribution ThisClass;
    typedef boost::random::poisson_distribution<uint32_t, double> DistributionType;
    typedef StdRng RngType;

public:
    StdPoissonDistribution(Ptr<RngType> rng, double mean) :
        rng_(rng),
        dist_(mean)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdPoissonDistribution(void) {}

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

    virtual double GetMean(void) NSFX_OVERRIDE
    {
        return dist_.mean();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPoissonDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_POISSON_DISTRIBUTION_H__B8471F7D_AEF9_4699_BDD6_0EAA8AE1D5F9

