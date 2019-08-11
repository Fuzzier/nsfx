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

#ifndef I_DATA_SOURCE_H__9979C39C_C242_47D8_A805_A368BE9266D6
#define I_DATA_SOURCE_H__9979C39C_C242_47D8_A805_A368BE9266D6


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/probe/i-probe-event.h>
#include <nsfx/statistics/probe/i-probe-enumerator.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A container of probes.
 */
class IProbeContainer :
    virtual public IObject
{
public:
    virtual ~IProbeContainer(void) BOOST_NOEXCEPT {}

    /**
     * @brief Get an enumerator.
     */
    virtual Ptr<IProbeEnumerator> GetEnumerator(void) = 0;

    /**
     * @brief Get a probe.
     *
     * @param[in] name The name of the probe.
     *
     * @throw ProbeNotRegistered
     */
    virtual Ptr<IProbeEvent> GetProbe(const std::string& name) = 0;

};


NSFX_DEFINE_CLASS_UID(IProbeContainer,
                      "edu.uestc.nsfx.statistics.IProbeContainer");


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // I_DATA_SOURCE_H__9979C39C_C242_47D8_A805_A368BE9266D6

