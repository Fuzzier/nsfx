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
#include <boost/concept_check.hpp>
#include <type_traits> // decay


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// EventSinkVisitorConcept.
/**
 * @ingroup Event
 * @brief Event sink visitor concept.
 *
 * @tparam Visitor The type of a callable object.
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
        typename std::decay<Visitor>::type* visitor = nullptr;
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
 * @tparam capacity The maximum number of connections.
 *
 * An event class implements <code>IEvent<></code> interface.
 * It implements the method \c Connect().
 * It also provides several non-virtual member functions that allow an event
 * source to access the event sinks.
 *
 * Event classes conform to \c ObjectImplConcept, thus they can be used with
 * \c Object and \c AggObject.
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
 *
 * The class provides a \c Visit() function template to visit the event sinks.
 * Users can use this function template to fire the event.
 *
 * The class also provides a \c Fire() function template to fire the event more
 * directly.
 * However, the \c Fire() function template performs perfect forwarding if and
 * only if <b>capacity == 1</b>.
 * For <b>capacity > 1</b>, it passes the arguments as <i>l-values</i>.
 * Therefore, the parameters of the event sink <b>must</b> be <i>l-values</i>,
 * in order to use \c Fire() for <code>capacity > 1</code>.
 */
template<class IEventName, uint32_t capacity = UINT32_MAX>
class Event :/*{{{*/
    public IEventName
{
    BOOST_CONCEPT_ASSERT((IEventConcept<IEventName>));
    static_assert(capacity > 0,
                  "Invalid capacity value for Event class template.");
    static_assert(sizeof (cookie_t) >= sizeof (capacity),
                  "Invalid cookie_t type.");

    typedef IEventName                           IEventType;
    typedef typename IEventType::Prototype       Prototype;
    typedef typename IEventType::IEventSinkType  IEventSinkType;

public:
    Event(void) :
        numSinks_(0),
        last_(0)
    {}

    virtual ~Event(void) {}

    // IEvent
public:
    virtual cookie_t Connect(Ptr<IEventSinkType> sink) NSFX_OVERRIDE
    {
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (numSinks_ == capacity)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        cookie_t cookie = Insert(std::move(sink));
        if (!cookie)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        return cookie;
    }

    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE
    {
        if (--cookie < sinks_.size())
        {
            if (sinks_[cookie])
            {
                --numSinks_;
                sinks_[cookie] = nullptr;
                while (last_ && !sinks_[last_-1])
                {
                    --last_;
                }
            }
        }
    }

private:
    cookie_t Insert(Ptr<IEventSinkType>&& sink)
    {
        cookie_t cookie = 0;
        // If there is a free slot, find and reuse the slot.
        if (numSinks_ < sinks_.size())
        {
            for (uint32_t i = 0; i < sinks_.size(); ++i)
            {
                // If this is a free slot.
                if (!sinks_[i])
                {
                    sinks_[i] = std::move(sink);
                    ++numSinks_;
                    cookie = i + 1;
                    if (last_ < cookie)
                    {
                        last_ = cookie;
                    }
                    break;
                }
            }
        }
        // If the slots are full, try to extend it.
        else // if (numSinks_ == sinks_.size())
        {
            try
            {
                sinks_.emplace_back(std::move(sink));
                ++numSinks_;
                cookie = numSinks_;
                last_  = cookie;
            }
            catch (std::bad_alloc& )
            {
                // Leave cookie to be 0.
            }
        }
        return cookie;
    }

public:
    IEventType* GetIntf(void) BOOST_NOEXCEPT
    {
        return this;
    }

    /**
     * @brief Get the number of sinks.
     */
    uint32_t GetNumSinks(void) const BOOST_NOEXCEPT
    {
        return numSinks_;
    }

    IEventSinkType* GetSink(cookie_t cookie) const
    {
        if (cookie == 0 || cookie > sinks_.size())
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        IEventSinkType* sink = sinks_[--cookie];
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        return sink;
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

        for (size_t i = 0; i < last_; ++i)
        {
            if (sinks_[i])
            {
                visitor(sinks_[i].Get());
            }
        }
    }

    void Fire(void)
    {
        if (last_)
        {
            size_t i = 0;
            while (i < last_)
            {
                if (sinks_[i])
                {
                    sinks_[i]->Fire();
                }
                ++i;
            }
        }
    }

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    /**
     * @brief Fire the event.
     *
     * @param[in] args Must be copyable.
     *
     * @remarks The arguments are perfectly forwarded for the <b>last</b> sink.
     *          For other sinks, the arguments are copied.
     */
    template<class... Args>
    void Fire(Args&&... args)
    {
        if (last_)
        {
            size_t i = 0;
            while (i < last_ - 1)
            {
                if (sinks_[i])
                {
                    sinks_[i]->Fire(args...);
                }
                ++i;
            }
            sinks_[i]->Fire(std::forward<Args>(args)...);
        }
    }

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/event/event.h>, 0))

# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    NSFX_INTERFACE_MAP_BEGIN(Event)
        NSFX_INTERFACE_ENTRY(IEventType)
    NSFX_INTERFACE_MAP_END()

private:
    uint32_t numSinks_;
    uint32_t last_; // The cookie of the last non-null sink.
    vector<Ptr<IEventSinkType>> sinks_;

}; // class Event /*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class IEventName>
class Event<IEventName, /* capacity = */ 1> :/*{{{*/
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
        return 1;
    }

    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE
    {
        if (cookie == 1)
        {
            sink_ = nullptr;
        }
    }

    /*}}}*/

public:
    IEventType* GetIntf(void) BOOST_NOEXCEPT
    {
        return this;
    }

    uint32_t GetNumSinks(void) const BOOST_NOEXCEPT
    {
        return sink_ ? 1 : 0;
    }

    IEventSinkType* GetSink(cookie_t cookie) const
    {
        if (cookie != 1 || !sink_)
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        return sink_;
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

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    /**
     * @brief Fire the event.
     *
     * @remarks The arguments are perfectly forwarded to the sink.
     */
    template<class... Args>
    void Fire(Args&&... args)
    {
        if (sink_)
        {
            sink_->Fire(std::forward<Args>(args)...);
        }
    }

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    void Fire(void)
    {
        if (sink_)
        {
            sink_->Fire();
        }
    }

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/event/event.h>, 1))

# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    NSFX_INTERFACE_MAP_BEGIN(Event)
        NSFX_INTERFACE_ENTRY(IEventType)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IEventSinkType> sink_;

}; // class Event /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // EVENT_H__93817278_8437_49D0_9357_68F41D4EDA05


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0
// template<class A0, class A1, ...>
// void Fire(A0&& a0, A1&& a1, ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
void Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    if (last_)
    {
        size_t i = 0;
        while (i < last_ - 1)
        {
            if (sinks_[i])
            {
                // sinks_[i]->Fire(a0, a1, ...);
                sinks_[i]->Fire(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), a));
            }
            ++i;
        }
        // sinks_[i]->Fire(std::forward<A0>(a0), std::forward<A1>(a1), ...);
        sinks_[i]->Fire(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    }
};

# elif BOOST_PP_ITERATION_FLAGS() == 1
// template<class A0, class A1, ...>
// void Fire(A0&& a0, A1&& a1, ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
void Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    if (sink_)
    {
        // sink_->Fire(std::forward<A0>(a0), std::forward<A1>(a1), ...);
        sink_->Fire(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    }
};

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

