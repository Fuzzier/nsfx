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
#include <nsfx/statistics/probe/i-probe-event.h>
#include <nsfx/event/event.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A probe.
 *
 * A probe provides a probe event.
 *
 * # Interfaces
 * * Provides
 * * Events
 *   + `IProbeEvent`
 */
class Probe :
    virtual public IObject
{
    typedef Probe ThisClass;

public:
    Probe(void);
    virtual ~Probe(void);

public:
    /**
     * @brief Report a sample point.
     *
     * @param[in] value The sample point.
     */
    void Fire(double value);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IProbeEvent, &probeEvent_)
    NSFX_INTERFACE_MAP_END()

private:
    MemberAggObject<Event<IProbeEvent>>  probeEvent_;
};


////////////////////////////////////////////////////////////////////////////////
inline Probe::Probe(void) :
    probeEvent_(/* controller = */this)
{
}

inline Probe::~Probe(void)
{
}

inline void Probe::Fire(double data)
{
    probeEvent_.GetImpl()->Fire(data);
}


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // PROBE_H__8B0EE0AE_C42E_46F0_AFA6_8C5446A1D7B0

