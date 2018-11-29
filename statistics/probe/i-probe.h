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

#ifndef I_PROBE_H__C972BA8A_EE32_4BCE_A055_87306ECDE9E6
#define I_PROBE_H__C972BA8A_EE32_4BCE_A055_87306ECDE9E6


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/probe/i-probe-event.h>
#include <nsfx/component/i-user.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
class IProbe :
    public IProbeEvent
{
public:
    virtual ~IProbe(void) BOOST_NOEXCEPT {}

    // IProbeEvent
    virtual cookie_t Connect(Ptr<IProbeEventSink> sink) NSFX_OVERRIDE = 0;
    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE = 0;

    /**
     * @brief Get the name of the probe.
     */
    virtual std::string GetName(void) = 0;

};


NSFX_DEFINE_CLASS_UID(IProbe, "edu.uestc.nsfx.statistics.IProbe");


////////////////////////////////////////////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IProbeUser, "edu.uestc.nsfx.statistics.IProbeUser",
    IProbe);


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // I_PROBE_H__C972BA8A_EE32_4BCE_A055_87306ECDE9E6

