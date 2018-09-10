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

#ifndef PACKET_H__5E4E4E99_1B44_44E3_925B_58B8E93BD581
#define PACKET_H__5E4E4E99_1B44_44E3_925B_58B8E93BD581


#include <nsfx/network/config.h>
#include <nsfx/network/buffer.h>
#include <nsfx/network/packet/tag/basic-tag-list.h>
#include <boost/core/swap.hpp>
#include <utility> // move


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


////////////////////////////////////////////////////////////////////////////////
// Tag.
typedef FixedBuffer       TagBuffer;
typedef ConstFixedBuffer  ConstTagBuffer;
typedef BasicTag<ConstTagBuffer>  Tag;


////////////////////////////////////////////////////////////////////////////////
// Packet.
/**
 * @ingroup Network
 * @brief A packet.
 *
 * # Packet models
 * ## Packet as solid buffer
 *    The simplest method is to model a packet as a solid buffer, and give
 *    the packet a reference count.
 *    When the packet is duplicated, each receiver holds a reference count of
 *    the same packet.
 *    If a receiver wants to modify the packet, it requests a private copy of
 *    the packet, and the entire buffer is duplicated if it is refered to by
 *    two or more receivers.
 *
 *    This process can be expensive, since it does not <i>exploit the natural
 *    structure and processing order of a network packet</i>.
 *    Usually, the application layer payload occupies the most bytes in the
 *    buffer of a packet, and this part of the buffer is rather stable and
 *    seldomly modified by network protocols.
 *    A lower layer entity only modifies the header part of the packet.
 *    However, in order to modify that small part of the packet, the entire
 *    buffer is duplicated.
 *
 * ## Packet as solid buffer with zero-compressed payload
 *    NS3 offers a buffer model that has a header part, a zero-compressed data
 *    part and a trailer part.
 *    It saves memory in pure simulation environments, and is still open to
 *    model solid packets.
 *    It introduces more processing overheads though.
 *    The library adopts this model.
 *
 * ## Packet as sequence of packets
 *    A packet consists of layers of encapsulations.
 *    The basic structure of a packet is shown as follows:
 *    <code>[header] [payload] [trailer]</code>.
 *    Each part is solid, and reference counted.
 *
 *    Since the payload part may consist of several packets, a more general
 *    model would be as follows:
 *    <code>[header] [payload1] [payload2] ... [trailer]</code>.
 *    i.e.,
 *    <code>[subpacket1] [subpacket2] ... </code>
 *
 *    However, it is too hard to recover a simulation packet from a physical
 *    packet due to lack of information about the actual structure (the headers)
 *    of the packet.
 *    Therefore, when dealing with physical packets, the solid buffer model has
 *    to be adopted.
 *
 * ## Packet as virtual fields
 *    OPNET adopts this approach that a packet consists of virtual fields.
 *    It also has the problem to convert a simulation packet from a physical
 *    packet.
 *
 * ## Packet as C++ class
 *    OMNET++ adopts this approach that a packet is modeled as a C++ class.
 *    It is suitable to model packets with fixed format, but is hard to model
 *    packets with variable fields.
 *    It also introduces a complicated message discription language, which
 *    makes the learning curve steeper.
 *
 * # Packet operations
 *   In reality, a network packet is structured.
 *   Each entity adds its own header or trailer to the packet.
 *   The header or trailer of the packet is processed/stripped in a FIFO
 *   (first-in first-out) order.
 *   These observations forms the basic assumptions of the design.
 *
 * ## Duplication
 * ### The problem
 *     In the real world, when a packet is transmitted, the packet is physically
 *     duplicated as the electromagnetic signal is radiated into the space, and
 *     received by multiple receivers.
 *     Each receiver obtains an independent copy of the signal, and transforms
 *     the signal into an independent (private) packet.
 *     The independency means that the modification of the signal or packet does
 *     not affect any other copeis of the signal or the packet.
 *
 *     All existing communication protocols are made to deal with independent
 *     signals and packets.
 *     Therefore, there is no problem in a network simulation if a packet is
 *     physically duplicated for each receiver.
 *
 *     The problem is that physically duplicate a packet can be expensive, since
 *     it can involve a cascade of memory/object allocation and duplication in
 *     the simulation.
 *
 * ### Discussion
 *     The idea is that, if a packet is supposed to be physically duplicated,
 *     the copies of the packet can share the same storage (memory block) as
 *     long as they do not modify the bytes used of other buffers.
 *     The bottom line is physically duplicating the storage, but one must
 *     try to delay/avoid such operation whenever possible.
 *
 *     The key is to identify which bytes are uses by other buffers.
 *
 * #### Shared storage
 *      A buffer is linked to a storage.
 *      When a packet is copied, the buffer of the copy is linked to the same
 *      storage.
 *      Thus, buffers of copied packets share the same storage.
 *      The lifetime of the storage is managed via reference counting, naturally.
 *
 * #### Responsibility of buffer
 *      The responsibility of a buffer is to mark its <i>private area</i>, i.e.,
 *      the area that is supposed to be owned by the buffer alone, and must not
 *      be modified by other entities.
 *
 *      An entity can modify bytes of a buffer <b>if and only if</b> the bytes
 *      are not within the private areas of any other buffers.
 *      For a buffer, the bytes in the private area of other buffers are
 *      considered <i>immutable</i> bytes of the buffer.
 *      When an entity tries to modify the immutable bytes, the storage must be
 *      physically duplicated for the buffer.
 *
 *      When an entity tries to modify the buffer, it must check whether the
 *      bytes are immutable.
 *      However, the buffer shall not be directly exposed to the users, as
 *      relying upon users' good memory to remember to check the immutable bytes
 *      is not a good idea.
 *
 * #### Packet as encapsulation
 *      Packet is an encapsulation of the buffer that enforces the accessing
 *      rules.
 *
 *      An entity can only add (or remove) header (or trailer) to the packet.
 *      When a header or trailer is added to a packet, if the header or trailer
 *      extends contain immutable bytes, the buffer storage is duplicated
 *      (copy-on-write).
 *
 *      When an entity removes a header or trailer from the packet, the private
 *      area of the buffer is reduced.
 *      The bytes in the removed header or trailer are out of the private area
 *      of the buffer, and may be modified by other entities.
 *
 *      Therefore, an entity <b>shall</b> copy out the bytes in the header or
 *      trailer, before it reduces the header or trailer.
 *
 * #### Track private areas
 *      The private area can be simply modeled as a range of continuous bytes.
 *
 *      To simplify management overhead, the storage does not keep track of
 *      every private area of each buffer.
 *      Instead, the storage uses a pessimestic/coarse management strategy that
 *      keeps track of the extent of all private areas, which is called the
 *      <i>dirty area</i> of the storage.
 *
 *      When the storage is owned by a single buffer, the dirty area of the
 *      storage is the same as the buffer.
 *      i.e., whenever an entity adds or removes a header or trailer to the
 *      buffer, the dirty area of the storage is updated according.
 *
 *      When the storage is shared among several buffers, if an entity adds a
 *      header or trailer to the buffer, and the header or trailer does not
 *      overlap with the private areas of any other buffer, the buffer occupies
 *      the header or trailer, and increases the range of the dirty area is to
 *      include the header or trailer.
 *
 *      However, if the added header or trailer overlaps with the private areas
 *      of some other buffers, the storage is duplicated for the buffer, and
 *      the dirty area is updated to the private area as the buffer.
 *
 * ## Fragmentation and reassembly
 *    Since solid buffer is used to model a packet, fragmentation can be done
 *    by creating a buffer with a smaller private area.
 *    And reassembly can be done by adding the buffer from a fragment to the
 *    start or end of a buffer.
 *
 * # Tags
 *   In the simulation, a packet can carry side information that is not present
 *   in its buffer.
 *   i.e., the information is not transmitted in the real world packet.
 *   Such as timestamps.
 *
 *   A simulation packet can hold a set of <i>tags</i> to carry the side
 *   information.
 *
 * ## Tags are implicit
 *    Tags can carry any type of side information.
 *    However, tags are hidden items within packets, and communication via tags
 *    is not encouraged.
 *    The key problem is that one cannot use a C++ interface to explicitly
 *    state what kind of side information is carried by the tags of a packet.
 *    This is one of the key problems in almost every scripting language that
 *    does not have a strong type system.
 *
 *    Relying upon tags to transfer information can be harmful to reusability,
 *    since there is little syntactic support to make a syntactic contract
 *    among components to agree upon the set of tags in a packet.
 *    Such contract is usually written in a separate document by the system
 *    designer, and expects the component writers to obey it.
 *    Burdens are placed upon component writers to be resilient to accept
 *    packets with missing or mistaken tags.
 *
 * ## Usage of tags
 *    The recommended approach to use tag is to adopt the existing rules of
 *    communication in practice.
 *
 *    First, tags <b>shall</b> only be transferred to peer entities across node
 *    boundary, as side information that cannot be transferred by packets in
 *    real systems.
 *
 *    Second, tags <b>must</b> not carry information that <i>impractically</i>
 *    affects the behavior of an entity.
 *    Tags shall be used to help debugging and collecting statistics, etc.
 *
 *    Third, tags <b>shall</b> not be used to coordinate local entities within
 *    a node.
 *    The cooperations among local entities <b>shall</b> be done via
 *    well-defined interfaces.
 *
 *    For example, unlike OPNET, OMNET++ or NS3, the library does not provide
 *    a unique integer id for each packet.
 *    For debugging purpose, tracing a packet via its id is not always useful,
 *    since the id of a packet will change via duplication, fragmentation and
 *    reassembly.
 *    OPNET and OMNET++ even provide a tree id to trace the duplicates of
 *    a packet.
 *    However, the id is useless to trace a packet during fragmention and
 *    reassembly.
 *    The id only tells that two packets are different.
 *    It losses information during duplication, fragmentation and reassembly.
 *    Tags are more suitable to trace the transmission and processing of the
 *    bytes of a packet.
 *
 * ### Placement of tags
 *     A tag can be considered as a virtual header or trailer that carries side
 *     information as a supplement to the physical header or trailer in a
 *     packet.
 *     Thus, a tag is associated with the header or trailer installed by an
 *     entity.
 *     This is similar to the 'byte-tag' in NS3.
 *
 *     When a packet is fragmented, the tag is carried by the fragment if any
 *     of the tagged bytes remain in the fragment.
 *     i.e., if the fragment contains any bytes that have a tag, the fragment
 *     shall also carries that tag.
 *     When the fragments are reassembled, the tags are merged as the tagged
 *     bytes are put together.
 *
 * ## Tags are read-only
 *    To make memory usage efficient, tags are shared among duplicated packets
 *    and packet fragments to avoid physically duplicating the tags.
 *
 *    A tag is considered a virtual header or trailer of a packet.
 *    Similar to physical headers and trailers of a packet, the tags are
 *    assumed to be owned by the packet alone.
 *    i.e., other entities must not modify the tags.
 *
 *    Thus, a tag is inserted once, and is read-only.
 *    A tag is removed automatically when the tagged bytes are removed from
 *    the packet.
 *
 * ## On complex transformation of bytes
 *    When the bytes of the packet is transformed in a complex way, there is
 *    problem.
 *    e.g., when the bytes of the packet are encoded, the original bytes are
 *    mixed with other bytes, and spread across the entire encoded packet,
 *    the range of associated bytes are no longer clear.
 *
 *    The tags of the original packet shall be preserved, until the original
 *    packet is recovered.
 *    Sometimes, it is also convenient to preverse the original packet.
 *
 *    To avoid duplication of the orignal packet or its tags, the simplest way
 *    is to treat the original packet as a tag, which is associated with
 *    the entire bytes of the encoded packet.
 *
 */
