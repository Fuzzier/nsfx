/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-29
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STD_BETA_DISTRIBUTION_H__194806C6_DF27_44BE_A502_6DACE2E78A10
#define STD_BETA_DISTRIBUTION_H__194806C6_DF27_44BE_A502_6DACE2E78A10


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-beta-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/beta_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A beta distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IBetaDistribution.
 */
template<class StdRng>
class StdBetaDistribution :
    public IBetaDistribution
{
    typedef StdBetaDistribution ThisClass;
    typedef boost::random::beta_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdBetaDistribution(Ptr<RngType> rng, double alpha, double beta) :
        rng_(rng),
        dist_(alpha, beta)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdBetaDistribution(void) {}

    virtual double Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual double GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual double GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual double GetAlpha(void) NSFX_OVERRIDE
    {
        return dist_.alpha();
    }

    virtual double GetBeta(void) NSFX_OVERRIDE
    {
        return dist_.beta();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IBetaDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_BETA_DISTRIBUTION_H__194806C6_DF27_44BE_A502_6DACE2E78A10

