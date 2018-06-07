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

#ifndef TAG_LIST_H__E97CCFD4_4B27_40D4_A750_379D8FB31667
#define TAG_LIST_H__E97CCFD4_4B27_40D4_A750_379D8FB31667


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag.h>
#include <nsfx/network/packet/tag-index-array.h>
#include <nsfx/network/packet/exception.h>
#include <boost/core/swap.hpp>
#include <utility> // forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The tag list.
 *
 * # Overview
 *  A tag has an id and a value.
 *  A tag may be attached to multiple bytes.
 *  A byte may also have multiple tags.
 *
 *  A packet uses a tag list to hold the set of tags.
 *  When a packet is duplicated, fragmented or reassembled, the tags are
 *  carried along with the tagged bytes.
 *  i.e., when a byte is transferred to a packet, all of its tags are also
 *  carried by the packet.
 *
 * # The 1-D byte space
 *   In order to locate the tagged bytes of a buffer, a tag list builds a 1-D
 *   byte space.
 *   The origin of a tag list is the origin of the 1-D byte space.
 *   The 1-D byte space can accommodate <code>MAX_SIZE + 1</code> bytes.
 *   The coordinate of the origin is \c 0, and the coordinate of the furthest
 *   byte is \c MAX_SIZE.
 *
 *   The origin of a tag list is determined when the packet is created.
 *   To allow the buffer to grow to the maximum extent, the origin is chosen as
 *   such that the current buffer is located at the middle of the space.
 *
 *   @code
 *   origin of byte space    start of buffer    end of buffer
 *   |                       |                  |
 *   v                       v                  v
 *   |-----------------------|--------|--------|-----------------------|
 *   0                                ^                                MAX_SIZE
 *                                    |
 *                                    middle of buffer
 *                                    middle of byte space
 *   @endcode
 *
 *   The \c TagList::REF_POINT is the half of \c MAX_SIZE.
 *   The origin is \c TagList::REF_POINT bytes ahead from the middle of the
 *   current buffer.
 *   The packet can add about \c TagList::REF_POINT bytes of headers and about
 *   \c TagList::REF_POINT bytes of trailers before overflow.
 *
 * # Track the position of bytes
 *   Once a byte is tagged, the coordinate of the tagged byte is fixed in the
 *   space.
 *   i.e., no matter how the buffer grows or shrinks, the coordinates of the
 *   tagged bytes are not changed.
 *   The coordinates of tagged bytes are only changed when the origin changes.
 *   e.g., a tag is transferred to another tag list with a different origin.
 *
 *   To obtain the offset of a tagged byte relative to the start of the
 *   current buffer, the tag list must keep track the location of the buffer.
 *   The tag list records the coordinates of the start and end bytes of the
 *   current buffer.
 *   Thus, whether a tagged byte is within the current buffer can be deduced.
 *
 *   The packet is responsibility to call \c TagList::AddAtStart(),
 *   \c TagList::RemoveAtStart(), \c TagList::AddAtEnd(), and
 *   \c TagList::RemoveAtEnd(), when it does the same action to its buffer.
 *
 *   @code
 *   origin of tag list       start of buffer                     end of buffer
 *   |                        |                                   |
 *   |----- buffer start ---->|                                   |
 *   |----- buffer end ------------------------------------------>|
 *   |                        v               tagged bytes        v
 *   |                        |==============|************|======|
 *   |                                       ^             ^
 *   |------------- tag start -------------->|             |
 *   |------------- tag end ------------------------------>|
 *   @endcode
 *
 * # Management of tags
 *   To avoid memory allocation and deallocation, the tag values are shared
 *   among packets.
 *
 * ## Tag value
 *    The value of a tag is a \c TagBuffer.
 *    A tag value is created once and read-only.
 *    A tag value is removed automatically when the attaches bytes are removed
 *    from the buffer.
 *
 * ## Tag index
 *    A \c TagIndex holds a pointer to the tag storage, and is responsible to
 *    record the tag id and the range of tagged bytes.
 *
 * ## Tag index array
 *    The tag indices are stored in a \c TagIndexArray.
 *    The tag indices is not ordered in the array.
 *
 * ## Tag list
 *    A \c TagList holds a pointer to the tag array, and records the number of
 *    used tag indices.
 *    It is also responsible to keep track of the location of the current
 *    buffer.
 *    Each packet holds a tag list.
 *
 *    Whether a tag is carried by the packet <b>is</b> determined by comparing
 *    the range of tagged bytes and the location of the current buffer.
 *
 *    To avoid memory allocation and deallocation, the tag index arrays are
 *    also shared among tag lists.
 *    The tag index array is managed in a copy-on-write manner.
 *    The array is not reallocated, as long as the following conditions are all
 *    held.
 *
 *    * The modification of the buffer location does not introduce tags that
 *      are not in the tag list.
 *    * The modification to the tag index array does not introduce, modify or
 *      remove tags in other tag lists that share the array.
 *
 *    @code
 *    TagList        [list]
 *                     |
 *    TagIndexArray  [idx1   idx2   idx3  ...]   (shared, copy-on-write)
 *                     |      |      |
 *    TagBuffer      [tag1] [tag2] [tag3] ...    (shared, store value)
 *    @endcode
 *
 * ## Coordinate transformation
 *    Duplicating, expanding or shrinking a packet does not change the origin
 *    of the tag list.
 *    The range of tagged bytes in a tag index is not changed when they are
 *    copied among the tag index arrays.
 *
 *    However, when packets are reassembled, the tag lists of different packets
 *    may have different origins.
 *    The coordinates of tagged bytes must be transformed when the tags are
 *    copied from one tag list to another.
 *
 * ### Add to start
 *     When one packet is added to the start of another packet, the location of
 *     tags shall be transformed.
 *
 *     @code
 *     packet 1
 *     origin 1
 *     |               |<------------- buffer 1 ----------->|
 *     |               |<------ buffer 2 ------>|           |
 *     |               |             tag        |           |
 *     |---------------|+++++++++++|*****|++++++|===========|-------
 *     |               |-- fixed ->|
 *     |------- new tag start ---->|
 *                     ^
 *                     |
 *                     reference point for transformation
 *                     |
 *                     v
 *         packet 2
 *         origin 2    |<------ buffer 2 ------>|
 *         |           |             tag        |
 *         |-----------|+++++++++++|*****|++++++|-------
 *         |           |-- fixed ->|
 *         |--- old tag start ---->|
 *     @endcode
 *
 * ### Add to end
 *     When one packet is added to the start of another packet, the location of
 *     tags shall also be transformed.
 *
 *     @code
 *     packet 1
 *     origin 1
 *     |               |<------------- buffer 1 ----------->|
 *     |               |           |<------ buffer 2 ------>|
 *     |               |           |        tag             |
 *     |---------------|===========|++++++|*****|+++++++++++|-------
 *     |                                        |-- fixed ->|
 *     |-------------------- new tag end ------------------>|
 *                                                          ^
 *                                                          |
 *                         reference point for transformation
 *                                                          |
 *                                                          v
 *         packet 2
 *         origin 2                |<------ buffer 2 ------>|
 *         |                       |        tag             |
 *         |-----------------------|++++++|*****|+++++++++++|-------
 *         |                                    |-- fixed ->|
 *         |---------------- old tag end ------------------>|
 *     @endcode
 *
 * ## Copy-on-write
 *    When a packet shrinks its buffer, it does not reallocate or modify the
 *    array, nor does it change the number of tag indices used by the tag list.
 *    It may leave some dirty tags in the array that are no longer in the tag
 *    list.
 *
 *    When a packet expands its buffer, the array is reallocated if dirty tags
 *    are to be introduced into the tag list.
 *
 *    When a packet is duplicated, it does not reallocate or modify the array.
 *
 *    When a packet is fragmented, it is duplicated and shrinked, the array is
 *    not reallocated.
 *
 *    When one packet is added to the start or end of an other packet, the later
 *    expands its buffer, and copies tag indices from the former.
 *    The former packet does not reallocate its array.
 *    And it depends on the situation whether the later packet reallocates its
 *    array or not.
 *
 *    When a tag is inserted to a packet, it depends on the situation whether
 *    the array is reallocates or not.
 */
