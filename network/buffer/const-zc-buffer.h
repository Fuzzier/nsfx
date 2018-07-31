/**
 * @file
 *
 * @brief Zero-compressed packet buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-08
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83
#define CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/const-zc-buffer-declare.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstZcBuffer.
inline ConstZcBuffer::ConstZcBuffer(const ZcBuffer& buffer) BOOST_NOEXCEPT :
    buffer_(buffer)
{
}

inline ConstZcBuffer::ConstZcBuffer(const ConstZcBuffer& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstZcBuffer& ConstZcBuffer::operator=(const ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstZcBuffer::ConstZcBuffer(ConstZcBuffer&& rhs) BOOST_NOEXCEPT :
    buffer_(rhs.buffer_)
{
}

inline ConstZcBuffer& ConstZcBuffer::operator=(ConstZcBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstZcBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline size_t ConstZcBuffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetInternalSize();
}

inline size_t ConstZcBuffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return buffer_.GetCapacity();
}

inline size_t ConstZcBuffer::GetStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStart();
}

inline size_t ConstZcBuffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroStart();
}

inline size_t ConstZcBuffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroEnd();
}

inline size_t ConstZcBuffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetEnd();
}

inline const BufferStorage* ConstZcBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstZcBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstZcBuffer ConstZcBuffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.MakeFragment(start, size);
}

inline ConstZcBuffer ConstZcBuffer::MakeRealBuffer(void) const
{
    return buffer_.MakeRealBuffer();
}

inline void ConstZcBuffer::Realize(void) const
{
    buffer_.Realize();
}

inline ConstZcBufferIterator ConstZcBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstZcBufferIterator ConstZcBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstZcBufferIterator ConstZcBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstZcBufferIterator ConstZcBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstZcBuffer::swap(ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    boost::swap(buffer_, rhs.buffer_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstZcBuffer& lhs, ConstZcBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_ZC_BUFFER_H__DC7C0CA7_7AB5_4BBF_A16C_5F6516BF1E83

