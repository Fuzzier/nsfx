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

#ifndef STD_EXPONENTIAL_DISTRIBUTION_H__0631BDA7_C5F1_46EE_A83C_D354E8999CAD
#define STD_EXPONENTIAL_DISTRIBUTION_H__0631BDA7_C5F1_46EE_A83C_D354E8999CAD


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-exponential-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/exponential_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A exponential distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IExponentialDistribution.
 */
template<class StdRng>
class StdExponentialDistribution :
    public IExponentialDistribution
{
    typedef StdExponentialDistribution ThisClass;
    typedef boost::random::exponential_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdExponentialDistribution(Ptr<RngType> rng, double lambda) :
        rng_(rng),
        dist_(lambda)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdExponentialDistribution(void) {}

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

    virtual double GetLambda(void) NSFX_OVERRIDE
    {
        return dist_.lambda();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IExponentialDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_EXPONENTIAL_DISTRIBUTION_H__0631BDA7_C5F1_46EE_A83C_D354E8999CAD