class TagList
{
public:
    /**
     * @brief Create an empty tag list.
     */
    TagList(void) BOOST_NOEXCEPT;

    /**
     * @brief Create an empty tag list.
     *
     * @param[in] capacity The initial capacity of the tag list.
     */
    explicit TagList(size_t capacity);

    /**
     * @brief Create an empty tag list.
     *
     * @param[in] capacity   The initial capacity of the tag list.
     * @param[in] bufferSize The current size of the buffer.
     */
    TagList(size_t capacity, size_t bufferSize);

    ~TagList(void);

    // Copyable.
public:
    TagList(const TagList& rhs) BOOST_NOEXCEPT;
    TagList& operator=(const TagList& rhs);

    // Buffer.
public:
    /**
     * @brief Expand the buffer.
     */
    void AddAtStart(size_t size);

    /**
     * @brief Expand the buffer and copy tags.
     *
     * The size of the buffer is given by \c rhs.
     */
    void AddAtStart(const TagList& rhs);

    /**
     * @brief Shrink the buffer.
     */
    void RemoveAtStart(size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Expand the buffer.
     */
    void AddAtEnd(size_t size);

    /**
     * @brief Expand the buffer and copy tags.
     *
     * The size of the buffer is given by \c rhs.
     */
    void AddAtEnd(const TagList& rhs);

    /**
     * @brief Shrink the buffer.
     */
    void RemoveAtEnd(size_t size) BOOST_NOEXCEPT;

    // Tag.
public:
    /**
     * @brief Insert a tag for a range of bytes in the buffer.
     *
     * @param[in] tag    The tag.
     * @param[in] start  The start of the byte, relative to the start of
     *                   the buffer.
     * @param[in] size   The number of bytes to tag.
     *                   All bytes <b>must</b> be within the current buffer.
     */
    void Insert(const Tag& tag, size_t start, size_t size);

    /**
     * @brief Insert a tag for a range of bytes in the buffer.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] buffer The buffer of the tag.
     * @param[in] start  The start of the byte, relative to the start of
     *                   the buffer.
     * @param[in] size   The number of bytes to tag.
     *                   All bytes <b>must</b> be within the current buffer.
     */
    void Insert(size_t tagId, const ConstTagBuffer& buffer,
                size_t start, size_t size);

    /**
     * @brief Whether the specified byte has a tag.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte, relative to the start of
     *                   the buffer.
     */
    bool Exists(size_t tagId, size_t offset) const BOOST_NOEXCEPT;

    /**
     * @brief Get the tag for the specified byte.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte, relative to the start of
     *                   the buffer.
     *
     * @remarks If multiple tags with the same id are attached to the byte,
     *          one of the tag is returned.
     *
     * @throw TagNotFound
     */
    Tag Get(size_t tagId, size_t offset) const;

    // Methods.
public:
    size_t GetSize(void) const BOOST_NOEXCEPT;
    size_t GetInternalSize(void) const BOOST_NOEXCEPT;
    size_t GetBufferStart(void) const BOOST_NOEXCEPT;
    size_t GetBufferEnd(void) const BOOST_NOEXCEPT;
    const TagIndexArray* GetTagIndexArray(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(TagList& rhs) BOOST_NOEXCEPT;

private:
    /**
     * @brief Prepare the tag index array to accommodate one more tag index.
     *
     * The tag index array is compacted or reallocated as necessary.
     *
     * @post <code>tia_->indices_[size_]</code> is not used by any tag lists,
     *       so it is available for this tag list to accommodate a new tag.
     */
    void PrepareToInsert(void);

    /**
     * @brief Compact the tag index array if its owned by this tag list alone.
     *
     * @pre  <code>tia_->refCount_ == 1</code>.
     * @post <code>size_ == tia_->dirty_</code>.
     */
    void Compact(void);

    /**
     * @brief Reallocate the tag index array.
     */
    void Reallocate(size_t newCapacity);

    /**
     * @brief Expand the buffer.
     *
     * @param[in] deltaStart The amount added to the start.
     * @param[in] deltaEnd   The amount added to the end.
     */
    void ExpandBuffer(size_t deltaStart, size_t deltaEnd);

    /**
     * @brief Does the tag list has the specified tag.
     */
    bool HasTag(size_t tagId, size_t tagStart, size_t tagEnd) const BOOST_NOEXCEPT;

    /**
     * @brief Insert the specified tag.
     *
     * If the tag is already in the list, it is not inserted.
     */
    void InsertTag(const Tag& tag, size_t tagStart, size_t tagEnd);

public:
    enum
    {
        REF_POINT = static_cast<size_t>(-1) / 2,
    };

private:
    size_t bufferStart_;  ///< The start of the buffer, relative to the origin.
    size_t bufferEnd_;    ///< The end of the buffer, relative to the origin.

    size_t size_;         ///< The number of indices used by this list.
    TagIndexArray* tia_;  ///< The array of indices.
};


////////////////////////////////////////////////////////////////////////////////
void swap(TagList& lhs, TagList& rhs) BOOST_NOEXCEPT;


////////////////////////////////////////////////////////////////////////////////
inline TagList::TagList(void) BOOST_NOEXCEPT :
    bufferStart_(REF_POINT),
    bufferEnd_(REF_POINT),
    size_(0),
    tia_(nullptr)
{
}

inline TagList::TagList(size_t capacity) :
    bufferStart_(REF_POINT),
    bufferEnd_(REF_POINT),
    size_(0),
    tia_(TagIndexArray::Allocate(capacity))
{
}

inline TagList::TagList(size_t capacity, size_t bufferSize) :
    bufferStart_(REF_POINT - bufferSize / 2),
    bufferEnd_(bufferStart_ + bufferSize),
    size_(0),
    tia_(TagIndexArray::Allocate(capacity))
{
}

inline TagList::~TagList(void)
{
    if (tia_)
    {
        TagIndexArray::Release(tia_);
    }
}

inline TagList::TagList(const TagList& rhs) BOOST_NOEXCEPT :
    bufferStart_(rhs.bufferStart_),
    bufferEnd_(rhs.bufferEnd_),
    size_(rhs.size_),
    tia_(rhs.tia_)
{
    if (tia_)
    {
        TagIndexArray::AddRef(tia_);
    }
}

inline TagList& TagList::operator=(const TagList& rhs)
{
    if (this != &rhs)
    {
        TagIndexArray* tmp = tia_;
        bufferStart_ = rhs.bufferStart_;
        bufferEnd_   = rhs.bufferEnd_;
        size_        = rhs.size_;
        tia_         = rhs.tia_;
        if (tia_)
        {
            TagIndexArray::AddRef(tia_);
        }
        if (tmp)
        {
            TagIndexArray::Release(tmp);
        }
    }
    return *this;
}

inline void TagList::AddAtStart(size_t size)
{
    BOOST_ASSERT_MSG(size <= bufferStart_,
                     "The buffer grows too large that the start position of "
                     "the buffer tracked by the tag list would be negative.");
    ExpandBuffer(size, 0);
}

inline void TagList::AddAtStart(const TagList& rhs)
{
    size_t size = rhs.bufferEnd_ - rhs.bufferStart_;
    AddAtStart(size);
    // Copy tags from 'rhs'.
    const TagIndex* idx = rhs.tia_->indices_;
    const TagIndex* end = rhs.tia_->indices_ + rhs.size_;
    while (idx != end)
    {
        size_t newTagStart = idx->GetStart() + (bufferStart_ - rhs.bufferStart_);
        size_t newTagEnd   = idx->GetEnd()   + (bufferStart_ - rhs.bufferStart_);
        // Shall obtain the same result if (rhs.bufferStart_ > bufferStart_).
        BOOST_ASSERT(newTagStart ==
                     idx->GetStart() - (rhs.bufferStart_ - bufferStart_));
        BOOST_ASSERT(newTagEnd ==
                     idx->GetEnd()   - (rhs.bufferStart_ - bufferStart_));
        InsertTag(idx->GetTag(), newTagStart, newTagEnd);
        ++idx;
    }
}

inline void TagList::RemoveAtStart(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferStart_ + size <= bufferEnd_,
                     "The start of the buffer cannot go beyond the end.");
    bufferStart_ += size;
}

inline void TagList::AddAtEnd(size_t size)
{
    BOOST_ASSERT_MSG(bufferEnd_ + size >= bufferEnd_,
                     "The buffer grows too large that the end position of "
                     "the buffer tracked by the tag list would overflow.");
    ExpandBuffer(0, size);
}

inline void TagList::AddAtEnd(const TagList& rhs)
{
    size_t size = rhs.bufferEnd_ - rhs.bufferStart_;
    AddAtEnd(size);
    // Copy tags from 'rhs'.
    const TagIndex* idx = rhs.tia_->indices_;
    const TagIndex* end = rhs.tia_->indices_ + rhs.size_;
    while (idx != end)
    {
        size_t newTagStart = idx->GetStart() + (bufferEnd_ - rhs.bufferEnd_);
        size_t newTagEnd   = idx->GetEnd()   + (bufferEnd_ - rhs.bufferEnd_);
        // Shall obtain the same result if (rhs.bufferEnd_ > bufferEnd_).
        BOOST_ASSERT(newTagStart ==
                     idx->GetStart() - (rhs.bufferEnd_ - bufferEnd_));
        BOOST_ASSERT(newTagEnd ==
                     idx->GetEnd()   - (rhs.bufferEnd_ - bufferEnd_));
        InsertTag(idx->GetTag(), newTagStart, newTagEnd);
        ++idx;
    }
}

inline void TagList::RemoveAtEnd(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferStart_ + size <= bufferEnd_,
                     "The end of the buffer cannot go beyond the start.");
    bufferEnd_ -= size;
}

