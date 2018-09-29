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

#ifndef STD_CAUCHY_DISTRIBUTION_H__ED707A58_D120_43EE_BF92_B0F23C83104C
#define STD_CAUCHY_DISTRIBUTION_H__ED707A58_D120_43EE_BF92_B0F23C83104C


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-cauchy-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/cauchy_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Cauchy distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c ICauchyDistribution.
 */
template<class StdRng>
class StdCauchyDistribution :
    public ICauchyDistribution
{
    typedef StdCauchyDistribution ThisClass;
    typedef boost::random::cauchy_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdCauchyDistribution(Ptr<RngType> rng, double location, double scale) :
        rng_(rng),
        dist_(location, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdCauchyDistribution(void) {}

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

    virtual double GetLocation(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual double GetScale(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ICauchyDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_CAUCHY_DISTRIBUTION_H__ED707A58_D120_43EE_BF92_B0F23C83104C

