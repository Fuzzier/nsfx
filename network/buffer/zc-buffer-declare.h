/**
 * @file
 *
 * @brief Zero-compressed packet buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-05
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ZC_BUFFER_DECLARE_H__81877B5A_6E6F_4095_B35B_4EC979BE6388
#define ZC_BUFFER_DECLARE_H__81877B5A_6E6F_4095_B35B_4EC979BE6388


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-storage.h>
#include <nsfx/network/buffer/zc-buffer-iterator.h>


NSFX_OPEN_NAMESPACE


class ConstBuffer;
class ConstZcBuffer;
class ConstTagBuffer;


////////////////////////////////////////////////////////////////////////////////
// ZcBuffer.
/**
 * @ingroup Network
 * @brief An zero-compressed, automatically resized and copy-on-write buffer.
 *
 * # Structure
 *   The buffer holds a storage that provides a memory space for the buffer.
 *   The storage is logically divided into three areas.
 *   The data area is located in middle of the storage, and the remaining space
 *   is naturally divided in to the pre-data (pre-header) area and the post-data
 *   (post-trailer) area.
 *
 *   To reduce memory consumption, the buffer models a <i>virtual data area</i>.
 *   The buffer is logically divided into three areas: the <i>header area</i>,
 *   the <i>zero-compressed data area</i>, and the <i>trailer area</i>.
 *   The zero-compressed data area does not consume any physical memory, which
 *   is usually used to represent the application layer payload.
 *
 *   When the zero-compressed data area is not empty, the buffer represents a
 *   <i>virtual data buffer</i>.
 *   When the zero-compressed data area is empty, the buffer represents a
 *   <i>real buffer</i>.
 *
 * # Reallocation on expansion
 *   Several buffers can link to the same buffer storage, and each buffer has
 *   its own view of the start and end positions of the data area.
 *
 *   When a buffer expands, its buffer storage may be reallocated.
 *   If the buffer storage is shared by other buffers, and the buffer expands
 *   to an area that has already been occupied by other buffers, the buffer
 *   storage is automatically reallocated and duplicated for the buffer.
 *   The copy-on-write operations are \c ZcBuffer::AddAtStart() and
 *   \c ZcBuffer::AddAtEnd().
 */
class ZcBuffer
{
public:
    typedef ZcBufferIterator      iterator;
    typedef ConstZcBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    ZcBuffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The initial apacity of the buffer.
     *
     * The zero-compressed data area is empty in this buffer.
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data toward the head of the storage.
     */
    explicit ZcBuffer(size_t capacity);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The initial apacity of the buffer.
     * @param[in] zeroSize The size of the zero-compressed data area.
     *
     * The zero-compressed data area is located at the end of the storage,
     * optimizing for adding data at the head of the storage.
     */
    ZcBuffer(size_t capacity, size_t zeroSize);

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity   The initial apacity of the buffer.
     * @param[in] zeroStart  The start of the zero-compressed data area.
     *                       <p>
     *                       <code>zeroStart <= capacity</code>.
     * @param[in] zeroSize   The size of the zero-compressed data area.
     */
    ZcBuffer(size_t capacity, size_t zeroStart, size_t zeroSize);

    // Conversions.
public:
    /**
     * @brief Deep copy from a ConstBuffer.
     */
    explicit ZcBuffer(const ConstBuffer& rhs);

