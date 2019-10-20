/**
 * @file
 *
 * @brief Buffer for Network Simulation Frameworks.
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

#ifndef BASIC_BUFFER_ITERATOR_H__5069DAEA_1491_4DAC_85C6_BD8DF0359C83
#define BASIC_BUFFER_ITERATOR_H__5069DAEA_1491_4DAC_85C6_BD8DF0359C83


#include <nsfx/network/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// BasicBufferIterator.
/**
 * @ingroup Network
 * @brief The iterator for accessing buffer data.
 *
 * @tparam readOnly The iterator provides read-only operations.
 * @tparam zcAware  The iterator operates on a zero-compressed buffer.
 *
 * # Design
 *
 * The buffer iterator is designed as a class template, so it is easier to
 * write iterator-based funtion templates.
 *
 * Iterators is only able to provide read/write methods for fundamental data
 * types (integers and floating points).
 * For user-defined data types, users have to provide their own methods.
 *
 * One approach is intrusive, i.e., define member funtions.
 * e.g.,
 * @code{.cpp}
 * class MyData
 * {
 * public:
 *   template<bool readOnly, bool zcAware>
 *   MyData(BasicBufferIterator<readOnly, zcAware>& iterator);
 *
 *   template<bool readOnly, bool zcAware>
 *   Read(BasicBufferIterator<readOnly, zcAware>& iterator);
 *
 *   template<bool readOnly, bool zcAware>
 *   Write(BasicBufferIterator<readOnly, zcAware>& iterator) const;
 * };
 * @endcode
 *
 * The other way is non-intrusive, i.e., define free functions.
 * e.g.,
 * @code{.cpp}
 * class MyData { ... };
 * template<class MyData, bool readOnly, bool zcAware>
 * MyData Read(BasicBufferIterator<readOnly, zcAware> &iterator);
 *
 * class MyData { ... };
 * template<bool readOnly, bool zcAware>
 * Write(BasicBufferIterator<readOnly, zcAware>& iterator, const MyData& data);
 * @endcode
 *
 * A buffer iterator is not associated with a buffer.
 * It is associated with the memory space that is owned/shared by a buffer.
 *
 * # Supported operations
 * * copyable
 *
 * * operator ++, --
 * * operator +, +=, -, -=
 * * operator >, >=, ==, !=, <=, <
 *
 * * Write<T>(T data)
 * * WriteL<T>(T data)
 * * WriteB<T>(T data)
 *
 * * Write<T>(const uint8_t* data, size_t n)
 * * WriteL<T>(const uint8_t* data, size_t n)
 * * WriteB<T>(const uint8_t* data, size_t n)
 *
 * * Fill(uint8_t v, size_t n)
 *
 * * T Read<T>()
 * * T ReadL<T>()
 * * T ReadB<T>()
 *
 * * Read<T>(uint8_t* data, size_t n)
 * * ReadL<T>(uint8_t* data, size_t n)
 * * ReadB<T>(uint8_t* data, size_t n)
 */
template<bool readOnly, bool zcAware>
class BasicBufferIterator;


NSFX_CLOSE_NAMESPACE


#endif // BASIC_BUFFER_ITERATOR_H__5069DAEA_1491_4DAC_85C6_BD8DF0359C83

