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
#include <nsfx/network/packet/tag-index.h>
#include <nsfx/network/packet/tag-storage.h>
#include <nsfx/network/packet/tag.h>
#include <nsfx/network/packet/exception.h>
#include <boost/type_index.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <utility> // forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The tag list.
 *
 * The origin of a buffer is a fixed position, which is determined when the
 * packet is created.
 *
 * When a packet is created, it sets the origin of its buffer list.
 * The origin is \c TagList::REF_POINT bytes ahead of the middle of the current
 * buffer.
 * The \c TagList::REF_POINT is half of the maximum value of \c size_t.
 * The packet can add about \c TagList::REF_POINT bytes of headers and about
 * \c TagList::REF_POINT bytes of trailers before overflow.
 *
 * The tag list must keep synchronized with the position of the buffer.
 * Thus, the origin the tag list can be deduced.
 * And whether the tagged bytes is within the buffer can be deduced.
 * A packet is responsibility to call \c TagList::AddAtStart(),
 * \c TagList::RemoveAtStart(), \c TagList::AddAtEnd(), and
 * \c TagList::RemoveAtEnd() when it does the same action to its buffer.
 *
 * The tag value is stored in a \c TagStorage.
 * A \c TagStorage is responsible to store the tag value, and provides a
 * reference count.
 * The tag storage is designed to be able to hold non-copyable, non-movable
 * tag values.
 *
 * A \c TagIndex holds the tag and its id.
 * It is also responsible to hold the start and end position of the tagged
 * bytes, relative to the origin of the tag list.
 *
 * The tag indices are stored in a \c TagIndexArray.
 * The tag indices may not be ordered.
 *
 * @code
 * origin of tag list       start of buffer                      end of buffer
 * |                        |                                    |
 * |----- buffer start ---->|                                    |
 * |----- buffer end ------------------------------------------->|
 * |                        v                tagged bytes        v
 * |                        |===============|************|======|
 * |                                        ^             ^
 * |------------- tag start --------------->|             |
 * |------------- tag end ------------------------------->|
 * @endcode
 *
 * @code
 * TagList        [list]
 *                  |
 * TagIndexArray  [idx1   idx2   idx3  ...]   (shared, copy-on-write)
 *                  |      |      |
 * TagStorage     [tag1] [tag2] [tag3] ...    (shared, store value)
 *
 * @endcode
 *
 */
class TagList
{
public:
    /**
     * @brief Create an empty tag list.
     *
     * The current size of the buffer is assumed to be \c 0.
     */
    TagList(void) BOOST_NOEXCEPT;

    /**
     * @brief Create an empty tag list.
     *
     * @param[in] capacity The initial capacity of the tag list.
     *
     * The current size of the buffer is assumed to be \c 0.
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

    // Synchronize with buffer.
public:
    void AddAtStart(size_t size) BOOST_NOEXCEPT;
    void RemoveAtStart(size_t size) BOOST_NOEXCEPT;
    void AddAtEnd(size_t size) BOOST_NOEXCEPT;
    void RemoveAtEnd(size_t size) BOOST_NOEXCEPT;

    // Tag.
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    /**
     * @brief Insert a tag for the specified bytes in the buffer.
     *
     * @tparam T  The type of the tag value.
     * @tparam Args  The argument types of the constructor of the tag value.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] start  The start of the byte, relative to the start of
     *                   the buffer.
     * @param[in] size   The number of bytes to tag.
     *                   All bytes <b>must</b> be within the current buffer.
     * @param[in] args   The arguments to construct the tag value.
     */
    template<class T, class... Args>
    void Insert(size_t tagId, size_t start, size_t size, Args&&... args);

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag-list.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    /**
     * @brief Whether the specified byte has a tag.
     *
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte, relative to the start of
     *                   the buffer.
     */
    bool Exists(size_t tagId, size_t offset) const;

    /**
     * @brief Get the value of the tag for the specified byte.
     *
     * @tparam T  The type of the tag value.
     * @param[in] tagId  The id of the tag.
     * @param[in] offset The offset of the byte, relative to the start of
     *                   the buffer.
     *
     * @throw TagTypeMismatch
     * @throw TagNotFound
     */
    template<class T>
    const T& Get(size_t tagId, size_t offset) const;

