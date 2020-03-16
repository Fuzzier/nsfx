/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_PROBE_EVENT_H__265AC6BA_6B45_47A1_BF80_0C0977AAF70D
#define I_PROBE_EVENT_H__265AC6BA_6B45_47A1_BF80_0C0977AAF70D


#include <nsfx/statistics/config.h>
#include <nsfx/event/i-event.h>
#include <nsfx/event/i-event-sink.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @class IProbeEvent
 * @brief A probe event.
 *
 * A probe event reports sample points.
 */
class IProbeEvent;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @class IProbeEventSink
 * @brief A probe event sink.
 *
 * A probe event sink collects sample points.
 *
 * Prototype:
 * @code
 * void Fire(double value);
 * @endcode
 */
class IProbeEventSink;


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IProbeEventSink, "edu.uestc.nsfx.IProbeEventSink",
    ( void(double) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IProbeEvent, "edu.uestc.nsfx.IProbeEvent",
    IProbeEventSink);


NSFX_CLOSE_NAMESPACE


#endif // I_PROBE_EVENT_H__265AC6BA_6B45_47A1_BF80_0C0977AAF70D

