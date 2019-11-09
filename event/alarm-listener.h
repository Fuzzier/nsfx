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

#ifndef ALARM_LISTENER_H__F2FD2E24_ADE7_4D2E_9DBA_2564AF520665
#define ALARM_LISTENER_H__F2FD2E24_ADE7_4D2E_9DBA_2564AF520665


#include "i-alarm-event.h"


class AlarmListener :
    virtual public IObject
{
public:
    AlarmListener(void);
    virtual ~AlarmListener(void);

private:
    // The function to process alarm event.
    void OnAlarm(void);

    NSFX_INTERFACE_MAP_BEGIN(AlarmListener)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAlarmEventSink, &alarmEventSink_)
    NSFX_INTERFACE_MAP_END()

private:
    MemberAggObject<MemberFunctionBasedEventSink<
        IAlarmEventSink, AlarmListener>>
             alarmEventSink_;
};

NSFX_REGISTER_CLASS(AlarmListener, "edu.uestc.nsfx.tutorial.AlarmListener");


#endif // ALARM_LISTENER_H__F2FD2E24_ADE7_4D2E_9DBA_2564AF520665

