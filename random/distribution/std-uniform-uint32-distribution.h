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

#ifndef STD_UNIFORM_UINT32_DISTRIBUTION_H__EC9BC948_8532_4BF2_A55E_64FDBF79EA5F
#define STD_UNIFORM_UINT32_DISTRIBUTION_H__EC9BC948_8532_4BF2_A55E_64FDBF79EA5F


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-uniform-uint32-distribution.h>
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
 * Provides \c IUniformUint32Distribution.
 */
template<class StdRng>
class StdUniformUint32Distribution :
    public IUniformUint32Distribution
{
    typedef StdUniformUint32Distribution ThisClass;
    typedef boost::random::uniform_int_distribution<uint32_t> DistributionType;
    typedef StdRng RngType;

public:
    StdUniformUint32Distribution(Ptr<RngType> rng, uint32_t lb, uint32_t ub) :
        rng_(rng),
        dist_(lb, ub)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdUniformUint32Distribution(void) {}

    virtual uint32_t Generate(void) NSFX_OVERRIDE
    {
        return dist_(rng_->GetRng());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual uint32_t GetMinValue(void) NSFX_OVERRIDE
    {
        return (dist_.min)();
    }

    virtual uint32_t GetMaxValue(void) NSFX_OVERRIDE
    {
        return (dist_.max)();
    }

    virtual uint32_t GetLowerBound(void) NSFX_OVERRIDE
    {
        return dist_.a();
    }

    virtual uint32_t GetUpperBound(void) NSFX_OVERRIDE
    {
        return dist_.b();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IUniformUint32Distribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_UNIFORM_UINT32_DISTRIBUTION_H__EC9BC948_8532_4BF2_A55E_64FDBF79EA5F

