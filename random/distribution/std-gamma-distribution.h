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

#ifndef STD_GAMMA_DISTRIBUTION_H__768EAAA7_F895_4203_98DE_4E65246320D2
#define STD_GAMMA_DISTRIBUTION_H__768EAAA7_F895_4203_98DE_4E65246320D2


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-gamma-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/gamma_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A gamma distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IGammaDistribution.
 */
template<class StdRng>
class StdGammaDistribution :
    public IGammaDistribution
{
    typedef StdGammaDistribution ThisClass;
    typedef boost::random::gamma_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdGammaDistribution(Ptr<RngType> rng, double shape, double scale) :
        rng_(rng),
        dist_(shape, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdGammaDistribution(void) {}

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

    virtual double GetShape(void) NSFX_OVERRIDE
    {
        return dist_.alpha();
    }

    virtual double GetScale(void) NSFX_OVERRIDE
    {
        return dist_.beta();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IGammaDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_GAMMA_DISTRIBUTION_H__768EAAA7_F895_4203_98DE_4E65246320D2

