/**
 * @file
 *
 * @brief Variant support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef VARIANT_TYPE_H__B123F27A_EBDE_47EC_8B98_FF30217B9E35
#define VARIANT_TYPE_H__B123F27A_EBDE_47EC_8B98_FF30217B9E35


#include <nsfx/interop/variant/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/chrono/config.h>
#include <type_traits>  // is_same


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
typedef uint32_t  vartype_t;


////////////////////////////////////////////////////////////////////////////////
enum VariantType
{
    VT_EMPTY  = 0,
    VT_BOOL   = 0x0001,
    VT_INT8   = 0x0002,
    VT_UINT8  = 0x0003,
    VT_INT16  = 0x0004,
    VT_UINT16 = 0x0005,
    VT_INT32  = 0x0006,
    VT_UINT32 = 0x0007,
    VT_INT64  = 0x0008,
    VT_UINT64 = 0x0009,
    VT_FLOAT  = 0x000a,
    VT_DOUBLE = 0x000b,

    VT_DURATION   = 0x000c,
    VT_TIME_POINT = 0x000d,

    VT_STRING = 0x000e,

    /**
     * @brief A pointer to \c IObject that holds a reference count.
     *
     * It is a reference to a component that does not expose \c IObjectInfo.
     */
    VT_OBJECT = 0x000f,

    /**
     * @brief A pointer to \c IObjectInfo that holds a reference count.
     *
     * It is a reference to a component that exposes \c IObjectInfo.
     */
    VT_OBJECT_INFO = 0x0010,

    /**
     * @brief A pointer to \c IInterfaceInfo that holds a reference count.
     */
    VT_INTERFACE_INFO = 0x0011,

    /**
     * @brief A pointer to \c IMethodInfo that holds a reference count.
     */
    VT_METHOD_INFO = 0x0012,
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Variant
 * @brief The union-based variant.
 */
struct variant_t
{
    vartype_t type_;

    union
    {
        bool        b;
        int8_t      i8;
        uint8_t     u8;
        int16_t     i16;
        uint16_t    u16;
        int32_t     i32;
        uint32_t    u32;
        int64_t     i64;
        uint64_t    u64;
        float       d32;
        double      d64;
        const char* str;
        int64_t     dt;   ///< \c nsfx::chrono::count_t.
        int64_t     t0;   ///< \c nsfx::chrono::count_t.
        IObject*    obj;  ///< Owns a reference count if it is not \c nullptr.

    } value_;
};


static_assert(std::is_same<chrono::count_t, int64_t>::value,
              "Unsupported type of nsfx::chrono::count_t, which must be int64_t.");


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_TYPE_H__B123F27A_EBDE_47EC_8B98_FF30217B9E35

