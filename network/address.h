/**
 * @file
 *
 * @brief Fixed length address.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ADDRESS_H__108114F0_8604_4291_A558_A29839946A8E
#define ADDRESS_H__108114F0_8604_4291_A558_A29839946A8E


#include <nsfx/network/config.h>


////////////////////////////////////////////////////////////////////////////////
#if BOOST_ENDIAN_BIG_BYTE
# error Unsupport endianness.
# include <nsfx/network/address/address-big-endian.h>
#elif BOOST_ENDIAN_LITTLE_BYTE
# include <nsfx/network/address/address-little-endian.h>
#endif // BOOST_ENDIAN_*_BYTE



NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
/**
 * @ingroup Network
 * @brief An address with a fixed bit length.
 *
 * --------------------------
 * Types of network addresses
 * --------------------------
 *
 * Network addresses are used to identify protocol entities.
 * There are generally two kinds of network addresses:
 * * The address of a peer entity.
 * * The address of a client entity (usually an upper-layer entity).
 *
 * Essientially, a network address is an unsigned integer value with
 * a pre-defined length.
 * Thus, naturally, network addresses can be copeied, compared, hashed,
 * as well as take part in certain arithmetic operations.
 *
 * In network simulation, the format of address usually depends upon the
 * requirements.
 * For example, the number of protocol entities in the network may determine
 * the size of the address of the protocol.
 *
 * --------------------
 * Problems of layering
 * --------------------
 *
 * When a protocol entity at layer bits+1 wants to deliver a packet to a peer
 * entity, it relies upon the communication service provided by protocol
 * entities at layer bits.
 *
 * For example, the protocol entity 'b1' at layer bits+1 wants to deliver a packet
 * to its peer entity 'b2'.
 * Actually, 'b1' requests the protocol entity 'a1' at layer bits to deliver
 * the packet to the peer entity 'a2', where 'b2' resides at its higher layer.
 * If 'a2' serves several protocol entities at its higher layer, 'b1' also has
 * to specify the service access point (SAP) of 'b2'.
 * @code
 *            ----------                       ----------
 * layer bits+1  |   b1   |                       |   b2   |
 *            ----------                       ----------
 *                | send to a2                    ^ forward to b2
 *                v (a2's address & b2's SAP)     | (via b2's SAP)
 *            ----------                       ----------
 *  layer bits   |   a1   | - - - - - - - - - - > |   a2   |
 *            ----------                       ----------
 * @endcode
 * Thus, 'b1' at layer bits+1 need to know the address of 'a2' at layer bits, and
 * the SAP of 'b2' allocated by 'a2'.
 * If the protocols uses different kinds of address schemes, then there must
 * have address mapping/conversion to bridge the protocols.
 *
 * However, when 'b1' is designed, one cannot always foretell what protocols
 * will be at its lower layer to serve it.
 * Therefore, to fight this kind of changeability, 'b1' <b>shall not</b> make
 * fixed assumptions about the address schemes and SAP allocations of the
 * lower-layer protocols.
 *
 * ----------------
 * Adaptation layer
 * ----------------
 *
 * When 'b1' wants to send a packet to 'b2', it shall use the address of 'b2',
 * instead of the address of 'a2'.
 * 'b1' shall not specify any SAP, either.
 * The address conversion and SAP mapping <b>shall</b> be done at a dictated
 * <i>adaptation layer</i> in the middle.
 * @code
 *            ----------                   ----------
 * layer bits+1  |   b1   |                   |   b2   |
 *            ----------                   ----------
 *                | send to b2                 ^ reassembly, etc.
 *                v b2's address               | forward to b2
 *   A<->B    ----------                   ----------
 * adaptation |  B->A  |                   |  A->B  |
 *   layer    ----------                   ----------
 *                | address converion,         ^
 *                | SAP mapping,               |
 *                | fragmentation, etc.        |
 *                v send to a2                 | forward to b2 via SAP
 *            ----------                   ----------
 *  layer bits   |   a1   | - - - - - - - - > |   a2   |
 *            ----------                   ----------
 * @endcode
 * An adaptation layer is usually dictated for bridging a specific pair of
 * protocols.
 * That is, the adaptation layer for bridging protocols A and B usually cannot
 * be used to bridge another pair of protocols.
 *
 * Adaptation layers are usually provided by a protocol to serve different
 * upper-layer protocols.
 * The protocol allocates a unique SAP for each upper-layer protocol it is
 * intended to serve.
 * If the address schemes are different, then the adaptation layer also has to
 * convert the addresses.
 *
 * To avoid address conversion and simplify the task of protocol bridging,
 * a straightforward way is to make the protocols use the <i>same</i> address
 * scheme for peer entities.
 *
 * ---------------
 * Protocol design
 * ---------------
 *
 * To be able to adapt to different network scale, and different address schemes
 * of upper/lower protocols, the address scheme of an experimental protocol
 * shall be made changeable at design-time, and fixed at compile-time.
 *
 * For performance considerations, it is not recommended to use a large-enough
 * address to abstract all possible kinds of addresses.
 *
 * A protocol <b>shall</b> use a fixed-length address to identify peer entities,
 * and a fixed-length address for SAP.
 * A protocol <b>shall</b> use \c typedef to declare the address types, and use
 * operations that are common to all kinds of addresses.
 *
 * The type of SAP shall be declared as simple integer types, such as
 * \c uint8_t and \c uint16_t.
 * However, the type of peer-entity address can be as larger as \c uint128_t.
 *
 * ----------------------
 * Address class template
 * ----------------------
 *
 * A network address is usually no larger than \c 128 bits.
 *
 * Multiple-precision integers are efficient for bit lengths that are
 * no smaller than \c 128.
 * However, they are usually inefficient for smaller bit lengths.
 *
 * The NSFX provides a high-performance class template to model addresses
 * with a fixed bit length.
 * Native \c uint32_t and \c uint64_t integers are used directly, unless
 * the bit length exceeds \c 64 bits.
 *
 * For efficiency, when the address is no larger than \c 32 bits,
 * a single \c uint32_t is used to implement an address.
 *
 * When the address is no larger than \c 64 bits,
 * a single \c uint64_t is used to implement an address.
 *
 * When the address is larger than \c 64 bits,
 * an array of \c uint32_t is used to implement an address.
 *
 * The layout of the memory is a continous block of bytes that are ordered
 * in the same endianness as the system, which can be directly accessed.
 *
 * To ease the initialization an address, several constructor overloads are
 * provided.
 * An address can be constructed via a byte array, a byte buffer, a value array, 
 * a value buffer or a value pack, with a user-specified endian order.
 * A single value can also be used.
 * Several operators are provided that are meaningful to addresses.
 */
template<size_t bits>
class Address;


NSFX_CLOSE_NAMESPACE


#endif // ADDRESS_H__108114F0_8604_4291_A558_A29839946A8E

