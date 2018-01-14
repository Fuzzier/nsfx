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


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Event
 * @brief Define a custom event interface that derives from \c IEvent.
 *
 * @param IEvent_ The name of the user-defined event interface.
 * @param ISink   The type of a user-defined event sink interface that conforms
 *                to \c IEventSinkConcept.
 */
#define NSFX_DEFINE_EVENT_INTERFACE(IEvent_, ISink)   \
    class IEvent_ :                                   \
        public ::nsfx::IEvent<ISink>                  \
    {                                                 \
    public:                                           \
        virtual ~IEvent_(void) BOOST_NOEXCEPT {}      \
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
 * connect and disconnect their event sinks.<br/>
 * An event source shall expose event interfaces for event listeners to query
 * them.<br/>
 *
 * An event interface has two methods \c Connect() and \c Disconnect().<br/>
 * It is associated with an event sink interface.<br/>
 * It has a nested type \c IEventSinkType that is the type of the event sink
 * interface.<br/>
 * It also has a nested \c Prototype that is the function type of the callback
 * of the event sink interface, and can be used to choose the correct template
 * specialization.<br/>
 *
 * Users shall <b>derive</b> their own event interfaces from this class
 * template, and associate the interfaces with UUIDs so they can be queried by
 * their UUIDs.<br/>
 * Users shall not use multiple inheritance to derive their own event interfaces.<br/>
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
 *     The macro \c NSFX_DEFINE_EVENT_INTERFACE() is provided.<br/>
 *
 * ### 2. Implement an event interface.
 *     The library provides <code>Event<></code> class template.
 *
 *     For example
 *     @code
 *     #include <nsfx/event/event.h>
 *
 *     // Define an event sink interface.
 *     NSFX_DEFINE_EVENT_SINK_INTERFACE(IMyEventSink, ( char(short, int) ));
 *
 *     // Associate the event sink interface with a UUID.
 *     NSFX_DEFINE_CLASS_UUID4(IMyEventSink, 0x80FF43BE, 0xA2ED, 0x4FA9, 0xB17A517A490A1897LL);
 *
 *     // Define an event interface for the sink.
 *     NSFX_DEFINE_EVENT_INTERFACE(IMyEvent, IMyEventSink);
 *
 *     // Associate the event interface with a UUID.
 *     NSFX_DEFINE_CLASS_UUID4(IMyEvent, 0xD867E76D, 0xA5A5, 0x4834, 0x9F7008661727185FLL);
 *
 *     // Define a class that exposes the event.
 *     class MyObject :
 *         public IMyEvent
 *     {
 *         // Declare the event class.
 *         // An event class does not have to have an associated a UUID.
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
 *     NSFX_DEFINE_CLASS_UUID4(MyObject, 0xE702D84A, 0xA4F6, 0x49A2, 0xB204001D00945B11LL);
 *     NSFX_REGISTER_CLASS(MyObject);
 *
 *     // Create an object of the class, and connect a sink to the event.
 *     try
 *     {
 *         Ptr<IObject> o = CreateObject<IObject>(uuid_of<MyObject>());
 *         Ptr<IMyEvent>(o)->Connect(CreateEventSink<IMyEventSink>(
 *             nullptr, [] (short s, int i) {
 *                 // Process the event.
 *                 return '0';
 *         }));
 *     }
 *     catch (boost::exception& e)
 *     {
 *         NSFX_TEST_EXPECT(false) << diagnostic_information(e);
 *     }
 *     catch (std::exception& e)
 *     {
 *         NSFX_TEST_EXPECT(false) << e.what();
 *     }
 *     @endcode
 *
 * @see \c IEventSink.
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
     * @return The cookie that can be used to disconnect the event sink.
     *
     * @throw ConnectionLimit
     */
    virtual cookie_t Connect(Ptr<IEventSinkType> sink) = 0;

    /**
     * @brief Disconnect an event sink.
     *
     * @remarks The method shall <b>not</b> throw even if the \c cookie does
     *          not represent a valid connection.
     */
    virtual void Disconnect(cookie_t cookie) = 0;

}; // class IEvent


////////////////////////////////////////////////////////////////////////////////
// IEventConcept.
/**
 * @ingroup Event
 * @brief The event interface concept.
 *
 * A class is an event if it satisfies the following conditions.<br/>
 * 1. It has a nested \c IEventSinkType that conforms to \c IEventSinkConcept.<br/>
 * 2. It is derived from <code>IEvent<></code> class template.<br/>
 * 3. It conforms to \c HasUuidConcept.<br/>
 *
 * Since <code>IEvent<></code> class template is derived from \c IObject,
 * the class also conforms to \c IObjectConcept.
 */
template<class IEvent_>
struct IEventConcept
{
    BOOST_CONCEPT_ASSERT((HasUuidConcept<IEvent_>));

    // Has s nested type: IEventSinkType.
    typedef char yes[1];
    typedef char no[2];

    template<class T>
    static yes& HasIEventSinkType(typename T::IEventSinkType*);
    template<class >
    static no&  HasIEventSinkType(...);
    BOOST_STATIC_CONSTANT(bool, hasIEventSinkType =
        sizeof (HasIEventSinkType<IEvent_>(nullptr)) == sizeof (yes));

    static_assert(hasIEventSinkType,
                  "The type does not conform to IEventConcept since it does "
                  "not have a nested 'IEventSinkType'.");

    // Conforms to IEventSinkConcept.
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<typename IEvent_::IEventSinkType>));

    // Derive from IEvent<> class template.
    typedef IEvent<typename IEvent_::IEventSinkType>  BaseType;
    static_assert(boost::is_base_of<BaseType, IEvent_>::value,
                  "The type does not conform to IEventConcept since it is "
                  "not derived from IEvent<> class template.");
};


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_H__8E91DE5E_C830_4E2A_A73D_CC41D9059D3B

