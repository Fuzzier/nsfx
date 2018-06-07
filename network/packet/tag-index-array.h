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

#ifndef TAG_INDEX_ARRAY_H__DC874317_24B1_47AE_8111_60350999C4E8
#define TAG_INDEX_ARRAY_H__DC874317_24B1_47AE_8111_60350999C4E8


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag-index.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index array (POD).
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

inline void TagIndexArray::AddRef(TagIndexArray* tia)
{
    BOOST_ASSERT(tia);
    BOOST_ASSERT(tia->refCount_ >= 0);
    ++tia->refCount_;
}

inline void TagIndexArray::Release(TagIndexArray* tia) BOOST_NOEXCEPT
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
            idx->~TagIndex();
            --idx;
        }
        uint8_t* p = reinterpret_cast<uint8_t*>(tia);
        delete[] p;
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_INDEX_ARRAY_H__DC874317_24B1_47AE_8111_60350999C4E8

