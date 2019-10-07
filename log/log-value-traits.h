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

#ifndef LOG_VALUE_TRAITS_H__2998CA64_47C5_43C8_920E_663857F623D1
#define LOG_VALUE_TRAITS_H__2998CA64_47C5_43C8_920E_663857F623D1


#include <nsfx/log/config.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Defines a class that provides information about a log value.
 *
 * @param Class The traits class.
 * @param name  The name of the log value.
 * @param type  The type of the log value.
 *
 * The traits class `Class` provides a static member function `GetName()` to obtain
 * the name of the attribute value.
 * It also provides a nested `Type` that is the type of the underlying value.
 */
#define NSFX_DEFINE_LOG_VALUE_TRAITS(Class, name, type)  \
    class Class                                          \
    {                                                    \
    public:                                              \
        static const char* GetName(void) BOOST_NOEXCEPT  \
        {                                                \
            return name;                                 \
        }                                                \
        typedef type Type;                               \
    };                                                   \


#endif // LOG_VALUE_TRAITS_H__2998CA64_47C5_43C8_920E_663857F623D1

