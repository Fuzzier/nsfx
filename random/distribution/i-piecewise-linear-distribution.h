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

#ifndef I_PIECEWISE_LINEAR_DISTRIBUTION_H__A55B4F29_3CFF_4799_BE52_59D0033D191F
#define I_PIECEWISE_LINEAR_DISTRIBUTION_H__A55B4F29_3CFF_4799_BE52_59D0033D191F


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The interface to the parameter of piecewise linear distribution.
 */
class IPiecewiseLinearDistributionParam :
    virtual public IObject
{
public:
    virtual ~IPiecewiseLinearDistributionParam(void) BOOST_NOEXCEPT {}
    virtual uint32_t GetNumBounds(void) = 0;
    virtual void AddBound(double bound, double weight) = 0;
    virtual double GetBound(uint32_t index) = 0;
    virtual double GetBoundWeight(uint32_t index) = 0;
};


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The parameter of piecewise linear distribution.
 */
class PiecewiseLinearDistributionParam :
    public IPiecewiseLinearDistributionParam
{
    typedef PiecewiseLinearDistributionParam ThisClass;

public:
    virtual ~PiecewiseLinearDistributionParam(void) {}

    virtual uint32_t GetNumBounds(void) NSFX_OVERRIDE
    {
        return bounds_.size();
    }

    virtual void AddBound(double bound, double weight) NSFX_OVERRIDE
    {
        if (bounds_.size())
        {
            BOOST_ASSERT(bounds_.back() < bound);
        }
        BOOST_ASSERT(weights >= 0);
        bounds_.push_back(bound);
        weights_.push_back(weight);
    }

    virtual double GetBound(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < bounds_.size());
        return bounds_[index];
    }

    virtual double GetBoundWeight(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < weights_.size());
        return weights_[index];
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IPiecewiseLinearDistributionParam)
    NSFX_INTERFACE_MAP_END()

private:
    vector<double> bounds_;
    vector<double> weights_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A piecewise linear distribution.
 *
 * It produces floating-point values that are distributed over a sequence of
 * contiguous subintervals.
 *
 * The probability density on the bounds is proportional to those weights
 * in such a way that the integral that covers all possible values is exactly
 * \c 1.0.
 * The probability of producing a value within each subinterval is the linear
 * value in between the probability density of its bounds.
 */
class IPiecewiseLinearDistribution :
    virtual public IObject
{
public:
    virtual ~IPiecewiseLinearDistribution(void) BOOST_NOEXCEPT {}

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
     * @return The lower bound of the interval.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return The upper bound of the interval.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter associated with the piecewise constant distribution.
     *
     * The number of subintervals.
     */
    virtual double GetNumIntervals(void) = 0;

    /**
     * @brief The parameter associated with the piecewise linear distribution.
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
     * @brief The parameter associated with the piecewise linear distribution.
     *
     * @param[in] index <code>0 <= index < GetNumIntervals() + 1</code>.
     *
     * The probability density at an inner bound of the subintervals.
     *
     * Densities represent the probability of the distribution to produce
     * a number in the subinterval divided by the length of that subinterval.
     *
     * Each subinterval bound is assigned a probability density proportional to
     * its weight, in such a way that the integral that covers all possible
     * values is exactly \c 1.0.
     */
    virtual double GetBoundDensity(uint32_t index) = 0;

};

NSFX_DEFINE_CLASS_UID(IPiecewiseLinearDistribution,
                      "edu.uestc.nsfx.IPiecewiseLinearDistribution");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IPiecewiseLinearDistributionUser, "edu.uestc.nsfx.IPiecewiseLinearDistributionUser",
    IPiecewiseLinearDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_PIECEWISE_LINEAR_DISTRIBUTION_H__A55B4F29_3CFF_4799_BE52_59D0033D191F

