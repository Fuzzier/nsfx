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

#ifndef BASIC_TAG_INDEX_ARRAY_H__8D7B076F_FFC3_4482_9105_92D6CA1C0E26
#define BASIC_TAG_INDEX_ARRAY_H__8D7B076F_FFC3_4482_9105_92D6CA1C0E26


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag/basic-tag-index.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network.
 * @brief The tag index array (POD).
 *
 * @tparam ValueType The type of the value of the tag.
 */
template<class ValueType>
struct BasicTagIndexArray
{
    typedef ValueType                    TagValue;
    typedef BasicTagIndex<TagValue>      TagIndex;
    typedef BasicTagIndexArray<TagValue> TagIndexArray;

    refcount_t refCount_;   ///< The reference count of the tag index.
    size_t     capacity_;   ///< Number of tag indices.
    size_t     dirty_;      ///< Number of used tag indices.
    TagIndex   indices_[1]; ///< The tag indices.


    ////////////////////////////////////////
    // Helper functions.
    static TagIndexArray* Allocate(size_t capacity)
    {
        BOOST_ASSERT_MSG(capacity, "Cannot allocate an empty tag index array.");
        size_t size = sizeof (TagIndexArray) + sizeof (TagIndex) * capacity;
        TagIndexArray* tia = reinterpret_cast<TagIndexArray*>(new uint8_t[size]);
        tia->refCount_ = 1;
        tia->capacity_ = capacity;
        tia->dirty_    = 0;
        return tia;
    }

    static void AddRef(TagIndexArray* tia) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(tia);
        BOOST_ASSERT(tia->refCount_ >= 0);
        ++tia->refCount_;
    }

    static void Release(TagIndexArray* tia)
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
};


NSFX_CLOSE_NAMESPACE


#endif // BASIC_TAG_INDEX_ARRAY_H__8D7B076F_FFC3_4482_9105_92D6CA1C0E26

