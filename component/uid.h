/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-04-15
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef UID_H__FA0E4336_645B_46BF_925D_4CB0FC2EAE40
#define UID_H__FA0E4336_645B_46BF_925D_4CB0FC2EAE40


#include <nsfx/component/config.h>
#include <nsfx/component/exception.h>
#include <nsfx/utility/type-identity.h>
#include <boost/type_index.hpp>
#include <cstring> // strlen, strcmp


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A human-readable universal identifier (UID).
 *
 * A UID is a string that identifies a class in a hierarchical namespace.
 * The general form is `"<organization>.<module>.<class>"`.
 * e.g., `"edu.uestc.nsfx.IObject"`.
 *
 * The use of string makes debugging much easier.
 */
class Uid
{
public:
    /**
     * @brief Construct a UID.
     *
     * @param[in] uid It **must** be a *string literal* or a string with
     *                static lifetime.
     */
    Uid(const char* uid) BOOST_NOEXCEPT :
        uid_(uid)
    {}

    operator const char*() const BOOST_NOEXCEPT
    {
        return uid_;
    }

    friend bool operator==(const Uid& lhs, const Uid& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const Uid& lhs, const Uid& rhs) BOOST_NOEXCEPT;

    friend bool operator==(const Uid& lhs, const char* rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const Uid& lhs, const char* rhs) BOOST_NOEXCEPT;

    friend bool operator==(const char* lhs, const Uid& rhs) BOOST_NOEXCEPT;
    friend bool operator!=(const char* lhs, const Uid& rhs) BOOST_NOEXCEPT;

    template<class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
           operator<<(std::basic_ostream<Char, Traits>& os, const Uid& uid);

    friend size_t hash_value(const Uid& uid);

private:
    const char* uid_;
};


////////////////////////////////////////////////////////////////////////////////
inline bool operator==(const Uid& lhs, const Uid& rhs) BOOST_NOEXCEPT
{
    return !std::strcmp(lhs.uid_, rhs.uid_);
}

inline bool operator!=(const Uid& lhs, const Uid& rhs) BOOST_NOEXCEPT
{
    return !!std::strcmp(lhs.uid_, rhs.uid_);
}

inline bool operator==(const Uid& lhs, const char* rhs) BOOST_NOEXCEPT
{
    return !std::strcmp(lhs.uid_, rhs);
}

inline bool operator!=(const Uid& lhs, const char* rhs) BOOST_NOEXCEPT
{
    return !!std::strcmp(lhs.uid_, rhs);
}

inline bool operator==(const char* lhs, const Uid& rhs) BOOST_NOEXCEPT
{
    return !std::strcmp(lhs, rhs.uid_);
}

inline bool operator!=(const char* lhs, const Uid& rhs) BOOST_NOEXCEPT
{
    return !!std::strcmp(lhs, rhs.uid_);
}

template<class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, const Uid& uid)
{
    return os << uid.uid_;
}

inline size_t hash_value(const Uid& uid)
{
    size_t length = std::strlen(uid.uid_);
    return boost::hash_range<const char*>(uid.uid_, uid.uid_ + length);
}


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
#if defined(NSFX_MSVC)
/* Define a global traits class template. */
template<class >
class NsfxUidTraits
{
public:
    BOOST_STATIC_CONSTANT(bool, value = false); /* UID is undefined. */
};

////////////////////////////////////////
// Macros.
# define NSFX_DEFINE_CLASS_UID(T, UID)                           \
    /* Declare an overloaded function. */                        \
    NsfxUidTraits<T>                                             \
    NsfxGetUidTraits(::nsfx::type_identity<T>) BOOST_NOEXCEPT;  \
    /* Specialization the global traits class template. */       \
    template<> class ::NsfxUidTraits<T>                          \
    {                                                            \
    public:                                                      \
        BOOST_STATIC_CONSTANT(bool, value = true);               \
        static ::nsfx::Uid GetUid(void) BOOST_NOEXCEPT           \
        {                                                        \
            return ::nsfx::Uid(UID);                             \
        }                                                        \
    }

////////////////////////////////////////////////////////////////////////////////
#else // !defined(NSFX_MSVC)
// A fallback UID traits class.
class NsfxUidTraitsFallback
{
public:
    BOOST_STATIC_CONSTANT(bool, value = false); /* UID is undefined. */
};

