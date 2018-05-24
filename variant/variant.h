/**
 * @file
 *
 * @brief Variant support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7
#define VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7


#include <nsfx/variant/config.h>
#include <nsfx/log/core/exception.h>
#include <boost/type_index.hpp>
#include <memory>  // shared_ptr
#include <utility> // move, forward
#include <cstring> // strlen, memcpy, memcmp


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
enum VariantType
{
    VT_EMPTY = 0,

    VT_BOOL,
    VT_INT8,
    VT_UINT8,
    VT_INT16,
    VT_UINT16,
    VT_INT32,
    VT_UINT32,
    VT_INT64,
    VT_UINT64,

    VT_FLOAT,
    VT_DOUBLE,

    VT_STRING,

    /**
     * @brief A pointer to \c IObject that holds a reference count.
     */
    VT_OBJECT,

};


typedef uint32_t  var_type_t;


/**
 * @ingroup Variant
 * @brief The union-based variant.
 */
class Variant
{
private:
    void Init(void) BOOST_NOEXCEPT;

public:
    // Xtructors.
    Variant(void) BOOST_NOEXCEPT;

    Variant(bool     b)   BOOST_NOEXCEPT;
    Variant(int8_t   i8)  BOOST_NOEXCEPT;
    Variant(uint8_t  u8)  BOOST_NOEXCEPT;
    Variant(int16_t  i16) BOOST_NOEXCEPT;
    Variant(uint16_t u16) BOOST_NOEXCEPT;
    Variant(int32_t  i32) BOOST_NOEXCEPT;
    Variant(uint32_t u32) BOOST_NOEXCEPT;
    Variant(int64_t  i64) BOOST_NOEXCEPT;
    Variant(uint64_t u64) BOOST_NOEXCEPT;
    Variant(float    d32) BOOST_NOEXCEPT;
    Variant(double   d64) BOOST_NOEXCEPT;
    Variant(const char* str);
    Variant(const std::string& str);
    Variant(Ptr<IObject> obj) BOOST_NOEXCEPT;

    ~Variant(void);

    // Copyable.
    Variant(const Variant& rhs);
    Variant& operator=(const Variant& rhs);

    // Movable.
    Variant(Variant&& rhs) BOOST_NOEXCEPT;
    Variant& operator=(Variant&& rhs);

public:
    Variant& operator=(bool     b);
    Variant& operator=(int8_t   i8);
    Variant& operator=(uint8_t  u8);
    Variant& operator=(int16_t  i16);
    Variant& operator=(uint16_t u16);
    Variant& operator=(int32_t  i32);
    Variant& operator=(uint32_t u32);
    Variant& operator=(int64_t  i64);
    Variant& operator=(uint64_t u64);
    Variant& operator=(float    d32);
    Variant& operator=(double   d64);
    Variant& operator=(const char* str);
    Variant& operator=(const std::string& str);
    Variant& operator=(Ptr<IObject> obj);

public:
    var_type_t GetType(void) const BOOST_NOEXCEPT;

    void Clear(void) BOOST_NOEXCEPT;

    void CopyFrom(const Variant& src);
    void CopyTo(Variant& dest);

    void swap(Variant& rhs) BOOST_NOEXCEPT;

public:
    bool operator==(const Variant& rhs) const BOOST_NOEXCEPT;
    bool operator!=(const Variant& rhs) const BOOST_NOEXCEPT;

private:
    var_type_t type_;

    union
    {
        bool     b;

        int8_t   i8;
        uint8_t  u8;
        int16_t  i16;
        uint16_t u16;
        int32_t  i32;
        uint32_t u32;
        int64_t  i64;
        uint64_t u64;

        float    d32;
        double   d64;

        char*    str;

        IObject* obj;

    } value_;
};


////////////////////////////////////////////////////////////////////////////////
inline void Variant::Init(void) BOOST_NOEXCEPT
{
    type_ = VT_EMPTY;
    value_.u64 = 0;
}

inline Variant::Variant(void) BOOST_NOEXCEPT
{
    Init();
}

inline Variant::Variant(bool b) BOOST_NOEXCEPT
{
    type_ = VT_BOOL;
    value_.b = b;
}

inline Variant::Variant(int8_t i8) BOOST_NOEXCEPT
{
    type_ = VT_I8;
    value_.i8 = i8;
}

inline Variant::Variant(uint8_t u8) BOOST_NOEXCEPT
{
    type_ = VT_U8;
    value_.u8 = u8;
}

inline Variant::Variant(int16_t i16) BOOST_NOEXCEPT
{
    type_ = VT_I16;
    value_.i16 = i16;
}

