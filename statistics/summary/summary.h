/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef SUMMARY_H__308CF850_53A1_4ADE_A024_EFBEF3126DD5
#define SUMMARY_H__308CF850_53A1_4ADE_A024_EFBEF3126DD5


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/summary/i-summary.h>
#include <nsfx/statistics/probe/i-probe.h>
#include <nsfx/event/event-sink.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>
#include <nsfx/component/class-registry.h>
#include <limits>
#include <cmath> // sqrt


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief The summary of sample.
 *
 * # Uid
 * @code
 * "edu.uestc.nsfx.statistics.Summary"
 * @endcode
 *
 * # Interfaces
 * * Provides
 *   + \c ISummary
 * * Event sinks
 *   + \c IProbeEventSink
 */
class Summary :
    public ISummary
{
    typedef Summary ThisClass;

public:
    Summary(void);
    virtual ~Summary(void);

    // ISummary.
    virtual double Min(void) NSFX_OVERRIDE;
    virtual double Max(void) NSFX_OVERRIDE;
    virtual double Count(void) NSFX_OVERRIDE;
    virtual double Sum(void) NSFX_OVERRIDE;
    virtual double Mean(void) NSFX_OVERRIDE;
    virtual double Stddev(void) NSFX_OVERRIDE;
    virtual void Reset(void) NSFX_OVERRIDE;

private:
    // IProbeEventSink
    void OnProbe(double data);

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ISummary)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IProbeEventSink, &probeEventSink_)
    NSFX_INTERFACE_MAP_END()

private:
    MemberAggObject<MemberFunctionBasedEventSink<IProbeEventSink, ThisClass>>
        probeEventSink_;

    double min_;
    double max_;
    double count_;
    double sum_;
    double sqrSum_;
};

NSFX_REGISTER_CLASS(Summary, "edu.uestc.nsfx.statistics.Summary");


////////////////////////////////////////////////////////////////////////////////
inline Summary::Summary(void) :
    probeEventSink_(/* controller = */this, /* o = */this,
                    /* ptmf = */&ThisClass::OnProbe),
    min_(std::numeric_limits<double>::quiet_NaN()),
    max_(std::numeric_limits<double>::quiet_NaN()),
    count_(0),
    sum_(0),
    sqrSum_(0)
{
}

inline Summary::~Summary(void)
{
}

inline double Summary::Min(void)
{
    return min_;
}

inline double Summary::Max(void)
{
    return max_;
}

inline double Summary::Count(void)
{
    return count_;
}

inline double Summary::Sum(void)
{
    return sum_;
}

inline double Summary::Mean(void)
{
    double mean;
    if (!count_)
    {
        mean = std::numeric_limits<double>::quiet_NaN();
    }
    else
    {
        mean = sum_ / count_;
    }
    return mean;
}

inline double Summary::Stddev(void)
{
    double stddev;
    if (count_ < 2)
    {
        stddev = std::numeric_limits<double>::quiet_NaN();
    }
    else
    {
        double mean = (sum_ / count_);
        double dev = (sqrSum_ + (-2.0 * sum_ + count_ * mean) * mean)
                   / (count_ - 1);
        stddev = std::sqrt(dev);
    }
    return stddev;
}

inline void Summary::Reset(void)
{
    min_ = std::numeric_limits<double>::quiet_NaN();
    max_ = std::numeric_limits<double>::quiet_NaN();
    count_ = 0;
    sum_ = 0;
    sqrSum_ = 0;
}

inline void Summary::OnProbe(double data)
{
    if (!count_)
    {
        min_ = data;
        max_ = data;
    }
    else
    {
        if (min_ > data)
        {
            min_ = data;
        }
        if (max_ < data)
        {
            max_ = data;
        }
    }
    ++count_;
    sum_ += data;
    sqrSum_ += data * data;
}


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // SUMMARY_H__308CF850_53A1_4ADE_A024_EFBEF3126DD5

