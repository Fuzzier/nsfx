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
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__4908DD8B_0960_4EDF_A60C_F5367B7D8748
#define CONFIG_H__4908DD8B_0960_4EDF_A60C_F5367B7D8748


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Statistics
 *
 * @brief Statistics support.
 *
 * # Concept
 *   Statistics is the study of data, such as collection, modeling,
 *   interpretation and prediction of the data.
 *   For example, the performance of a network is usually interpretted by load,
 *   throughput, delay, etc.
 *
 * # Model
 *   The reporting and processing of data are decoupled via the event model.
 *   A statistic source reports data, and statistic sinks collect, process and
 *   present the data.
 *   The interface between the source and sink is called a *statistic*.
 *
 * ## Statistic
 *    A statistic source fires events to report data to the sinks.
 *    A source can provide multiple statistics, and each statistic is
 *    identified by a name.
 *
 *    A statistic is associated with an event interface.
 *    A sink implements the corresponding event sink interface, and connects to
 *    the event to collect the reported data.
 *
 *    A statistic can report any type and number of data to the collectors.
 *
 * ## Source
 *    For generality, a source **shall** report *raw data*.
 *
 *    e.g., the number of packets sent, the number of bits received, the delay
 *    of a single packet, etc.
 *
 *    If a source reports processed data, e.g., mean delay, then it is hardly
 *    possible to calculate the mean delay of the whole network, since the
 *    processing of data can cause lost of information.
 *
 *    The statistic is **not** modeled as the *probe* of OPNET Modeler, since a *probe*
 *    can only report a single `double` value, and it is cumbersome to report
 *    *grouped* data and *parameterized* data where multiple data **shall** be reported
 *    together to be meaningful.
 *
 *    For example, a piece of data may contain the x, y, z coordinates, or
 *    it may be associated with a priority.
 *    A single-value *probe* is far from a generic model.
 *
 * ## Summary
 *    The library provides `Summary` object that has a primary data processing
 *    capability.
 *
 *    The common types of data processing are:
 *    * count
 *    * sum
 *    * min
 *    * max
 *    * sample mean
 *    * time average
 *    * sum/time
 *
 *    The statistics also provides various collection methods:
 *    * bucket-based
 *    * reset-able
 */


#endif // CONFIG_H__4908DD8B_0960_4EDF_A60C_F5367B7D8748

