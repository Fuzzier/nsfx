/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
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

#ifndef STD_BERNOULLI_DISTRIBUTION_H__C5091921_3F6B_46E3_BB9A_9E1BCAE83AEA
#define STD_BERNOULLI_DISTRIBUTION_H__C5091921_3F6B_46E3_BB9A_9E1BCAE83AEA


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-bernoulli-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/bernoulli_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Bernoulli distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IBernoulliDistribution.
 */
template<class StdRng>
class StdBernoulliDistribution :
    public IBernoulliDistribution
{
    typedef StdBernoulliDistribution ThisClass;
    typedef boost::random::bernoulli_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdBernoulliDistribution(Ptr<RngType> rng, double prob) :
        rng_(rng),
        dist_(prob)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdBernoulliDistribution(void) {}

    virtual double Generate(void) NSFX_OVERRIDE
    {
        return dist_(*rng_->GetEngine());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual double GetMinValue(void) NSFX_OVERRIDE
    {
        return dist_.(min)();
    }

    virtual double GetMaxValue(void) NSFX_OVERRIDE
    {
        return dist_.(max)();
    }

    virtual double GetProbability(void) NSFX_OVERRIDE
    {
        return dist_.p();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IBernoulliDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_BERNOULLI_DISTRIBUTION_H__C5091921_3F6B_46E3_BB9A_9E1BCAE83AEA

