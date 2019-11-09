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

#include "config.h"
#include "countdown-alarm.h"


CountdownAlarm::CountdownAlarm(void) :
    counter_(0),
    alarmEvent_(/* controller = */this)
{
}

CountdownAlarm::~CountdownAlarm(void)
{
}

void CountdownAlarm::SetCounter(uint32_t c)
{
    counter_ = c;
}

void CountdownAlarm::Decrement(void)
{
    if (--counter_ == 0)
    {
        alarmEvent_.GetImpl()->Fire();
    }
}

