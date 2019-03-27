/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-22
 *
 * @copyright Copyright (c) 2019.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef DURATION_IO_H__ACC3CA04_1EDF_4492_A20B_1E1EFAC9CA1D
#define DURATION_IO_H__ACC3CA04_1EDF_4492_A20B_1E1EFAC9CA1D


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/chrono/duration.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Duration
template<bool zcAware, class Res>
void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const chrono::Duration<Res>& dt)
{
    it.Write<chrono::count_t>(dt.GetCount());
}

template<bool zcAware, class Res>
void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const chrono::Duration<Res>& dt)
{
    it.WriteL<chrono::count_t>(dt.GetCount());
}

template<bool zcAware, class Res>
void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const chrono::Duration<Res>& dt)
{
    it.WriteB<chrono::count_t>(dt.GetCount());
}

////////////////////////////////////////
template<bool readOnly, bool zcAware, class Res>
void Read(BasicBufferIterator<readOnly, zcAware>& it,
          chrono::Duration<Res>* dt)
{
    BOOST_ASSERT(dt);
    chrono::count_t count = it.Read<chrono::count_t>();
    *dt = chrono::Duration<Res>(count);
}

template<bool readOnly, bool zcAware, class Res>
void ReadL(BasicBufferIterator<readOnly, zcAware>& it,
           chrono::Duration<Res>* dt)
{
    BOOST_ASSERT(dt);
    chrono::count_t count = it.ReadL<chrono::count_t>();
    *dt = chrono::Duration<Res>(count);
}

template<bool readOnly, bool zcAware, class Res>
void ReadB(BasicBufferIterator<readOnly, zcAware>& it,
           chrono::Duration<Res>* dt)
{
    BOOST_ASSERT(dt);
    chrono::count_t count = it.ReadB<chrono::count_t>();
    *dt = chrono::Duration<Res>(count);
}


NSFX_CLOSE_NAMESPACE


#endif // DURATION_IO_H__ACC3CA04_1EDF_4492_A20B_1E1EFAC9CA1D

