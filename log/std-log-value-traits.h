/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef STD_LOG_VALUE_TRAITS_H__B4EBD9AE_1FCE_4890_82B3_9B596391524B
#define STD_LOG_VALUE_TRAITS_H__B4EBD9AE_1FCE_4890_82B3_9B596391524B


#include <nsfx/log/config.h>
#include <nsfx/log/log-value-traits.h>
#include <nsfx/log/log-severity.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The traits class for log message.
 */
NSFX_DEFINE_LOG_VALUE_TRAITS(LogMessageTraits, "LogMessage", const char*);


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The traits class for log severity.
 */
NSFX_DEFINE_LOG_VALUE_TRAITS(LogSeverityTraits, "LogSeverity", LogSeverity);


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The traits class for log function name.
 */
NSFX_DEFINE_LOG_VALUE_TRAITS(LogFunctionTraits, "LogFunction", const char*);


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The traits class for log file name.
 */
NSFX_DEFINE_LOG_VALUE_TRAITS(LogFileNameTraits, "LogFile", const char*);


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The traits class for log line number.
 */
NSFX_DEFINE_LOG_VALUE_TRAITS(LogLineNumberTraits, "LogLine", uint32_t);


NSFX_CLOSE_NAMESPACE


#endif // STD_LOG_VALUE_TRAITS_H__B4EBD9AE_1FCE_4890_82B3_9B596391524B

