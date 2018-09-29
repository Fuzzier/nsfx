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

#ifndef STD_UNIFORM_REAL_DISTRIBUTION_H__D99597F6_4339_49F6_9CFB_220EE5660DFC
#define STD_UNIFORM_REAL_DISTRIBUTION_H__D99597F6_4339_49F6_9CFB_220EE5660DFC


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-uniform-real-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/uniform_real_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A uniform distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IUniformRealDistribution.
 */
template<class StdRng>
class StdUniformRealDistribution :
    public IUniformRealDistribution
{
    typedef StdUniformRealDistribution ThisClass;
    typedef boost::random::uniform_real_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdUniformRealDistribution(Ptr<RngType> rng, double lb, double ub) :
        rng_(rng),
        dist_(lb, ub)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdUniformRealDistribution(void) {}

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

    virtual double GetLowerBound(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual double GetUpperBound(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IUniformRealDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_UNIFORM_REAL_DISTRIBUTION_H__D99597F6_4339_49F6_9CFB_220EE5660DFC

