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
#include <nsfx/network/packet/packet-buffer.h>
#include <nsfx/network/packet/tag/basic-tag-list.h>
#include <boost/core/swap.hpp>
#include <utility> // move
#include <memory> // unique_ptr


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// TagBuffer of byte tags.
typedef FixedBuffer              TagBuffer;
typedef ConstFixedBuffer         ConstTagBuffer;


////////////////////////////////////////////////////////////////////////////////
// Packet.
/**
 * @ingroup Network
 * @brief A packet.
 *
 * =============
 * Packet models
 * =============
 *
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
 *    packet, due to the lack of information about the actual structure of the
 *    packet.
 *    Since the headers and trailers are added by different protocols, it
 *    requires complete knowledge about the set of protocols and the structures
 *    of headers and trailers added by each protocol.
 *    It complicates the problem in order to obtain such global knowledge.
 *    Therefore, when dealing with physical packets, the solid buffer model is
 *    preferred.
 *
 * ## Packet as virtual fields
 *    OPNET adopts this approach that a packet consists of virtual fields.
 *    It also has the problem to convert a physical packet to a simulation
 *    packet.
 *
 * ## Packet as C++ class
 *    OMNET++ adopts this approach that a packet is modeled as a C++ class.
 *    It is suitable to model packets with fixed format, but is hard to model
 *    all kinds of packets, such as packets with variable fields.
 *    It also introduces a complicated message discription language, which
 *    makes the learning curve steeper.
 *
 * =================
 * Packet operations
 * =================
 *
 *   In reality, a network packet is structured.
 *   Each entity adds its own header or trailer to the packet.
 *   The header or trailer of the packet is processed in a FIFO (first-in
 *   first-out) order.
 *   These observations forms the foundation of the design.
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
 *     Moreover, a packet may be discarded without being modified, and
 *     physically duplicating it becomes a waste.
 *
 * ### Discussion
 *     The idea is that, if a packet is supposed to be physically duplicated,
 *     the copies of the packet can share the same storage (memory block) as
 *     long as they do not modify the bytes owned by other packets.
 *     The bottom line is physically duplicating the storage, but it tries to
 *     delay/avoid the operation whenever possible.
 *
 *     The key is to identify which bytes are uses by other packets.
 *
 * #### Shared storage
 *      The content of a packet is managed by a buffer.
 *      A buffer is linked to a storage.
 *      When a packet is copied, the buffer of the copy is linked to the same
 *      storage.
 *      i.e, the buffers of copies of a packet share the same storage.
 *      The lifetime of the storage is managed via reference counting,
 *      naturally.
 *
 * #### Responsibility of buffer
 *      The responsibility of a buffer is to mark its <i>private area</i>, i.e.,
 *      the area that contains bytes that is supposed to be exclusively owned
 *      by the buffer, and must not be modified by other entities.
 *
 *      An entity can modify the bytes of a buffer <b>if and only if</b> the
 *      bytes are not within the private areas of any other buffers.
 *      For a buffer, the bytes in the private area of other buffers are
 *      considered <i>immutable</i> bytes of the buffer.
 *      When an entity tries to modify the immutable bytes, the storage must be
 *      physically duplicated for the buffer.
 *
 *      When an entity tries to modify the buffer, it must check whether the
 *      bytes are immutable.
 *      However, the buffer shall not be directly exposed to the users, as
 *      relying upon users' good memory to check the immutability of the bytes
 *      is not a good idea.
 *
 * #### Packet as encapsulation
 *      Packet is an encapsulation of the buffer that enforces the accessing
 *      rules.
 *
 *      An entity can only add (or remove) headers (or trailers) to the packet.
 *      When a header or trailer is added to a packet, if the header or trailer
 *      extends to an area that contains immutable bytes, the buffer storage
 *      is duplicated (copy-on-resize).
 *      The purpose is to make it safe to write in the area added by the header
 *      or trailer.
 *
 *      When an entity removes a header or trailer from the packet, the private
 *      area of the buffer is shrinked.
 *      The bytes in the removed header or trailer are out of the private area
 *      of the buffer, which may be modified by other entities.
 *      Thus, before an entity removes a header or trailer, it <b>shall</b>
 *      copy out the bytes in the header or trailer.
 *
 * #### Track private areas
 *      The private area can be simply modeled as a range of continuous bytes.
 *
 *      To reduce management overhead, the storage does not keep track of all
 *      private areas of each buffer.
 *      Instead, the storage uses a pessimestic/coarse management strategy that
 *      keeps track of the range of the union of all private areas, which is
 *      called the <i>dirty area</i> of the storage.
 *
 *      When the storage is owned by a single buffer, the dirty area of the
 *      storage is the same as the buffer's private area.
 *      i.e., whenever an entity adds or removes a header or trailer to the
 *      buffer, the dirty area of the storage is updated according.
 *
 *      When the storage is shared among several buffers, if an entity adds a
 *      header or trailer to the buffer, and the header or trailer does not
 *      overlap with the private areas of any other buffers, the buffer owns
 *      the added area, and the dirty area is also extended to include the
 *      added area.
 *
 *      However, if the added header or trailer overlaps with the private areas
 *      of other buffers, the storage is duplicated for the buffer, and
 *      the dirty area of the new storage is the same as the private area of
 *      the buffer.
 *
 * ## Fragmentation and reassembly
 *    Since solid buffer is used to model a packet, fragmentation can be done
 *    by creating a buffer with a smaller private area.
 *    And reassembly can be done by adding the buffer of one fragment to the
 *    start or end of the buffer of another fragment.
 *
 * ====
 * Tags
 * ====
 *
 *   In a simulation, a packet can carry side information that is not present
 *   in its buffer.
 *   i.e., the information is not transmitted in the real world packet.
 *   The side information is usually used to calculate performance metrics that
 *   is hard to obtain in real systems in a non-intrusive way or without
 *   overhead.
 *
 *   For example, a timestamp is a kind of side information that can be used to
 *   calculate end-to-end delay.
 *
 *   A simulation packet holds a set of <i>tags</i> to carry side information.
 *
 * ## Tags are implicit
 *    Tags can carry any type of side information.
 *    A type-neutral interface is provided to access the data of a tag.
 *
 *    However, communications <b>shall</b> be driven by the explicit information
 *    carried by physical headers and trailers, while communications via
 *    the side information carried by tags are <b>not</b> encouraged.
 *    The key problem is that tags are type-erased, and more or less similar
 *    to virtual fields.
 *
 *    Relying upon tags to transfer information can be harmful to reusability,
 *    since there is little syntactic support to make an explict contract
 *    among entities to agree upon the set of tags carried by a packet.
 *
 *    Tags <b>shall</b> be optional.
 *    For example, in hardware-in-the-loop (HIL) simulations, packets from real
 *    systems does not carries tags, and an entity <b>shall</b> be coded to
 *    tolerate packets with missing tags.
 *    For example, a missing tag of timestamp shall not break the program.
 *
 *    Entities within a node <b>shall</b> provide interfaces to explicitly
 *    speicify what kind of data is required for inter-entity communication.
 *
 *    For interface-based communication, since it is hard to serialize and
 *    deserialize the data transferred via interfaces, it is also hard to put
 *    entities in different processes.
 *    For packet-based communication, since it is easy to serialize and
 *    deserialize packets, entities in different processes can communicate via
 *    packets.
 *
 *    Although interface-based communication can harm parallism, but interfaces
 *    are more expressive and much clearer.
 *    e.g., when an entity at upper layer sends a packet to an entity at lower
 *    layer, the entity at lower layer does not interpret the bytes of the
 *    packet.
 *    How to process the packet is specified by the interface control
 *    information sent along with the packet.
 *    Besides the delivery of packets, an entity may also expose interfaces for
 *    configuration.
 *
 *    Even if packet-based communication is applied, tags still <b>shall not</b>
 *    be used to transfer explicit information.
 *    If tags are used for inter-entity communication, one also have to allocate
 *    global unique identifiers (usually an index of integral type) for each tag.
 *
 * ## Usage of tags
 *    The recommended approach to use tag is to adopt the existing rules of
 *    communication in practice.
 *
 *    First, tags <b>shall</b> only be recognized by peer entities across node
 *    boundary.
 *    Thus, tags shall be associated with the header or trailer added by a
 *    protocol entity in a packet, and accessed by the entities of the same
 *    protocol.
 *    Hence, each protocol can define protocol-local unique identifiers to
 *    distinguish tags.
 *
 *    Second, tags <b>shall not</b> carry information that <i>impractically</i>
 *    affects the behavior of an entity.
 *    Tags can be used to help debugging, collecting statistics, etc.
 *
 *    Third, tags <b>shall not</b> be used to coordinate local entities within
 *    a node.
 *    The cooperations among local entities <b>shall</b> be done via
 *    well-defined interfaces, or the headers of packets.
 *
 *    Finally, tags <b>must</b> be serializable, so they can be transferred
 *    across simulation sub-systems in parallel simulations.
 *
 *    Unlike OPNET, OMNET++ or NS3, the library does not provide a unique
 *    integer id for each packet.
 *    For debugging purpose, tracing a packet via its id is not always useful,
 *    since the id of a packet will change via duplication, fragmentation and
 *    reassembly.
 *    OPNET and OMNET++ provide a tree id to trace the duplications of a packet.
 *    However, the id is useless to trace a packet during fragmention and
 *    reassembly, unless the packets are purely virtual, i.e., packets are
 *    encapsulated into packets as virtual fields of packet type.
 *    However, virtual packets are too hard to convert to or from real packets.
 *    The id only tells that two packets are different.
 *    It losses information during duplication, fragmentation and reassembly.
 *    Tags are more suitable to trace the transmission and processing of the
 *    bytes of a packet.
 *
 * ### Placement of tags
 *     A tag can be considered as a <i>virtual</i> header or trailer that
 *     carries side information as a supplement to the <i>physical</i> header
 *     or trailer in a packet.
 *     Similar to physical headers and trailers, a tag stores side-information
 *     in a buffer of bytes.
 *     Such kind of tags are called <i>byte tags</i>.
 *     And a tag is associated with the header or trailer added by the entity.
 *
 *     When a packet is fragmented, the tag is carried by the fragment, as long
 *     as any of the tagged bytes remain in the fragment.
 *     i.e., if the fragment contains any bytes that have a tag, the fragment
 *     also carries the tag.
 *     When the fragments are reassembled, the tags are merged as the tagged
 *     bytes are put together.
 *
 * ## Tags are read-only
 *    To make memory usage efficient, tags are shared among duplicated packets
 *    and packet fragments to avoid physically duplicating the tags.
 *
 *    A tag is considered a virtual header or trailer of a packet.
 *    Similar to physical headers and trailers of a packet, tags are assumed
 *    to be owned by the packet exclusively.
 *    i.e., other entities must not modify the tags.
 *
 *    Thus, once a tag is added to a packet, it becomes read-only.
 *    A tag is removed automatically when the tagged bytes are removed from
 *    the packet.
 *
 * ## On complex transformation of bytes
 *    When the bytes of the packet is transformed in a complex way, there is
 *    problem.
 *    e.g., when the bytes of the packet are encoded, the bytes are mixed, and
 *    spread across the entire encoded packet, how to store the tags in the
 *    encoded packet?
 *
 *    One cannot simply let all tags associate with all encoded bytes, since
 *    it loses information of tags: the range of bytes a tag was associated
 *    with, and the packet a tag was belong to.
 *    First, the tags added by different entities may have the same identifier,
 *    and it is impossible to distinguish the tags and recover the associated
 *    ranges of bytes when the packet is decoded.
 *    Second, when two or more packets are encoded into one packet, the tags
 *    from different packets may have the same identifier, and it is impossible
 *    to distinguish the tags and recover the associated ranges of bytes when
 *    the packet is decoded.
 *
 *    To preserve the information of tags, the simplest way is to store the
 *    the original packet into a tag, and associate the tag with the entire
 *    bytes of the encoded packet.
 *    Such tags are called <i>packet tags</i>.
 *
 *    A tag that carries a buffer of bytes is called a <i>byte tag</i>; while
 *    a tag that carries a packet is called a <i>packet tag</i>.
 *
 *    Since a packet tag is associated with all bytes of an encoded packet,
 *    it is preversed no matter how the encoded packet is transformed
 *    (fragmentation/reassembly, encoding/decoding, etc).
 *
 *    A packet tag carries a copy of the original packet.
 *    The copy is cheap, since the buffer and tag list are shared among the
 *    packet and its copies.
 *    The tags of the original packet is preserved, and the buffer of the
 *    original packet is also preversed.
 *    It is easy to recover the tags when the encoded packets are decoded.
 *
 * =============================
 * Passing a packet via function
 * =============================
 *
 * There are several use cases regarding passing a packet via a function.
 * 1. A caller may want to pass the ownership of a packet.
 * 2. An event source may pass a packet to multiple event sinks.
 *
 * The key requirement is to make an entity assume the packet is owned by
 * itself alone.
 * Any modifications to the packet is not visible to other packets.
 *
 * The memory footprint of \c Packet is the size of a pointer, and is ideal
 * to be used as the function parameters to pass packets.
 *
 * Internally, the \c Packet holds a pointer to a \c Packet::Body, which has
 * a reference counter, a buffer, and tag lists.
 * Passing a packet is cheap.
 * If the caller wants to pass the ownership of the packet, it can move the
 * packet, and transfer the ownership to the callee.
 *
 * If an event source wants to send the packet to multiple event sinks, it
 * passes the packet by value.
 * In this case, each event sink holds a \c Packet that points to the same
 * buffer and tag lists.
 *
 * The \c Packet provides <i>copy-on-write</i> capability.
 * When the content of the packet are about to be modified, the \c Packet
 * checks the reference counter.
 * If the content is shared, it makes a private copy of the content to make
 * sure the modification is local to the \c Packet, so the key requirement
 * is satisfied.
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
    explicit Packet(const PacketBuffer& buffer);

    ~Packet(void);

    // Copyable.
public:
    Packet(const Packet& rhs) BOOST_NOEXCEPT;
    Packet& operator=(const Packet& rhs);

    // Movable.
public:
    Packet(Packet&& rhs) BOOST_NOEXCEPT;
    Packet& operator=(Packet&& rhs);

    // Operators.
    bool operator!(void) const BOOST_NOEXCEPT
    {
        return !body_;
    }

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    explicit operator bool() const BOOST_NOEXCEPT
    {
        return !!body_;
    }

#else // defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
private:
    bool ToBool(void) const BOOST_NOEXCEPT
    {
        return !!body_;
    }
    typedef bool (Packet::* BoolType)(void) const;

public:
    operator BoolType() const BOOST_NOEXCEPT
    {
        return body_ ? &Packet::ToBool : nullptr;
    }

#endif // !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

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

    // ByteTag.
public:
    /**
     * @brief Tag a range of bytes.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] buffer The buffer carried by the tag.
     * @param[in] start  The start of the tagged bytes.
     * @param[in] size   The number of tagged bytes.
     */
    void AddByteTag(uint32_t tagId, const ConstTagBuffer& buffer,
                    size_t start, size_t size);

    /**
     * @brief Is the byte tagged?
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     */
    bool HasByteTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT;

    /**
     * @brief Get the tag.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     *
     * @throw TagNotFound
     */
    ConstTagBuffer GetByteTag(uint32_t tagId, size_t offset) const;

    // PacketTag.
