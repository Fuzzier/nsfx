/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5
#define CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5


#include <nsfx/config.h>


/**
 * @defgroup Simulator
 * @brief The network simulator.
 *
 * Users can define their own \c Duration and \c TimePoint types in the global
 * namespace or in the \c nsfx namespace, and define macro
 * \c NSFX_SIMULATOR_CHRONO_DEFINED before including simulator headers.<br/>
 * The simulator library will use the user-defined types.
 */


////////////////////////////////////////////////////////////////////////////////
// Chrono definitions.
#if !defined(NSFX_SIMULATOR_CHRONO_DEFINED)
# include <nsfx/chrono/duration.h>
# include <nsfx/chrono/virtual-time-point.h>
# define NSFX_SIMULATOR_CHRONO_DEFINED
  typedef chrono::Duration          Duration;
  typedef chrono::VirtualTimePoint  TimePoint;
#else // if defined(NSFX_SIMULATOR_CHRONO_DEFINED)
BOOST_CONCEPT_ASSERT((TimePointConcept<TimePoint>));
#endif // !defined(NSFX_SIMULATOR_CHRONO_DEFINED)


#endif // CONFIG_H__CC642C06_21DD_48A9_92CD_4C753FCDE2C5