inline Variant::Variant(uint16_t u16) BOOST_NOEXCEPT
{
    type_ = VT_U16;
    value_.u16 = u16;
}

inline Variant::Variant(int32_t i32) BOOST_NOEXCEPT
{
    type_ = VT_I32;
    value_.i32 = i32;
}

inline Variant::Variant(uint32_t u32) BOOST_NOEXCEPT
{
    type_ = VT_U32;
    value_.u32 = u32;
}

inline Variant::Variant(int64_t i64) BOOST_NOEXCEPT
{
    type_ = VT_I64;
    value_.i64 = i64;
}

inline Variant::Variant(uint64_t u64) BOOST_NOEXCEPT
{
    type_ = VT_U64;
    value_.u64 = u64;
}

inline Variant::Variant(float d32) BOOST_NOEXCEPT
{
    type_ = VT_D32;
    value_.d32 = d32;
}

inline Variant::Variant(double d64) BOOST_NOEXCEPT
{
    type_ = VT_D64;
    value_.d64 = d64;
}

inline Variant::Variant(const char* str)
{
    type_ = VT_STRING;
    value_.str = nullptr;
    size_t len = str ? std::strlen(str) : 0;
    if (len)
    {
        value_.str = new char[len + 1];
        if (value_.str)
        {
            std::memcpy(value_.str, str, len + 1);
        }
    }
}

inline Variant::Variant(const std::string& str)
{
    type_ = VT_STRING;
    value_.str = nullptr;
    size_t len = str.size();
    if (len)
    {
        value_.str = new char[len + 1];
        if (value_.str)
        {
            std::memcpy(value_.str, str.c_str(), len + 1);
        }
    }
}

inline Variant::Variant(Ptr<IObject> obj) BOOST_NOEXCEPT
{
    type_ = VT_OBJECT;
    value_.obj = obj.Detach();
}

inline Variant::~Variant(void)
{
    Clear();
}

inline Variant::Variant(const Variant& rhs)
{
    CopyFrom(rhs);
}

inline Variant& Variant::operator=(const Variant& rhs)
{
    CopyFrom(rhs);
    return *this;
}

inline Variant::Variant(Variant&& rhs) BOOST_NOEXCEPT
{
    Init();
    swap(rhs);
}

inline Variant& Variant::operator=(Variant&& rhs)
{
    if (this != &rhs)
    {
        Clear();
        swap(rhs);
    }
    return *this;
}

inline Variant& Variant::operator=(bool b)
{
    if (type_ != VT_BOOL)
    {
        Clear();
        type_ = VT_BOOL;
    }
    value_.b = b;
    return *this;
}

inline Variant& Variant::operator=(int8_t i8)
{
    if (type_ != VT_I8)
    {
        Clear();
        type_ = VT_I8;
    }
    value_.i8 = i8;
    return *this;
}

inline Variant& Variant::operator=(uint8_t u8)
{
    if (type_ != VT_U8)
    {
        Clear();
        type_ = VT_U8;
    }
    value_.u8 = u8;
    return *this;
}

inline Variant& Variant::operator=(int16_t i16)
{
    if (type_ != VT_I16)
    {
        Clear();
        type_ = VT_I16;
    }
    value_.i16 = i16;
    return *this;
}

inline Variant& Variant::operator=(uint16_t u16)
{
    if (type_ != VT_U16)
    {
        Clear();
        type_ = VT_U16;
    }
    value_.u16 = u16;
    return *this;
}

inline Variant& Variant::operator=(int32_t i32)
{
    if (type_ != VT_I32)
    {
        Clear();
        type_ = VT_I32;
    }
    value_.i32 = i32;
    return *this;
}

inline Variant& Variant::operator=(uint32_t u32)
{
    if (type_ != VT_U32)
    {
        Clear();
        type_ = VT_U32;
    }
    value_.u32 = u32;
    return *this;
}

inline Variant& Variant::operator=(int64_t i64)
{
    if (type_ != VT_I64)
    {
        Clear();
        type_ = VT_I64;
    }
    value_.i64 = i64;
    return *this;
}

inline Variant& Variant::operator=(uint64_t u64)
{
    if (type_ != VT_U64)
    {
        Clear();
        type_ = VT_U64;
    }
    value_.u64 = u64;
    return *this;
}

inline Variant& Variant::operator=(float d32)
{
    if (type_ != VT_D32)
    {
        Clear();
        type_ = VT_D32;
    }
    value_.d32 = d32;
    return *this;
}

inline Variant& Variant::operator=(double d64)
{
    if (type_ != VT_D64)
    {
        Clear();
        type_ = VT_D64;
    }
    value_.d64 = d64;
    return *this;
}