    // Methods.
public:
    size_t GetSize(void) const BOOST_NOEXCEPT;
    size_t GetBufferStart(void) const BOOST_NOEXCEPT;
    size_t GetBufferEnd(void) const BOOST_NOEXCEPT;
    const TagIndexArray* GetTagIndexArray(void) const BOOST_NOEXCEPT;

private:
    /**
     * @brief Whether any of the tagged bytes are within the buffer.
     */
    bool BufferHasTaggedByte(const TagIndex* idx) const BOOST_NOEXCEPT;

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
        if (tia_)
        {
            TagIndexArray* tmp = tia_;
            tia_ = nullptr;
            TagIndexArray::Release(tmp);
        }
        bufferStart_ = rhs.bufferStart_;
        bufferEnd_   = rhs.bufferEnd_;
        size_        = rhs.size_;
        tia_         = rhs.tia_;
        if (tia_)
        {
            TagIndexArray::AddRef(tia_);
        }
    }
    return *this;
}

inline void TagList::AddAtStart(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferStart_ >= size,
                     "The buffer grows too large that the start position of "
                     "the buffer tracked by the tag list would be negative.");
    bufferStart_ -= size;
}

inline void TagList::RemoveAtStart(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferStart_ + size <= bufferEnd_,
                     "The start of the buffer cannot go beyond the end.");
    bufferStart_ += size;
}

inline void TagList::AddAtEnd(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferEnd_ + size >= bufferEnd_,
                     "The buffer grows too large that the end position of "
                     "the buffer tracked by the tag list would overflow.");
    bufferEnd_ += size;
}

inline void TagList::RemoveAtEnd(size_t size) BOOST_NOEXCEPT
{
    BOOST_ASSERT_MSG(bufferStart_ + size <= bufferEnd_,
                     "The end of the buffer cannot go beyond the start.");
    bufferEnd_ -= size;
}

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<class T, class... Args>
inline void
TagList::Insert(size_t tagId, size_t start, size_t size, Args&&... args)
{
    // Be careful that 'size' and 'size_' are confusion.
    BOOST_ASSERT_MSG(start + size <= bufferEnd_ - bufferStart_,
                     "Cannot tag bytes that are outside of the buffer.");
    // Reallocate?
    bool dirty = false;
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
            dirty = true;
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
            dirty = true;
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
                dirty = true;
                Reallocate(tia_->capacity_);
            }
        }
    }
    BOOST_ASSERT(tia_->dirty_ < tia_->capacity_);
    TagStorage* tag = TagStorage::Allocate<T>(std::forward<Args>(args)...);
    TagIndex* idx   = tia_->indices_ + size_;
    size_t tagStart = bufferStart_ + start;
    size_t tagEnd   = bufferStart_ + start + size;
    TagIndex::Ctor(idx, tagId, tagStart, tagEnd, tag);
    // Increase the size after the construction succeeded.
    ++size_;
    ++tia_->dirty_;
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag-list.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


inline bool TagList::Exists(size_t tagId, size_t offset) const
{
    bool found = false;
    size_t pos = bufferStart_ + offset;
    TagIndex* idx = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->tagId_ == tagId &&
            idx->tagStart_ <= pos && pos < idx->tagEnd_)
        {
            found = true;
            break;
        }
        ++idx;
    }
    return found;
}

template<class T>
inline const T& TagList::Get(size_t tagId, size_t offset) const
{
    size_t pos = bufferStart_ + offset;
    TagIndex* idx = tia_->indices_;
    TagIndex* end = tia_->indices_ + size_;
    while (idx != end)
    {
        if (idx->tagId_ == tagId &&
            idx->tagStart_ <= pos && pos < idx->tagEnd_)
        {
            if (tag->intf_->GetTypeId() != boost::typeindex::type_id<T>())
            {
                BOOST_THROW_EXCEPTION(
                    TagTypeMismatch() <<
                    ErrorMessage("The requested value type mismatches "
                                 "with the value type of the tag."));
            }
            return TagStorage::GetValue<T>(idx->tag_);
        }
        ++idx;
    }
    BOOST_THROW_EXCEPTION(
        TagNotFound() <<
        ErrorMessage("Cannot find the requested tag in the tag list."));
}

