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
#include <boost/mpl/identity.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Import uuid from BOOST. It is a POD (Plain Old Data).
using boost::uuids::uuid;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define a uuid by 11 data parts.
 *
 * @param l  The first 8 hexadecimal digits of the uuid.
 * @param w1 The first group of 4 hexadecimal digits of the uuid.
 * @param w2 The second group of 4 hexadecimal digits of the uuid.
 * @param b1 The third group of 4 hexadecimal digits of the uuid, and
 *           the final 12 hexadecimal digits of the uuid.
 *
 * e.g., \c NSFX_DEFINE_UUID(id, 0x01234567, 0x89ab, 0xcdef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef)
 * defines a uuid "{01234567-89ab-cdef-0123-456789abcdef}".
 */
#define NSFX_DEFINE_UUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)  \
    const ::nsfx::uuid name = {                                            \
        (l >> 24) & 0xff, (l >> 16) & 0xff, (l >> 8) & 0xff, l & 0xff,     \
        (w1 >> 8) & 0xff, (w1     ) & 0xff,                                \
        (w2 >> 8) & 0xff, (w2     ) & 0xff,                                \
        b1, b2, b3, b4, b5, b6, b7, b8                                     \
    }


/**
 * @ingroup Component
 * @brief Define a uuid by 4 data parts.
 *
 * @param l  The first 8 hexadecimal digits of the uuid.
 * @param w1 The first group of 4 hexadecimal digits of the uuid.
 * @param w2 The second group of 4 hexadecimal digits of the uuid.
 * @param ll The third group of 4 hexadecimal digits of the uuid, and
 *           the final 12 hexadecimal digits of the uuid.<br/>
 *           It is a long long value that <b>should</b> be suffixed with
 *           "LL" or "ULL".
 *
 * e.g., \c NSFX_DEFINE_UUID4(id, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL)
 * defines a uuid "{01234567-89ab-cdef-0123-456789abcdef}".
 */
#define NSFX_DEFINE_UUID4(name, l, w1, w2, ll)              \
    NSFX_DEFINE_UUID(name, l, w1, w2,                       \
                     (ll >> 56) & 0xff, (ll >> 48) & 0xff,  \
                     (ll >> 40) & 0xff, (ll >> 32) & 0xff,  \
                     (ll >> 24) & 0xff, (ll >> 16) & 0xff,  \
                     (ll >>  8) & 0xff, (ll      ) & 0xff )


/**
 * @ingroup Component
 * @brief Declare a const uuid with external linkage.
 */
#define NSFX_DECLARE_UUID(name)  extern const ::nsfx::uuid name


////////////////////////////////////////////////////////////////////////////////
template<class T>
struct uuid_guard {};

////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define a uuid and associate it with a class in a non-intrusive way.
 *
 * For example:
 * @code
 * class Class {};
 * NSFX_DEFINE_IID4(Class, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
 * // Template-based query.
 * uuid id0 = uuid_of<Class>();
 * // Pointer-based query.
 * Class* p = nullptr;
 * id = uuid_of(p);
 * @endcode
 *
 * To enable macro-based query, define a macro like this:
 * @code
 * #define IID_Class  uuid_of<Class>()
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
 *          This macro defines a free function
 *          <code>const uuid& uuid_value() BOOST_NOEXCEPT</code> in the
 *          current namespace.<br/>
 *          The free function \c uuid_of() calls \c uuid_value().<br/>
 *          Even if \c uuid_of() is defined in the namespace of \c nsfx,
 *          it depends upon <i>argument-dependent lookup</i> to call the
 *          correct overload of \c uuid_value().
 *          <p>
 *          \c uuid_guard is placed here as a guard to prevent the compiler from
 *          automatically converting a pointer from a child type to its parent
 *          type.
 *
 * @see \c NSFX_DEFINE_UUID4, \c uuid_of().
 */
#define NSFX_DEFINE_CLASS_UUID4(type, l, w1, w2, ll)       \
    inline const ::nsfx::uuid& uuid_value(                 \
        type* p, ::nsfx::uuid_guard<type>) BOOST_NOEXCEPT  \
    {                                                      \
        static NSFX_DEFINE_UUID4(id, l, w1, w2, ll);       \
        return id;                                         \
    }


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
 * @see \c NSFX_DEFINE_CLASS_UUID4.
 */
template<class T>
const uuid& uuid_of(typename boost::mpl::identity<T>::type* p = nullptr) BOOST_NOEXCEPT
{
    // The uuid_value() is unqualified, it depends upon argument-dependent
    // lookup to call the correct overload.
    // A dummy<T> is placed here to prevent p from being converted to its
    // parent type.
    return uuid_value(p, uuid_guard<T>());
}


NSFX_CLOSE_NAMESPACE


#endif // UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0