inline Variant& Variant::operator=(const char* str)
{
    Clear();
    type_ = VT_STRING;
    size_t len = str ? std::strlen(str) : 0;
    if (len)
    {
        value_.str = new char[len + 1];
        std::memcpy(value_.str, str, len + 1);
    }
    return *this;
}

inline Variant& Variant::operator=(const std::string& str)
{
    Clear();
    type_ = VT_STRING;
    size_t len = str.size();
    if (len)
    {
        value_.str = new char[len + 1];
        std::memcpy(value_.str, str.c_str(), len + 1);
    }
    return *this;
}

inline Variant& Variant::operator=(Ptr<IObject> obj)
{
    Clear();
    type_ = VT_OBJECT;
    value_ = obj.Detach();
    return *this;
}

inline var_type_t Variant::GetType(void) const BOOST_NOEXCEPT
{
    return type_;
}

inline void Variant::Clear(void)
{
    switch (type_)
    {
    case VT_EMPTY:
    case VT_INT8:
    case VT_UINT8:
    case VT_INT16:
    case VT_UINT16:
    case VT_INT32:
    case VT_UINT32:
    case VT_INT64:
    case VT_UINT64:
    case VT_FLOAT:
    case VT_DOUBLE:
        value_.u64 = 0;
        break;

    case VT_STRING:
        if (value_.str)
        {
            char* tmp = value_.str;
            value_.str = nullptr;
            delete[] tmp;
        }
        break;

    case VT_OBJECT:
        if (value_.obj)
        {
            value_.obj->Release();
            value_.obj = nullptr;
        }
        break;

    default:
        BOOST_ASSERT_MSG(false, "Invalid variant type.");
        break;
    }
    type_ = VT_EMPTY;
}

inline void Variant::CopyFrom(const Variant& src)
{
    if (this != &src)
    {
        Clear();
        switch (src.type_)
        {
        case VT_EMPTY:
        case VT_INT8:
        case VT_UINT8:
        case VT_INT16:
        case VT_UINT16:
        case VT_INT32:
        case VT_UINT32:
        case VT_INT64:
        case VT_UINT64:
        case VT_FLOAT:
        case VT_DOUBLE:
            value_.u64 = src.value_.u64;
            break;

        case VT_STRING:
            {
                size_t len = src.value_.str ? strlen(src.value_.str) : 0;
                if (len)
                {
                    value_.str = new char[len + 1];
                    std::memcpy(value_.str, src.value_.str, len + 1);
                }
            }
            break;

        case VT_OBJECT:
            if (src.value_.obj)
            {
                src.value_.obj->AddRef();
                value_.obj = src.value_.obj;
            }
            break;
        }
        type_ = src.type_;
    }
}

inline void Variant::CopyTo(Variant& dest)
{
    dest.CopyFrom(*this);
}

inline void Variant::swap(Variant& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(type_, rhs.type_);
        boost::swap(value_.u64, rhs.value_.u64);
    }
}

bool Variant::operator==(const Variant& rhs) const BOOST_NOEXCEPT
{
    bool result;
    if (this == &rhs)
    {
        result = true;
    }
    else // if (this != &rhs)
    {
        if (type_ != rhs.type_)
        {
            result = false;
        }
        else // if (type_ == rhs.type_)
        {
            switch (type_)
            {
            case VT_BOOL:
            case VT_INT8:
            case VT_UINT8:
                result = (value_.u8 == rhs.value_.u8)
                break;

            case VT_INT16:
            case VT_UINT16:
                result = (value_.u16 == rhs.value_.u16)
                break;

            case VT_INT32:
            case VT_UINT32:
            case VT_FLOAT:
                result = (value_.u32 == rhs.value_.u32)
                break;

            case VT_INT64:
            case VT_UINT64:
            case VT_DOUBLE:
                result = (value_.u64 == rhs.value_.u64)
                break;

            case VT_STRING:
                {
                    size_t len1 =     value_.str ? std::strlen(    value_.str) : 0;
                    size_t len2 = rhs.value_.str ? std::strlen(rhs.value_.str) : 0;
                    result = (len1 == len2) &&
                             (std::memcmp(value_.str, rhs.value_.str, len1) == 0);
                }
                break;

            case VT_OBJECT:
                result = (value.obj == rhs.value_.obj);
                break;

            default:
                BOOST_ASSERT_MSG(false, "Invalid variant type.");
                break;
            }
        }
    }
    return result;
}

bool Variant::operator!=(const Variant& rhs) const BOOST_NOEXCEPT
{
    return !(*this == rhs);
}


////////////////////////////////////////////////////////////////////////////////
inline swap(Variant& lhs, Variant& rhs)  BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7