// An overloaded fallback function that provides explicit error information.
template<class T>
NsfxUidTraitsFallback NsfxGetUidTraits(T) BOOST_NOEXCEPT;

////////////////////////////////////////
// Macros.
/**
 * @ingroup Component
 *
 * @brief Associate a UID with a class in a non-intrusive way.
 *
 * @param T   The name of the class.
 *            It **must not** be a common identifier, not a template specialization.
 *            e.g., `MyClass` is OK, but `MyTemplate<C>` is illegal.
 * @param UID The UID of the class.
 *            It **must** be a string literal or a string with static lifetime.
 */
# define NSFX_DEFINE_CLASS_UID(T, UID)                           \
    /* Declare a local traits class template. */                 \
    template<class > class NsfxUidTraits;                        \
    /* Declare an overloaded function. */                        \
    NsfxUidTraits<T>                                             \
    NsfxGetUidTraits(::nsfx::type_identity<T>) BOOST_NOEXCEPT;  \
    /* Specialization the local traits class template. */        \
    template<> class NsfxUidTraits<T>                            \
    {                                                            \
    public:                                                      \
        BOOST_STATIC_CONSTANT(bool, value = true);               \
        static ::nsfx::Uid GetUid(void) BOOST_NOEXCEPT           \
        {                                                        \
            return ::nsfx::Uid(UID);                             \
        }                                                        \
    }

