/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5
#define CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5


#include <nsfx/config.h>
#include <nsfx/chrono/duration.h>
#include <nsfx/chrono/virtual-time-point.h>


/**
 * @defgroup Simulator
 * @brief The network simulator.
 */


////////////////////////////////////////////////////////////////////////////////
typedef chrono::Duration          Duration;
typedef chrono::VirtualTimePoint  TimePoint;


#endif // CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5
