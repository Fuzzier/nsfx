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

#ifndef STD_WEIBULL_DISTRIBUTION_H__FDC91027_98E8_42DB_957E_8D7427FA0488
#define STD_WEIBULL_DISTRIBUTION_H__FDC91027_98E8_42DB_957E_8D7427FA0488


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-weibull-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/weibull_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Weibull distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IWeibullDistribution.
 */
template<class StdRng>
class StdWeibullDistribution :
    public IWeibullDistribution
{
    typedef StdWeibullDistribution ThisClass;
    typedef boost::random::weibull_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdWeibullDistribution(Ptr<RngType> rng, double shape, double scale) :
        rng_(rng),
        dist_(shape, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdWeibullDistribution(void) {}

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

    virtual double GetShape(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual double GetScale(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IWeibullDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_WEIBULL_DISTRIBUTION_H__FDC91027_98E8_42DB_957E_8D7427FA0488

