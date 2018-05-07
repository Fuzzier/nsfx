/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_LOG_H__D7007569_C140_4E3C_800F_52AD0ACFC645
#define I_LOG_H__D7007569_C140_4E3C_800F_52AD0ACFC645


#include <nsfx/log/config.h>
#include <nsfx/log/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Events.
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ILog, "edu.uestc..nsfx.log.ILog",
    ( void(const std::shared_ptr<Record>&) ));


NSFX_DEFINE_EVENT_INTERFACE(
    ILogEvent, "edu.uestc.nsfx.log.ILogEvent",
    ILog);


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_LOG_H__D7007569_C140_4E3C_800F_52AD0ACFC645

