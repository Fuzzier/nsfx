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

#ifndef I_LOGGER_H__DC3EE0B0_113A_4CD3_970A_5AF55437A377
#define I_LOGGER_H__DC3EE0B0_113A_4CD3_970A_5AF55437A377


#include <nsfx/log/config.h>
#include <nsfx/log/record/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/event/i-event.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Events.
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ILogger, "edu.uestc.nsfx.log.ILogger",
    ( void(const std::shared_ptr<Record>&) ));


NSFX_DEFINE_EVENT_INTERFACE(
    ILoggerEvent, "edu.uestc.nsfx.log.ILoggerEvent",
    ILogger);


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_LOGGER_H__DC3EE0B0_113A_4CD3_970A_5AF55437A377

