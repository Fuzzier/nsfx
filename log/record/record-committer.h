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

#ifndef RECORD_COMMITTER_H__47B50854_6C4C_4F63_99BB_DDD346974727
#define RECORD_COMMITTER_H__47B50854_6C4C_4F63_99BB_DDD346974727


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>
#include <nsfx/log/record/record.h>
#include <nsfx/log/misc/attribute-value-info.h>


#define NSFX_LOG(logger, severityLevel)                                                  \
    for (bool go = true; go; go = false)                                                 \
    for (auto record = std::make_shared<::nsfx::log::Record>(); go; go = false)          \
    for ([&record] {                                                                     \
            record->Add(SeverityLevelInfo::GetName(),                                       \
                        ::nsfx::log::MakeConstantAttributeValue<uint32_t>(severityLevel));  \ 
        }();}                                                                            \
    for (record->Add(SeverityLevelInfo::GetName(),                                       \
                     ::nsfx::log::MakeConstantAttributeValue<uint32_t>(severityLevel));  \
         go; go = false)                                                                 \
    for (record->Add(FileNameInfo::GetName(),                                            \
                     ::nsfx::log::MakeConstantAttributeValue<std::string>(__FILE__));    \
         go; go = false)                                                                 \
    for (record->Add(LineNumberInfo::GetName(), __LINE__); go; go = false)               \
    for (record->Add(FunctionNameInfo::GetName(), __FILE__); go; go = false)             \


#endif // RECORD_COMMITTER_H__47B50854_6C4C_4F63_99BB_DDD346974727

