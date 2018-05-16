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

#ifndef ATTRIBUTE_VALUE_INFO_H__FBD60529_FF16_48B6_A1AB_937B2C4D79BA
#define ATTRIBUTE_VALUE_INFO_H__FBD60529_FF16_48B6_A1AB_937B2C4D79BA


#include <nsfx/log/config.h>
#include <nsfx/log/misc/severity-level.h>
#include <boost/type_traits/integral_constant.hpp>


template<class T>
class NsfxIsAttributeValueInfo : public ::boost::false_type {};


NSFX_LOG_OPEN_NAMESPACE


#define NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(Class, name, type)  \
    class Class                                              \
    {                                                        \
    public:                                                  \
        static const char* GetName(void)                     \
        {                                                    \
            return name;                                     \
        }                                                    \
        typedef type Type;                                   \
    };                                                       \
    template<>                                               \
    class ::NsfxIsAttributeValueInfo<Class> :                \
        public ::boost::true_type {}


NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(SeverityLevelInfo, "SeverityLevel", uint32_t);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(MessageInfo,       "Message",       std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(FunctionNameInfo,  "FunctionName",  std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(FileNameInfo,      "FileName",      std::string);
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(LineNumberInfo,    "LineNumber",    size_t);


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_VALUE_INFO_H__FBD60529_FF16_48B6_A1AB_937B2C4D79BA

