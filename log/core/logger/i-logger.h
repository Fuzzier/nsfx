/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
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
#include <nsfx/log/core/record/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/i-user.h>
#include <nsfx/event/i-event.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Events.
/**
 * @ingroup Log
 * @brief The logger interface.
 *
 * The logger interface is an event sink interface.
 * It has a single method <code>void Fire(const std::shared_ptr<Record>&)</code>.
 */
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ILogger, "edu.uestc.nsfx.log.ILogger",
    ( void(const std::shared_ptr<Record>&) ));

/**
 * @ingroup Log
 * @brief The logger event interface.
 */
NSFX_DEFINE_EVENT_INTERFACE(
    ILoggerEvent, "edu.uestc.nsfx.log.ILoggerEvent",
    ILogger);


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The logger user interface.
 */
NSFX_DEFINE_USER_INTERFACE(
    ILoggerUser, "edu.uestc.nsfx.log.ILoggerUser",
    ILogger);


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_LOGGER_H__DC3EE0B0_113A_4CD3_970A_5AF55437A377

