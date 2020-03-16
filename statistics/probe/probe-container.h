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


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A container of probes.
 *
 * # Interfaces
 * * Provides
 *   + `IProbeContainer`
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
    virtual Ptr<IProbeEvent> GetProbe(const std::string& name) NSFX_OVERRIDE;

public:
    Ptr<Probe> Add(const std::string& name);
    bool Has(const std::string& name) const;

    /**
     * @brief Merge probes from a probe enumerator.
     *
     * @param[in] prefix The prefix to the names of the merged probes.
     * @param[in] merged The probe container to be merged.
     *
     * The current set of probes in the `merged` is merged.
     * If the `merged` is changed, there is no guarantee that the this
     * container would change accordingly.
     *
     * If the name already exists, the existing probe is **not** replaced.
     */
    void MergeFrom(const std::string& prefix, Ptr<IProbeContainer> merged);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IProbeContainer)
    NSFX_INTERFACE_MAP_END()

private:
    typedef unordered_map<std::string, Ptr<IProbeEvent>>  ContainerType;
    ContainerType  items_;

private:
    // KeyIterator./*{{{*/
public:
    class KeyIterator
    {
        friend class ProbeContainer;

    public:
        typedef ProbeContainer::ContainerType             ContainerType;
        typedef ContainerType::const_iterator             BaseIteratorType;
        typedef BaseIteratorType::difference_type         difference_type;
        typedef BaseIteratorType::value_type::first_type  value_type;
        typedef BaseIteratorType::value_type::first_type* pointer;
        typedef BaseIteratorType::value_type::first_type& reference;
        typedef BaseIteratorType::iterator_category       iterator_category;

    private:
        explicit KeyIterator(BaseIteratorType it) :
            it_(it)
        {
        }

    public:
        KeyIterator& operator++(void)
        {
            ++it_;
            return *this;
        }

        KeyIterator operator++(int)
        {
            return KeyIterator(it_++);
        }

        reference operator*(void) const
        {
            return it_->first;
        }

        pointer operator->(void) const
        {
            return &(it_->first);
        }

        bool operator==(const KeyIterator& rhs) const
        {
            return it_ == rhs.it_;
        }

        bool operator!=(const KeyIterator& rhs) const
        {
            return it_ != rhs.it_;
        }

    private:
        BaseIteratorType it_;

    }; // class KeyIterator

public:
    KeyIterator kbegin(void) const
    {
        return KeyIterator(items_.cbegin());
    }

    KeyIterator kend(void) const
    {
        return KeyIterator(items_.cend());
    }

    KeyIterator erase(KeyIterator it)
    {
        return KeyIterator(items_.erase(it.it_));
    }

    /*}}}*/

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
    return Ptr<IProbeEnumerator>(
        new Object<ProbeEnumerator>(kbegin(), kend()));
}

inline Ptr<IProbeEvent> ProbeContainer::GetProbe(const std::string& name)
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

inline Ptr<Probe> ProbeContainer::Add(const std::string& name)
{
    auto result = items_.emplace(name, nullptr);
    if (!result.second)
    {
        BOOST_THROW_EXCEPTION(
            ProbeAlreadyRegistered() <<
            ProbeNameErrorInfo(name));
    }
    auto it = result.first;
    Ptr<Probe> probe(new Object<Probe>);
    it->second = probe;
    return std::move(probe);
}

inline bool ProbeContainer::Has(const std::string& name) const
{
    return !!items_.count(name);
}

inline void ProbeContainer::MergeFrom(const std::string& prefix, Ptr<IProbeContainer> merged)
{
    Ptr<IProbeEnumerator> e = merged->GetEnumerator();
    while (e->HasNext())
    {
        std::string name  = e->Next();
        Ptr<IProbeEvent> probe = merged->GetProbe(name);

        std::string pname = prefix + name;
        auto result = items_.emplace(pname, nullptr);
        if (result.second)
        {
            auto it = result.first;
            it->second = std::move(probe);
        }
    }
}


NSFX_CLOSE_NAMESPACE


#endif // PROBE_CONTAINER_H__6B28DFC9_2116_4EFE_8076_E8E1CE07D8A8

