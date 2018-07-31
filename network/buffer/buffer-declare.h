/**
 * @file
 *
 * @brief Packet buffer for Network Simulation Frameworks.
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

#ifndef BUFFER_DECLARE_H__E075A13B_5E50_4382_9BCC_167FB919D6EE
#define BUFFER_DECLARE_H__E075A13B_5E50_4382_9BCC_167FB919D6EE


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-storage.h>
#include <nsfx/network/buffer/buffer-iterator.h>


NSFX_OPEN_NAMESPACE


class ConstBuffer;
class ConstZcBuffer;
class ConstTagBuffer;


////////////////////////////////////////////////////////////////////////////////
// Buffer.
/**
 * @ingroup Network
 * @brief An automatically resized and copy-on-write buffer.
 *
 * \c Buffer provides the same set of public interfaces as \c ZcBuffer.
 *
 * \c Buffer and \c ZcBuffer are designed as such that \c Packet can use
 * \c Buffer and \c ZcBuffer interchangeably without problem.
 *
 * In other situations, the interchangeability of \c Buffer and \c ZcBuffer is
 * <b>not</b> guaranteed.
 * The following issues shall be considered.
 * * For a \c Buffer, the zero-compressed area is always assumed to be empty,
 *   and located at the end of the buffer.
 * * The zero-compressed area in a \c ZcBuffer is not writable.
 */
class Buffer
{
public:
    typedef BufferIterator      iterator;
    typedef ConstBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    Buffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] capacity The initial capacity of the buffer.
     */
    explicit Buffer(size_t capacity);

    /**
     * @brief Create a buffer.
     *
     * @param[in] reserved The size of reserved space of the buffer in addition
     *                     to zero data.
     * @param[in] zeroSize The size of the zero data.
     *                     <p>
     *                     <code>zeroSize <= size</code>.
     *
     * The initial capacity of the buffer is <code>reserved + zeroSize</code>.
     *
     * The zero data is located at the end of the storage,
     * optimizing for adding data at the head of the storage.
     */
    Buffer(size_t reserved, size_t zeroSize);

    /**
     * @brief Create a buffer.
     *
     * @param[in] reserved  The size of reserved space of the buffer in addition
     *                      to zero data.
     * @param[in] zeroStart The start of the zero data.
     *                      <p>
     *                      <code>zeroStart <= reserved</code>.
     * @param[in] zeroSize  The size of the zero data.
     *
     * The initial capacity of the buffer is <code>reserved + zeroSize</code>.
     */
    Buffer(size_t reserved, size_t zeroStart, size_t zeroSize);

    // Conversions.
public:
    /**
     * @brief Deep copy from a ConstBuffer.
     */
    explicit Buffer(const ConstBuffer& rhs);

    /**
     * @brief Deep copy from a ConstZcBuffer.
     */
    explicit Buffer(const ConstZcBuffer& rhs);

    /**
     * @brief Deep copy from a ConstTagBuffer.
     */
    explicit Buffer(const ConstTagBuffer& rhs);

private:
    /**
     * @brief Create a buffer.
     *
     * @param[in] storage  The reference count is taken by the buffer.
     *
     * @see \c MakeFragment().
     * @internal
     */
    Buffer(BufferStorage* storage, size_t start, size_t end) BOOST_NOEXCEPT;

public:
    ~Buffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    Buffer(const Buffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    Buffer& operator=(const Buffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    Buffer(Buffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    Buffer& operator=(Buffer&& rhs) BOOST_NOEXCEPT;

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
     * @remarks Potentially invalidates existing iterators of the buffer.
     */
    void AddAtStart(const uint8_t* src, size_t size);

    /**
     * @brief Expand the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtStart(const ConstBuffer& src);

private:
    void InternalAddAtStart(size_t size, AdjustOffsetTag) BOOST_NOEXCEPT;
    void InternalAddAtStart(size_t size, size_t newCapacity,
                            size_t newStart, size_t dataSize, ReallocateTag);
    void InternalAddAtStart(size_t size, size_t dataSize, MoveMemoryTag) BOOST_NOEXCEPT;

public:
    /**
     * @brief Expand the data area toward the end of the buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(size_t size);

    /**
     * @brief Expand the buffer and copy the specified contents.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const uint8_t* src, size_t size);

    /**
     * @brief Expand the buffer and copy the contents from the specified buffer.
     *
     * @remarks Invalidates existing iterators of the buffer.
     */
    void AddAtEnd(const ConstBuffer& src);

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
    Buffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    Buffer MakeRealBuffer(void) const BOOST_NOEXCEPT;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const BOOST_NOEXCEPT;

private:
    Buffer InternalGetRealBuffer(ReallocateTag) const BOOST_NOEXCEPT;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    BufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    BufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(Buffer& rhs) BOOST_NOEXCEPT;

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
     * @brief The logical offset of the end of the trailer area (on-byte-beyond).
     */
    size_t end_;

};


NSFX_CLOSE_NAMESPACE


#endif // BUFFER_DECLARE_H__E075A13B_5E50_4382_9BCC_167FB919D6EE

