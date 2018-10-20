/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-13
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_H__8E91DE5E_C830_4E2A_A73D_CC41D9059D3B
#define I_EVENT_H__8E91DE5E_C830_4E2A_A73D_CC41D9059D3B


#include <nsfx/event/config.h>
#include <nsfx/event/i-event-sink.h>
#include <nsfx/component/ptr.h>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/concept_check.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Event
 * @brief Define a custom event interface that derives from \c IEvent.
 *
 * @param IEventName The name of the user-defined event interface.
 * @param iid        The UID of the user-defined event interface.
 * @param ISink      The type of a user-defined event sink interface that
 *                   conforms to \c IEventSinkConcept.
 *
 * @example For example
 *    @code
 *     NSFX_DEFINE_EVENT_INTERFACE(
 *         IMyEvent, "edu.uestc.nsfx.example.IMyEvent",
 *         IMyEventSink);
 *    @endcode
 */
#define NSFX_DEFINE_EVENT_INTERFACE(IEventName, iid, ISink)  \
    NSFX_DEFINE_CLASS_UID(class IEventName, iid);            \
    class IEventName :                                       \
        public ::nsfx::IEvent<ISink>                         \
    {                                                        \
    public:                                                  \
        virtual ~IEventName(void) BOOST_NOEXCEPT {}          \
    }


////////////////////////////////////////////////////////////////////////////////
// IEvent.
/**
 * @ingroup Event
 * @brief The event interface.
 *
 * @tparam ISink The type of a user-defined event sink interface that conforms
 *               to \c IEventSinkConcept.
 *
 * The responsibility of an event interface is to allow event listeners to
 * connect and disconnect their event sinks.
 * An event source shall expose event interfaces for event listeners to query
 * them.
 *
 * An event interface has two methods \c Connect() and \c Disconnect().
 * It is associated with an event sink interface.
 * It has a nested type \c IEventSinkType that is the type of the event sink
 * interface.
 * It also has a nested \c Prototype that is the function type of the callback
 * of the event sink interface, and can be used to choose the correct template
 * specialization.
 *
 * Users shall <b>derive</b> their own event interfaces from this class
 * template, and associate the interfaces with UIDs so they can be queried by
 * their UIDs.
 * Users shall not use multiple inheritance to derive their own event interfaces.
 *
 * @code
 * Proto ---> IEventSink<> ---> user-defined event sink interface
 *                                |
 *                                V
 *                              IEvent<> ---> user-defined event interface
 * @endcode
 *
 * ## Event interface definition and implementation.
 *    The library provides several tools for event interface definition and
 *    implementation.
 *
 * ### 1. Define an event interface.
 *     The macro \c NSFX_DEFINE_EVENT_INTERFACE() is provided.
 *
 * ### 2. Implement an event interface.
 *     The library provides <code>Event<></code> class template.
 *
 *     For example
 *     @code
 *     #include <nsfx/event/event.h>
 *
 *     // Define an event sink interface.
 *     NSFX_DEFINE_EVENT_SINK_INTERFACE(
 *         IMyEventSink, "edu.uestc.nsfx.example.IMyEventSink",
 *         ( char(short, int) ));
 *
 *     // Define an event interface for the sink.
 *     NSFX_DEFINE_EVENT_INTERFACE(
 *         IMyEvent, "edu.uestc.nsfx.example.IMyEvent",
 *         IMyEventSink);
 *
 *     // Define a class that exposes the event.
 *     class MyObject :
 *         public IMyEvent
 *     {
 *         // Declare the event class.
 *         // An event class does not have to have an associated a UID.
 *         typedef Event<IMyEvent>  MyEvent;
 *
 *         // Envelope in AggObject<>.
 *         // Set 'autoDelete' to false, so the instances of this class can be
 *         // member variables.
 *         typedef AggObject<IMyEvent, false>  MyEventClass;
 *
 *     public:
 *         MyObject(void) :
 *             myEvent_(this)  // Set the controller of myEvent_.
 *         {}
 *
 *         // Define a member function to signal the event.
 *         void SignalMyEvent(short s, int i)
 *         {
 *             myEvent_.GetEnveloped()->Visit([] (IMyEventSink* sink) {
 *                 sink->Fire(s, i);
 *             });
 *         }
 *
 *         // Other methods ...
 *
 *         NSFX_INTERFACE_MAP_BEGIN(MyObject)
 *             // Expose the event interface.
 *             NSFX_INTERFACE_AGGREGATED_ENTRY(IMyEvent, &myEvent_)
 *         NSFX_INTERFACE_MAP_END()
 *
 *     private:
 *         MyEventClass myEvent_;
 *     };
 *
 *     NSFX_REGISTER_CLASS(MyObject, "edu.uestc.nsfx.example.MyObject");
 *
 *     // Create an object of the class, and connect a sink to the event.
 *     try
 *     {
 *         Ptr<IObject> o = CreateObject<IObject>("edu.uestc.nsfx.example.MyObject");
 *         cookie_t cookie =
 *             Ptr<IMyEvent>(o)->Connect(CreateEventSink<IMyEventSink>(
 *                 nullptr, [] (short s, int i) {
 *                     // Process the event.
 *                     return '0';
 *             }));
 *         Ptr<IMyEvent>(o)->Disconnect(cookie);
 *     }
 *     catch (boost::exception& e)
 *     {
 *         NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
 *     }
 *     catch (std::exception& e)
 *     {
 *         NSFX_TEST_EXPECT(false) << e.what() << std::endl;
 *     }
 *     @endcode
 *
 * @see <code>IEventSink<></code>.
 */
