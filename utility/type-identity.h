/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-06-03
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TYPE_IDENTITY_H__548E7EBC_4231_49C5_9804_03524DAF2EB6
#define TYPE_IDENTITY_H__548E7EBC_4231_49C5_9804_03524DAF2EB6


#include <nsfx/utility/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
template<class T>
struct type_identity
{
    typedef T type;
};


NSFX_CLOSE_NAMESPACE


#endif // TYPE_IDENTITY_H__548E7EBC_4231_49C5_9804_03524DAF2EB6

