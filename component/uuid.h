/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-07
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0
#define UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0


#include <nsfx/component/config.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/type_traits/type_identity.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Import uuid from BOOST. It is a POD (Plain Old Data).
using boost::uuids::uuid;


////////////////////////////////////////////////////////////////////////////////
template<class T>
struct type_guard {};

/**
 * @ingroup Component
 * @brief Get the uuid of a class.
 *
 * @tparam T A class that has a uuid.
 *
 * @param  p A pointer to an object of the class.
 *
 * @remarks \c boost::mpl::identity is used here to prevent automatical template
 *          argument deduction.<br/>
 *          Users have to explicitly specialize this function template in order
 *          to use it.
 *
 * @see \c NSFX_DEFINE_CLASS_UUID.
 */
template<class T>
const uuid& uuid_of(typename boost::type_identity<T>::type* = nullptr) BOOST_NOEXCEPT
{
    // The uuid_value() is unqualified, it depends upon argument-dependent
    // lookup to call the correct overload.
    // A dummy<T> is placed here to prevent p from being converted to its
    // parent type.
    return uuid_value(type_guard<T>());
}


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Macros.
/**
 * @ingroup Component
 * @brief Define a uuid and associate it with a class in a non-intrusive way.
 *
 * For example:
 * @code
 * class Class {};
 * NSFX_DEFINE_CLASS_UUID(Class, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
 * // Template-based query.
 * uuid id0 = uuid_of<Class>();
 * // Pointer-based query.
 * Class* p = nullptr;
 * id = uuid_of(p);
 * @endcode
 *
 * @remarks This macro should <b>not</b> be used within class scope.
 *          <p>
 *          An intrusive way is always troublesome, as it involves defining
 *          some public members in the class.<br/>
 *          If a user forgets to define the public member in a derived class,
 *          the derived class derives the public member from its parent, and
 *          exposes wrong information.
 *          <p>
 *          This macro defines a free function \c uuid_value() in the current
 *          namespace.<br/>
 *          The free function \c uuid_of() calls \c uuid_value().<br/>
 *          Even if \c uuid_of() is defined in the namespace of \c nsfx,
 *          it depends upon <i>argument-dependent lookup</i> to call the
 *          correct overload of \c uuid_value().
 *          <p>
 *          \c type_guard is placed here as a guard to prevent the compiler from
 *          automatically converting a pointer from a child type to its parent
 *          type.
 *
 * @see \c uuid_of()<br/>
 *      \c NSFX_UUID_OF()<br/>
 */
#define NSFX_DEFINE_CLASS_UUID(type, l, w1, w2, ll)   \
    inline const ::nsfx::uuid& uuid_value(            \
        ::nsfx::type_guard<type>) BOOST_NOEXCEPT      \
    {                                                 \
        static ::nsfx::uuid id = {                    \
            (l  >> 24) & 0xff, (l  >> 16) & 0xff,     \
            (l  >>  8) & 0xff, (l       ) & 0xff,     \
            (w1 >>  8) & 0xff, (w1      ) & 0xff,     \
            (w2 >>  8) & 0xff, (w2      ) & 0xff,     \
            (ll >> 56) & 0xff, (ll >> 48) & 0xff,     \
            (ll >> 40) & 0xff, (ll >> 32) & 0xff,     \
            (ll >> 24) & 0xff, (ll >> 16) & 0xff,     \
            (ll >>  8) & 0xff, (ll      ) & 0xff };   \
        return id;                                    \
    }


/**
 * @ingroup Component
 * @brief A short cut of <code>::nsfx::uuid_of<T>()</code>.
 */
#define NSFX_UUID_OF(T)  ::nsfx::uuid_of<T>()


/**
 * @ingroup Component
 * @brief Declare a external constant uuid in a header file.
 *
 * @example For example
 *   @code
 *   // Header file: myclass.h
 *   NSFX_DECLARE_UUID(CID_MyClass);
 *
 *   // Source file: myclass.cpp
 *   class MyClass;
 *   NSFX_DEFINE_CLASS_UUID(MyClass, 0xBB5756DB, 0x42C2, 0x40D8, 0x99A2D53D170F8F22LL);
 *   NSFX_DEFINE_UUID(CID_MyClass, MyClass);
 *   @endcode
 *
 * @see \c NSFX_DEFINE_UUID()
 */
#define NSFX_DECLARE_UUID(name)  extern const ::nsfx::uuid name


/**
 * @ingroup Component
 * @brief Define a constant uuid to be the uuid of type T in a source file.
 *
 * @see \c NSFX_DECLARE_UUID()
 */
#define NSFX_DEFINE_UUID(name, T)  NSFX_UUID_OF(T)


#endif // UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0

