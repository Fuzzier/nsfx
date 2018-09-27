/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2018-09-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef ROTL_H__A35B0738_BDBC_4905_A503_32CBCE5B3D26
#define ROTL_H__A35B0738_BDBC_4905_A503_32CBCE5B3D26


#include <nsfx/random/config.h>


NSFX_OPEN_NAMESPACE


namespace random {
namespace aux {

template<class UIntType>
inline UIntType rotl(const UIntType n, size_t k)
{
    return (n << k) | (n >> (sizeof (UIntType) * 8 - k));
}

template<class UIntType>
inline UIntType rotr(const UIntType n, size_t k)
{
    return (n >> k) | (n << (sizeof (UIntType) * 8 - k));
}

} // namespace aux
} // namespace random


NSFX_CLOSE_NAMESPACE


#endif // ROTL_H__A35B0738_BDBC_4905_A503_32CBCE5B3D26

