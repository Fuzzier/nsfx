/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F
#define TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/tag-buffer.h>
#include <nsfx/network/buffer/const-tag-buffer.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A free tag that is not associated with any bytes.
 *
 *  A tag is considered as a virtual header or trailer that provides side
 *  information to supplement the physical header or trailer in a packet.
 *
 *  A tag has an id and a value.
 *  The value of a tag is a buffer.
 *
 *  Originally, a tag value is implemented via a type-erased interface that
 *  supports any C++ type, including non-copyable, non-movable types.
 *  However, in distributed simulations, such tag values are not easily
 *  transmitted across machine boundaries.
 *  Use a buffer as the tag value is more direct.
 *  Moreover, since a header or trailer is modeled as a buffer, modeling a tag
 *  value as a buffer is also sound.
 */
class Tag
{
    // Xtructors.
public:
    /**
     * @brief Create a tag.
     *
     * @param[in] id     The id of the tag.
     * @param[in] buffer The tag buffer.
     */
    Tag(size_t id, const ConstTagBuffer& buffer);

    // Copyable.
public:
    Tag(const Tag& rhs) BOOST_NOEXCEPT;
    Tag& operator=(const Tag& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    Tag(Tag&& rhs) BOOST_NOEXCEPT;
    Tag& operator=(Tag&& rhs) BOOST_NOEXCEPT;

public:
    /**
     * @brief Get the id of the tag.
     */
    size_t GetId(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a read-only iterator of the tag buffer.
     */
    const ConstTagBuffer& GetBuffer(void) const BOOST_NOEXCEPT;

public:
    void swap(Tag& rhs) BOOST_NOEXCEPT;

private:
    size_t id_;
    ConstTagBuffer buffer_;
};


////////////////////////////////////////////////////////////////////////////////
void swap(Tag& lhs, Tag& rhs) BOOST_NOEXCEPT;


////////////////////////////////////////////////////////////////////////////////
inline Tag::Tag(size_t id, const ConstTagBuffer& buffer) :
    id_(id),
    buffer_(buffer)
{
}

inline Tag::Tag(const Tag& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    buffer_(rhs.buffer_)
{
}

inline Tag& Tag::operator=(const Tag& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        id_ = rhs.id_;
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline Tag::Tag(Tag&& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    buffer_(std::move(rhs.buffer_))
{
}

inline Tag& Tag::operator=(Tag&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        id_ = rhs.id_;
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t Tag::GetId(void) const BOOST_NOEXCEPT
{
    return id_;
}

inline const ConstTagBuffer& Tag::GetBuffer(void) const BOOST_NOEXCEPT
{
    return buffer_;
}

inline void Tag::swap(Tag& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(id_, rhs.id_);
        boost::swap(buffer_, rhs.buffer_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(Tag& lhs, Tag& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F

