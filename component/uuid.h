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
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


NSFX_OPEN_NAMESPACE


using boost::uuids::uuid;


/**
 * @brief Define a uuid by 11 data parts.
 *
 * e.g., \c NSFX_DEFINE_UUID(id, 0x01234567, 0x89ab, 0xcdef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef)
 * defines a uuid "{01234567-89ab-cdef-0123-456789abcdef}".
 */
#define NSFX_DEFINE_UUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)  \
    const uuid name = {  \
        (l >> 24) & 0xff, (l >> 16) & 0xff, (l >> 8) & 0xff, l & 0xff,  \
        (w1 >> 8) & 0xff, (w1     ) & 0xff,  \
        (w2 >> 8) & 0xff, (w2     ) & 0xff,  \
        b1, b2, b3, b4, b5, b6, b7, b8  \
    }


/**
 * @brief Define a uuid by 4 data parts.
 *
 * @param ll A long long that <b>should</b> have a suffix of "LL" or "ULL".
 *
 * e.g., \c NSFX_DEFINE_UUID4(id, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL)
 * defines a uuid "{01234567-89ab-cdef-0123-456789abcdef}".
 */
#define NSFX_DEFINE_UUID4(name, l, w1, w2, ll)  \
    NSFX_DEFINE_UUID(name, l, w1, w2,  \
                     (ll >> 56) & 0xff, (ll >> 48) & 0xff,  \
                     (ll >> 40) & 0xff, (ll >> 32) & 0xff,  \
                     (ll >> 24) & 0xff, (ll >> 16) & 0xff,  \
                     (ll >>  8) & 0xff, (ll      ) & 0xff )


NSFX_CLOSE_NAMESPACE


#endif // UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0

