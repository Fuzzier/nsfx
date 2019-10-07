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

#ifndef TIME_POINT_IO_H__BEEF741E_E8AF_4021_AE1B_6BA1D0E0FD0E
#define TIME_POINT_IO_H__BEEF741E_E8AF_4021_AE1B_6BA1D0E0FD0E


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <nsfx/chrono/time-point.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// TimePoint
template<bool zcAware, class Clock, class Duration>
void Write(BasicBufferIterator</*readOnly*/false, zcAware>& it,
           const chrono::TimePoint<Clock, Duration>& t0)
{
    it.template Write<chrono::count_t>(t0.GetDuration().GetCount());
}

template<bool zcAware, class Clock, class Duration>
void WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const chrono::TimePoint<Clock, Duration>& t0)
{
    it.template WriteL<chrono::count_t>(t0.GetDuration().GetCount());
}

template<bool zcAware, class Clock, class Duration>
void WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it,
            const chrono::TimePoint<Clock, Duration>& t0)
{
    it.template WriteB<chrono::count_t>(t0.GetDuration().GetCount());
}

////////////////////////////////////////
template<bool readOnly, bool zcAware, class Clock, class Duration>
void Read(BasicBufferIterator<readOnly, zcAware>& it,
          chrono::TimePoint<Clock, Duration>* t0)
{
    BOOST_ASSERT(t0);
    chrono::count_t count = it.template Read<chrono::count_t>();
    *t0 = chrono::TimePoint<Clock, Duration>(Duration(count));
}

template<bool readOnly, bool zcAware, class Clock, class Duration>
void ReadL(BasicBufferIterator<readOnly, zcAware>& it,
           chrono::TimePoint<Clock, Duration>* t0)
{
    BOOST_ASSERT(t0);
    chrono::count_t count = it.template ReadL<chrono::count_t>();
    *t0 = chrono::TimePoint<Clock, Duration>(Duration(count));
}

template<bool readOnly, bool zcAware, class Clock, class Duration>
void ReadB(BasicBufferIterator<readOnly, zcAware>& it,
           chrono::TimePoint<Clock, Duration>* t0)
{
    BOOST_ASSERT(t0);
    chrono::count_t count = it.template ReadB<chrono::count_t>();
    *t0 = chrono::TimePoint<Clock, Duration>(Duration(count));
}


NSFX_CLOSE_NAMESPACE


#endif // TIME_POINT_IO_H__BEEF741E_E8AF_4021_AE1B_6BA1D0E0FD0E

