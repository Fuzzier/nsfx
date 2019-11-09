/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-22
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_ALARM_EVENT_H__391E14EF_0394_4141_8024_502BFD593DE5
#define I_ALARM_EVENT_H__391E14EF_0394_4141_8024_502BFD593DE5


#include <nsfx/all.h>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    IAlarmEventSink, "edu.uestc.nsfx.tutorial.IAlarmEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    IAlarmEvent, "edu.uestc.nsfx.tutorial.IAlarmEvent",
    IAlarmEventSink);


#endif // I_ALARM_EVENT_H__391E14EF_0394_4141_8024_502BFD593DE5

