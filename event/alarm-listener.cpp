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
#include "alarm-listener.h"


AlarmListener::AlarmListener(void) :
    alarmEventSink_(/* controller = */this, /* o = */this,
                    /* ptmf = */&AlarmListener::OnAlarm)
{
}

AlarmListener::~AlarmListener(void)
{
}

void AlarmListener::OnAlarm(void)
{
    std::cout << "Alarm event is processed by OnAlarm()!" << std::endl;
}

