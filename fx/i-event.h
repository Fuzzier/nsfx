/**
 * @file
 *
 * @brief Xxx events.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-07-24
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

// Header file guard.
#ifndef I_EVENT_H
#define I_EVENT_H


// NSFX.
#include <nsfx/all.h>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
// Event definition.
// Uuu
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IUuuEventSink, "edu.uestc.nsfx.IUuuEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IUuuEvent, "edu.uestc.nsfx.IUuuEvent",
    IUuuEventSink);


////////////////////////////////////////////////////////////////////////////////
// Vvv
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IVvvEventSink, "edu.uestc.nsfx.IVvvEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IVvvEvent, "edu.uestc.nsfx.IVvvEvent",
    IVvvEventSink);


////////////////////////////////////////////////////////////////////////////////
// Sss
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ISssEventSink, "edu.uestc.nsfx.ISssEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    ISssEvent, "edu.uestc.nsfx.ISssEvent",
    ISssEventSink);


////////////////////////////////////////////////////////////////////////////////
// Ttt
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ITttEventSink, "edu.uestc.nsfx.ITttEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    ITttEvent, "edu.uestc.nsfx.ITttEvent",
    ITttEventSink);


#endif // I_EVENT_H