public:
    /**
     * @brief Tag a range of bytes.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] packet The packet carried by the tag.
     *                   The packet itself can be passed in as \c packet.
     * @param[in] start  The start of the tagged bytes.
     * @param[in] size   The number of tagged bytes.
     */
    void AddPacketTag(uint32_t tagId, Packet packet,
                      size_t start, size_t size);

    /**
     * @brief Is the byte tagged?
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     */
    bool HasPacketTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT;

    /**
     * @brief Get the tag.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte.
     *
     * @throw TagNotFound
     */
    Packet GetPacketTag(uint32_t tagId, size_t offset) const;

    /**
     * @brief Copy the tags from a packet.
     *
     * @param[in] src The packet that provide tags.
     *
     * @pre This packet and the source packet have the same buffer size.
     *
     * All tags in this packet are removed, and the tags of the source packet
     * are copied to this packet.
     *
     * It has no effect if either packet is empty.
     *
     * @throw InvalidArgument The source packet has a different buffer size.
     */
    void CopyTagsFrom(const Packet& src);

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
    /**
     * @brief Add a packet as the header.
     *
     * @param[in] packet The packet itself can be passed in as \c packet.
     *
     * The buffer and tags are added.
     */
    void AddHeader(Packet packet);

    /**
     * @brief Add a packet as the trailer.
     *
     * @param[in] packet The packet itself can be passed in as \c packet.
     *
     * The buffer and tags are added.
     */
    void AddTrailer(Packet packet);

    // Clone.
