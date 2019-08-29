/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-14
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__8DAD42E7_C0A1_4C12_81D1_D9808047D683
#define CONFIG_H__8DAD42E7_C0A1_4C12_81D1_D9808047D683


#include <nsfx/config.h>


/**
 * @defgroup Event
 * @brief Support for events.
 *
 * Depends upon Exception and Component modules.
 *
 */


NSFX_OPEN_NAMESPACE


/**
 * @ingroup Event
 * @brief A cookie value that represents an event connection.
 *
 * A cookie is returned by an event source when an event sink is connected.
 * It can be used to disconnect the event sink from the event source.
 *
 * A cookie value of \c 0 represents no connection.
 * A cookie <b>shall</b> be initialized to \c 0.
 */
typedef uint32_t cookie_t;


NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__8DAD42E7_C0A1_4C12_81D1_D9808047D683