inline void TagList::Insert(const Tag& tag, size_t start, size_t size)
{
    BOOST_ASSERT_MSG(start + size <= bufferEnd_ - bufferStart_,
                     "Cannot tag bytes that are outside of the buffer.");
    PrepareToInsert();
    TagIndex* idx   = tia_->indices_ + size_;
    size_t tagStart = bufferStart_ + start;
    size_t tagEnd   = bufferStart_ + start + size;
    new (idx) TagIndex(tag, tagStart, tagEnd);
    // Increase the size after the construction succeeded.
    ++size_;
    ++tia_->dirty_;
}

inline void TagList::Insert(size_t tagId, const ConstTagBuffer& buffer,
                            size_t start, size_t size)
{
    Insert(Tag(tagId, buffer), start, size);
}

inline bool TagList::Exists(size_t tagId, size_t offset) const BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(offset < bufferEnd_ - bufferStart_,
                     "The offset is outside of the buffer.");
    bool found = false;
    size_t pos = bufferStart_ + offset;
    TagIndex* idx = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->GetTag().GetId() == tagId &&
            idx->GetStart() <= pos && pos < idx->GetEnd())
        {
            found = true;
            break;
        }
        ++idx;
    }
    return found;
}

inline Tag TagList::Get(size_t tagId, size_t offset) const
{
    BOOST_ASSERT_MSG(offset < bufferEnd_ - bufferStart_,
                     "The offset is outside of the buffer.");
    size_t pos = bufferStart_ + offset;
    TagIndex* idx = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->GetTag().GetId() == tagId &&
            idx->GetStart() <= pos && pos < idx->GetEnd())
        {
            return idx->GetTag();
        }
        ++idx;
    }
    BOOST_THROW_EXCEPTION(
        TagNotFound() <<
        ErrorMessage("Cannot find the requested tag in the tag list."));
}

