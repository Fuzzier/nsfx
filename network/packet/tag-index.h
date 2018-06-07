/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TAG_INDEX_H__2CC52BF3_7FE2_4F85_A67B_B4764D414923
#define TAG_INDEX_H__2CC52BF3_7FE2_4F85_A67B_B4764D414923


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index.
 * @internal
 */
class TagIndex
{
public:
    TagIndex(const Tag& tag, size_t tagStart, size_t tagEnd) BOOST_NOEXCEPT;

    TagIndex(const TagIndex& rhs) BOOST_NOEXCEPT;
    TagIndex& operator=(const TagIndex& rhs) BOOST_NOEXCEPT;

    TagIndex(TagIndex&& rhs) BOOST_NOEXCEPT;
    TagIndex& operator=(TagIndex&& rhs) BOOST_NOEXCEPT;

    const Tag& GetTag(void) const BOOST_NOEXCEPT;
    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;

    bool HasTaggedByte(size_t bufferStart, size_t bufferEnd) const BOOST_NOEXCEPT;

    static bool HasTaggedByte(size_t tagStart, size_t tagEnd,
                              size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT;

    void swap(Tag& rhs) BOOST_NOEXCEPT;

private:
    Tag     tag_;      ///< The tag.
    size_t  tagStart_; ///< The start of tagged bytes (inclusive).
    size_t  tagEnd_;   ///< The end of tagged bytes (exclusive).
};


////////////////////////////////////////////////////////////////////////////////
void swap(Tag& lhs, Tag& rhs) BOOST_NOEXCEPT;


////////////////////////////////////////////////////////////////////////////////
inline TagIndex::TagIndex(const Tag& tag, size_t tagStart, size_t tagEnd) BOOST_NOEXCEPT :
    tag_(tag),
    tagStart_(tagStart),
    tagEnd_(tagEnd),
{
}

inline TagIndex::TagIndex(const TagIndex& rhs) BOOST_NOEXCEPT :
    tag_(rhs.tag),
    tagStart_(rhs.tagStart),
    tagEnd_(rhs.tagEnd)
{
}

inline TagIndex& TagIndex::operator=(const TagIndex& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        tag_      = rhs.tag_;
        tagStart_ = rhs.tagStart_;
        tagEnd_   = rhs.tagEnd_;
    }
    return *this;
}

inline TagIndex::TagIndex(TagIndex&& rhs) BOOST_NOEXCEPT :
    tag_(std::move(rhs.tag)),
    tagStart_(rhs.tagStart),
    tagEnd_(rhs.tagEnd)
{
}

inline TagIndex& TagIndex::operator=(TagIndex&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        tag_      = std::move(rhs.tag_);
        tagStart_ = rhs.tagStart_;
        tagEnd_   = rhs.tagEnd_;
    }
    return *this;
}

inline void TagIndex::swap(Tag& rhs) BOOST_NOEXCEPT
{
    boost::swap(tag_, rhs.tag_);
    boost::swap(tagStart_, rhs.tagStart_);
    boost::swap(tagEnd_, rhs.tagEnd_);
}

inline const Tag& TagIndex::GetTag(void) const BOOST_NOEXCEPT
{
    return tag_;
}

inline size_t TagIndex::GetStart(void) const BOOST_NOEXCEPT
{
    return tagStart_;
}

inline size_t TagIndex::GetEnd(void) const BOOST_NOEXCEPT
{
    return tagEnd_;
}

inline bool TagIndex::HasTaggedByte(size_t bufferStart, size_t bufferEnd) const BOOST_NOEXCEPT
{
    return HasTaggedByte(tagStart_, tagEnd_, bufferStart, bufferEnd);
}

inline bool TagIndex::HasTaggedByte(size_t tagStart, size_t tagEnd,
                                    size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT
{
    return (tagStart < bufferEnd &&
            tagEnd   > bufferStart);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(TagIndex& lhs, TagIndex& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_INDEX_H__2CC52BF3_7FE2_4F85_A67B_B4764D414923

