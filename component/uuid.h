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
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


NSFX_OPEN_NAMESPACE


class Uuid
{
public:
    BOOST_CONSTEXPR Uuid(void) BOOST_NOEXCEPT
    {
        id_.data[0]  = 0;
        id_.data[1]  = 0;
        id_.data[2]  = 0;
        id_.data[3]  = 0;

        id_.data[4]  = 0;
        id_.data[5]  = 0;

        id_.data[6]  = 0;
        id_.data[7]  = 0;

        id_.data[8]  = 0;
        id_.data[9]  = 0;
        id_.data[10] = 0;
        id_.data[11] = 0;
        id_.data[12] = 0;
        id_.data[13] = 0;
        id_.data[14] = 0;
        id_.data[15] = 0;
    }

    /**
     * @brief Creates a uuid.
     *
     * @param d1 The first 8 hexadecimal digits. Big-endian.
     * @param d2 The first group of 4 hexadecimal digits. Big-endian.
     * @param d3 The second group of 4 hexadecimal digits. Big-endian.
     * @param d4 The third group of 4 hexadecimal digits and the final 12 hexadecimal digits. Big-endian.
     */
    BOOST_CONSTEXPR Uuid(uint32_t d1, uint16_t d2, uint16_t d3, uint64_t d4) BOOST_NOEXCEPT
    {
        id_.data[0]  = (d1 >> 24) & 0xff;
        id_.data[1]  = (d1 >> 16) & 0xff;
        id_.data[2]  = (d1 >>  8) & 0xff;
        id_.data[3]  = (d1      ) & 0xff;

        id_.data[4]  = (d2 >>  8) & 0xff;
        id_.data[5]  = (d2      ) & 0xff;

        id_.data[6]  = (d3 >>  8) & 0xff;
        id_.data[7]  = (d3      ) & 0xff;

        id_.data[8]  = (d4 >> 56) & 0xff;
        id_.data[9]  = (d4 >> 48) & 0xff;
        id_.data[10] = (d4 >> 40) & 0xff;
        id_.data[11] = (d4 >> 32) & 0xff;
        id_.data[12] = (d4 >> 24) & 0xff;
        id_.data[13] = (d4 >> 16) & 0xff;
        id_.data[14] = (d4 >>  8) & 0xff;
        id_.data[15] = (d4      ) & 0xff;
    }

    /**
     * @brief Creates a uuid.
     *
     * @param s A string looks like "01234567-89ab-cdef-0123-456789abcdef" or
     *                             "{01234567-89ab-cdef-0123-456789abcdef}".
     */
    explicit Uuid(const char* s)
    {
        boost::uuids::string_generator gen;
        id_ = gen(s);
    }

public:
    friend size_t hash_value(const Uuid& id) BOOST_NOEXCEPT;
    friend void swap(Uuid& lhs, Uuid& rhs) BOOST_NOEXCEPT;

public:
    bool IsNil(void) const BOOST_NOEXCEPT
    {
        return id_.is_nil();
    }

    bool operator== (const Uuid& rhs) const BOOST_NOEXCEPT
    {
        return (id_ == rhs.id_);
    }

    bool operator!= (const Uuid& rhs) const BOOST_NOEXCEPT
    {
        return (id_ != rhs.id_);
    }

public:
    std::string ToString(void) const
    {
        return boost::uuids::to_string(id_);
    }

private:
    boost::uuids::uuid id_;
};


////////////////////////////////////////////////////////////////////////////////
// Algorithms./*{{{*/
inline size_t
hash_value(const Uuid& id) BOOST_NOEXCEPT
{
    return boost::uuids::hash_value(id.id_);
}

inline void
swap(Uuid& lhs, Uuid& rhs) BOOST_NOEXCEPT
{
    return boost::uuids::swap(lhs.id_, rhs.id_);
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// I/O./*{{{*/
template<class CharT, class TraitsT>
inline std::basic_ostream<CharT, TraitsT>&
operator<<(std::basic_ostream<CharT, TraitsT>& os, const Uuid& rhs)
{
    return (os << rhs.ToString());
}
/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // UUID_H__E8DDE5BC_4E16_48D3_BD4E_352C7CAC8BA0