public:
    /**
     * @brief Make a clone.
     */
    Packet Clone(void) const;

public:
    void swap(Packet& rhs) BOOST_NOEXCEPT;

    // Body.
private:
    void AddRef(void) BOOST_NOEXCEPT;
    void Release(void);

    /**
     * @brief Make the packet body a private copy.
     *
     * This function is called when a packet is about to be modified.
     */
    void MakePrivate(void);

private:
    /**
     * @ingroup Network
     * @brief The contents of a packet.
     */
    struct Body
    {
        Body(void) BOOST_NOEXCEPT;

        Body(const Body& rhs) BOOST_NOEXCEPT;

        /**
         * @brief A reference counter to enable copy-on-write.
         *
         * \c refcount_ is is the number of users that shares the body.
         * When the body is about to be modified, the packet will duplicate
         * the body, if <code>refcount_ > 1</code>.
         * Thus, other holders of the packet will not see the modification.
         */
        refcount_t refcount_;

        PacketBuffer buffer_;

        typedef BasicTagList<ConstTagBuffer>  ByteTagList;
        ByteTagList byteTagList_;

        typedef BasicTagList<Packet>  PacketTagList;
        PacketTagList packetTagList_;
    };

    Body* body_;
};


////////////////////////////////////////////////////////////////////////////////
inline Packet::Body::Body(void) BOOST_NOEXCEPT :
    refcount_(1)
{
}

