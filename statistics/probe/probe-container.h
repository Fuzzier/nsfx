/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PROBE_CONTAINER_H__6B28DFC9_2116_4EFE_8076_E8E1CE07D8A8
#define PROBE_CONTAINER_H__6B28DFC9_2116_4EFE_8076_E8E1CE07D8A8


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/probe/i-probe-container.h>
#include <nsfx/statistics/probe/probe.h>
#include <nsfx/statistics/probe/probe-enumerator.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A container of probes.
 */
class ProbeContainer :
    public IProbeContainer
{
	typedef ProbeContainer ThisClass;

public:
    ProbeContainer(void);
    virtual ~ProbeContainer(void);

    // IProbeContainer
    virtual Ptr<IProbeEnumerator> GetEnumerator(void) NSFX_OVERRIDE;
    virtual Ptr<IProbe> GetProbe(const std::string& name) NSFX_OVERRIDE;

public:
    bool Has(const std::string& name) const;
    Ptr<Probe> Get(const std::string& name) const;
    Ptr<Probe> Insert(const std::string& name);
    void Remove(const std::string& name);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IProbeContainer)
    NSFX_INTERFACE_MAP_END()

private:
    unordered_map<std::string, Ptr<Probe>>  items_;

};


////////////////////////////////////////////////////////////////////////////////
inline ProbeContainer::ProbeContainer(void)
{
}

inline ProbeContainer::~ProbeContainer(void)
{
}

inline Ptr<IProbeEnumerator> ProbeContainer::GetEnumerator(void)
{
    vector<std::string> names;
    for (auto it = items_.cbegin(); it != items_.cend(); ++it)
    {
        names.push_back(it->first);
    }
    return Ptr<IProbeEnumerator>(new Object<ProbeEnumerator>(std::move(names)));
}

inline Ptr<IProbe> ProbeContainer::GetProbe(const std::string& name)
{
    auto it = items_.find(name);
    if (it == items_.cend())
    {
        BOOST_THROW_EXCEPTION(
            ProbeNotRegistered() <<
            ProbeNameErrorInfo(name));
    }
    return it->second;
}

inline bool ProbeContainer::Has(const std::string& name) const
{
    return !!items_.count(name);
}

inline Ptr<Probe> ProbeContainer::Get(const std::string& name) const
{
    auto it = items_.find(name);
    if (it == items_.cend())
    {
        BOOST_THROW_EXCEPTION(
            ProbeNotRegistered() <<
            ProbeNameErrorInfo(name));
    }
    return it->second;
}

inline Ptr<Probe> ProbeContainer::Insert(const std::string& name)
{
    auto result = items_.emplace(name, nullptr);
    if (!result.second)
    {
        BOOST_THROW_EXCEPTION(
            ProbeAlreadyRegistered() <<
            ProbeNameErrorInfo(name));
    }
    auto it = result.first;
    it->second.Reset(new Object<Probe>(name));
    return it->second;
}

inline void ProbeContainer::Remove(const std::string& name)
{
    items_.erase(name);
}


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // PROBE_CONTAINER_H__6B28DFC9_2116_4EFE_8076_E8E1CE07D8A8

