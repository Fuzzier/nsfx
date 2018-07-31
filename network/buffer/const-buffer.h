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

#ifndef CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32
#define CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/const-buffer-declare.h>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ConstBuffer.
inline ConstBuffer::ConstBuffer(const Buffer& buffer) :
    buffer_(buffer)
{
}

inline ConstBuffer::ConstBuffer(const ConstBuffer& rhs) BOOST_NOEXCEPT :
   buffer_(rhs.buffer_)
{
}

inline ConstBuffer& ConstBuffer::operator=(const ConstBuffer& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = rhs.buffer_;
    }
    return *this;
}

inline ConstBuffer::ConstBuffer(ConstBuffer&& rhs) BOOST_NOEXCEPT :
        buffer_(std::move(rhs.buffer_))
{
}

inline ConstBuffer& ConstBuffer::operator=(ConstBuffer&& rhs) BOOST_NOEXCEPT
{
    if (this != &rhs)
    {
        buffer_ = std::move(rhs.buffer_);
    }
    return *this;
}

inline size_t ConstBuffer::GetSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetSize();
}

inline size_t ConstBuffer::GetInternalSize(void) const BOOST_NOEXCEPT
{
    return buffer_.GetInternalSize();
}

inline size_t ConstBuffer::GetCapacity(void) const BOOST_NOEXCEPT
{
    return buffer_.GetCapacity();
}

inline size_t ConstBuffer::GetStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStart();
}

inline size_t ConstBuffer::GetZeroStart(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroStart();
}

inline size_t ConstBuffer::GetZeroEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetZeroEnd();
}

inline size_t ConstBuffer::GetEnd(void) const BOOST_NOEXCEPT
{
    return buffer_.GetEnd();
}

inline const BufferStorage* ConstBuffer::GetStorage(void) const BOOST_NOEXCEPT
{
    return buffer_.GetStorage();
}

inline size_t ConstBuffer::CopyTo(uint8_t* dst, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.CopyTo(dst, size);
}

inline ConstBuffer ConstBuffer::MakeFragment(size_t start, size_t size) const BOOST_NOEXCEPT
{
    return buffer_.MakeFragment(start, size);
}

inline ConstBuffer ConstBuffer::MakeRealBuffer(void) const BOOST_NOEXCEPT
{
    return buffer_.MakeRealBuffer();
}

inline void ConstBuffer::Realize(void) const BOOST_NOEXCEPT
{
    buffer_.Realize();
}

inline ConstBufferIterator ConstBuffer::begin(void) BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstBuffer::end(void) BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline ConstBufferIterator ConstBuffer::cbegin(void) const BOOST_NOEXCEPT
{
    return buffer_.cbegin();
}

inline ConstBufferIterator ConstBuffer::cend(void) const BOOST_NOEXCEPT
{
    return buffer_.cend();
}

inline void ConstBuffer::swap(ConstBuffer& rhs) BOOST_NOEXCEPT
{
    boost::swap(buffer_, rhs.buffer_);
}


////////////////////////////////////////////////////////////////////////////////
inline void swap(ConstBuffer& lhs, ConstBuffer& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // CONST_BUFFER_H__7944BA7C_20BF_40B5_92C5_05564EB81C32

