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
#include <nsfx/network/buffer/tag-buffer-storage.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index.
 * @internal
 */
struct TagIndex
{
    size_t  tagId_;       ///< The id of the tag.
    size_t  tagStart_;    ///< The start of tagged bytes (inclusive).
    size_t  tagEnd_;      ///< The end of tagged bytes (exclusive).
    TagBufferStorage* storage_;  ///< The storage of the tag.

    // Helper functions.
    static void Ctor(TagIndex* idx, size_t tagId, size_t tagStart,
                     size_t tagEnd, TagBufferStorage* storage) BOOST_NOEXCEPT;
    static void CopyCtor(TagIndex* lhs, const TagIndex* rhs) BOOST_NOEXCEPT;
    static void CopyAssign(TagIndex* lhs, const TagIndex* rhs);
    static void Release(TagIndex* idx);
    static void Swap(TagIndex* lhs, TagIndex* rhs) BOOST_NOEXCEPT;
    static bool HasTaggedByte(size_t tagStart, size_t tagEnd,
                              size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT;
    static bool HasTaggedByte(const TagIndex* idx,
                              size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index array.
 * @internal
 */
struct TagIndexArray
{
    refcount_t refCount_;  ///< The reference count of the tag index.
    size_t  capacity_;     ///< Number of tag indices.
    size_t  dirty_;        ///< Number of used tag indices.
    TagIndex indices_[1];  ///< The tag indices.

    // Helper functions.
    static TagIndexArray* Allocate(size_t capacity);
    static void AddRef(TagIndexArray* tia) BOOST_NOEXCEPT;
    static void Release(TagIndexArray* tia);
};


////////////////////////////////////////////////////////////////////////////////
inline void TagIndex::Ctor(TagIndex* idx, size_t tagId, size_t tagStart,
                           size_t tagEnd, TagBufferStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(idx);
    idx->tagId_    = tagId;
    idx->tagStart_ = tagStart;
    idx->tagEnd_   = tagEnd;
    idx->storage_  = storage;
}

inline void TagIndex::CopyCtor(TagIndex* lhs, const TagIndex* rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT(lhs);
    BOOST_ASSERT(rhs);
    BOOST_ASSERT(lhs != rhs);
    *lhs = *rhs;
    if (lhs->storage_)
    {
        TagBufferStorage::AddRef(lhs->storage_);
    }
}

inline void TagIndex::CopyAssign(TagIndex* lhs, const TagIndex* rhs)
{
    BOOST_ASSERT(lhs);
    BOOST_ASSERT(rhs);
    if (lhs != rhs)
    {
        TagBufferStorage* tmp = lhs->storage_;
        *lhs = *rhs;
        if (lhs->storage_)
        {
            TagBufferStorage::AddRef(lhs->storage_);
        }
        if (tmp)
        {
            TagBufferStorage::Release(tmp);
        }
    }
}

inline void TagIndex::Release(TagIndex* idx)
{
    BOOST_ASSERT(idx);
    if (idx->storage_)
    {
        TagBufferStorage::Release(idx->storage_);
    }
}

inline void TagIndex::Swap(TagIndex* lhs, TagIndex* rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT(lhs);
    BOOST_ASSERT(rhs);
    if (lhs != rhs)
    {
        TagIndex tmp = *lhs;
        *lhs = *rhs;
        *rhs = tmp;
    }
}

inline bool TagIndex::HasTaggedByte(size_t tagStart, size_t tagEnd,
                                    size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT
{
    return (tagStart < bufferEnd &&
            tagEnd   > bufferStart);
}

inline bool TagIndex::HasTaggedByte(const TagIndex* idx,
                                    size_t bufferStart, size_t bufferEnd) BOOST_NOEXCEPT
{
    return HasTaggedByte(idx->tagStart_, idx->tagEnd_, bufferStart, bufferEnd);
}


////////////////////////////////////////////////////////////////////////////////
inline TagIndexArray* TagIndexArray::Allocate(size_t capacity)
{
    BOOST_ASSERT_MSG(capacity, "Cannot allocate an empty tag index array.");
    size_t size = sizeof (TagIndexArray) + sizeof (TagIndex) * capacity;
    TagIndexArray* tia = reinterpret_cast<TagIndexArray*>(new uint8_t[size]);
    tia->refCount_ = 1;
    tia->capacity_ = capacity;
    tia->dirty_    = 0;
    return tia;
}

inline void TagIndexArray::AddRef(TagIndexArray* tia) BOOST_NOEXCEPT
{
    BOOST_ASSERT(tia);
    BOOST_ASSERT(tia->refCount_ >= 0);
    ++tia->refCount_;
}

inline void TagIndexArray::Release(TagIndexArray* tia)
{
    BOOST_ASSERT(tia);
    BOOST_ASSERT(tia->refCount_ > 0);
    if (--tia->refCount_ == 0)
    {
        TagIndex* idx = tia->indices_ + (tia->dirty_ - 1);
        TagIndex* end = tia->indices_;
        while (idx >= end)
        {
            --tia->dirty_;
            TagIndex::Release(idx);
            --idx;
        }
        uint8_t* p = reinterpret_cast<uint8_t*>(tia);
        delete[] p;
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_INDEX_H__2CC52BF3_7FE2_4F85_A67B_B4764D414923

