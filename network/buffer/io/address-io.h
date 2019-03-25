/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-25
 *
 * @copyright Copyright (c) 2019.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ADDRESS_IO_H__8EF61214_7874_4A25_90CE_29D327FF8027
#define ADDRESS_IO_H__8EF61214_7874_4A25_90CE_29D327FF8027


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/network/address.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Address
template<bool zcAware, size_t bits>
void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const Address<bits>& addr)
{
    addr.Write(it);
}

template<bool zcAware, size_t bits>
void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const Address<bits>& addr)
{
    addr.WriteL(it);
}

template<bool zcAware, size_t bits>
void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const Address<bits>& addr)
{
    addr.WriteB(it);
}

////////////////////////////////////////
template<bool readOnly, bool zcAware, size_t bits>
void Read(BasicBufferIterator<readOnly, zcAware>& it,
          Address<bits>* addr)
{
    BOOST_ASSERT(addr);
    addr->Read(it);
}

template<bool readOnly, bool zcAware, size_t bits>
void ReadL(BasicBufferIterator<readOnly, zcAware>& it,
           Address<bits>* addr)
{
    BOOST_ASSERT(addr);
    addr->ReadL(it);
}

template<bool readOnly, bool zcAware, size_t bits>
void ReadB(BasicBufferIterator<readOnly, zcAware>& it,
           Address<bits>* addr)
{
    BOOST_ASSERT(addr);
    addr->ReadB(it);
}


NSFX_CLOSE_NAMESPACE


#endif // ADDRESS_IO_H__8EF61214_7874_4A25_90CE_29D327FF8027