    /**
     * @brief Deep copy from a ConstZcBuffer.
     */
    explicit ZcBuffer(const ConstZcBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Deep copy from a ConstTagBuffer.
     */
    explicit ZcBuffer(const ConstTagBuffer& rhs);

private:
    /**
     * @brief Create a buffer.
     *
     * @param[in] storage  The reference count is taken by the buffer.
     *
     * @see \c InternalMakeRealCopy().
     * @internal
     */
    ZcBuffer(BufferStorage* storage, size_t start, size_t zeroStart,
           size_t zeroEnd, size_t end) BOOST_NOEXCEPT;

public:
    ~ZcBuffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ZcBuffer(const ZcBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ZcBuffer& operator=(const ZcBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ZcBuffer(ZcBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ZcBuffer& operator=(ZcBuffer&& rhs) BOOST_NOEXCEPT;

    // Acquire/release buffer storage.
private:
    void Acquire(void) BOOST_NOEXCEPT;

    void Release(void) BOOST_NOEXCEPT;

    // Methods.
public:
    /**
     * @brief Get the size of the represented data.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the size of the actually used memory.
     */
    size_t GetInternalSize(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the capacity of the storage.
     */
    size_t GetCapacity(void) const BOOST_NOEXCEPT;

    size_t GetStart(void) const BOOST_NOEXCEPT;
    size_t GetZeroStart(void) const BOOST_NOEXCEPT;
    size_t GetZeroEnd(void) const BOOST_NOEXCEPT;
    size_t GetEnd(void) const BOOST_NOEXCEPT;
    const BufferStorage* GetStorage(void) const BOOST_NOEXCEPT;

    /**
     * @brief Copy data to a memory block.
     * @return The number of bytes copied.
     */
    size_t CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT;

private:
    struct ContinuousTag {};
    struct SegmentedTag {};

    size_t InternalCopyTo(uint8_t* dst, size_t size, ContinuousTag) const BOOST_NOEXCEPT;
    size_t InternalCopyTo(uint8_t* dst, size_t size, SegmentedTag) const BOOST_NOEXCEPT;

    // Add/remove.
private:
    struct AdjustOffsetTag {};
    struct ReallocateTag {};
    struct MoveMemoryTag {};

public:
    /**
     * @brief Expand the buffer toward the start.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(size_t size);

    /**
     * @brief Expand the buffer and copy the specified contents.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(const uint8_t* src, size_t size);

    /**
     * @brief Expand the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(const ConstZcBuffer& src);

private:
    void InternalAddAtStart(size_t size, AdjustOffsetTag) BOOST_NOEXCEPT;
    void InternalAddAtStart(size_t size, size_t newCapacity,
                            size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtStart(size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT;

public:
    /**
     * @brief Expand the buffer toward the end.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(size_t size);

    /**
     * @brief Extend the buffer and copy the specified contents.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const uint8_t* src, size_t size);

    /**
     * @brief Extend the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const ConstZcBuffer& src);

private:
    void InternalAddAtEnd(size_t size, size_t dataSize, AdjustOffsetTag) BOOST_NOEXCEPT;
    void InternalAddAtEnd(size_t size, size_t newCapacity,
                          size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtEnd(size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT;

public:
    /**
     * @brief Shrink the buffer from the start.
     *
     * @param[in] size The number of bytes to remove.
     *                 <p>
     *                 If it is no less than the size of the buffer, the buffer
     *                 becomes empty.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void RemoveAtStart(size_t size) BOOST_NOEXCEPT;

    /**
     * @brief Shrink the buffer from the end.
     *
     * @param[in] size The number of bytes to remove.
     *                 <p>
     *                 If it is no less than the size of the buffer, the buffer
     *                 becomes empty.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void RemoveAtEnd(size_t size) BOOST_NOEXCEPT;

    // Fragmentation.
public:
    /**
     * @brief Make a fragment of the buffer.
     *
     * @param[in] start The start of the fragment.
     * @param[in] size  The size of the fragment.
     */
    ZcBuffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    ZcBuffer MakeRealBuffer(void) const;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const;

private:
    ZcBuffer InternalGetRealBuffer(ReallocateTag) const;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    ZcBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ZcBufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstZcBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstZcBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(ZcBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The storage.
     */
    BufferStorage* storage_;

    /**
     * @brief The logical offset of the start of the header area.
     */
    size_t start_;

    /**
     * @brief The logical offset of the start of the zero-compressed data area.
     */
    size_t zeroStart_;

    /**
     * @brief The logical offset of the end of the zero-compressed data area (on-byte-beyond).
     */
    size_t zeroEnd_;

    /**
     * @brief The logical offset of the end of the trailer area (on-byte-beyond).
     */
    size_t end_;

};


NSFX_CLOSE_NAMESPACE


#endif // ZC_BUFFER_DECLARE_H__81877B5A_6E6F_4095_B35B_4EC979BE6388