inline Packet::Body::Body(const Body& rhs) BOOST_NOEXCEPT :
    refcount_(1),
    buffer_(rhs.buffer_),
    byteTagList_(rhs.byteTagList_),
    packetTagList_(rhs.packetTagList_)
{
}


////////////////////////////////////////////////////////////////////////////////
inline Packet::Packet(void) BOOST_NOEXCEPT :
    body_(nullptr)
{
}

inline Packet::Packet(const PacketBuffer& buffer) :
    body_(new Body())
{
    body_->buffer_ = buffer;
    // Will not throw for an empty tag list.
    body_->byteTagList_.AddAtEnd(buffer.GetSize());
    body_->packetTagList_.AddAtEnd(buffer.GetSize());
}

inline Packet::~Packet(void)
{
    Release();
}

inline Packet::Packet(const Packet& rhs) BOOST_NOEXCEPT :
    body_(rhs.body_)
{
    AddRef();
}

inline Packet& Packet::operator=(const Packet& rhs)
{
    if (body_ != rhs.body_)
    {
        Release();
        body_ = rhs.body_;
        AddRef();
    }
    return *this;
}

inline Packet::Packet(Packet&& rhs) BOOST_NOEXCEPT :
    body_(rhs.body_)
{
    rhs.body_ = nullptr;
}

