/**
 * @file
 *
 * @brief Variant support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-20
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef VARIANT_TRAITS_H__9180389D_9AB4_4AC8_9BE8_8CDAEDF1BEAB
#define VARIANT_TRAITS_H__9180389D_9AB4_4AC8_9BE8_8CDAEDF1BEAB


#include <nsfx/interop/variant/config.h>
#include <nsfx/interop/variant/variant-type.h>
#include <nsfx/chrono/duration.h>
#include <nsfx/chrono/time-point.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Variant
 * @brief The type traits for variant.
 *
 * @tparam T  A type.
 */
template<class T>
struct VariantTraits
{
    static const vartype_t type = VT_EMPTY;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "unsupported";
    }
};


template<class T>
struct VariantTraits<const T> : VariantTraits<T> {};


#define NSFX_DEFINE_VARIANT_TRAITS(T, vartype, name)     \
    template<>                                           \
    struct VariantTraits<T>                              \
    {                                                    \
        static const vartype_t type = vartype;           \
        static const char* GetTypeName() BOOST_NOEXCEPT  \
        {                                                \
            return ##name;                               \
        }                                                \
    }


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_VARIANT_TRAITS(void,         VT_EMPTY,   "void");
NSFX_DEFINE_VARIANT_TRAITS(bool,         VT_BOOL,    "bool");
NSFX_DEFINE_VARIANT_TRAITS(int8_t,       VT_INT8,    "int8");
NSFX_DEFINE_VARIANT_TRAITS(uint8_t,      VT_UINT8,   "uint8");
NSFX_DEFINE_VARIANT_TRAITS(int16_t,      VT_INT16,   "int16");
NSFX_DEFINE_VARIANT_TRAITS(uint16_t,     VT_UINT16,  "uint16");
NSFX_DEFINE_VARIANT_TRAITS(int32_t,      VT_INT32,   "int32");
NSFX_DEFINE_VARIANT_TRAITS(uint32_t,     VT_UINT32,  "uint32");
NSFX_DEFINE_VARIANT_TRAITS(int64_t,      VT_INT64,   "int64");
NSFX_DEFINE_VARIANT_TRAITS(uint64_t,     VT_UINT64,  "uint64");
NSFX_DEFINE_VARIANT_TRAITS(float,        VT_FLOAT,   "float");
NSFX_DEFINE_VARIANT_TRAITS(double,       VT_DOUBLE,  "double");
NSFX_DEFINE_VARIANT_TRAITS(const char*,  VT_STRING,  "string");
NSFX_DEFINE_VARIANT_TRAITS(std::string,  VT_STRING,  "string");

template<class Res>
struct VariantTraits<chrono::Duration<Res> >
{
    static const vartype_t type = VT_DURATION;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "Duration";
    }
};

template<class Clock, class Duration>
struct VariantTraits<chrono::TimePoint<Clock, Duration> >
{
    static const vartype_t type = VT_TIME_POINT;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "TimePoint";
    }
};

// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IObjectInfo>,    VT_OBJECT_INFO,    "object_info");
// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IInterfaceInfo>, VT_INTERFACE_INFO, "interface_info");
// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IMethodInfo>,    VT_METHOD_INFO,    "method_info");


template<class I>
struct VariantTraits<Ptr<I> >
{
    static const vartype_t type = VT_OBJECT;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "IObject*";
    }
};


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_VARIANT_TRAITS(bool*,         VT_BOOL   | VT_BYREF,  "bool*");
NSFX_DEFINE_VARIANT_TRAITS(int8_t*,       VT_INT8   | VT_BYREF,  "int8*");
NSFX_DEFINE_VARIANT_TRAITS(uint8_t*,      VT_UINT8  | VT_BYREF,  "uint8*");
NSFX_DEFINE_VARIANT_TRAITS(int16_t*,      VT_INT16  | VT_BYREF,  "int16*");
NSFX_DEFINE_VARIANT_TRAITS(uint16_t*,     VT_UINT16 | VT_BYREF,  "uint16*");
NSFX_DEFINE_VARIANT_TRAITS(int32_t*,      VT_INT32  | VT_BYREF,  "int32*");
NSFX_DEFINE_VARIANT_TRAITS(uint32_t*,     VT_UINT32 | VT_BYREF,  "uint32*");
NSFX_DEFINE_VARIANT_TRAITS(int64_t*,      VT_INT64  | VT_BYREF,  "int64*");
NSFX_DEFINE_VARIANT_TRAITS(uint64_t*,     VT_UINT64 | VT_BYREF,  "uint64*");
NSFX_DEFINE_VARIANT_TRAITS(float*,        VT_FLOAT  | VT_BYREF,  "float*");
NSFX_DEFINE_VARIANT_TRAITS(double*,       VT_DOUBLE | VT_BYREF,  "double*");
NSFX_DEFINE_VARIANT_TRAITS(const char**,  VT_STRING | VT_BYREF,  "string*");
NSFX_DEFINE_VARIANT_TRAITS(std::string*,  VT_STRING | VT_BYREF,  "string*");

template<class Res>
struct VariantTraits<chrono::Duration<Res>* >
{
    static const vartype_t type = VT_DURATION | VT_BYREF;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "Duration*";
    }
};

template<class Clock, class Duration>
struct VariantTraits<chrono::TimePoint<Clock, Duration>* >
{
    static const vartype_t type = VT_TIME_POINT | VT_BYREF;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "TimePoint*";
    }
};


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_TRAITS_H__9180389D_9AB4_4AC8_9BE8_8CDAEDF1BEAB

