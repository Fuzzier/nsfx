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

#ifndef BASIC_TAG_INDEX_H__4EBE6E70_CF6E_45F2_B95A_0ACD793E9967
#define BASIC_TAG_INDEX_H__4EBE6E70_CF6E_45F2_B95A_0ACD793E9967


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag/basic-tag.h>
#include <boost/core/swap.hpp>
#include <type_traits>  // true_type, false_type


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index.
 *
 * @tparam ValueType The type of the value of the tag.
 */
template<class ValueType>
class BasicTagIndex
{
public:
    typedef ValueType               TagValue;
    typedef BasicTag<TagValue>      Tag;
    typedef BasicTagIndex<TagValue> TagIndex;

public:
    BasicTagIndex(const Tag& tag, size_t tagStart, size_t tagEnd) BOOST_NOEXCEPT;

    BasicTagIndex(uint32_t tagId, const TagValue& value,
                  size_t tagStart, size_t tagEnd) BOOST_NOEXCEPT;

    BasicTagIndex(const TagIndex& rhs) BOOST_NOEXCEPT;
    TagIndex& operator=(const TagIndex& rhs) BOOST_NOEXCEPT;

    BasicTagIndex(TagIndex&& rhs) BOOST_NOEXCEPT;
    TagIndex& operator=(TagIndex&& rhs) BOOST_NOEXCEPT;

    const Tag& GetTag(void) const BOOST_NOEXCEPT;
    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;

    bool HasTaggedByte(size_t bufferStart, size_t bufferEnd) const BOOST_NOEXCEPT;

    static bool HasTaggedByte(size_t tagStart, size_t tagEnd,
                              size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT;

    void swap(TagIndex& rhs) BOOST_NOEXCEPT;

private:
    Tag     tag_;      ///< The tag.
    size_t  tagStart_; ///< The start of tagged bytes (inclusive).
    size_t  tagEnd_;   ///< The end of tagged bytes (exclusive).
};


////////////////////////////////////////////////////////////////////////////////
template<class T>
inline BasicTagIndex<T>::BasicTagIndex(const Tag& tag,
                                       size_t tagStart,
                                       size_t tagEnd) BOOST_NOEXCEPT :
    tag_(tag),
    tagStart_(tagStart),
    tagEnd_(tagEnd)
{
}

template<class T>
inline BasicTagIndex<T>::BasicTagIndex(uint32_t tagId,
                                       const TagValue& value,
                                       size_t tagStart,
                                       size_t tagEnd) BOOST_NOEXCEPT :
    tag_(tagId, value),
    tagStart_(tagStart),
    tagEnd_(tagEnd)
{
}

template<class T>
inline BasicTagIndex<T>::BasicTagIndex(const BasicTagIndex<T>& rhs) BOOST_NOEXCEPT :
    tag_(rhs.tag_),
    tagStart_(rhs.tagStart_),
    tagEnd_(rhs.tagEnd_)
{
}

template<class T>
inline BasicTagIndex<T>&
BasicTagIndex<T>::operator=(const BasicTagIndex<T>& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        tag_      = rhs.tag_;
        tagStart_ = rhs.tagStart_;
        tagEnd_   = rhs.tagEnd_;
    }
    return *this;
}

template<class T>
inline BasicTagIndex<T>::BasicTagIndex(BasicTagIndex<T>&& rhs) BOOST_NOEXCEPT :
    tag_(std::move(rhs.tag_)),
    tagStart_(rhs.tagStart_),
    tagEnd_(rhs.tagEnd_)
{
}

template<class T>
inline BasicTagIndex<T>&
BasicTagIndex<T>::operator=(BasicTagIndex<T>&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        tag_      = std::move(rhs.tag_);
        tagStart_ = rhs.tagStart_;
        tagEnd_   = rhs.tagEnd_;
    }
    return *this;
}

template<class T>
inline void BasicTagIndex<T>::swap(BasicTagIndex<T>& rhs) BOOST_NOEXCEPT
{
    boost::swap(tag_,      rhs.tag_);
    boost::swap(tagStart_, rhs.tagStart_);
    boost::swap(tagEnd_,   rhs.tagEnd_);
}

template<class T>
inline const typename BasicTagIndex<T>::Tag&
BasicTagIndex<T>::GetTag(void) const BOOST_NOEXCEPT
{
    return tag_;
}

template<class T>
inline size_t BasicTagIndex<T>::GetStart(void) const BOOST_NOEXCEPT
{
    return tagStart_;
}

template<class T>
inline size_t BasicTagIndex<T>::GetEnd(void) const BOOST_NOEXCEPT
{
    return tagEnd_;
}

template<class T>
inline bool
BasicTagIndex<T>::HasTaggedByte(size_t bufferStart, size_t bufferEnd) const BOOST_NOEXCEPT
{
    return HasTaggedByte(tagStart_, tagEnd_, bufferStart, bufferEnd);
}

template<class T>
inline bool
BasicTagIndex<T>::HasTaggedByte(size_t tagStart, size_t tagEnd,
                                size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT
{
    return (tagStart < bufferEnd &&
            tagEnd   > bufferStart);
}


NSFX_CLOSE_NAMESPACE


#endif // BASIC_TAG_INDEX_H__4EBE6E70_CF6E_45F2_B95A_0ACD793E9967