inline Packet& Packet::operator=(Packet&& rhs)
{
    if (body_ != rhs.body_)
    {
        Release();
        boost::swap(body_, rhs.body_);
    }
    return *this;
}

inline size_t Packet::GetSize(void) const BOOST_NOEXCEPT
{
    return body_ ? body_->buffer_.GetSize() : 0;
}

inline ConstPacketBuffer Packet::GetBuffer(void) const BOOST_NOEXCEPT
{
    return body_ ? body_->buffer_ : PacketBuffer();
}

inline PacketBuffer Packet::AddHeader(size_t size)
{
    MakePrivate();
    body_->buffer_.AddAtStart(size);
    body_->byteTagList_.AddAtStart(size);
    body_->packetTagList_.AddAtStart(size);
    return body_->buffer_.MakeFragment(0, size);
}

inline PacketBuffer Packet::AddTrailer(size_t size)
{
    MakePrivate();
    body_->buffer_.AddAtEnd(size);
    body_->byteTagList_.AddAtEnd(size);
    body_->packetTagList_.AddAtEnd(size);
    return body_->buffer_.MakeFragment(body_->buffer_.GetSize() - size, size);
}

inline void Packet::AddHeader(const ConstPacketBuffer& buffer)
{
    MakePrivate();
    body_->buffer_.AddAtStart(buffer);
    body_->byteTagList_.AddAtStart(buffer.GetSize());
    body_->packetTagList_.AddAtStart(buffer.GetSize());
}

inline void Packet::AddTrailer(const ConstPacketBuffer& buffer)
{
    MakePrivate();
    body_->buffer_.AddAtEnd(buffer);
    body_->byteTagList_.AddAtEnd(buffer.GetSize());
    body_->packetTagList_.AddAtEnd(buffer.GetSize());
}

inline void Packet::RemoveHeader(size_t size) BOOST_NOEXCEPT
{
    MakePrivate();
    body_->buffer_.RemoveAtStart(size);
    body_->byteTagList_.RemoveAtStart(size);
    body_->packetTagList_.RemoveAtStart(size);
}

