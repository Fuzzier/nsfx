/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STD_LOGNORMAL_DISTRIBUTION_H__E5754D3B_E10A_4B86_B4C8_C3749CF4CD3A
#define STD_LOGNORMAL_DISTRIBUTION_H__E5754D3B_E10A_4B86_B4C8_C3749CF4CD3A


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-lognormal-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/lognormal_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A lognormal distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c ILognormalDistribution.
 */
template<class StdRng>
class StdLognormalDistribution :
    public ILognormalDistribution
{
    typedef StdLognormalDistribution ThisClass;
    typedef boost::random::lognormal_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdLognormalDistribution(Ptr<RngType> rng, double location, double scale) :
        rng_(rng),
        dist_(location, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdLognormalDistribution(void) {}

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

    virtual double GetMean(void) NSFX_OVERRIDE
    {
        return dist_.m();
    }

    virtual double GetStddev(void) NSFX_OVERRIDE
    {
        return dist_.s();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILognormalDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_LOGNORMAL_DISTRIBUTION_H__E5754D3B_E10A_4B86_B4C8_C3749CF4CD3A

