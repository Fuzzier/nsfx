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
    static const vartype_t type  = VT_EMPTY;
    static const bool      cpp   = false;
    static const bool      byref = false;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "unsupported";
    }
};


template<class T>
struct VariantTraits<const T> : VariantTraits<T> {};


#define NSFX_DEFINE_VARIANT_TRAITS(T, VT, CPP, BYREF, NAME)  \
    template<>                                               \
    struct VariantTraits<T>                                  \
    {                                                        \
        static const vartype_t type  = VT;                   \
        static const bool      cpp   = CPP;                  \
        static const bool      byref = BYREF;                \
        static const char* GetTypeName() BOOST_NOEXCEPT      \
        {                                                    \
            return ##NAME;                                   \
        }                                                    \
    }


////////////////////////////////////////////////////////////////////////////////
//                         T             VT          CPP     BYREF   NAME
NSFX_DEFINE_VARIANT_TRAITS(void,         VT_EMPTY,   false,  false,  "void");
NSFX_DEFINE_VARIANT_TRAITS(bool,         VT_BOOL,    false,  false,  "bool");
NSFX_DEFINE_VARIANT_TRAITS(int8_t,       VT_INT8,    false,  false,  "int8");
NSFX_DEFINE_VARIANT_TRAITS(uint8_t,      VT_UINT8,   false,  false,  "uint8");
NSFX_DEFINE_VARIANT_TRAITS(int16_t,      VT_INT16,   false,  false,  "int16");
NSFX_DEFINE_VARIANT_TRAITS(uint16_t,     VT_UINT16,  false,  false,  "uint16");
NSFX_DEFINE_VARIANT_TRAITS(int32_t,      VT_INT32,   false,  false,  "int32");
NSFX_DEFINE_VARIANT_TRAITS(uint32_t,     VT_UINT32,  false,  false,  "uint32");
NSFX_DEFINE_VARIANT_TRAITS(int64_t,      VT_INT64,   false,  false,  "int64");
NSFX_DEFINE_VARIANT_TRAITS(uint64_t,     VT_UINT64,  false,  false,  "uint64");
NSFX_DEFINE_VARIANT_TRAITS(float,        VT_FLOAT,   false,  false,  "float");
NSFX_DEFINE_VARIANT_TRAITS(double,       VT_DOUBLE,  false,  false,  "double");
NSFX_DEFINE_VARIANT_TRAITS(const char*,  VT_STRING,  false,  false,  "string");
NSFX_DEFINE_VARIANT_TRAITS(std::string,  VT_STRING,  true,   false,  "string");

template<class Res>
struct VariantTraits<chrono::Duration<Res>>
{
    static const vartype_t type  = VT_DURATION;
    static const bool      cpp   = true;
    static const bool      byref = false;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "Duration";
    }
};

template<class Clock, class Duration>
struct VariantTraits<chrono::TimePoint<Clock, Duration>>
{
    static const vartype_t type  = VT_TIME_POINT;
    static const bool      cpp   = true;
    static const bool      byref = false;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "TimePoint";
    }
};

// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IObjectInfo>,    VT_OBJECT_INFO,    "object_info");
// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IInterfaceInfo>, VT_INTERFACE_INFO, "interface_info");
// NSFX_DEFINE_VARIANT_TRAITS(Ptr<IMethodInfo>,    VT_METHOD_INFO,    "method_info");


template<class I>
struct VariantTraits<Ptr<I>>
{
    static const vartype_t type  = VT_OBJECT;
    static const bool      cpp   = true;
    static const bool      byref = false;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "IObject*";
    }
};


////////////////////////////////////////////////////////////////////////////////
//                         T              VT          CPP     BYREF   NAME
NSFX_DEFINE_VARIANT_TRAITS(bool*,         VT_BOOL,    false,  true,  "bool*");
NSFX_DEFINE_VARIANT_TRAITS(int8_t*,       VT_INT8,    false,  true,  "int8*");
NSFX_DEFINE_VARIANT_TRAITS(uint8_t*,      VT_UINT8,   false,  true,  "uint8*");
NSFX_DEFINE_VARIANT_TRAITS(int16_t*,      VT_INT16,   false,  true,  "int16*");
NSFX_DEFINE_VARIANT_TRAITS(uint16_t*,     VT_UINT16,  false,  true,  "uint16*");
NSFX_DEFINE_VARIANT_TRAITS(int32_t*,      VT_INT32,   false,  true,  "int32*");
NSFX_DEFINE_VARIANT_TRAITS(uint32_t*,     VT_UINT32,  false,  true,  "uint32*");
NSFX_DEFINE_VARIANT_TRAITS(int64_t*,      VT_INT64,   false,  true,  "int64*");
NSFX_DEFINE_VARIANT_TRAITS(uint64_t*,     VT_UINT64,  false,  true,  "uint64*");
NSFX_DEFINE_VARIANT_TRAITS(float*,        VT_FLOAT,   false,  true,  "float*");
NSFX_DEFINE_VARIANT_TRAITS(double*,       VT_DOUBLE,  false,  true,  "double*");
NSFX_DEFINE_VARIANT_TRAITS(const char**,  VT_STRING,  false,  true,  "string*");
NSFX_DEFINE_VARIANT_TRAITS(std::string*,  VT_STRING,  true,   true,  "string*");


template<class Res>
struct VariantTraits<chrono::Duration<Res>* >
{
    static const vartype_t type  = VT_DURATION;
    static const bool      cpp   = true;
    static const bool      byref = true;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "Duration*";
    }
};

template<class Clock, class Duration>
struct VariantTraits<chrono::TimePoint<Clock, Duration>* >
{
    static const vartype_t type  = VT_TIME_POINT;
    static const bool      cpp   = true;
    static const bool      byref = true;
    static const char* GetTypeName() BOOST_NOEXCEPT
    {
        return "TimePoint*";
    }
};


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_TRAITS_H__9180389D_9AB4_4AC8_9BE8_8CDAEDF1BEAB

