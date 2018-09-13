/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-04
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PACKET_BUFFER_H__E83E7082_098C_4A68_8FE3_7B8EE530AEB7
#define PACKET_BUFFER_H__E83E7082_098C_4A68_8FE3_7B8EE530AEB7


#include <nsfx/network/config.h>
#include <nsfx/network/buffer.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
#if !defined(NSFX_PACKET_USES_SOLID_BUFFER)
/**
 * @ingroup Network
 * @brief The buffer of a packet.
 *
 * @remarks By default, this is \c ZcBuffer.
 *          <p>
 *          If \c NSFX_PACKET_USES_SOLID_BUFFER is defined, this is \c Buffer.
 */
typedef ZcBuffer  PacketBuffer;

/**
 * @ingroup Network
 * @brief The read-only buffer of a packet.
 *
 * @remarks By default, this is \c ConstZcBuffer.
 *          <p>
 *          If \c NSFX_PACKET_USES_SOLID_BUFFER is defined, this is
 *          \c ConstBuffer.
 */
typedef ConstZcBuffer  ConstPacketBuffer;

/**
 * @ingroup Network
 * @brief The iterator of a packet buffer.
 *
 * @remarks By default, this is \c ZcBufferIterator.
 *          <p>
 *          If \c NSFX_PACKET_USES_SOLID_BUFFER is defined, this is
 *          \c BufferIterator.
 */
typedef ZcBufferIterator  PacketBufferIterator;

/**
 * @ingroup Network
 * @brief The read-only iterator of a packet buffer.
 *
 * @remarks By default, this is \c ConstZcBufferIterator.
 *          <p>
 *          If \c NSFX_PACKET_USES_SOLID_BUFFER is defined, this is
 *          \c ConstBufferIterator.
 */
typedef ConstZcBufferIterator  ConstPacketBufferIterator;

#else // defined(NSFX_PACKET_USES_SOLID_BUFFER)
typedef Buffer                 PacketBuffer;
typedef ConstBuffer            ConstPacketBuffer;
typedef BufferIterator         PacketBufferIterator;
typedef ConstBufferIterator    ConstPacketBufferIterator;

#endif // !defined(NSFX_PACKET_USES_SOLID_BUFFER)


NSFX_CLOSE_NAMESPACE


#endif // PACKET_BUFFER_H__E83E7082_098C_4A68_8FE3_7B8EE530AEB7

