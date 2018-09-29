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

#ifndef STD_LAPLACE_DISTRIBUTION_H__FE6593B4_BAE5_40ED_B87E_124123B26EC5
#define STD_LAPLACE_DISTRIBUTION_H__FE6593B4_BAE5_40ED_B87E_124123B26EC5


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-laplace-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/laplace_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Laplace distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c ILaplaceDistribution.
 */
template<class StdRng>
class StdLaplaceDistribution :
    public ILaplaceDistribution
{
    typedef StdLaplaceDistribution ThisClass;
    typedef boost::random::laplace_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdLaplaceDistribution(Ptr<RngType> rng, double location, double scale) :
        rng_(rng),
        dist_(location, scale)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdLaplaceDistribution(void) {}

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
        return dist_.mean();
    }

    virtual double GetScale(void) NSFX_OVERRIDE
    {
        return dist_.beta();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILaplaceDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_LAPLACE_DISTRIBUTION_H__FE6593B4_BAE5_40ED_B87E_124123B26EC5

