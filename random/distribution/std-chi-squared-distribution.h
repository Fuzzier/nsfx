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

#ifndef STD_CHI_SQUARED_DISTRIBUTION_H__7C1EC473_2E73_43D7_ACA1_FE04029F95DD
#define STD_CHI_SQUARED_DISTRIBUTION_H__7C1EC473_2E73_43D7_ACA1_FE04029F95DD


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-chi-squared-distribution.h>
#include <nsfx/component/object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A chi-squared distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IChiSquaredDistribution.
 */
template<class StdRng>
class StdChiSquaredDistribution :
    public IChiSquaredDistribution
{
    typedef StdChiSquaredDistribution ThisClass;
    typedef std::bernoulli_distribution DistributionType;
    typedef StdRng RngType;

public:
    StdChiSquaredDistribution(Ptr<RngType> rng, double degreesOfFreedom) :
        rng_(rng),
        dist_(degreesOfFreedom)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdChiSquaredDistribution(void) {}

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

    virtual double GetDegreesOfFreedom(void) NSFX_OVERRIDE
    {
        return dist_.n();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IChiSquaredDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_CHI_SQUARED_DISTRIBUTION_H__7C1EC473_2E73_43D7_ACA1_FE04029F95DD

