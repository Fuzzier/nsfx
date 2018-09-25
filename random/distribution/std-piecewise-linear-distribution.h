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

#ifndef STD_PIECEWISE_LINEAR_DISTRIBUTION_H__6CDBB672_1055_4545_867F_92CE3A220963
#define STD_PIECEWISE_LINEAR_DISTRIBUTION_H__6CDBB672_1055_4545_867F_92CE3A220963


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-piecewise-linear-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/piecewise_linear_distribution.hpp>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A piecewise linear distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IPiecewiseLinearDistribution.
 */
template<class StdRng>
class StdPiecewiseLinearDistribution :
    public IPiecewiseLinearDistribution
{
    typedef StdPiecewiseLinearDistribution ThisClass;
    typedef boost::random::piecewise_linear_distribution<double, double> DistributionType;
    typedef StdRng RngType;

    struct BoundIterator
    {
        BoundIterator(IPiecewiseLinearDistributionParam* param,
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

        IPiecewiseLinearDistributionParam* param_;
        uint32_t index_;
    };

    struct WeightIterator
    {
        WeightIterator(IPiecewiseLinearDistributionParam* param,
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
            return param_->GetBoundWeight(index_);
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

        IPiecewiseLinearDistributionParam* param_;
        uint32_t index_;
    };

public:
    StdPiecewiseLinearDistribution(
            Ptr<RngType> rng, Ptr<IPiecewiseLinearDistributionParam> param) :
        rng_(rng),
        dist_(BoundIterator(param.Get(), 0),
              BoundIterator(param.Get(), param->GetNumIntervals() + 1),
              WeightIterator(param.Get(), 0)),
        intervals_(dist_.intervals()),
        densities_(dist_.densities())
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdPiecewiseLinearDistribution(void) {}

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

    virtual double GetBoundDensity(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < densities_.size());
        return densities_[index];
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPiecewiseLinearDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
    std::vector<double> intervals_;
    std::vector<double> densities_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_PIECEWISE_LINEAR_DISTRIBUTION_H__6CDBB672_1055_4545_867F_92CE3A220963

