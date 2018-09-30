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
#include <boost/functional/hash.hpp>
#include <type_traits> // add_pointer
#include <cstring> // strlen, strcmp


NSFX_OPEN_NAMESPACE


class Uid
{
public:
    Uid(const char* uid) :
        uid_(uid)
    {}

    operator const char*() const BOOST_NOEXCEPT
    {
        return uid_;
    }

    friend bool operator==(const Uid& lhs, const Uid& rhs);
    friend bool operator!=(const Uid& lhs, const Uid& rhs);

    friend bool operator==(const Uid& lhs, const char* rhs);
    friend bool operator!=(const Uid& lhs, const char* rhs);

    friend bool operator==(const char* lhs, const Uid& rhs);
    friend bool operator!=(const char* lhs, const Uid& rhs);

    template<class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
           operator<<(std::basic_ostream<Char, Traits>& os, const Uid& uid);

    friend size_t hash_value(const Uid& uid);

private:
    const char* uid_;
};


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
    size_t length = strlen(uid.uid_);
    return boost::hash_range<const char*>(uid.uid_, uid.uid_ + length);
}


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief The primary template for class traits.
 *
 * The class traits includes a UID (universal identifier).
 *
 * A UID is a string that identifies a class in a hierarchical namespace.
 * The general form is <code>"<organization>.<module>.<class>"</code>.</br>
 * e.g., <code>"edu.uestc.nsfx.IObject"</code>.
 *
 * The primary template is left without a definition.
 * Thus, if the template is used without been specialized for user-defined class,
 * the compiler reports errors.
 *
 * The class template is defined in global namespace, since it is not legal
 * to specialize a class template in a parallel namespace (a namespace that
 * is neither \c nsfx, nor enclosed in \c nsfx).
 *
 * The specialized class template must provide a static function that returns
 * the UID of the class.
 *
 * For example,
 * @code
 * template<>
 * class NsfxClassTraits<MyClass>
 * {
 * public:
 *     static const char* GetClassName(void)
 *     {
 *         return "edu.uestc.nsfx.MyClass";
 *     }
 * };
 * @endcode
 */
template<class T>
class NsfxClassTraits;


////////////////////////////////////////////////////////////////////////////////
// Macros.
/**
 * @ingroup Component
 *
 * @brief Associate a UID with an class in a non-intrusive way.
 */
#define NSFX_DEFINE_CLASS_UID(type, uid)        \
    template<>                                  \
    class NsfxClassTraits<type>                 \
    {                                           \
    public:                                     \
        static const ::nsfx::Uid& GetUid(void)  \
        {                                       \
            static const ::nsfx::Uid  id(uid);  \
            return id;                          \
        }                                       \
    }


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 *
 * @brief Get the UID of a class.
 *
 * @tparam T A class that has a UID.
 *
 * @remarks \c std::add_pointer is used here to prevent automatical template
 *          argument deduction.
 *          Users have to explicitly specialize this function template in order
 *          to use it.
 *
 * @see \c NSFX_DEFINE_CLASS_UID.
 */
template<class T>
inline const Uid& uid_of(typename std::add_pointer<T>::type = nullptr)
{
    return ::NsfxClassTraits<T>::GetUid();
}


NSFX_CLOSE_NAMESPACE


#endif // UID_H__FA0E4336_645B_46BF_925D_4CB0FC2EAE40