#endif // defined(NSFX_MSVC)


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief The traits of a class.
 *
 * The traits include a class identifier (IID).
 *
 * # Discussion
 * The target is to map a class type to an IID.
 * It is prefered to provide a *non-intrusive* method to associate an IID to
 * a class.
 *
 * The main obstacle is to bind some information for an unknown class that may
 * reside in another namespace.
 *
 * ## Method 1
 * The class type is bound to the type argument of a class template.
 * Information is provided as members of the its specialization.
 *
 * The method is to define a class template in the global namespace,
 * so it is visible in any namespaces.
 * Then let the users specialize the template in the local namespace.
 * The specialization provides necessary members to obtain the IID.
 *
 * MSVC supports this method.
 *
 * However, this method is not standard conforming, since a template **shall not**
 * be specialized in a different namespace.
 * Thus, GCC does not support this method.
 *
 * ## Method 2
 * The class type is bound to the argument of a function template.
 * The traits class is bound to its return type.
 * The return type is deduced by `decltype` at compile time.
 *
 * This method is to exploit the argument dependent lookup (ADL) mechanism
 * that resolves function overloading, which makes it possible to call functions
 * define in other namespaces.
 *
 * For example,
 * @code{.cpp}
 * class NsfxUidTraitsXxx
 * {
 * public:
 *     static const bool value = true;
 *     static ::nsfx::Uid GetUid(void) BOOST_NOEXCEPT
 *     {
 *         return ::nsfx::Uid(UID);
 *     }
 * };
 *
 * // Just declare the function without defining it.
 * // Use ::nsfx::type_identity<> to prevent argument deduction to
 * // accedentially bring in unwanted related classes (inheritance, et al).
 * NsfxUidTraitsXxx NsfxGetUidTraits(::nsfx::type_identity<Xxx>);
 *
 * // There can be several overloads.
 * NsfxUidTraitsYyy NsfxGetUidTraits(::nsfx::type_identity<Yyy>);
 *
 * // Obtain the traits class, and the correct overload will be selected.
 * typedef decltype(NsfxGetUidTraits<::nsfx::type_identity<Xxx>()) TraitsType;
 * @endcode
 *
 * If an overloaded function `NsfxGetUidTraits()` is not defined for
 * the user-defined class, the fallback function template will be found.
 * The fallback function template uses the fallback traits class
 * `NsfxUidTraitsFallback` as its return type.
 * The library uses the fallback class to detect such event, and provides
 * explicit information for users: `"UID is undefined!"`.
 *
 * ## GCC implementation
 * The library provides `NSFX_DEFINE_CLASS_UID(T, UID)` macro.
 * The first argument `T` is the user-defined class.
 * It is used to form a local traits class.
 * The local traits class is not formed by macro concatenation, since macro
 * concatenation is quite limited.
 *
 * First, `T` cannot be a class template specialization.
 * e.g., `MyTemplate<C>` would become `NsfxUidTraitsMyTemplate<C>`, which refers to
 * an undefined class template.
 *
 * Second, `T` cannot be a class declaration.
 * e.g., `class MyClass` would become `NsfxUidTraitsclass MyClass`, which is illegal
 * syntactically.
 *
 * To workaround, a traits class template `NsfxUidTraits` is declared and
 * specialized locally.
 *
 * For example,
 * @code{.cpp}
 * template<class > class NsfxUidTraits;
 * template<> class NsfxUidTraits<X> { ... };
 *
 * namespace nsfx
 * {
 *     template<class > class NsfxUidTraits;
 *     template<> class NsfxUidTraits<Y> { ... };
 * }
 * @endcode
 *
 * This way, `X` and `Y` can be either class template specializations or class
 * declarations.
 * That is, `NsfxUidTraits<MyTemplate<>>` and `NsfxUidTraits<class MyClass>` are
 * both syntactically legal.
 *
 * Since GCC does not permit a class template to be specialized in a different
 * namespace, even if the class template `::NsfxUidTraits` is visible in
 * the namespace `nsfx`, the specialization `NsfxUidTraits<Y>` is clearly
 * a specialization of the local class template `::nsfx::NsfxUidTraits`.
 *
 * Please note that, this method does not work in MSVC, since `NsfxUidTraits<Y>`
 * is a specialization of an ambiguous class template, which can be
 * `::NsfxUidTraits` or `::nsfx::NsfxUidTraits`.
 *
 * Please note that, the method of utilizing overloaded functions also works
 * under MSVC 2017 and above versions.
 *
 * However, MSVC 2010~2015 does not support forward class declaration.
 * MSVC 2010~2015 will find the fallback version of the overloaded function
 * in the following example.
 * @code{.cpp}
 * template<> class ::NsfxUidTraits<class A> { ... };
 * ::NsfxUidTraits<A> NsfxGetUidTraits(::nsfx::type_identity<A>);
 *
 * class A; // Declared after the overloaded function NsfxGetUidTraits().
 * @endcode
 *
 * MSVC 2010~2015 will select the correct version of the overloaded function
 * in the following example.
 * @code{.cpp}
 * class A; // Declared before the overloaded function NsfxGetUidTraits().
 *
 * template<> class ::NsfxUidTraits<class A> { ... };
 * ::NsfxUidTraits<A> NsfxGetUidTraits(::nsfx::type_identity<A>);
 * @endcode
 *
 * ## MSVC implementation
 * For MSVC, `NsfxUidTraits` is defined as a global class template.
 * It is specialized in local namespaces.
 *
 * Instead of declaring overloaded functions and use `decltype` to obtain
 * the return type, the UID is obtained directly via the specialization of
 * the global class template.
 *
 * ## Features
 * * Support class template specialization.
 * @code{.cpp}
 * NSFX_DEFINE_CLASS_UID(MyTemplate<C>, "MyTemplate");
 * @endcode
 *
 * * Support forward class declaration.
 * @code{.cpp}
 * NSFX_DEFINE_CLASS_UID(class MyClass, "MyClass");
 * @endcode
 *
 */
template<class T>
class UidTraits
{
#if defined(NSFX_MSVC)
    typedef ::NsfxUidTraits<T>  TraitsType;
#else // !defined(NSFX_MSVC)
    typedef decltype(NsfxGetUidTraits(typename ::nsfx::type_identity<T>()))
            TraitsType;
#endif // defined(NSFX_MSVC)

    // If the UID is not defined for the class, give an explicit error message.
    static_assert(TraitsType::value, "UID is not defined!");

public:
    static Uid GetUid(void) BOOST_NOEXCEPT
    {
        return TraitsType::GetUid();
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 *
 * @brief Get the UID of an interface.
 *
 * @tparam T A class that has a UID.
 *
 * @remarks `std::add_pointer` is used here to prevent automatical template
 *          argument deduction.
 *          Users must explicitly specify the type template argument `T`
 *          in order to call it.
 *
 * @see `NSFX_DEFINE_CLASS_UID`.
 */
template<class T>
inline Uid uid_of(typename std::add_pointer<T>::type = nullptr)
{
    return ::nsfx::UidTraits<T>::GetUid();
}


NSFX_CLOSE_NAMESPACE


#endif // UID_H__FA0E4336_645B_46BF_925D_4CB0FC2EAE40