inline size_t TagList::GetSize(void) const BOOST_NOEXCEPT
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

inline bool TagList::BufferHasTaggedByte(const TagIndex* idx) const BOOST_NOEXCEPT
{
    BOOST_ASSERT(idx);
    return (idx->tagStart_ < bufferEnd_ &&
            idx->tagEnd_   > bufferStart_);
}

inline void TagList::Compact(void)
{
    BOOST_ASSERT(tia_);
    BOOST_ASSERT(tia_->refCount_ == 1);
    // Compact the array from the tail.
    // i.e., remove elements from the tail of the array.
    TagIndex* head = tia_->indices_;
    TagIndex* tail = tia_->indices_ + (tia_->dirty_ - 1);
    TagIndex* end  = tia_->indices_ + size_;
    // Release the elements that are not held by the list.
    while (tail >= end)
    {
        // Reduce the size of the array before releasing the tag index.
        // Even if the release operation throws, the array is still valid.
        --tia_->dirty_;
        TagIndex::Release(tail);
        --tail;
    }
    // Preserve the elements at the head of the list.
    while (tail >= head)
    {
        if (BufferHasTaggedByte(head))
        {
            ++head;
        }
        else
        {
            TagIndex::Swap(tail, head);
            --size_;
            --tia_->dirty_;
            TagIndex::Release(tail);
            --tail;
            break;
        }
    }
    while (tail >= head)
    {
        if (BufferHasTaggedByte(tail))
        {
            // The tail is swapped to the head, and preserved.
            TagIndex::Swap(tail, head);
            ++head;
        }
        else
        {
            --size_;
            --tia_->dirty_;
            TagIndex::Release(tail);
            --tail;
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
        if (BufferHasTaggedByte(src))
        {
            // The retain the origin of the list, thus the position of the
            // the tagged bytes is also retained.
            TagIndex::CopyCtor(dst, src);
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


NSFX_CLOSE_NAMESPACE


#endif // TAG_LIST_H__E97CCFD4_4B27_40D4_A750_379D8FB31667


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

////////////////////////////////////////
# if BOOST_PP_ITERATION_FLAGS() == 0
// template<class T, class A0, class A1, ...>
// void Insert(size_t tagId, size_t start, size_t size, A0&& a0, A1&& a1, ...);
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
void Insert(size_t tagId, size_t start, size_t size
            BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
            BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a));


////////////////////////////////////////
# elif BOOST_PP_ITERATION_FLAGS() == 1
// template<class T, class A0, class A1, ...>
// inline void TagList::Insert(size_t tagId, size_t start, size_t size,
//                             A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline void
TagList::Insert(size_t tagId, size_t start, size_t size
                BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    // Be careful that 'size' and 'size_' are confusion.
    BOOST_ASSERT_MSG(start + size <= bufferEnd_ - bufferStart_,
                     "Cannot tag bytes that are outside of the buffer.");
    // Reallocate?
    bool dirty = false;
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
            dirty = true;
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
            dirty = true;
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
                dirty = true;
                Reallocate(tia_->capacity_);
            }
        }
    }
    BOOST_ASSERT(tia_->dirty_ < tia_->capacity_);
    // TagStorage* tag = TagStorage::Allocate<T>(
    //     std::forward<A0>(a0), std::forward<A1>(a1), ...);
    TagStorage* tag = TagStorage::Allocate<T>(
        BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    TagIndex* idx   = tia_->indices_ + size_;
    size_t tagStart = bufferStart_ + start;
    size_t tagEnd   = bufferStart_ + start + size;
    TagIndex::Ctor(idx, tagId, tagStart, tagEnd, tag);
    // Increase the size after the construction succeeded.
    ++size_;
    ++tia_->dirty_;
}


# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

