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

#ifndef VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7
#define VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7


#include <nsfx/interop/variant/config.h>
#include <nsfx/interop/variant/variant-type.h>
#include <nsfx/interop/variant/variant-traits.h>
#include <nsfx/interop/variant/variant-string.h>
#include <nsfx/chrono/duration.h>
#include <nsfx/chrono/time-point.h>
#include <boost/type_traits/type_identity.hpp>
#include <string>
#include <utility> // move
#include <type_traits>  // decay, is_standard_layout


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Variant
 * @brief A standard-layout C++ encapsulation of \c variant_t.
 */
class Variant :
    private variant_t
{
private:
    /**
     * @brief Make an empty variant.
     */
    void Init(void) BOOST_NOEXCEPT;

    /**
     * @brief Copy from a variant.
     *
     * @pre This variant is either uninitialized or empty.
     */
    void CopyCtor(const Variant& rhs);

public:
    // Xtructors.
    /**
     * @brief Construct an empty variant.
     */
    Variant(void) BOOST_NOEXCEPT;

    Variant(bool         value) BOOST_NOEXCEPT;
    Variant(int8_t       value) BOOST_NOEXCEPT;
    Variant(uint8_t      value) BOOST_NOEXCEPT;
    Variant(int16_t      value) BOOST_NOEXCEPT;
    Variant(uint16_t     value) BOOST_NOEXCEPT;
    Variant(int32_t      value) BOOST_NOEXCEPT;
    Variant(uint32_t     value) BOOST_NOEXCEPT;
    Variant(int64_t      value) BOOST_NOEXCEPT;
    Variant(uint64_t     value) BOOST_NOEXCEPT;
    Variant(float        value) BOOST_NOEXCEPT;
    Variant(double       value) BOOST_NOEXCEPT;
    Variant(const char*  value);
    Variant(const std::string& value);
    Variant(Ptr<IObject> value) BOOST_NOEXCEPT;

    template<class Res>
    Variant(chrono::Duration<Res> value) BOOST_NOEXCEPT;

    template<class Clock, class Duration>
    Variant(chrono::TimePoint<Clock, Duration> value) BOOST_NOEXCEPT;

    ~Variant(void);

    // Copyable.
    Variant(const Variant& rhs);
    Variant& operator=(const Variant& rhs);

    // Movable.
    Variant(Variant&& rhs) BOOST_NOEXCEPT;
    Variant& operator=(Variant&& rhs);

public:
    Variant& operator=(bool         value);
    Variant& operator=(int8_t       value);
    Variant& operator=(uint8_t      value);
    Variant& operator=(int16_t      value);
    Variant& operator=(uint16_t     value);
    Variant& operator=(int32_t      value);
    Variant& operator=(uint32_t     value);
    Variant& operator=(int64_t      value);
    Variant& operator=(uint64_t     value);
    Variant& operator=(float        value);
    Variant& operator=(double       value);
    Variant& operator=(const char*  value);
    Variant& operator=(const std::string& value);
    Variant& operator=(Ptr<IObject> value);

    template<class Res>
    Variant& operator=(chrono::Duration<Res> value);

    template<class Clock, class Duration>
    Variant& operator=(chrono::TimePoint<Clock, Duration> value);

public:
    vartype_t GetType(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the value.
     *
     * @tparam T  The type of the value.
     *
     * @pre \c T matches the vartype of the variant.
     *      <p>
     *      The vartype of the variant must not have the \c VT_BYREF flag.
     *
     * @remarks This is a no-throw function.
     *          Users <b>must</b> ensure the type \c T matches the vartype of
     *          the variant.
     */
    template<class T>
    T GetValue(void) const BOOST_NOEXCEPT;

private:
    template<class T>
    struct ValueTypeTag {};

    bool        GetValue(ValueTypeTag<bool>)        const BOOST_NOEXCEPT;
    int8_t      GetValue(ValueTypeTag<int8_t>)      const BOOST_NOEXCEPT;
    uint8_t     GetValue(ValueTypeTag<uint8_t>)     const BOOST_NOEXCEPT;
    int16_t     GetValue(ValueTypeTag<int16_t>)     const BOOST_NOEXCEPT;
    uint16_t    GetValue(ValueTypeTag<uint16_t>)    const BOOST_NOEXCEPT;
    int32_t     GetValue(ValueTypeTag<int32_t>)     const BOOST_NOEXCEPT;
    uint32_t    GetValue(ValueTypeTag<uint32_t>)    const BOOST_NOEXCEPT;
    int64_t     GetValue(ValueTypeTag<int64_t>)     const BOOST_NOEXCEPT;
    uint64_t    GetValue(ValueTypeTag<uint64_t>)    const BOOST_NOEXCEPT;
    float       GetValue(ValueTypeTag<float>)       const BOOST_NOEXCEPT;
    double      GetValue(ValueTypeTag<double>)      const BOOST_NOEXCEPT;
    const char* GetValue(ValueTypeTag<const char*>) const BOOST_NOEXCEPT;
    std::string GetValue(ValueTypeTag<std::string>) const BOOST_NOEXCEPT;

    template<class Res>
    chrono::Duration<Res>
    GetValue(ValueTypeTag<chrono::Duration<Res>>) const BOOST_NOEXCEPT;

    template<class Clock, class Duration>
    chrono::TimePoint<Clock, Duration>
    GetValue(ValueTypeTag<chrono::TimePoint<Clock, Duration>>) const BOOST_NOEXCEPT;

public:
    void Clear(void) BOOST_NOEXCEPT;

    void swap(Variant& rhs) BOOST_NOEXCEPT;

public:
    bool operator==(const Variant& rhs) const BOOST_NOEXCEPT;
    bool operator!=(const Variant& rhs) const BOOST_NOEXCEPT;
};


// static_assert(std::is_standard_layout<Variant>::value,
//               "The Variant class does not have a standard layout.");


////////////////////////////////////////////////////////////////////////////////
inline void Variant::Init(void) BOOST_NOEXCEPT
{
    type_ = VT_EMPTY;
    value_.dt = 0;
}

inline void Variant::CopyCtor(const Variant& rhs)
{
    switch (rhs.type_)
    {
    case VT_EMPTY:
    case VT_BOOL:
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
    case VT_DURATION:
    case VT_TIME_POINT:
        value_.dt = rhs.value_.dt;
        break;

    case VT_STRING:
        {
            value_.str = VariantStringCopy(rhs.value_.str);
        }
        break;

    case VT_OBJECT:
        if (rhs.value_.obj)
        {
            rhs.value_.obj->AddRef();
        }
        value_.obj = rhs.value_.obj;
        break;

    default:
        BOOST_ASSERT_MSG(false, "Unsupported variant type.");
        break;
    }
    type_ = rhs.type_;
}

inline Variant::Variant(void) BOOST_NOEXCEPT
{
    Init();
}

inline Variant::Variant(bool value) BOOST_NOEXCEPT
{
    type_ = VT_BOOL;
    value_.b = value;
}

inline Variant::Variant(int8_t value) BOOST_NOEXCEPT
{
    type_ = VT_INT8;
    value_.i8 = value;
}

inline Variant::Variant(uint8_t value) BOOST_NOEXCEPT
{
    type_ = VT_UINT8;
    value_.u8 = value;
}

inline Variant::Variant(int16_t value) BOOST_NOEXCEPT
{
    type_ = VT_INT16;
    value_.i16 = value;
}

inline Variant::Variant(uint16_t value) BOOST_NOEXCEPT
{
    type_ = VT_UINT16;
    value_.u16 = value;
}

inline Variant::Variant(int32_t value) BOOST_NOEXCEPT
{
    type_ = VT_INT32;
    value_.i32 = value;
}

inline Variant::Variant(uint32_t value) BOOST_NOEXCEPT
{
    type_ = VT_UINT32;
    value_.u32 = value;
}

inline Variant::Variant(int64_t value) BOOST_NOEXCEPT
{
    type_ = VT_INT64;
    value_.i64 = value;
}

inline Variant::Variant(uint64_t value) BOOST_NOEXCEPT
{
    type_ = VT_UINT64;
    value_.u64 = value;
}

inline Variant::Variant(float value) BOOST_NOEXCEPT
{
    type_ = VT_FLOAT;
    value_.d32 = value;
}

inline Variant::Variant(double value) BOOST_NOEXCEPT
{
    type_ = VT_DOUBLE;
    value_.d64 = value;
}

template<class Res>
inline Variant::Variant(chrono::Duration<Res> value) BOOST_NOEXCEPT
{
    type_ = VT_DURATION;
    value_.dt = value.GetCount();
}

template<class Clock, class Duration>
inline Variant::Variant(chrono::TimePoint<Clock, Duration> value) BOOST_NOEXCEPT
{
    type_ = VT_TIME_POINT;
    value_.t0 = value.GetDuration().GetCount();
}

inline Variant::Variant(const char* str)
{
    type_ = VT_STRING;
    value_.str = VariantStringCopy(str);
}

inline Variant::Variant(const std::string& str)
{
    type_ = VT_STRING;
    value_.str = VariantStringCopy(str.c_str());
}

inline Variant::Variant(Ptr<IObject> value) BOOST_NOEXCEPT
{
    type_ = VT_OBJECT;
    value_.obj = value.Detach();
}

inline Variant::~Variant(void)
{
    Clear();
}

inline Variant::Variant(const Variant& rhs)
{
    CopyCtor(rhs);
}

inline Variant& Variant::operator=(const Variant& rhs)
{
    Clear();
    CopyCtor(rhs);
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

inline Variant& Variant::operator=(bool value)
{
    if (type_ != VT_BOOL)
    {
        Clear();
        type_ = VT_BOOL;
    }
    value_.b = value;
    return *this;
}

inline Variant& Variant::operator=(int8_t value)
{
    if (type_ != VT_INT8)
    {
        Clear();
        type_ = VT_INT8;
    }
    value_.i8 = value;
    return *this;
}

inline Variant& Variant::operator=(uint8_t value)
{
    if (type_ != VT_UINT8)
    {
        Clear();
        type_ = VT_UINT8;
    }
    value_.u8 = value;
    return *this;
}

inline Variant& Variant::operator=(int16_t value)
{
    if (type_ != VT_INT16)
    {
        Clear();
        type_ = VT_INT16;
    }
    value_.i16 = value;
    return *this;
}

inline Variant& Variant::operator=(uint16_t value)
{
    if (type_ != VT_UINT16)
    {
        Clear();
        type_ = VT_UINT16;
    }
    value_.u16 = value;
    return *this;
}

inline Variant& Variant::operator=(int32_t value)
{
    if (type_ != VT_INT32)
    {
        Clear();
        type_ = VT_INT32;
    }
    value_.i32 = value;
    return *this;
}

inline Variant& Variant::operator=(uint32_t value)
{
    if (type_ != VT_UINT32)
    {
        Clear();
        type_ = VT_UINT32;
    }
    value_.u32 = value;
    return *this;
}

inline Variant& Variant::operator=(int64_t value)
{
    if (type_ != VT_INT64)
    {
        Clear();
        type_ = VT_INT64;
    }
    value_.i64 = value;
    return *this;
}

inline Variant& Variant::operator=(uint64_t value)
{
    if (type_ != VT_UINT64)
    {
        Clear();
        type_ = VT_UINT64;
    }
    value_.u64 = value;
    return *this;
}

inline Variant& Variant::operator=(float value)
{
    if (type_ != VT_FLOAT)
    {
        Clear();
        type_ = VT_FLOAT;
    }
    value_.d32 = value;
    return *this;
}

inline Variant& Variant::operator=(double value)
{
    if (type_ != VT_DOUBLE)
    {
        Clear();
        type_ = VT_DOUBLE;
    }
    value_.d64 = value;
    return *this;
}

template<class Res>
inline Variant& Variant::operator=(chrono::Duration<Res> value) BOOST_NOEXCEPT
{
    if (type_ != VT_DURATION)
    {
        Clear();
        type_ = VT_DURATION;
    }
    value_.dt = value.GetCount();
    return *this;
}

template<class Clock, class Duration>
inline Variant& Variant::operator=(chrono::TimePoint<Clock, Duration> value) BOOST_NOEXCEPT
{
    if (type_ != VT_TIME_POINT)
    {
        Clear();
        type_ = VT_TIME_POINT;
    }
    value_.t0 = value.GetDuration().GetCount();
    return *this;
}

inline Variant& Variant::operator=(const char* str)
{
    Clear();
    type_ = VT_STRING;
    value_.str = VariantStringCopy(str);
    return *this;
}

inline Variant& Variant::operator=(const std::string& str)
{
    return operator=(str.c_str());
}

inline Variant& Variant::operator=(Ptr<IObject> value)
{
    Clear();
    type_ = VT_OBJECT;
    value_.obj = value.Detach();
    return *this;
}

inline vartype_t Variant::GetType(void) const BOOST_NOEXCEPT
{
    return type_;
}

template<class T>
inline T Variant::GetValue(void) const BOOST_NOEXCEPT
{
    typedef typename std::decay<T>::type  Type;
    static_assert(VariantTraits<Type>::type != VT_EMPTY,
                  "Cannot get the value of the variant, "
                  "since the requested type is unsupported.");
    BOOST_ASSERT_MSG(VariantTraits<Type>::type == type_,
                  "Cannot get the value of the variant, "
                  "since the requested type mismatches the type of the variant.");
    return GetValue(ValueTypeTag<Type>());
}

inline bool Variant::GetValue(ValueTypeTag<bool>) const BOOST_NOEXCEPT
{
    return value_.b;
}

inline int8_t Variant::GetValue(ValueTypeTag<int8_t>) const BOOST_NOEXCEPT
{
    return value_.i8;
}

inline uint8_t Variant::GetValue(ValueTypeTag<uint8_t>) const BOOST_NOEXCEPT
{
    return value_.u8;
}

inline int16_t Variant::GetValue(ValueTypeTag<int16_t>) const BOOST_NOEXCEPT
{
    return value_.i16;
}

inline uint16_t Variant::GetValue(ValueTypeTag<uint16_t>) const BOOST_NOEXCEPT
{
    return value_.u16;
}

inline int32_t Variant::GetValue(ValueTypeTag<int32_t>) const BOOST_NOEXCEPT
{
    return value_.i32;
}

inline uint32_t Variant::GetValue(ValueTypeTag<uint32_t>) const BOOST_NOEXCEPT
{
    return value_.u32;
}

inline int64_t Variant::GetValue(ValueTypeTag<int64_t>) const BOOST_NOEXCEPT
{
    return value_.i64;
}

inline uint64_t Variant::GetValue(ValueTypeTag<uint64_t>) const BOOST_NOEXCEPT
{
    return value_.u64;
}

inline float Variant::GetValue(ValueTypeTag<float>) const BOOST_NOEXCEPT
{
    return value_.d32;
}

inline double Variant::GetValue(ValueTypeTag<double>) const BOOST_NOEXCEPT
{
    return value_.d64;
}

inline const char* Variant::GetValue(ValueTypeTag<const char*>) const BOOST_NOEXCEPT
{
    return value_.str;
}

inline std::string Variant::GetValue(ValueTypeTag<std::string>) const BOOST_NOEXCEPT
{
    return std::string(value_.str);
}

template<class Res>
inline chrono::Duration<Res>
Variant::GetValue(ValueTypeTag<chrono::Duration<Res>>) const BOOST_NOEXCEPT
{
    return chrono::Duration<Res>(value_.dt);
}

template<class Clock, class Duration>
inline chrono::TimePoint<Clock, Duration>
Variant::GetValue(ValueTypeTag<chrono::TimePoint<Clock, Duration>>) const BOOST_NOEXCEPT
{
    return chrono::TimePoint<Clock, Duration>(Duration(value_.t0));
}

inline void Variant::Clear(void)
{
    switch (type_)
    {
    case VT_EMPTY:
    case VT_BOOL:
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
    case VT_DURATION:
    case VT_TIME_POINT:
        value_.dt = 0;
        break;

    case VT_STRING:
        if (value_.str)
        {
            const char* tmp = value_.str;
            value_.str = nullptr;
            VariantStringFree(tmp);
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
        BOOST_ASSERT_MSG(false, "Unsupported variant type.");
        break;
    }
    type_ = VT_EMPTY;
}

inline void Variant::swap(Variant& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(type_, rhs.type_);
        boost::swap(value_.dt, rhs.value_.dt);
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
            case VT_EMPTY:
                result = true;
                break;

            case VT_BOOL:
            case VT_INT8:
            case VT_UINT8:
                result = (value_.u8 == rhs.value_.u8);
                break;

            case VT_INT16:
            case VT_UINT16:
                result = (value_.u16 == rhs.value_.u16);
                break;

            case VT_INT32:
            case VT_UINT32:
                result = (value_.u32 == rhs.value_.u32);
                break;

            case VT_INT64:
            case VT_UINT64:
                result = (value_.u64 == rhs.value_.u64);
                break;

            case VT_FLOAT:
                result = (value_.d32 == rhs.value_.d32);
                break;

            case VT_DOUBLE:
                result = (value_.d64 == rhs.value_.d64);
                break;

            case VT_DURATION:
            case VT_TIME_POINT:
                result = (value_.dt == rhs.value_.dt);
                break;

            case VT_STRING:
                result = VariantStringCompare(value_.str, rhs.value_.str);
                break;

            case VT_OBJECT:
                // TODO
                result = (value_.obj == rhs.value_.obj);
                break;

            default:
                BOOST_ASSERT_MSG(false, "Unsupported variant type.");
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
inline void swap(Variant& lhs, Variant& rhs)  BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_H__CFD78002_28EE_4DFE_98B8_4A938E7A17A7

