/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef COMMON_ATTRIBUTE_VALUE_INFO_H__0C15BECC_76C3_4560_9731_4A9CC6E877FB
#define COMMON_ATTRIBUTE_VALUE_INFO_H__0C15BECC_76C3_4560_9731_4A9CC6E877FB


#include <nsfx/log/config.h>
#include <nsfx/log/core/record/attribute-value-info.h>
#include <string>


NSFX_LOG_OPEN_NAMESPACE


NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(MessageInfo,      "Message",      std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(FunctionNameInfo, "FunctionName", std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(FileNameInfo,     "FileName",     std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(LineNumberInfo,   "LineNumber",   size_t);


NSFX_LOG_CLOSE_NAMESPACE


#endif // COMMON_ATTRIBUTE_VALUE_INFO_H__0C15BECC_76C3_4560_9731_4A9CC6E877FB

