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
#if !defined(NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER)
/**
 * @ingroup Network
 * @brief The buffer of a packet.
 *
 * @remarks By default, this is \c Buffer.
 *          <p>
 *          If \c NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER is defined, this is
 *          \c Buffer.
 */
typedef Buffer  PacketBuffer;

/**
 * @ingroup Network
 * @brief The read-only buffer of a packet.
 *
 * @remarks By default, this is \c ConstBuffer.
 *          <p>
 *          If \c NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER is defined, this is
 *          \c ConstBuffer.
 */
typedef ConstBuffer  ConstPacketBuffer;

/**
 * @ingroup Network
 * @brief The iterator of a packet buffer.
 *
 * @remarks By default, this is \c BufferIterator.
 *          <p>
 *          If \c NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER is defined, this is
 *          \c BufferIterator.
 */
typedef BufferIterator  PacketBufferIterator;

/**
 * @ingroup Network
 * @brief The read-only iterator of a packet buffer.
 *
 * @remarks By default, this is \c ConstBufferIterator.
 *          <p>
 *          If \c NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER is defined, this is
 *          \c ConstBufferIterator.
 */
typedef ConstBufferIterator  ConstPacketBufferIterator;

#else // defined(NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER)
typedef ZcBuffer                 PacketBuffer;
typedef ConstZcBuffer            ConstPacketBuffer;
typedef ZcBufferIterator         PacketBufferIterator;
typedef ConstZcBufferIterator    ConstPacketBufferIterator;

#endif // !defined(NSFX_PACKET_USES_ZERO_COMPRESSED_BUFFER)


NSFX_CLOSE_NAMESPACE


#endif // PACKET_BUFFER_H__E83E7082_098C_4A68_8FE3_7B8EE530AEB7

