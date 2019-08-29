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
#include <type_traits> // true_type, false_type


template<class T>
class NsfxIsLogValueTraits : public ::std::false_type {};


/**
 * @ingroup Log
 * @brief Defines a class that provides information about an attribute value.
 *
 * The \c Class provides a static member function \c GetName() to obtain the
 * name of the attribute value.
 * It also provides a nested \c Type that is the type of the underlying value.
 */
#define NSFX_DEFINE_LOG_VALUE_TRAITS(Class, name, type)   \
    class Class                                           \
    {                                                     \
    public:                                               \
        static BOOST_CONSTEXPR const char* GetName(void)  \
        {                                                 \
            return name;                                  \
        }                                                 \
        typedef type Type;                                \
    };                                                    \
    template<>                                            \
    class ::NsfxIsLogValueTraits<Class> :                 \
        public ::std::true_type {}


#endif // LOG_VALUE_TRAITS_H__2998CA64_47C5_43C8_920E_663857F623D1

