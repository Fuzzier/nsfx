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

#ifndef STD_EXTREME_VALUE_DISTRIBUTION_H__01F01A31_12F3_4BEB_B7E7_C1E82E94F95F
#define STD_EXTREME_VALUE_DISTRIBUTION_H__01F01A31_12F3_4BEB_B7E7_C1E82E94F95F


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-extreme-value-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/extreme_value_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A extreme value distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IExtremeValueDistribution.
 */
template<class StdRng>
class StdExtremeValueDistribution :
    public IExtremeValueDistribution
{
    typedef StdExtremeValueDistribution ThisClass;
    typedef boost::random::extreme_value_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdExtremeValueDistribution(Ptr<RngType> rng, double location, double scale) :
        rng_(rng),
        dist_(location, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdExtremeValueDistribution(void) {}

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
        NSFX_INTERFACE_ENTRY(IExtremeValueDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_EXTREME_VALUE_DISTRIBUTION_H__01F01A31_12F3_4BEB_B7E7_C1E82E94F95F

