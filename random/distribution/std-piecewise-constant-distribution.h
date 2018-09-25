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

#ifndef STD_PIECEWISE_CONSTANT_DISTRIBUTION_H__BC1033BF_27DE_4D50_87BB_4575E30B9522
#define STD_PIECEWISE_CONSTANT_DISTRIBUTION_H__BC1033BF_27DE_4D50_87BB_4575E30B9522


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-piecewise-constant-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/piecewise_constant_distribution.hpp>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A piecewise constant distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IPiecewiseConstantDistribution.
 */
template<class StdRng>
class StdPiecewiseConstantDistribution :
    public IPiecewiseConstantDistribution
{
    typedef StdPiecewiseConstantDistribution ThisClass;
    typedef boost::random::piecewise_constant_distribution<double, double> DistributionType;
    typedef StdRng RngType;

    struct BoundIterator
    {
        BoundIterator(IPiecewiseConstantDistributionParam* param,
                      uint32_t index) :
            param_(param),
            index_(index)
        {}

        BoundIterator(const BoundIterator& rhs) :
            param_(rhs.param_),
            index_(rhs.index_)
        {}

        BoundIterator& operator=(const BoundIterator& rhs)
        {
            if (this != &rhs)
            {
                param_ = rhs.param_;
                index_ = rhs.index_;
            }
            return *this;
        }

        double operator*(void) const
        {
            return param_->GetBound(index_);
        }

        BoundIterator& operator++(void)
        {
            ++index_;
        }

        BoundIterator operator++(int)
        {
            return BoundIterator(param_, index_++);
        }

        bool operator==(const BoundIterator& rhs) const
        {
            return (param_ == rhs.param_ && index_ == rhs.index_);
        }

        bool operator!=(const BoundIterator& rhs) const
        {
            return !(*this == rhs);
        }

        void swap(BoundIterator& rhs)
        {
            boost::swap(param_, rhs.param_);
            boost::swap(index_, rhs.index_);
        }

        IPiecewiseConstantDistributionParam* param_;
        uint32_t index_;
    };

    struct WeightIterator
    {
        WeightIterator(IPiecewiseConstantDistributionParam* param,
                      uint32_t index) :
            param_(param),
            index_(index)
        {}

        WeightIterator(IPiecewiseLinearDistributionParam* param,
                      uint32_t index) :
            param_(param),
            index_(index)
        {}

        WeightIterator& operator=(const WeightIterator& rhs)
        {
            if (this != &rhs)
            {
                param_ = rhs.param_;
                index_ = rhs.index_;
            }
            return *this;
        }

        double operator*(void) const
        {
            return param_->GetIntervalWeight(index_);
        }

        WeightIterator& operator++(void)
        {
            ++index_;
            return *this;
        }

        WeightIterator operator++(int)
        {
            return WeightIterator(param_, index_++);
        }

        bool operator==(const WeightIterator& rhs) const
        {
            return (param_ == rhs.param_ && index_ == rhs.index_);
        }

        bool operator!=(const WeightIterator& rhs) const
        {
            return !(*this == rhs);
        }

        void swap(WeightIterator& rhs)
        {
            boost::swap(param_, rhs.param_);
            boost::swap(index_, rhs.index_);
        }

        IPiecewiseConstantDistributionParam* param_;
        uint32_t index_;
    };

public:
    StdPiecewiseConstantDistribution(
            Ptr<RngType> rng, Ptr<IPiecewiseConstantDistributionParam> param) :
        rng_(rng),
        dist_(BoundIterator(param.Get(), 0),
              BoundIterator(param.Get(), param->GetNumIntervals() + 1),
              WeightIterator(param.Get(), 0)),
        intervals_(dist_.intervals()),
        densities_(dist_.densities())
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdPiecewiseConstantDistribution(void) {}

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

    virtual uint32_t GetNumIntervals(void) NSFX_OVERRIDE
    {
        return densities_.size();
    }

    virtual double GetBound(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < intervals_.size());
        return intervals_[index];
    }

    virtual double GetIntervalDensity(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < densities_.size());
        return densities_[index];
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPiecewiseConstantDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
    std::vector<double> intervals_;
    std::vector<double> densities_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_PIECEWISE_CONSTANT_DISTRIBUTION_H__BC1033BF_27DE_4D50_87BB_4575E30B9522

