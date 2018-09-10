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
 * A buffer iterator is not associated with a buffer.
 * It is associated with the memory space that is owned/shared by a buffer.
 *
 * ### Supported methods:
 * * copyable
 * * operator ++, --
 * * operator +, +=, -, -=
 * * operator >, >=, ==, !=, <=, <
 * * Write<T>(T data)
 * * WriteL<T>(T data)
 * * WriteB<T>(T data)
 * * T Read<T>()
 * * T ReadL<T>()
 * * T ReadB<T>()
 */
template<bool readOnly, bool zcAware>
class BasicBufferIterator;


NSFX_CLOSE_NAMESPACE


#endif // BASIC_BUFFER_ITERATOR_H__5069DAEA_1491_4DAC_85C6_BD8DF0359C83