template<class ISink>
class IEvent:
    virtual public IObject
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

public:
    typedef ISink    IEventSinkType;
    typedef typename ISink::Prototype  Prototype;

    virtual ~IEvent(void) BOOST_NOEXCEPT {}

    /**
     * @brief Connect an event sink.
     *
     * @return A non-zero cookie value that can be used to disconnect
     *         the event sink.
     *
     * @throw InvalidPointer
     * @throw ConnectionLimit
     */
    virtual cookie_t Connect(Ptr<IEventSinkType> sink) = 0;

    /**
     * @brief Disconnect an event sink.
     *
     * This function does not throw even if the cookie does not represent
     * a valid connection.
     */
    virtual void Disconnect(cookie_t cookie) = 0;

}; // class IEvent


////////////////////////////////////////////////////////////////////////////////
// IEventConcept.
/**
 * @ingroup Event
 * @brief The event interface concept.
 *
 * A class is an event if it satisfies the following conditions.
 * 1. It has a nested \c IEventSinkType that conforms to \c IEventSinkConcept.
 * 2. It is derived from <code>IEvent<></code> class template.
 * 3. It conforms to \c HasUidConcept.
 *
 * Since <code>IEvent<></code> class template is derived from \c IObject,
 * the class also conforms to \c IObjectConcept.
 */
template<class IEventName>
struct IEventConcept
{
    BOOST_CONCEPT_ASSERT((HasUidConcept<IEventName>));

    // Has s nested type: IEventSinkType.
    typedef char yes[1];
    typedef char no[2];

    template<class T>
    static yes& HasIEventSinkType(typename T::IEventSinkType*);
    template<class >
    static no&  HasIEventSinkType(...);
    BOOST_STATIC_CONSTANT(bool, hasIEventSinkType =
        sizeof (HasIEventSinkType<IEventName>(nullptr)) == sizeof (yes));

    static_assert(hasIEventSinkType,
                  "The type does not conform to IEventConcept, since it does "
                  "not have a nested 'IEventSinkType'.");

    // Conforms to IEventSinkConcept.
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<typename IEventName::IEventSinkType>));

    // Derive from IEvent<> class template.
    typedef IEvent<typename IEventName::IEventSinkType>  BaseType;
    static_assert(boost::is_base_of<BaseType, IEventName>::value,
                  "The type does not conform to IEventConcept, since it is "
                  "not derived from IEvent<> class template.");
};


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_H__8E91DE5E_C830_4E2A_A73D_CC41D9059D3B

