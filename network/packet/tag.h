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


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A free tag that is not associated with any bytes.
 *
 *  A tag has an id and a value.
 *  The value of a tag is a buffer.
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
    Tag(size_t id, TagBuffer&& buffer);

    // Copyable.
public:
    Tag(const Tag& rhs) BOOST_NOEXCEPT;
    Tag& operator=(const Tag& rhs) BOOST_NOEXCEPT;

public:
    /**
     * @brief Get the id of the tag.
     */
    size_t GetId(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a read-only iterator of the tag buffer.
     */
    ConstTagBufferIterator GetBufferBegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a read-only iterator of the tag buffer.
     */
    ConstTagBufferIterator GetBufferEnd(void) const BOOST_NOEXCEPT;

private:
    size_t id_;
    TagBuffer buffer_;
};


////////////////////////////////////////////////////////////////////////////////
inline Tag::Tag(size_t id, TagBuffer&& buffer) :
    id_(id),
    buffer_(std::move(buffer))
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

inline size_t Tag::GetId(void) const BOOST_NOEXCEPT
{
    return id_;
}

inline ConstTagBufferIterator Tag::GetBufferBegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstTagBufferIterator Tag::GetBufferEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F

