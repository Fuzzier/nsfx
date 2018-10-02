/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
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

#ifndef BASIC_TAG_H__723142D3_5C69_4B96_95F6_4C22074599CF
#define BASIC_TAG_H__723142D3_5C69_4B96_95F6_4C22074599CF


#include <nsfx/network/config.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A tag that can be carried by a packet to provide side information.
 *
 *  A tag is considered as a virtual header or trailer that provides side
 *  information to supplement the physical header or trailer in a packet.
 *
 *  A tag has an id and a value.
 *  The value of a tag must be copyable.
 *
 *  A tag is associated with a range of bytes in a packet.
 *  The range is maintained by the tag list of the packet.
 *
 *  Originally, a tag value is implemented via a type-erased interface that
 *  supports any C++ type, including non-copyable, non-movable types.
 *  However, in distributed simulations, such tag values are not easily
 *  serialized and transmitted across machine boundaries.
 *
 *  Use a buffer or a packet as the tag value is more direct.
 *  Moreover, since a header or trailer is modeled as a buffer, modeling
 *  the tag value as a buffer is also sound.
 *  Using packet as tag value eases the encoding and decoding of packets.
 */
template<class ValueType>
class BasicTag
{
public:
    typedef ValueType  TagValue;

    // Xtructors.
public:
    /**
     * @brief Create a tag.
     *
     * @param[in] id     The id of the tag.
     * @param[in] buffer The tag buffer.
     */
    BasicTag(uint32_t id, const ValueType& buffer);

    // Copyable.
public:
    BasicTag(const BasicTag& rhs) BOOST_NOEXCEPT;
    BasicTag& operator=(const BasicTag& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    BasicTag(BasicTag&& rhs) BOOST_NOEXCEPT;
    BasicTag& operator=(BasicTag&& rhs) BOOST_NOEXCEPT;

public:
    /**
     * @brief Get the id of the tag.
     */
    uint32_t GetId(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a read-only iterator of the tag buffer.
     */
    const ValueType& GetValue(void) const BOOST_NOEXCEPT;

public:
    void swap(BasicTag& rhs) BOOST_NOEXCEPT;

private:
    uint32_t  id_;
    ValueType value_;
};


////////////////////////////////////////////////////////////////////////////////
template<class ValueType>
inline BasicTag<ValueType>::BasicTag(uint32_t id, const ValueType& value) :
    id_(id),
    value_(value)
{
}

template<class ValueType>
inline BasicTag<ValueType>::BasicTag(const BasicTag<ValueType>& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    value_(rhs.value_)
{
}

template<class ValueType>
inline BasicTag<ValueType>&
BasicTag<ValueType>::operator=(const BasicTag<ValueType>& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        id_    = rhs.id_;
        value_ = rhs.value_;
    }
    return *this;
}

template<class ValueType>
inline BasicTag<ValueType>::BasicTag(BasicTag<ValueType>&& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    value_(std::move(rhs.value_))
{
}

template<class ValueType>
inline BasicTag<ValueType>&
BasicTag<ValueType>::operator=(BasicTag<ValueType>&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        id_ = rhs.id_;
        value_ = std::move(rhs.value_);
    }
    return *this;
}

template<class ValueType>
inline uint32_t
BasicTag<ValueType>::GetId(void) const BOOST_NOEXCEPT
{
    return id_;
}

template<class ValueType>
inline const ValueType&
BasicTag<ValueType>::GetValue(void) const BOOST_NOEXCEPT
{
    return value_;
}

template<class ValueType>
inline void
BasicTag<ValueType>::swap(BasicTag<ValueType>& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(id_,    rhs.id_);
        boost::swap(value_, rhs.value_);
    }
}


NSFX_CLOSE_NAMESPACE


#endif // BASIC_TAG_H__723142D3_5C69_4B96_95F6_4C22074599CF

