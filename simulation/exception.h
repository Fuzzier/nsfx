/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-14
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__6222290A_60F2_4FC1_935E_D13DB50330F4
#define EXCEPTION_H__6222290A_60F2_4FC1_935E_D13DB50330F4


#include <nsfx/simulation/config.h>
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Event scheduler.
/**
 * @ingroup Simulator
 * @brief Cannot run a finished simulator.
 */
struct SimulatorFinished : ComponentException {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__6222290A_60F2_4FC1_935E_D13DB50330F4

