/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__4DB73FF2_5428_461D_B64F_52242E8744BB
#define EXCEPTION_H__4DB73FF2_5428_461D_B64F_52242E8744BB


#include <nsfx/statistics/config.h>
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Probe registration.
/**
 * @ingroup Statistics
 * @brief Cannot obtain the probe since it is not registered.
 */
struct ProbeNotRegistered : ComponentException {};

/**
 * @ingroup Statistics
 * @brief Cannot register a probe that is already registered.
 */
struct ProbeAlreadyRegistered : ComponentException {};


////////////////////////////////////////////////////////////////////////////////
typedef boost::error_info<struct tag_ProbeName, std::string>  ProbeNameErrorInfo;


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__4DB73FF2_5428_461D_B64F_52242E8744BB

