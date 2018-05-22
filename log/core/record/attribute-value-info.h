/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
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
#include <boost/type_traits/integral_constant.hpp>


template<class T>
class NsfxIsAttributeValueInfo : public ::boost::false_type {};


/**
 * @ingroup Log
 * @brief Defines a class that provides information about an attribute value.
 *
 * The \c Class provides a static member function \c GetName() to obtain the
 * name of the attribute value.
 * It also provides a nested \c Type that is the type of the underlying value.
 */
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


#endif // ATTRIBUTE_VALUE_INFO_H__FBD60529_FF16_48B6_A1AB_937B2C4D79BA

