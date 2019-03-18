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

#ifndef STD_UNIFORM_INT32_DISTRIBUTION_H__4C96A7DE_4BFF_40A2_AF57_789E75C9C299
#define STD_UNIFORM_INT32_DISTRIBUTION_H__4C96A7DE_4BFF_40A2_AF57_789E75C9C299


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-uniform-int32-distribution.h>
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
 * Provides \c IUniformInt32Distribution.
 */
template<class StdRng>
class StdUniformInt32Distribution :
    public IUniformInt32Distribution
{
    typedef StdUniformInt32Distribution ThisClass;
    typedef boost::random::uniform_int_distribution<int32_t> DistributionType;
    typedef StdRng RngType;

public:
    StdUniformInt32Distribution(Ptr<RngType> rng, int32_t lb, int32_t ub) :
        rng_(rng),
        dist_(lb, ub)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdUniformInt32Distribution(void) {}

    virtual int32_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual int32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual int32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual int32_t GetLowerBound(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual int32_t GetUpperBound(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IUniformInt32Distribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_UNIFORM_INT32_DISTRIBUTION_H__4C96A7DE_4BFF_40A2_AF57_789E75C9C299

