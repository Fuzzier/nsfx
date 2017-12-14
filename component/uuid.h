/**
 * @file
 *
 * @brief Component support for Network Simulation Framework.
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
    const uuid name = {                                                    \
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
#define NSFX_DECLARE_UUID(name)  extern const uuid name


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define a uuid and associate it with a class.
 *
 * For example:
 * @code
 * class Interface
 * {
 *   NSFX_DEFINE_CLASS_UUID4(0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
 * };
 * uuid id = uuid_of<Interface>();
 * Interface* p = nullptr;
 * id = uuid_of(p);
 * @endcode
 *
 * @remarks This macro <b>must</b> be used within class scope.<br/>
 *          It defines a static member function
 *          <code>const uuid& GetUuid_(void) BOOST_NOEXCEPT</code>.<br/>
 *          This way avoids declaring a const uuid in a header file, and define
 *          it in a source file.<br/>
 *          The static member function is suffixed with an underscore, so it is
 *          <i>not</i> intended to be called by users.<br/>
 *          Users should call the \c uuid_of() function to obtain the uuid of
 *          an interface.
 *          <p>
 *          For convenience, this macro changes the access right to \c public.
 *
 * @see \c NSFX_DEFINE_UUID, \c NSFX_DEFINE_UUID4.
 */
#define NSFX_DEFINE_CLASS_UUID4(l, w1, w2, ll)        \
    public:                                           \
    static const uuid& GetUuid_(void) BOOST_NOEXCEPT  \
    {                                                 \
        static NSFX_DEFINE_UUID4(id, l, w1, w2, ll);  \
        return id;                                    \
    }


/**
 * @ingroup Component
 * @brief Get the uuid of a class.
 *
 * @tparam T A class that has a uuid.
 *
 * @param  p A pointer to an object of the class.
 *
 * @see \c NSFX_DEFINE_CLASS_UUID4.
 */
template<class T>
const uuid& uuid_of(T* p = nullptr) BOOST_NOEXCEPT
{
    return T::GetUuid_();
}


NSFX_CLOSE_NAMESPACE


#endif // UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0

