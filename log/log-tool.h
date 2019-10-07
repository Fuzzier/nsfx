/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef LOG_TOOL_H__49018020_E3FF_46A1_AB21_63653C313D2C
#define LOG_TOOL_H__49018020_E3FF_46A1_AB21_63653C313D2C


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <nsfx/log/make-log-value.h>
#include <nsfx/log/log-record.h>
#include <nsfx/log/std-log-value-traits.h>
#include <sstream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a log record.
 */
inline LogRecord MakeLogRecord(
    std::string&&  message,
    const char*    functionName,
    const char*    fileName,
    uint32_t       lineNumber)
{
    LogRecord record;

    record.Add(LogMessageTraits::GetName(),
           MakeCstrLogValue(std::move(message)));

#if NSFX_LOG_ENABLE_FUNCTION_NAME
    record.Add(LogFunctionTraits::GetName(),
           aux::MakeConstantLogValueC<LogFunctionTraits::Type>(functionName));
#endif // NSFX_LOG_ENABLE_FUNCTION_NAME

#if NSFX_LOG_ENABLE_FILE_NAME
    record.Add(LogFileNameTraits::GetName(),
           aux::MakeConstantLogValueC<LogFileNameTraits::Type>(fileName));
#endif // NSFX_LOG_ENABLE_FILE_NAME

#if NSFX_LOG_ENABLE_LINE_NUMBER
    record.Add(LogLineNumberTraits::GetName(),
           MakeConstantLogValue<LogLineNumberTraits::Type>(lineNumber));
#endif // NSFX_LOG_ENABLE_LINE_NUMBER

    return std::move(record);
}


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a log record with a severity level.
 */
inline LogRecord MakeLogRecordWithSeverity(
    LogSeverity    severity,
    std::string&&  message,
    const char*    functionName,
    const char*    fileName,
    uint32_t       lineNumber)
{
    LogRecord record = MakeLogRecord(std::move(message),
                                     functionName, fileName, lineNumber);

    record.Add(LogSeverityTraits::GetName(),
           MakeConstantLogValue<LogSeverityTraits::Type>(severity));

    return std::move(record);
}


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make and commit a log record.
 */
template<class Sink>
inline void CommitLogRecord(Sink& sink, LogRecord&& record)
{
    sink->Fire(std::move(record));
}


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a log.
 *
 * For example,
 *
 *     NSFX_LOG(logger) << "Some message";
 *
 */
#define NSFX_LOG(logger)                                              \
    for (bool go = ::nsfx::IsLogSinkEnabled(logger); go; go = false)  \
    for (::std::ostringstream oss; go;                                \
         ::nsfx::CommitLogRecord((logger),                            \
             ::nsfx::MakeLogRecord(                                   \
                        oss.str(),                                    \
                        __FUNCTION__,                                 \
                        __FILE__,                                     \
                        __LINE__)),                                   \
         go = false)                                                  \
        oss

////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a log with a severity level.
 *
 * For example,
 *
 *     NSFX_LOG_LEVEL(logger, LOG_INFO) << "Some message";
 *
 */
#define NSFX_LOG_LEVEL(logger, severity)                              \
    for (bool go = ::nsfx::IsLogSinkEnabled(logger); go; go = false)  \
    for (::std::ostringstream oss; go;                                \
         ::nsfx::CommitLogRecord((logger),                            \
             ::nsfx::MakeLogRecordWithSeverity(                       \
                        (severity),                                   \
                        oss.str(),                                    \
                        __FUNCTION__,                                 \
                        __FILE__,                                     \
                        __LINE__)),                                   \
         go = false)                                                  \
        oss


////////////////////////////////////////
#define NSFX_LOG_FATAL(logger)  NSFX_LOG_LEVEL(logger, ::nsfx::LOG_FATAL)
#define NSFX_LOG_ERROR(logger)  NSFX_LOG_LEVEL(logger, ::nsfx::LOG_ERROR)
#define NSFX_LOG_WARN(logger)   NSFX_LOG_LEVEL(logger, ::nsfx::LOG_WARN)
#define NSFX_LOG_INFO(logger)   NSFX_LOG_LEVEL(logger, ::nsfx::LOG_INFO)
#define NSFX_LOG_DEBUG(logger)  NSFX_LOG_LEVEL(logger, ::nsfx::LOG_DEBUG)
#define NSFX_LOG_TRACE(logger)  NSFX_LOG_LEVEL(logger, ::nsfx::LOG_TRACE)


#endif // LOG_TOOL_H__49018020_E3FF_46A1_AB21_63653C313D2C

