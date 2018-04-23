/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-08
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EVENT_H__93817278_8437_49D0_9357_68F41D4EDA05
#define EVENT_H__93817278_8437_49D0_9357_68F41D4EDA05


#include <nsfx/event/config.h>
#include <nsfx/event/i-event.h>
#include <nsfx/event/exception.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <boost/type_traits/decay.hpp>
#include <boost/limits.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// EventSinkVisitorConcept.
/**
 * @ingroup Event
 * @brief Event sink visitor concept.
 *
 * @tparam Visitor The type of a callable object.<br/>
 *                 The prototype is <code>void(ISink*)</code>.
 * @tparam ISink   The type of an event sink.
 */
template<class Visitor, class ISink>
class EventSinkVisitorConcept
{
public:
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

    BOOST_CONCEPT_USAGE(EventSinkVisitorConcept)
    {
        Test();
    }

    void Test(void)
    {
        ISink* sink = nullptr;
        typename boost::decay<Visitor>::type* visitor = nullptr;
        (*visitor)(sink);
    }
};


////////////////////////////////////////////////////////////////////////////////
// Event.
/**
 * @ingroup Event
 * @brief The event class.
 *
 * @tparam IEventName The type of a user-defined event sink interface that
 *                    conforms to \c IEventSinkConcept.
 * @tparam limit The maximum number of connections.
 *
 * An event class implements <code>IEvent<></code> interface.<br/>
 * It implements the two methods \c Connect() and \c Disconnect().<br/>
 * It also provides several non-virtual member functions that allow an event
 * source to access the event sinks.<br/>
 *
 * Event classes conform to \c EnvelopableConcept, thus they can be enveloped
 * in \c Object and \c AggObject.
 *
 * @code
 * Proto ---> IEventSink<> ---> user-defined event sink interface
 *                                |
 *                                V
 *                              IEvent<> ---> user-defined event interface
 *                                              |
 *                                              V
 *                                            Event<> (a concrete class)
 * @endcode
 */
template<class IEventName, size_t limit = 0x7fffffff>
class Event :/*{{{*/
    public IEventName
{
    BOOST_CONCEPT_ASSERT((IEventConcept<IEventName>));
    static_assert(limit > 0, "Invalid limit value for Event class template.");

    typedef IEventName                           IEventType;
    typedef typename IEventType::Prototype       Prototype;
    typedef typename IEventType::IEventSinkType  IEventSinkType;

public:
    Event(void) BOOST_NOEXCEPT :
        cookie_(0)
    {
    }

    virtual ~Event(void) {}

    // IEvent /*{{{*/
public:
    virtual cookie_t Connect(Ptr<IEventSinkType> sink) NSFX_OVERRIDE
    {
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (cookie_ == limit)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        sinks_.emplace(++cookie_, std::move(sink));
        return cookie_;
    }

    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE
    {
        sinks_.erase(cookie);
    }

    /*}}}*/

public:
    void DisconnectAll(void)
    {
        sinks_.clear();
    }

    /**
     * @brief Get the number of sinks.
     */
    size_t GetNumSinks(void) const BOOST_NOEXCEPT
    {
        return sinks_.size();
    }

    /**
     * @brief Visit the sinks in an arbitrary order.
     *
     * @tparam Visitor A callable object that has a prototype of
     *                 <code>void Visitor(IEventSinkType*)</code>.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((EventSinkVisitorConcept<Visitor, IEventSinkType>));

        for (auto it = sinks_.cbegin(); it != sinks_.cend(); ++it)
        {
            visitor(it->second.Get());
        }
    }

    NSFX_INTERFACE_MAP_BEGIN(Event)
        NSFX_INTERFACE_ENTRY(IEventType)
    NSFX_INTERFACE_MAP_END()

private:
    cookie_t  cookie_;
    unordered_map<cookie_t, Ptr<IEventSinkType> >  sinks_;

}; // class Event /*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class IEventName>
class Event<IEventName, /* limit = */ 1> :/*{{{*/
    public IEventName
{
    BOOST_CONCEPT_ASSERT((IEventConcept<IEventName>));

    typedef IEventName                           IEventType;
    typedef typename IEventType::Prototype       Prototype;
    typedef typename IEventType::IEventSinkType  IEventSinkType;

public:
    virtual ~Event(void) {}

    // IEvent /*{{{*/
public:
    virtual cookie_t Connect(Ptr<IEventSinkType> sink) NSFX_OVERRIDE
    {
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (sink_)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        sink_ = std::move(sink);
        return 0x7fffffff;
    }

    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE
    {
        sink_ = nullptr;
    }

    /*}}}*/

public:
    void DisconnectAll(void)
    {
        sinks_.clear();
    }

    size_t GetNumSinks(void) const BOOST_NOEXCEPT
    {
        return sink_ ? 1 : 0;
    }

    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((EventSinkVisitorConcept<Visitor, IEventSinkType>));
        if (sink_)
        {
            visitor(sink_.Get());
        }
    }

    NSFX_INTERFACE_MAP_BEGIN(Event)
        NSFX_INTERFACE_ENTRY(IEventType)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IEventSinkType> sink_;

}; // class Event /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // EVENT_H__93817278_8437_49D0_9357_68F41D4EDA05

