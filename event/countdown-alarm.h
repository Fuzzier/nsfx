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

#ifndef COUNTDOWN_ALARM_H__5766D3E7_CB64_4B90_B982_5A0883A3C369
#define COUNTDOWN_ALARM_H__5766D3E7_CB64_4B90_B982_5A0883A3C369


#include "i-alarm-event.h"
#include "i-countdown.h"


class CountdownAlarm :
    public ICountdown
{
public:
    CountdownAlarm(void);
    virtual ~CountdownAlarm(void);

    // ICountdown
    virtual void SetCounter(uint32_t c) NSFX_OVERRIDE;
    virtual void Decrement(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(CountdownAlarm)
        NSFX_INTERFACE_ENTRY(ICountdown)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAlarmEvent, &alarmEvent_)
    NSFX_INTERFACE_MAP_END()

private:
    uint32_t counter_;

    // Reuse Event<> to implement IAlarmEvent.
    MemberAggObject<Event<IAlarmEvent>> alarmEvent_;
};

NSFX_REGISTER_CLASS(CountdownAlarm, "edu.uestc.nsfx.tutorial.CountdownAlarm");


#endif // COUNTDOWN_ALARM_H__5766D3E7_CB64_4B90_B982_5A0883A3C369

