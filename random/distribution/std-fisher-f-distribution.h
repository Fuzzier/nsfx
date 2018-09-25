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

#ifndef STD_FISHER_F_DISTRIBUTION_H__130A98DE_21C2_48F6_A5B2_B2E041D1499D
#define STD_FISHER_F_DISTRIBUTION_H__130A98DE_21C2_48F6_A5B2_B2E041D1499D


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-fisher-f-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/fisher_f_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Fisher F-distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IFisherFDistribution.
 */
template<class StdRng>
class StdFisherFDistribution :
    public IFisherFDistribution
{
    typedef StdFisherFDistribution ThisClass;
    typedef boost::random::fisher_f_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdFisherFDistribution(Ptr<RngType> rng,
                           double numerator, double denominator) :
        rng_(rng),
        dist_(numerator, denominator)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdFisherFDistribution(void) {}

    virtual double Generate(void) NSFX_OVERRIDE
    {
        return dist_(*rng_->GetRng());
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

    virtual double GetNumerator(void) NSFX_OVERRIDE
    {
        return dist_.m();
    }

    virtual double GetDenominator(void) NSFX_OVERRIDE
    {
        return dist_.n();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IFisherFDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_FISHER_F_DISTRIBUTION_H__130A98DE_21C2_48F6_A5B2_B2E041D1499D

