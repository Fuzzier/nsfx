/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-23
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_DISCRETE_DISTRIBUTION_H__E239613F_7595_45DF_8A30_3089E317D5A3
#define I_DISCRETE_DISTRIBUTION_H__E239613F_7595_45DF_8A30_3089E317D5A3


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The interface to the parameter of discrete distribution.
 */
class IDiscreteDistributionParam :
    virtual public IObject
{
public:
    virtual ~IDiscreteDistributionParam(void) BOOST_NOEXCEPT {}
    virtual uint32_t GetNumWeights(void) = 0;
    virtual void AddWeight(double weight) = 0;
    virtual double GetWeight(uint32_t index) = 0;
};

NSFX_DEFINE_CLASS_UID(IDiscreteDistributionParam,
                      "edu.uestc.nsfx.IDiscreteDistributionParam");


////////////////////////////////////////
/**
 * @ingroup Random
 * @brief The parameter of discrete distribution.
 */
class DiscreteDistributionParam :
    public IDiscreteDistributionParam
{
    typedef DiscreteDistributionParam ThisClass;

public:
    virtual ~DiscreteDistributionParam(void) {}

    virtual uint32_t GetNumWeights(void) NSFX_OVERRIDE
    {
        return weights_.size();
    }

    virtual void AddWeight(double weight) NSFX_OVERRIDE
    {
        BOOST_ASSERT(weight >= 0);
        weights_.push_back(weight);
    }

    virtual double GetWeight(uint32_t index) NSFX_OVERRIDE
    {
        BOOST_ASSERT(index < weights_.size());
        return weights_[index];
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IDiscreteDistributionParam)
    NSFX_INTERFACE_MAP_END()

private:
    vector<double> weights_;
};

NSFX_REGISTER_CLASS(DiscreteDistributionParam,
                    "edu.uestc.nsfx.DiscreteDistributionParam");


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete distribution.
 *
 * This distribution produces random integers on the interval <i>[0, n)</i>.
 * The probability of each of the <i>n</i> possible numbers to be produced is
 * their corresponding weight divided by the total of all weights.
 */
class IDiscreteDistribution :
    virtual public IObject
{
public:
    virtual ~IDiscreteDistribution(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual uint32_t Generate(void) = 0;

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
     * @return \c 0.
     */
    virtual uint32_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return <code>GetNumValues() - 1</code>.
     */
    virtual uint32_t GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>n</i> associated with the discrete distribution.
     *
     * The number of values.
     */
    virtual uint32_t GetNumValues(void) = 0;

    /**
     * @brief The parameter associated with the discrete distribution.
     *
     * @param[in] index <code>0 <= index < GetNumValues()</code>.
     */
    virtual double GetProbability(uint32_t index) = 0;

};

NSFX_DEFINE_CLASS_UID(IDiscreteDistribution,
                      "edu.uestc.nsfx.IDiscreteDistribution");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IDiscreteDistributionUser, "edu.uestc.nsfx.IDiscreteDistributionUser",
    IDiscreteDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_DISCRETE_DISTRIBUTION_H__E239613F_7595_45DF_8A30_3089E317D5A3

