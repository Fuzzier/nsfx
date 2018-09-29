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

#ifndef STD_DISCRETE_DISTRIBUTION_H__3A9529FE_A68C_4004_B069_88D0FB894446
#define STD_DISCRETE_DISTRIBUTION_H__3A9529FE_A68C_4004_B069_88D0FB894446


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-discrete-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/discrete_distribution.hpp>
#include <boost/core/swap.hpp>
#include <iterator>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IDiscreteDistribution.
 */
template<class StdRng>
class StdDiscreteDistribution :
    public IDiscreteDistribution
{
    typedef StdDiscreteDistribution ThisClass;
    typedef boost::random::discrete_distribution<uint32_t, double> DistributionType;
    typedef StdRng RngType;

    struct Iterator
    {
        typedef std::forward_iterator_tag iterator_category;
        typedef int32_t difference_type;
        typedef double value_type;
        typedef double* pointer;
        typedef double& reference;

        Iterator(IDiscreteDistributionParam* param, uint32_t index) :
            param_(param),
            index_(index)
        {}

        Iterator(const Iterator& rhs) :
            param_(rhs.param_),
            index_(rhs.index_)
        {}

        Iterator& operator=(const Iterator& rhs)
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
            return param_->GetWeight(index_);
        }

        Iterator& operator++(void)
        {
            ++index_;
            return *this;
        }

        Iterator operator++(int)
        {
            return Iterator(param_, index_++);
        }

        bool operator==(const Iterator& rhs) const
        {
            return (param_ == rhs.param_ && index_ == rhs.index_);
        }

        bool operator!=(const Iterator& rhs) const
        {
            return !(*this == rhs);
        }

        void swap(Iterator& rhs)
        {
            boost::swap(param_, rhs.param_);
            boost::swap(index_, rhs.index_);
        }

        IDiscreteDistributionParam* param_;
        uint32_t index_;
    };

public:
    StdDiscreteDistribution(Ptr<RngType> rng,
                            Ptr<IDiscreteDistributionParam> param) :
        rng_(rng),
        dist_(Iterator(param.Get(), 0),
              Iterator(param.Get(), param->GetNumWeights())),
        probabilities_(dist_.probabilities())
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdDiscreteDistribution(void) {}

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

    virtual uint32_t GetNumValues(void) NSFX_OVERRIDE
    {
        return probabilities_.size();
    }

    virtual double GetProbability(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < probabilities_.size());
        return probabilities_[index];
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IDiscreteDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
    std::vector<double> probabilities_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_DISCRETE_DISTRIBUTION_H__3A9529FE_A68C_4004_B069_88D0FB894446