inline size_t TagList::GetSize(void) const BOOST_NOEXCEPT
{
    size_t count = 0;
    TagIndex* idx = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->HasTaggedByte(bufferStart_, bufferEnd_))
        {
            ++count;
        }
        ++idx;
    }
    return count;
}

inline size_t TagList::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return size_;
}

inline size_t TagList::GetBufferStart(void) const BOOST_NOEXCEPT
{
    return bufferStart_;
}

inline size_t TagList::GetBufferEnd(void) const BOOST_NOEXCEPT
{
    return bufferEnd_;
}

inline const TagIndexArray* TagList::GetTagIndexArray(void) const BOOST_NOEXCEPT
{
    return tia_;
}

inline void TagList::PrepareToInsert(void)
{
    // If the array is not allocated yet.
    if (!tia_)
    {
        // Allocate an array for this list.
        size_t newCapacity = 4;
        TagIndexArray* newTia = TagIndexArray::Allocate(newCapacity);
        tia_ = newTia;
    }
    // If the array is owned by this list alone.
    else if (tia_->refCount_ == 1)
    {
        // Compact the array.
        Compact();
        // If the array has no free elements.
        if (tia_->dirty_ == tia_->capacity_)
        {
            // Reallocate the array for this list.
            Reallocate(tia_->capacity_ * 2);
        }
        // If the array has free elements.
        else
        {
            // The next element can be used by this list.
            // dirty = false;
        }
    }
    // If the array is shared with other lists.
    else // if (tia->refCount_ > 1)
    {
        // If the array has no free elements.
        if (tia_->dirty_ == tia_->capacity_)
        {
            // Reallocate the array for this list.
            Reallocate(tia_->capacity_ * 2);
        }
        // If the array has free elements.
        else // if (tia_->dirty_ < tia_->capacity_)
        {
            // If the next element is not used by other lists.
            if (size_ == tia_->dirty_)
            {
                // The next element can be used by this list.
                // dirty = false;
            }
            // If the next element is used by other lists.
            // This list cannot use the next element.
            else // if (size_ < tia_->dirty_)
            {
                // Reallocate the array for this list.
                Reallocate(tia_->capacity_);
            }
        }
    }
    BOOST_ASSERT(tia_->dirty_ < tia_->capacity_);
    BOOST_ASSERT(size_ == tia_->dirty_);
}

