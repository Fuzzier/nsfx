/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
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

#ifndef I_TRIANGLE_DISTRIBUTION_H__F10D540E_E770_4F6C_B10B_12DB2FEA506D
#define I_TRIANGLE_DISTRIBUTION_H__F10D540E_E770_4F6C_B10B_12DB2FEA506D


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A triangle distribution.
 */
class ITriangleDistribution :
    virtual public IObject
{
public:
    virtual ~ITriangleDistribution(void) BOOST_NOEXCEPT {}

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
     * @return Parameter \c a.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The least upper bound of the range of values that can be potentially
     * generated.
     *
     * @return Parameter \c c.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The parameter <i>a</i> associated with the beta distribution.
     *
     * The smallest value.
     */
    virtual double GetA(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the beta distribution.
     *
     * It is the most probable value.
     */
    virtual double GetB(void) = 0;

    /**
     * @brief The parameter <i>b</i> associated with the beta distribution.
     *
     * The largest value.
     */
    virtual double GetC(void) = 0;

};

NSFX_DEFINE_CLASS_UID(ITriangleDistribution,
                      "edu.uestc.nsfx.ITriangleDistribution");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    ITriangleDistributionUser, "edu.uestc.nsfx.ITriangleDistributionUser",
    ITriangleDistribution);


NSFX_CLOSE_NAMESPACE


#endif // I_TRIANGLE_DISTRIBUTION_H__F10D540E_E770_4F6C_B10B_12DB2FEA506D

