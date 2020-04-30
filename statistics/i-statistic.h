/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-04-30
 *
 * @copyright Copyright (c) 2020.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_STATISTIC_H__6727893B_09E6_47BA_B566_41F42F5069C8
#define I_STATISTIC_H__6727893B_09E6_47BA_B566_41F42F5069C8


#include <nsfx/statistics/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @class IStatistic
 * @brief A statistic.
 *
 * A statistic is a connectable component.
 */
class IStatistic :
    virtual public IObject
{
public:
    ~IStatistic(void) BOOST_NOEXCEPT {}

    /**
     * @brief Connect a collector to the statistic.
     *
     * @param[in] sink The collector of the statistic that implements
     *                 the event sink interface of the statistic.
     *
     * @return A cookie that can be used to disconnect the collector.
     *
     * @throw NoInterface Thrown when the `sink` does not provide the event sink
     *                    interface of the statistic.
     */
    virtual cookie_t Connect(Ptr<IObject> sink) = 0;

    /**
     * @brief Disconnect.
     *
     * @param[in] cookie The cookie of a connection.
     */
    virtual void Disconnect(cookie_t cookie) = 0;
};

NSFX_DEFINE_CLASS_UID(IStatistic, "edu.uestc.nsfx.IStatistic");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat0Sink, "edu.uestc.nsfx.IStat0Sink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat0, "edu.uestc.nsfx.IStat0",
    IStat0Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat1Sink, "edu.uestc.nsfx.IStat1Sink",
    ( void(double v1) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat1, "edu.uestc.nsfx.IStat1",
    IStat1Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat2Sink, "edu.uestc.nsfx.IStat2Sink",
    ( void(double v1, double v2) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat2, "edu.uestc.nsfx.IStat2",
    IStat2Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat3Sink, "edu.uestc.nsfx.IStat3Sink",
    ( void(double v1, double v2, double v3) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat3, "edu.uestc.nsfx.IStat3",
    IStat3Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat4Sink, "edu.uestc.nsfx.IStat4Sink",
    ( void(double v1, double v2, double v3, double v4) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat4, "edu.uestc.nsfx.IStat4",
    IStat4Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat5Sink, "edu.uestc.nsfx.IStat5Sink",
    ( void(double v1, double v2, double v3, double v4, double v5) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat5, "edu.uestc.nsfx.IStat5",
    IStat5Sink);

////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IStat6Sink, "edu.uestc.nsfx.IStat6Sink",
    ( void(double v1, double v2, double v3, double v4, double v5, double v6) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IStat6, "edu.uestc.nsfx.IStat6",
    IStat6Sink);


NSFX_CLOSE_NAMESPACE


#endif // I_STATISTIC_H__6727893B_09E6_47BA_B566_41F42F5069C8

