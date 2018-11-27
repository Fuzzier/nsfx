/**
 * @file
 *
 * @brief Chrono support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-03-14
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__8B760702_A321_4446_BC5D_DABB064B77BB
#define CONFIG_H__8B760702_A321_4446_BC5D_DABB064B77BB


#include <nsfx/config.h>
#include <boost/ratio.hpp>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Chrono
 * @brief Support for time point and duration.
 *
 * Basic concepts:
 * <b>Time Point</b>: </br>
 *    A location in time. e.g., 2013-03-14 10:33:29.
 *    Also known as a Time Instant.
 *
 * <b>Time Duration</b>: </br>
 *    A free length of time which is not associated with a time point.
 *    e.g., 3 seconds.
 *    Also known as a Time Period.
 *
 * <b>Time Interval</b>: </br>
 *    A length of time between two time points.
 *    e.g., from 2013-03-14 10:33:29 to 2013-04-28 09:00:00.
 *
 * <b>Time Resolution</b>: </br>
 *    The smallest representable time duration.
 *    It is also called the fundamental time period.
 *
 * <b>Time Epoch</b>: </br>
 *    The referred starting time point.
 *    A time point is represented by the duration from the epoch.
 *    e.g. suppose the epoch is 1970-01-01 00:00:00 GMT, then the time point at
 *    1970-01-01 00:00:01 GMT can be represented by a duration of 1 second.
 *
 * <b>Time System</b>: </br>
 *    Provides all of the above time types, and rules and calculations between
 *    them.
 *    e.g., different time systems provide different time resolutions,
 *          and different time epoch.
 *    e.g., in the time system provided in the standard library,
 *          time() function returns a time point represents the
 *          current time. It has a time resolution of 1 second, and
 *          the earliest time point it can represent is 1970-01-01 00:00:00 GMT.
 *    e.g., Microsoft Windows provide time system based around
 *          FILETIME type. GetSystemTimeAsFileTime() function returns
 *          a time point prepresents the current time. It has a time
 *          resolution of 100 nanoseconds, and the earliest time point
 *          it can represent is 1601-01-01 00:00:00 UTC.
 *
 * <b>Clock Device</b>: </br>
 *    A software component that provides the current date or time with respect
 *    to a time system.
 */


////////////////////////////////////////////////////////////////////////////////
#define NSFX_CHRONO_OPEN_NAMESPACE  \
            NSFX_OPEN_NAMESPACE     \
            namespace chrono {


#define NSFX_CHRONO_CLOSE_NAMESPACE   \
            } /* namespace chrono */  \
            NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
NSFX_CHRONO_OPEN_NAMESPACE

typedef int64_t  count_t;

NSFX_CHRONO_CLOSE_NAMESPACE


#endif // CONFIG_H__8B760702_A321_4446_BC5D_DABB064B77BB

