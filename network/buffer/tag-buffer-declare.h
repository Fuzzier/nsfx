/**
 * @file
 *
 * @brief Tag buffer for Network Simulation Frameworks.
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

#ifndef TAG_BUFFER_DECLARE_H__2B65398A_6284_4354_8981_BF469B7AAE3F
#define TAG_BUFFER_DECLARE_H__2B65398A_6284_4354_8981_BF469B7AAE3F


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/tag-buffer-storage.h>
#include <nsfx/network/buffer/buffer-iterator.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
typedef BufferIterator       TagBufferIterator;
typedef ConstBufferIterator  ConstTagBufferIterator;

class ConstBuffer;
class ConstZcBuffer;
class ConstTagBuffer;


////////////////////////////////////////////////////////////////////////////////
// TagBuffer.
/**
 * @ingroup Network
 * @brief A reference-counted fixed-size buffer.
 */
class TagBuffer
{
public:
    typedef TagBufferIterator      iterator;
    typedef ConstTagBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create an empty buffer.
     */
    TagBuffer(void) BOOST_NOEXCEPT;

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     */
    explicit TagBuffer(size_t size);

    /**
     * @brief Create a buffer.
     *
     * @param[in] size The size of the buffer.
     * @param[in] zero Initialize the buffer to zeros.
     */
    TagBuffer(size_t size, bool zeroInit);

    // Conversions.
public:
    /**
     * @brief Deep copy from a ConstBuffer.
     */
    explicit TagBuffer(const ConstBuffer& rhs);

    /**
     * @brief Deep copy from a ConstZcBuffer.
     */
    explicit TagBuffer(const ConstZcBuffer& rhs);

    /**
     * @brief Deep copy from a ConstTagBuffer.
     */
    explicit TagBuffer(const ConstTagBuffer& rhs);

public:
    ~TagBuffer(void) BOOST_NOEXCEPT;

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    TagBuffer(const TagBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    TagBuffer& operator=(const TagBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    TagBuffer(TagBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    TagBuffer& operator=(TagBuffer&& rhs) BOOST_NOEXCEPT;

private:
    void TagBuffer::Acquire(void) BOOST_NOEXCEPT;
    void TagBuffer::Release(void) BOOST_NOEXCEPT;

    // Methods.
public:
    /**
     * @brief Get the size of the represented data.
     */
    size_t GetSize(void) const BOOST_NOEXCEPT;

    const TagBufferStorage* GetStorage(void) const BOOST_NOEXCEPT;

    /**
     * @brief Copy data to a memory block.
     * @return The number of bytes copied.
     */
    size_t CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    TagBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    TagBufferIterator end(void) BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points to the first byte of the data.
     */
    ConstTagBufferIterator cbegin(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get a const iterator that points one byte after the last byte of the data area.
     */
    ConstTagBufferIterator cend(void) const BOOST_NOEXCEPT;

    // Swappable.
public:
    void swap(TagBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The storage.
     */
    TagBufferStorage* storage_;

};


NSFX_CLOSE_NAMESPACE


#endif // TAG_BUFFER_DECLARE_H__2B65398A_6284_4354_8981_BF469B7AAE3F