inline void Packet::RemoveTrailer(size_t size) BOOST_NOEXCEPT
{
    MakePrivate();
    body_->buffer_.RemoveAtEnd(size);
    body_->byteTagList_.RemoveAtEnd(size);
    body_->packetTagList_.RemoveAtEnd(size);
}

inline void Packet::AddByteTag(uint32_t tagId, const ConstTagBuffer& buffer,
                               size_t start, size_t size)
{
    MakePrivate();
    body_->byteTagList_.Insert(tagId, buffer, start, size);
}

inline bool Packet::HasByteTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT
{
    return body_ ? body_->byteTagList_.Exists(tagId, offset) : false;
}

inline ConstTagBuffer Packet::GetByteTag(uint32_t tagId, size_t offset) const
{
    if (!body_)
    {
        BOOST_THROW_EXCEPTION(TagNotFound());
    }
    return body_->byteTagList_.Get(tagId, offset).GetValue();
}

inline void Packet::AddPacketTag(uint32_t tagId, Packet packet,
                                 size_t start, size_t size)
{
    // If this packet itself is added in a packet tag, the packet in the tag
    // must have a separate copy of the packet body.
    // Otherwise, it forms circular reference counting, and causes memory leaks.
    // Since the packet is passed by value, it holds a reference count, and
    // MakePrivate() will ensure the body is copied.
    MakePrivate();
    body_->packetTagList_.Insert(tagId, packet, start, size);
}

inline bool Packet::HasPacketTag(uint32_t tagId, size_t offset) const BOOST_NOEXCEPT
{
    return body_ ? body_->packetTagList_.Exists(tagId, offset) : false;
}

inline Packet Packet::GetPacketTag(uint32_t tagId, size_t offset) const
{
    if (!body_)
    {
        BOOST_THROW_EXCEPTION(TagNotFound());
    }
    return body_->packetTagList_.Get(tagId, offset).GetValue();
}

inline void Packet::CopyTagsFrom(const Packet& src)
{
    if (body_ && src.body_)
    {
        if (GetSize() != src.GetSize())
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("Cannot copy tags, since the source packet has "
                             "a different buffer size."));
        }
        // Since the packet is passed by value, it holds a reference count, and
        // MakePrivate() will ensure the body is copied.
        body_->byteTagList_   = src.body_->byteTagList_;
        body_->packetTagList_ = src.body_->packetTagList_;
    }
}

inline Packet Packet::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    Packet fragment(*this);
    fragment.RemoveHeader(start);
    fragment.RemoveTrailer(fragment.GetSize() - size);
    return fragment;
}

inline void Packet::AddHeader(Packet packet)
{
    if (packet.body_)
    {
        // Even if this packet itself is passed in, since the packet is passed
        // by value, it holds a reference count, and MakePrivate() will ensure
        // the body is copied.
        MakePrivate();
        body_->buffer_.AddAtStart(packet.body_->buffer_);
        body_->byteTagList_.AddAtStart(packet.body_->byteTagList_);
        body_->packetTagList_.AddAtStart(packet.body_->packetTagList_);
    }
}

inline void Packet::AddTrailer(Packet packet)
{
    if (packet.body_)
    {
        // Even if this packet itself is passed in, since the packet is passed
        // by value, it holds a reference count, and MakePrivate() will ensure
        // the body is copied.
        MakePrivate();
        body_->buffer_.AddAtEnd(packet.body_->buffer_);
        body_->byteTagList_.AddAtEnd(packet.body_->byteTagList_);
        body_->packetTagList_.AddAtEnd(packet.body_->packetTagList_);
    }
}

inline void Packet::swap(Packet& rhs) BOOST_NOEXCEPT
{
    boost::swap(body_, rhs.body_);
}

inline void Packet::AddRef(void) BOOST_NOEXCEPT
{
    if (body_)
    {
        ++body_->refcount_;
    }
}

inline void Packet::Release(void)
{
    if (body_)
    {
        if (--body_->refcount_ == 0)
        {
            delete body_;
        }
        body_ = nullptr;
    }
}

inline void Packet::MakePrivate(void)
{
    if (body_)
    {
        if (body_->refcount_ > 1)
        {
            std::unique_ptr<Body> copy(new Body(*body_));
            --body_->refcount_;
            Body* tmp = copy.release();
            boost::swap(tmp, body_);
        }
    }
    else
    {
        body_ = new Body();
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(Packet& lhs, Packet& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // PACKET_H__5E4E4E99_1B44_44E3_925B_58B8E93BD581

