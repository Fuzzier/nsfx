/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-29
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STD_TRIANGLE_DISTRIBUTION_H__22BFDA6B_4BD2_40A4_93EF_4A0A662AEB0E
#define STD_TRIANGLE_DISTRIBUTION_H__22BFDA6B_4BD2_40A4_93EF_4A0A662AEB0E


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-triangle-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/triangle_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A triangle distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c ITriangleDistribution.
 */
template<class StdRng>
class StdTriangleDistribution :
    public ITriangleDistribution
{
    typedef StdTriangleDistribution ThisClass;
    typedef boost::random::triangle_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdTriangleDistribution(Ptr<RngType> rng, double a, double b, double c) :
        rng_(rng),
        dist_(a, b, c)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdTriangleDistribution(void) {}

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

    virtual double GetA(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual double GetB(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    virtual double GetC(void) NSFX_OVERRIDE
    {
        return dist_.c();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ITriangleDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_TRIANGLE_DISTRIBUTION_H__22BFDA6B_4BD2_40A4_93EF_4A0A662AEB0E

