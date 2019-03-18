/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-18
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STD_UNIFORM_FLOAT_DISTRIBUTION_H__5B4CF68A_F99E_4E96_B3B1_BF42943F6863
#define STD_UNIFORM_FLOAT_DISTRIBUTION_H__5B4CF68A_F99E_4E96_B3B1_BF42943F6863


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-uniform-float-distribution.h>
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
 * Provides \c IUniformFloatDistribution.
 */
template<class StdRng>
class StdUniformFloatDistribution :
    public IUniformFloatDistribution
{
    typedef StdUniformFloatDistribution ThisClass;
    typedef boost::random::uniform_real_distribution<float> DistributionType;
    typedef StdRng RngType;

public:
    StdUniformFloatDistribution(Ptr<RngType> rng, float lb, float ub) :
        rng_(rng),
        dist_(lb, ub)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdUniformFloatDistribution(void) {}

    virtual float Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual float GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual float GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual float GetLowerBound(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual float GetUpperBound(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IUniformFloatDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_UNIFORM_FLOAT_DISTRIBUTION_H__5B4CF68A_F99E_4E96_B3B1_BF42943F6863

