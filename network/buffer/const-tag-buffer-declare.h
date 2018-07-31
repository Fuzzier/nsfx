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

#ifndef CONST_TAG_BUFFER_DECLARE_H__59870F14_A793_4DAB_AF76_9653AB7F0746
#define CONST_TAG_BUFFER_DECLARE_H__59870F14_A793_4DAB_AF76_9653AB7F0746


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/tag-buffer-declare.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstTagBuffer.
/**
 * @ingroup Network
 * @brief A reference-counted, fixed-size and read-only buffer.
 */
class ConstTagBuffer
{
public:
    typedef ConstTagBufferIterator iterator;
    typedef ConstTagBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create a buffer.
     *
     * @param[in] buffer A tag buffer.
     *
     * A \c TagBuffer can be converted to \c ConstTagBuffer implicitly.
     */
    ConstTagBuffer(const TagBuffer& buffer);

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstTagBuffer(const ConstTagBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstTagBuffer& operator=(const ConstTagBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ConstTagBuffer(ConstTagBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ConstTagBuffer& operator=(ConstTagBuffer&& rhs) BOOST_NOEXCEPT;

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
    ConstTagBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ConstTagBufferIterator end(void) BOOST_NOEXCEPT;

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
    void swap(ConstTagBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The tag buffer.
     */
    TagBuffer buffer_;

};


NSFX_CLOSE_NAMESPACE


#endif // CONST_TAG_BUFFER_DECLARE_H__59870F14_A793_4DAB_AF76_9653AB7F0746