inline void TagList::Compact(void)
{
    BOOST_ASSERT(tia_);
    BOOST_ASSERT(tia_->refCount_ == 1);
    // Compact the array from the tail.
    // i.e., remove elements from the tail of the array.
    TagIndex* tail = tia_->indices_ + (tia_->dirty_ - 1);
    // Release the elements that are not held by the list.
    while (tia_->dirty_ > size_)
    {
        // Reduce the size of the array before releasing the tag index.
        // Even if the release operation throws, the array is still valid.
        --tia_->dirty_;
        tail->~TagIndex();
        --tail;
    }
    // Preserve the elements at the head of the list.
    TagIndex* head = tia_->indices_;
    while (head <= tail && head->HasTaggedByte(bufferStart_, bufferEnd_))
    {
        ++head;
    }
    if (head <= tail)
    {
        // Now, head is the first index that does not belong to the tag list.
        --tia_->dirty_;
        --size_;
        head->~TagIndex();
        // Release the remaining invalid elements.
        // Move the remaining valid elements toward head.
        TagIndex* next = head + 1;
        while (next <= tail)
        {
            if (next->HasTaggedByte(bufferStart_, bufferEnd_))
            {
                new (head) TagIndex(std::move(*next));
                ++head;
            }
            else
            {
                --tia_->dirty_;
                --size_;
                next->~TagIndex();
            }
            ++next;
        }
    }
    BOOST_ASSERT(size_ == tia_->dirty_);
}

