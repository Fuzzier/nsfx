/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef BASIC_BUFFER_H__84E718F8_061D_447C_913C_AC56EA3B1637
#define BASIC_BUFFER_H__84E718F8_061D_447C_913C_AC56EA3B1637


#include <nsfx/network/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A buffer.
 *
 * A buffer has an underlying storage, which may be shared with other buffers.
 * The shared ownership aims to avoid memory reallocation and copying for users
 * that only read the buffer without modifying it.
 *
 * @tparam readOnly     The buffer provides read-only operations.
 * @tparam copyOnResize The buffer supports copy-on-resize.
 * @tparam zeroArea     The buffer has a compressed zero area.
 *                      The compressed zero area is usually used to represent
 *                      the payload of the top protocol layer.
 */
template<bool readOnly, bool copyOnResize, bool zeroArea>
class BasicBuffer;


NSFX_CLOSE_NAMESPACE


#endif // BASIC_BUFFER_H__84E718F8_061D_447C_913C_AC56EA3B1637