class Packet
{
public:
    /**
     * @brief Create an empty packet.
     */
    Packet(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a packet from a buffer.
     */
    explicit Packet(const PacketBuffer& buffer) BOOST_NOEXCEPT;

    // Copyable.
public:
    Packet(const Packet& rhs) BOOST_NOEXCEPT;
    Packet& operator=(const Packet& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    Packet(Packet&& rhs) BOOST_NOEXCEPT;
    Packet& operator=(Packet&& rhs) BOOST_NOEXCEPT;

    // Buffer.
public:
    /**
     * @brief Get the size of the packet in bytes.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the read-only buffer of the packet.
     */
    ConstPacketBuffer GetBuffer(void) const BOOST_NOEXCEPT;

    /**
     * @brief Add a header to the packet.
     *
     * @param[in] size The byte size of the header.
     *
     * @return A writable buffer of the header.
     *         The buffer is valid until the size of the packet is changed.
     */
    PacketBuffer AddHeader(size_t size);

    /**
     * @brief Add a trailer to the packet.
     *
     * @param[in] size The byte size of the trailer.
     *
     * @return A writable buffer of the trailer.
     *         The buffer is valid until the size of the packet is changed.
     */
    PacketBuffer AddTrailer(size_t size);

    /**
     * @brief Add a header to the packet.
     *
     * @param[in] buffer The buffer of the header.
     */
    void AddHeader(const ConstPacketBuffer& buffer);

    /**
     * @brief Add a trailer to the packet.
     *
     * @param[in] buffer The buffer of the trailer.
     */
    void AddTrailer(const ConstPacketBuffer& buffer);

    /**
     * @brief Remove the header from the packet.
     *
     * @param[in] size The byte size of the header.
     */
    void RemoveHeader(size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Remove the trailer from the packet.
     *
     * @param[in] size The byte size of the trailer.
     */
    void RemoveTrailer(size_t size) BOOST_NOEXCEPT;

    // Tag.
public:
    /**
     * @brief Tag a range of bytes.
     *
     * @param[in] tag   The tag.
     * @param[in] start The start of the tagged bytes.
     * @param[in] size  The number of tagged bytes.
     */
    void AddTag(const Tag& tag, size_t start, size_t size);

    /**
     * @brief Tag a range of bytes.
     *
     * @param[in] tagId     The id of the tag.
     * @param[in] tagBuffer The buffer of the tag.
     * @param[in] start     The start of the tagged bytes.
     * @param[in] size      The number of tagged bytes.
     */
    void AddTag(uint32_t tagId, const ConstTagBuffer& tagBuffer,
                size_t start, size_t size);

    /**
     * @brief Is the byte tagged?
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     */
    bool HasTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT;

    /**
     * @brief Get the tag.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     *
     * @throw TagNotFound
     */
    Tag GetTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT;

    // Fragmentation.
public:
    /**
     * @brief Make a fragment.
     *
     * @param[in] start The start of the bytes.
     * @param[in] size  The number of bytes.
     */
    Packet MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Reassembly.
public:
    void AddHeader(const Packet& packet);
    void AddTrailer(const Packet& packet);

public:
    void swap(Packet& rhs) BOOST_NOEXCEPT;

private:
    PacketBuffer buffer_;

    typedef BasicTagList<ConstTagBuffer>  ByteTagList;
    ByteTagList tagList_;
};


////////////////////////////////////////////////////////////////////////////////
inline Packet::Packet(void) BOOST_NOEXCEPT
{
}

inline Packet::Packet(const PacketBuffer& buffer) BOOST_NOEXCEPT :
    buffer_(buffer)
{
    // Will not throw for an empty tag list.
    tagList_.AddAtEnd(buffer.GetSize());
}

inline Packet::Packet(const Packet& rhs) :
    buffer_(rhs.buffer_),
    tagList_(rhs.tagList_)
{
}

inline Packet& Packet::operator=(const Packet& rhs)
{
    if (this != &rhs)
    {
        buffer_  = rhs.buffer_;
        tagList_ = rhs.tagList_;
    }
    return *this;
}

inline Packet::Packet(Packet&& rhs) BOOST_NOEXCEPT :
    buffer_(std::move(rhs.buffer_)),
    tagList_(rhs.tagList_)
{
    rhs.tagList_ = ByteTagList();
}

inline Packet& Packet::operator=(Packet&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_  = std::move(rhs.buffer_);
        tagList_ = rhs.tagList_;
        rhs.tagList_ = ByteTagList();
    }
    return *this;
}

inline size_t Packet::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline ConstPacketBuffer Packet::GetBuffer(void) const BOOST_NOEXCEPT
{
    return buffer_;
}

inline PacketBuffer Packet::AddHeader(size_t size)
{
    buffer_.AddAtStart(size);
    tagList_.AddAtStart(size);
    return buffer_.MakeFragment(0, size);
}

inline PacketBuffer Packet::AddTrailer(size_t size)
{
    buffer_.AddAtEnd(size);
    tagList_.AddAtEnd(size);
    return buffer_.MakeFragment(buffer_.GetSize() - size, size);
}

inline void Packet::AddHeader(const ConstPacketBuffer& buffer)
{
    buffer_.AddAtStart(buffer);
}

inline void Packet::AddTrailer(const ConstPacketBuffer& buffer)
{
    buffer_.AddAtEnd(buffer);
}

inline void Packet::RemoveHeader(size_t size) BOOST_NOEXCEPT
{
    buffer_.RemoveAtStart(size);
    tagList_.RemoveAtStart(size);
}

inline void Packet::RemoveTrailer(size_t size) BOOST_NOEXCEPT
{
    buffer_.RemoveAtEnd(size);
    tagList_.RemoveAtEnd(size);
}

inline void Packet::AddTag(const Tag& tag, size_t start, size_t size)
{
    tagList_.Insert(tag, start, size);
}

inline void Packet::AddTag(uint32_t tagId, const ConstTagBuffer& tagBuffer,
                           size_t start, size_t size)
{
    tagList_.Insert(tagId, tagBuffer, start, size);
}

inline bool Packet::HasTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT
{
    return tagList_.Exists(tagId, offset);
}

inline Tag Packet::GetTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT
{
    return tagList_.Get(tagId, offset);
}

inline Packet Packet::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    Packet fragment(*this);
    fragment.RemoveHeader(start);
    fragment.RemoveTrailer(fragment.GetSize() - size);
    return fragment;
}

inline void Packet::AddHeader(const Packet& packet)
{
    buffer_.AddAtStart(packet.buffer_);
    tagList_.AddAtStart(packet.tagList_);
}

inline void Packet::AddTrailer(const Packet& packet)
{
    buffer_.AddAtEnd(packet.buffer_);
    tagList_.AddAtEnd(packet.tagList_);
}

inline void Packet::swap(Packet& rhs) BOOST_NOEXCEPT
{
    boost::swap(buffer_, rhs.buffer_);
    boost::swap(tagList_, rhs.tagList_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(Packet& lhs, Packet& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // PACKET_H__5E4E4E99_1B44_44E3_925B_58B8E93BD581

