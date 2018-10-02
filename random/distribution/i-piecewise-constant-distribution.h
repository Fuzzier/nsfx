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

#ifndef I_PIECEWISE_CONSTANT_DISTRIBUTION_H__124F4965_A80A_41FE_A545_F72E2A3A9569
#define I_PIECEWISE_CONSTANT_DISTRIBUTION_H__124F4965_A80A_41FE_A545_F72E2A3A9569


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/class-registry.h>
#include <limits> // numeric_limits


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The interface to the parameter of piecewise constant distribution.
 */
class IPiecewiseConstantDistributionParam :
    virtual public IObject
{
public:
    virtual ~IPiecewiseConstantDistributionParam(void) BOOST_NOEXCEPT {}
    virtual uint32_t GetNumIntervals(void) = 0;
    virtual void SetLowerBound(double bound) = 0;
    virtual void AddInterval(double bound, double weight) = 0;
    virtual double GetBound(uint32_t index) = 0;
    virtual double GetIntervalWeight(uint32_t index) = 0;
};

NSFX_DEFINE_CLASS_UID(IPiecewiseConstantDistributionParam,
                      "edu.uestc.nsfx.IPiecewiseConstantDistributionParam");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The parameter of piecewise constant distribution.
 */
class PiecewiseConstantDistributionParam :
    public IPiecewiseConstantDistributionParam
{
    typedef PiecewiseConstantDistributionParam ThisClass;

public:
    virtual ~PiecewiseConstantDistributionParam(void) {}

    virtual uint32_t GetNumIntervals(void) NSFX_OVERRIDE
    {
        return static_cast<uint32_t>(weights_.size());
    }

    virtual void SetLowerBound(double bound) NSFX_OVERRIDE
    {
        BOOST_ASSERT(bounds_.size() == 0);
        bounds_.push_back(bound);
    }

    virtual void AddInterval(double bound, double weight) NSFX_OVERRIDE
    {
        BOOST_ASSERT(bounds_.size());
		BOOST_ASSERT(bounds_.size() < (std::numeric_limits<uint32_t>::max)());
        BOOST_ASSERT(bounds_.back() < bound);
        BOOST_ASSERT(weight >= 0);
        bounds_.push_back(bound);
        weights_.push_back(weight);
    }

    virtual double GetBound(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < bounds_.size());
        return bounds_[index];
    }

    virtual double GetIntervalWeight(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < weights_.size());
        return weights_[index];
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPiecewiseConstantDistributionParam)
    NSFX_INTERFACE_MAP_END()

private:
    vector<double> bounds_;
    vector<double> weights_;
};

NSFX_REGISTER_CLASS(PiecewiseConstantDistributionParam,
                    "edu.uestc.nsfx.PiecewiseConstantDistributionParam");


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A piecewise constant distribution.
 *
 * This distribution produces floating-point values that are uniformly
 * distributed over each of a sequence of contiguous subintervals.
 *
 * The probability of producing a value within each subinterval being their
 * corresponding weight divided by the total of all weights.
 */
class IPiecewiseConstantDistribution :
    virtual public IObject
{
public:
    virtual ~IPiecewiseConstantDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual double Generate(void) = 0;

    /**
     * @brief Reset the distribution.
     *
     * Reset the distribution, so that the subsequently generated values do not
     * depend on values already produced by it.
     *
     * This function may have no effect if the implementation for this
     * distribution class produces independent values.
     */
    virtual void Reset(void) = 0;

    /**
     * @brief The minimum value that can be potentially generated.
     *
     * The greatest lower bound of the range of values that can be potentially
     * generated.
     *
     * @return The lower bound of the intervals.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return The upper bound of the intervals.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter associated with the piecewise constant distribution.
     *
     * The number of subintervals.
     */
    virtual uint32_t GetNumIntervals(void) = 0;

    /**
     * @brief The parameter associated with the piecewise constant distribution.
     *
     * @param[in] index <code>0 <= index < GetNumIntervals() + 1</code>.
     *
     * The subintervals are consecutive, and each inner bound indicates where
     * one subinterval ends and where the next begins, with the first and last
     * bounds denoting the limits of the entire interval of values the
     * distribution can produce.
     */
    virtual double GetBound(uint32_t index) = 0;

    /**
     * @brief The parameter associated with the piecewise constant distribution.
     *
     * @param[in] index <code>0 <= index < GetNumIntervals()</code>.
     *
     * The probability density of a subinterval.
     *
     * Densities represent the probability of the distribution to produce
     * a number in the subinterval divided by the length of that subinterval.
     *
     * Each subinterval is assigned a probability density equal to its weight
     * divided by the sum of all weights and the length of the subinterval.
     */
    virtual double GetIntervalDensity(uint32_t index) = 0;

};

NSFX_DEFINE_CLASS_UID(IPiecewiseConstantDistribution,
                      "edu.uestc.nsfx.IPiecewiseConstantDistribution");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IPiecewiseConstantDistributionUser, "edu.uestc.nsfx.IPiecewiseConstantDistributionUser",
    IPiecewiseConstantDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_PIECEWISE_CONSTANT_DISTRIBUTION_H__124F4965_A80A_41FE_A545_F72E2A3A9569

