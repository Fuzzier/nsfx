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

#ifndef TOOL_H__12A9F14D_AD2D_450F_B462_CBAB2B6E9405
#define TOOL_H__12A9F14D_AD2D_450F_B462_CBAB2B6E9405


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>
#include <nsfx/log/record/record.h>
#include <nsfx/log/attribute-value/const-attribute-value.h>
#include <nsfx/log/misc/severity-level.h>
#include <sstream>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
inline void CommitRecord(
    const Ptr<ILogger>& logger,
    uint32_t            severityLevel,
    const char*         functionName,
    const char*         fileName,
    size_t              lineNumber,
    std::string&&       message)
{
    std::shared_ptr<Record> record(std::make_shared<::nsfx::log::Record>());
    record->Add(
        SeverityLevelInfo::GetName(),
        MakeConstantAttributeValue<typename SeverityLevelInfo::Type>(severityLevel));
    record->Add(
        FunctionNameInfo::GetName(),
        MakeConstantAttributeValue<typename FunctionNameInfo::Type>(functionName));
    record->Add(
        FileNameInfo::GetName(),
        MakeConstantAttributeValue<typename FileNameInfo::Type>(fileName));
    record->Add(
        LineNumberInfo::GetName(),
        MakeConstantAttributeValue<typename LineNumberInfo::Type>(lineNumber));
    record->Add(
        MessageInfo::GetName(),
        MakeConstantAttributeValue<typename MessageInfo::Type>(std::move(message)));
    logger->Fire(record);
}


NSFX_LOG_CLOSE_NAMESPACE


#define NSFX_LOG(logger, severityLevel)              \
    for (bool go = true; go; go = false)             \
    for (::std::ostringstream oss; go;               \
         ::nsfx::log::CommitRecord((logger),         \
                                   (severityLevel),  \
                                   __FUNCTION__,     \
                                   __FILE__,         \
                                   __LINE__,         \
                                   oss.str()),       \
         go = false                                  \
        )                                            \
        oss


#define NSFX_LOG_FATAL(logger)     NSFX_LOG(logger, ::nsfx::log::LOG_FATAL)
#define NSFX_LOG_ERROR(logger)     NSFX_LOG(logger, ::nsfx::log::LOG_ERROR)
#define NSFX_LOG_WARNING(logger)   NSFX_LOG(logger, ::nsfx::log::LOG_WARNING)
#define NSFX_LOG_INFO(logger)      NSFX_LOG(logger, ::nsfx::log::LOG_INFO)
#define NSFX_LOG_DEBUG(logger)     NSFX_LOG(logger, ::nsfx::log::LOG_DEBUG)
#define NSFX_LOG_FUNCTION(logger)  NSFX_LOG(logger, ::nsfx::log::LOG_FUNCTION)
#define NSFX_LOG_TRACE(logger)     NSFX_LOG(logger, ::nsfx::log::LOG_TRACE)


#endif // TOOL_H__12A9F14D_AD2D_450F_B462_CBAB2B6E9405

