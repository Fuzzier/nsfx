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


NSFX_OPEN_NAMESPACE


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

    /**
     * @brief Connect a sink to a probe.
     *
     * @param[in] name The name of the probe.
     * @param[in] sink The sink of the probe.
     *
     * @throw StatisticsNotRegistered
     * @throw NoInterface
     *
     * @return A cookie that can be used to disconnect.
     */
    virtual cookie_t Connect(const std::string& name, Ptr<IProbeEventSink> sink) = 0;

    /**
     * @brief Disconnect a sink from a probe.
     *
     * @param[in] name The name of the probe.
     * @param[in] sink The cookie.
     *
     * @throw StatisticsNotRegistered
     */
    virtual void Disconnect(const std::string& name, cookie_t cookie) = 0;

};


NSFX_DEFINE_CLASS_UID(IProbeContainer, "edu.uestc.nsfx.IProbeContainer");


NSFX_CLOSE_NAMESPACE


#endif // I_DATA_SOURCE_H__9979C39C_C242_47D8_A805_A368BE9266D6

