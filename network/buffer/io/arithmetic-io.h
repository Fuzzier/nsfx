/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-04-04
 *
 * @copyright Copyright (c) 2019.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ARITHMETIC_IO_H__94A4E982_8239_4E2A_8E62_185B1DFC4D35
#define ARITHMETIC_IO_H__94A4E982_8239_4E2A_8E62_185B1DFC4D35


#include <nsfx/network/config.h>
#include <nsfx/network/buffer/iterator/basic-buffer-iterator.h>
#include <type_traits> // enable_if, is_arithmetic


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Address
template<class T, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
Write(BasicBufferIterator</*readOnly*/false, zcAware>& it, T v)
{
    it.template Write<T>(v);
}

template<class T, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
WriteL(BasicBufferIterator</*readOnly*/false, zcAware>& it, T v)
{
    it.template WriteL<T>(v);
}

template<class T, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
WriteB(BasicBufferIterator</*readOnly*/false, zcAware>& it, T v)
{
    it.template WriteB<T>(v);
}

////////////////////////////////////////
template<class T, bool readOnly, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
Read(BasicBufferIterator<readOnly, zcAware>& it, T* v)
{
    BOOST_ASSERT(v);
    *v = it.template Read<T>();
}

template<class T, bool readOnly, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
ReadL(BasicBufferIterator<readOnly, zcAware>& it, T* v)
{
    BOOST_ASSERT(v);
    *v = it.template ReadL<T>();
}

template<class T, bool readOnly, bool zcAware>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
ReadB(BasicBufferIterator<readOnly, zcAware>& it, T* v)
{
    BOOST_ASSERT(v);
    *v = it.template ReadB<T>();
}


NSFX_CLOSE_NAMESPACE


#endif // ARITHMETIC_IO_H__94A4E982_8239_4E2A_8E62_185B1DFC4D35

