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

#ifndef CONST_TAG_BUFFER_H__AF7095C6_5F6C_4DA3_B044_9EAA604FE308
#define CONST_TAG_BUFFER_H__AF7095C6_5F6C_4DA3_B044_9EAA604FE308


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/const-tag-buffer-declare.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstTagBuffer.
inline ConstTagBuffer::ConstTagBuffer(const TagBuffer& buffer) :
    buffer_(buffer)
{
}

inline ConstTagBuffer::ConstTagBuffer(const ConstTagBuffer& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstTagBuffer& ConstTagBuffer::operator=(const ConstTagBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstTagBuffer::ConstTagBuffer(ConstTagBuffer&& rhs) BOOST_NOEXCEPT :
    buffer_(std::move(rhs.buffer_))
{
}

inline ConstTagBuffer& ConstTagBuffer::operator=(ConstTagBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstTagBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline const TagBufferStorage* ConstTagBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstTagBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstTagBufferIterator ConstTagBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstTagBufferIterator ConstTagBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstTagBufferIterator ConstTagBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstTagBufferIterator ConstTagBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstTagBuffer::swap(ConstTagBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        boost::swap(buffer_, rhs.buffer_);
    }
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstTagBuffer& lhs, ConstTagBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_TAG_BUFFER_H__AF7095C6_5F6C_4DA3_B044_9EAA604FE308

