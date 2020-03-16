/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_SUMMARY_H__F35B6455_16D3_4798_B5C4_78E5D2A5AB21
#define I_SUMMARY_H__F35B6455_16D3_4798_B5C4_78E5D2A5AB21


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/probe/i-probe-event.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief The summary of sample.
 */
class ISummary :
    virtual public IObject
{
public:
    virtual ~ISummary(void) BOOST_NOEXCEPT {}

    /**
     * @brief The minimum point of the sample.
     *
     * @return If no data has been collected, a quiet NaN (Not-a-Number) is
     *         returned.
     */
    virtual double Min(void) = 0;

    /**
     * @brief The maximum point of the sample.
     *
     * @return If no data has been collected, a quiet NaN (Not-a-Number) is
     *         returned.
     */
    virtual double Max(void) = 0;

    /**
     * @brief The number of the sample points.
     *
     * @return If no data has been collected, `0` is returned.
     */
    virtual double Count(void) = 0;

    /**
     * @brief The summation of the sample points.
     *
     * @return If no data has been collected, `0` is returned.
     */
    virtual double Sum(void) = 0;

    /**
     * @brief The sample mean.
     *
     * @return If no data has been collected, a quiet NaN (Not-a-Number) is
     *         returned.
     */
    virtual double Mean(void) = 0;

    /**
     * @brief The sample standard deviation.
     *
     * @return If there are less than `2` sample points, a quiet NaN
     *         (Not-a-Number) is returned.
     */
    virtual double Stddev(void) = 0;

    /**
     * @brief Reset the summary.
     */
    virtual void Reset(void) = 0;

};


NSFX_DEFINE_CLASS_UID(ISummary, "edu.uestc.nsfx.ISummary");


NSFX_CLOSE_NAMESPACE


#endif // I_SUMMARY_H__F35B6455_16D3_4798_B5C4_78E5D2A5AB21