inline void TagList::Reallocate(size_t newCapacity)
{
    BOOST_ASSERT(tia_);
    BOOST_ASSERT(newCapacity >= tia_->capacity_);
    // Expand the array.
    TagIndexArray* newTia = TagIndexArray::Allocate(newCapacity);
    // Copy the elements.
    TagIndex* src = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    TagIndex* dst = newTia->indices_;
    // For all tags used by this list.
    while (src != end)
    {
        // If any of the tagged bytes are within the current buffer.
        if (src->HasTaggedByte(bufferStart_, bufferEnd_))
        {
            // The retain the origin of the list, thus the position of the
            // the tagged bytes is also retained.
            new (dst) TagIndex(*src);
            ++dst;
            ++newTia->dirty_;
        }
        ++src;
    }
    // Release the array.
    TagIndexArray::Release(tia_);
    // Use the new array.
    tia_ = newTia;
    size_ = tia_->dirty_;
}

inline void TagList::ExpandBuffer(size_t deltaStart, size_t deltaEnd)
{
    // If the array is not allocated yet.
    if (!tia_)
    {
        // Do nothing, since it is safe to update the location of the buffer.
    }
    // If the array is owned by this list alone.
    else if (tia_->refCount_ == 1)
    {
        // Compact the array.
        Compact();
    }
    // If the array is shared with other lists.
    else // if (tia->refCount_ > 1)
    {
        size_t newBufferStart = bufferStart_ - deltaStart;
        size_t newBufferEnd   = bufferEnd_   + deltaEnd;
        // Whether dirty tags are to be introduced into the list?
        bool dirty = false;
        TagIndex* tail = tia_->indices_ + (size_ - 1);
        // The remaining number of tag indices to examine.
        size_t count = size_;
        while (count && !tail->HasTaggedByte(bufferStart_, bufferEnd_))
        {
            --size_;
            --tail;
            --count;
        }
        TagIndex* idx = tail;
        while (count)
        {
            size_t newBufferStart = bufferStart_ - deltaStart;
            size_t newBufferEnd   = bufferEnd_   + deltaEnd;
            // If a dirty tag that is not in the tag list would be added into
            // the tag list after buffer expansion.
            if (!idx->HasTaggedByte(bufferStart_, bufferEnd_) &&
                 idx->HasTaggedByte(newBufferStart, newBufferEnd))
            {
                dirty = true;
                break;
            }
            --idx;
            --count;
        }
        if (dirty)
        {
            // Reallocate the array for this list.
            Reallocate(tia_->capacity_);
        }
    }
    // Update the location of the buffer.
    bufferStart_ -= deltaStart;
    bufferEnd_   += deltaEnd;
}

