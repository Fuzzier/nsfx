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
 *   throughput, end-to-end delay, etc.
 *
 * # Model
 *   The reporting and processing of statistics data are decoupled via callback.
 *   A statistic source reports data, and statistic sinks collect, process and
 *   present the data.
 *   The interface between the source and sink is called a *probe*.
 *   This name is adopted from OPNET Modeler.
 *
 * ## Probe
 *    A probe provides an event that reports data to sinks.
 *    A source can provide multiple probes, and each probe is identified by
 *    a name.
 *
 *    A probe provides `IProbeEvent` interface.
 *    A sink implements `IProbeEventSink`, and connects it to the event to
 *    collect the reported data.
 *
 *    For simplicity and unification, the type of the reported data is `double`.
 *
 * ## Source
 *    For generality, a source **shall** report *raw data*.
 *
 *    e.g., the number of packets sent, the number of bits received,
 *    the end-to-end delay of a single packet, etc.
 *    If a source reports processed data, e.g., mean delay, then it is hardly
 *    possible to calculate the mean delay of the whole network, since the
 *    processed data loses information.
 *
 * ## Sink
 *    The library provides a set of sinks for processing the probed data.
 *    These sinks are called *statistics*.
 *
 *    The common statistics are:
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


#define NSFX_STATISTICS_OPEN_NAMESPACE  \
    NSFX_OPEN_NAMESPACE                 \
    namespace statistics {


#define NSFX_STATISTICS_CLOSE_NAMESPACE  \
    } /* namespace statistics */         \
    NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__4908DD8B_0960_4EDF_A60C_F5367B7D8748

