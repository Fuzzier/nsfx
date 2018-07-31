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

#ifndef CONST_BUFFER_DECLARE_H__D039DBD3_8499_464B_80E5_5B8886B827F7
#define CONST_BUFFER_DECLARE_H__D039DBD3_8499_464B_80E5_5B8886B827F7


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/buffer-declare.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstBuffer.
/**
 * @ingroup Network
 * @brief A read-only buffer.
 */
class ConstBuffer
{
public:
    typedef ConstBufferIterator iterator;
    typedef ConstBufferIterator const_iterator;

    // Xtructors.
public:
    /**
     * @brief Create a buffer.
     *
     * @param[in] buffer A buffer.
     *
     * A \c Buffer can be converted implicitly to a \c ConstBuffer.
     */
    ConstBuffer(const Buffer& buffer);

    // Copyable.
public:
    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstBuffer(const ConstBuffer& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Make a shallow copy of the buffer.
     */
    ConstBuffer& operator=(const ConstBuffer& rhs) BOOST_NOEXCEPT;

    // Movable.
public:
    /**
     * @brief Move a buffer.
     */
    ConstBuffer(ConstBuffer&& rhs) BOOST_NOEXCEPT;

    /**
     * @brief Move a buffer.
     */
    ConstBuffer& operator=(ConstBuffer&& rhs) BOOST_NOEXCEPT;

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

    // Fragmentation.
public:
    /**
     * @brief Make a fragment of the buffer.
     *
     * @param[in] start The start of the fragment.
     * @param[in] size  The size of the fragment.
     */
    ConstBuffer MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT;

    // Decompression.
public:
    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    ConstBuffer MakeRealBuffer(void) const BOOST_NOEXCEPT;

    /**
     * @brief Expand the zero-compressed data as part of the header.
     */
    void Realize(void) const BOOST_NOEXCEPT;

    // Iterator.
public:
    /**
     * @brief Get an iterator that points to the first byte of the data.
     */
    ConstBufferIterator begin(void) BOOST_NOEXCEPT;

    /**
     * @brief Get an iterator that points one byte after the last byte of the data area.
     */
    ConstBufferIterator end(void) BOOST_NOEXCEPT;

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
    void swap(ConstBuffer& rhs) BOOST_NOEXCEPT;

    // Properties.
private:
    /**
     * @brief The buffer.
     */
    Buffer buffer_;

};


NSFX_CLOSE_NAMESPACE


#endif // CONST_BUFFER_DECLARE_H__D039DBD3_8499_464B_80E5_5B8886B827F7