inline bool TagList::HasTag(size_t tagId, size_t tagStart, size_t tagEnd) const BOOST_NOEXCEPT
{
    bool found = false;
    const TagIndex* idx = tia_->indices_;
    const TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->GetTag().GetId() == tagId &&
            idx->GetStart() == tagStart &&
            idx->GetEnd()   == tagEnd)
        {
            found = true;
            break;
        }
        ++idx;
    }
    return found;
}

inline void TagList::InsertTag(const Tag& tag, size_t tagStart, size_t tagEnd)
{
    if (!HasTag(tag.GetId(), tagStart, tagEnd) &&
        TagIndex::HasTaggedByte(tagStart, tagEnd, bufferStart_, bufferEnd_))
    {
        PrepareToInsert();
        TagIndex* idx = tia_->indices_ + size_;
        new (idx) TagIndex(tag, tagStart, tagEnd);
        // Increase the size after the construction succeeded.
        ++size_;
        ++tia_->dirty_;
    }
}

inline void TagList::swap(TagList& rhs) BOOST_NOEXCEPT
{
    boost::swap(bufferStart_, rhs.bufferStart_);
    boost::swap(bufferEnd_, rhs.bufferEnd_);
    boost::swap(size_, rhs.size_);
    boost::swap(tia_, rhs.tia_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(TagList& lhs, TagList& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_LIST_H__E97CCFD4_4B27_40D4_A750_379D8FB31667

