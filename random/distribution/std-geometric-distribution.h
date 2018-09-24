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

#ifndef STD_GEOMETRIC_DISTRIBUTION_H__12830A62_D838_4414_BBC4_F15792FA378B
#define STD_GEOMETRIC_DISTRIBUTION_H__12830A62_D838_4414_BBC4_F15792FA378B


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-geometric-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/geometric_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A geometric distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IGeometricDistribution.
 */
template<class StdRng>
class StdGeometricDistribution :
    public IGeometricDistribution
{
    typedef StdGeometricDistribution ThisClass;
    typedef boost::random::geometric_distribution<uint32_t, double> DistributionType;
    typedef StdRng RngType;

public:
    StdGeometricDistribution(Ptr<RngType> rng, double prob) :
        rng_(rng),
        dist_(prob)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdGeometricDistribution(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(*rng_->GetEngine());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return dist_.(min)();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return dist_.(max)();
    }

    virtual double GetProbability(void) NSFX_OVERRIDE
    {
        return dist_.p();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IGeometricDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_GEOMETRIC_DISTRIBUTION_H__12830A62_D838_4414_BBC4_F15792FA378B

