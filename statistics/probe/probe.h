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

#ifndef PROBE_H__8B0EE0AE_C42E_46F0_AFA6_8C5446A1D7B0
#define PROBE_H__8B0EE0AE_C42E_46F0_AFA6_8C5446A1D7B0


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/probe/i-probe.h>
#include <nsfx/event/event.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A probe.
 *
 * A probe provides a probe event.
 */
class Probe :
    public IProbe
{
    typedef Probe ThisClass;

public:
    explicit Probe(const std::string& name);
    virtual ~Probe(void);

    // IProbe
    virtual cookie_t Connect(Ptr<IProbeEventSink> sink) NSFX_OVERRIDE;
    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE;
    virtual std::string GetName(void) NSFX_OVERRIDE;

public:
    /**
     * @brief Report a sample point.
     *
     * @param[in] data The sample point.
     */
    void Fire(double data);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IProbe)
        NSFX_INTERFACE_ENTRY(IProbeEvent)
    NSFX_INTERFACE_MAP_END()

private:
    std::string name_;
    MutualObject<Event<IProbeEvent>>  probeEvent_;
};


////////////////////////////////////////////////////////////////////////////////
inline Probe::Probe(const std::string& name) :
    name_(name),
    probeEvent_(/* controller = */this)
{
}

inline Probe::~Probe(void)
{
}

inline cookie_t Probe::Connect(Ptr<IProbeEventSink> sink)
{
    return probeEvent_.GetImpl()->Connect(sink);
}

inline void Probe::Disconnect(cookie_t cookie)
{
    probeEvent_.GetImpl()->Disconnect(cookie);
}

inline std::string Probe::GetName(void)
{
    return name_;
}

inline void Probe::Fire(double data)
{
    probeEvent_.GetImpl()->Fire(data);
}


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // PROBE_H__8B0EE0AE_C42E_46F0_AFA6_8C5446A1D7B0

