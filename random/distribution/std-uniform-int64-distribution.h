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

#ifndef STD_UNIFORM_INT64_DISTRIBUTION_H__68FCE93F_C05F_4E6A_ADEB_74BE873CB337
#define STD_UNIFORM_INT64_DISTRIBUTION_H__68FCE93F_C05F_4E6A_ADEB_74BE873CB337


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-uniform-int64-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/uniform_int_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete uniform distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IUniformInt64Distribution.
 */
template<class StdRng>
class StdUniformInt64Distribution :
    public IUniformInt64Distribution
{
    typedef StdUniformInt64Distribution ThisClass;
    typedef boost::random::uniform_int_distribution<int64_t> DistributionType;
    typedef StdRng RngType;

public:
    StdUniformInt64Distribution(Ptr<RngType> rng, int64_t lb, int64_t ub) :
        rng_(rng),
        dist_(lb, ub)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdUniformInt64Distribution(void) {}

    virtual int64_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual int64_t GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual int64_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual int64_t GetLowerBound(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual int64_t GetUpperBound(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IUniformInt64Distribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_UNIFORM_INT64_DISTRIBUTION_H__68FCE93F_C05F_4E6A_ADEB_74BE873CB337

