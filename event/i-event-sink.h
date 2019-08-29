/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-11
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1
#define I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1


#include <nsfx/event/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/exception.h>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/concept_check.hpp>
#include <type_traits> // remove_pointer, is_same, is_base_of, is_function


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Event
 * @brief Define a custom event sink interface that derives from \c IEventSink.
 *
 * @param ISink The name of the user-defined event sink interface.
 * @param iid   The UID of the user-defined event sink interface.
 * @param Proto The prototype of the event callback.
 *              It <b>must</b> be placed within parenthesis.
 *
 * The event sink interface conforms to \c IEventSinkConcept.
 *
 * \c Proto must be placed <b>within parenthesis</b>, so commas can be used in
 * the \c Proto.
 *
 * For example,
 *
 *     NSFX_DEFINE_EVENT_SINK_INTERFACE(
 *         IMyEventSink, "edu.uestc.nsfx.example.IMyEventSink",
 *         ( char(short, int) ));
 */
#define NSFX_DEFINE_EVENT_SINK_INTERFACE(ISink, iid, Proto)    \
    NSFX_DEFINE_CLASS_UID(class ISink, iid);                   \
    class ISink :                                              \
        public ::nsfx::IEventSink<BOOST_PP_TUPLE_ENUM(Proto)>  \
    {                                                          \
    public:                                                    \
        virtual ~ISink(void) BOOST_NOEXCEPT {}                 \
    }


////////////////////////////////////////////////////////////////////////////////
// IEventSink.
/**
 * @ingroup Event
 * @brief The event sink class template.
 *
 * @tparam Proto The prototype of a callable object that looks like
 *               \c Ret(A0, A1, ...).
 *
 * The responsibility of an event sink interface is to hold a callback that can
 * be invoked by an event source when the event is fired.
 *
 * An event sink interface has a single method <code>Ret Fire(A0, A1, ...)</code>.
 * It also has a nested \c Prototype that is the function type
 * <code>Ret(A0, A1, ...)</code>, and can be used to choose the correct template
 * specialization.
 *
 * <code>IEventSink<void(void)></code> is the <b>only</b> event sink interface
 * that has an associated UID defined by the library.
 * Users shall <b>derive</b> their own event sink interfaces from this class
 * template, and associate the interfaces with UIDs to enable <code>Ptr</code>
 * class template to manage their pointers.
 * Users shall not use multiple inheritance to derive their own event sink
 * interfaces.
 *
 * @verbatim
 * Proto IEventSink<> ---> user-defined event sink interface
 *                           |
 *                           V
 *                         EventSinkCreator<>, CreateEventSink<>()
 * @endverbatim
 *
 * # Event sink interface definition and implementation.
 *    The library provides several tools for event sink interface definition
 *    and implementation.
 *
 * ## 1. Define an event sink interface.
 *    The macro \c NSFX_DEFINE_EVENT_SINK_INTERFACE() is provided.
 *
 * ## 2. Implement an event sink interface.
 *    The library provides three tiers of tools.
 *
 * ### 2.1 Class templates.
 *     The library provides three class templates that implements \c IEventSink:
 *     * \c FunctorBasedEventSink
 *     * \c FunctionPointerBasedEventSink
 *     * \c MemberFunctionBasedEventSink
 *
 *     Users can specialize these class templates to fit their own needs.
 *     The specialized templates conform to \c ObjectImplConcept, thus they
 *     can be used with \c Object and \c AggObject.
 *
 *     For example,
 *     @code
 *     #include <nsfx/event/event-sink.h>
 *
 *     // Define an event sink interface.
 *     NSFX_DEFINE_EVENT_SINK_INTERFACE(
 *         IMyEventSink, "edu.uestc.nsfx.example.IMyEventSink"
 *         ( char(short, int) ));
 *
 *     // Create event sink object:
 *     // Functor based event sink.
 *     struct Functor
 *     {
 *         char operator()(short, int)
 *         {
 *             return '0';
 *         }
 *     };
 *     typedef FunctorBasedEventSink<IMyEventSink, Functor>  MyEventSink1;
 *     typedef Object<MyEventSink1>  MyEventSinkClass1;
 *     Ptr<IMyEventSink> s1(new MyEventSinkClass1(nullptr, Functor()));
 *
 *     // Function pointer based event sink.
 *     char Bar(short, int)
 *     {
 *         return '0';
 *     }
 *     typedef FunctionPointerBasedEventSink<IMyEventSink>  MyEventSink2;
 *     typedef Object<MyEventSink2>  MyEventSinkClass2;
 *     Ptr<IMyEventSink> s2(new MyEventSinkClass2(nullptr, &Bar));
 *
 *     // Member function based event sink.
 *     struct Obj
 *     {
 *         char Qux(short, int)
 *         {
 *             return '0';
 *         }
 *     };
 *     static Obj o;
 *     typedef MemberFunctionBasedEventSink<IMyEventSink, Obj>  MyEventSink3;
 *     typedef Object<MyEventSink3>  MyEventSinkClass3;
 *     Ptr<IMyEventSink> s3(new MyEventSinkClass3(nullptr, &o, &Obj::Qux));
 *     @endcode
 *
 * ### 2.2 Maker class.
 *     The library provides \c EventSinkCreator class template.
 *     The class template supplies three overloaded function call operators
 *     that create event sink objects.
 *
 *     For example,
 *     @code
 *     // Functor based event sink.
 *     Ptr<IMyEventSink> s4 =
 *         EventSinkCreator<IMyEventSink>()(nullptr, [] (short, int) { return '0'; });
 *
 *     // Function pointer based event sink.
 *     char Bar(short, int)
 *     {
 *         return '0';
 *     }
 *     Ptr<IMyEventSink> s5 = EventSinkCreator<IMyEventSink>()(nullptr, &Bar);
 *
 *     // Member function based event sink.
 *     struct Obj
 *     {
 *         char Qux(short, int)
 *         {
 *             return '0';
 *         }
 *     };
 *     static Obj o;
 *     Ptr<IMyEventSink> s6 =
 *         EventSinkCreator<IMyEventSink>()(nullptr, &o, &Obj::Qux));
 *     @endcode
 *
 * ### 2.3 Maker function.
 *     The library provides \c CreateEventSink() function template.
 *     The function template supplies three overloaded functions that create
 *     event sink objects.
 *
 *     For example,
 *     @code
 *     // Functor based event sink.
 *     Ptr<IMyEventSink> s7 =
 *         CreateEventSink<IMyEventSink>(nullptr, [] (short, int) { return '0'; });
 *
 *     // Function pointer based event sink.
 *     char Bar(short, int)
 *     {
 *         return '0';
 *     }
 *     Ptr<IMyEventSink> s8 = CreateEventSink<IMyEventSink>(nullptr, &Bar);
 *
 *     // Member function based event sink.
 *     struct Obj
 *     {
 *         char Qux(short, int)
 *         {
 *             return '0';
 *         }
 *     };
 *     static Obj o;
 *     Ptr<IMyEventSink> s9 =
 *         CreateEventSink<IMyEventSink>(nullptr, &o, &Obj::Qux));
 *     @endcode
 *
 * ## 3. The management of connections.
 *    The <code>IEvent<></code> interface template provides a \c Connect()
 *    method which returns a cookie value.
 *    To disconnect, users call \c Disconnect() and pass the cookie value to
 *    the event source.
 *    Users have to manage the cookie value.
 *
 * ### 3.1 OPNET and OMNET++
 *     In OPNET and OMNET++, a module do not invoke methods on other modules
 *     directly.
 *     Instead, each module exposes several ports (gates), which are connected
 *     via connectors, such as data streams in OPNET and channels in OMNET++.
 *
 *     A module sends a packet (message) to a port (gate).
 *     The connector delivers the packet to the peer port (gate), and notifies
 *     the peer module.
 *
 *     Since peer modules are hidden behind ports (gates), the modules do not
 *     see each other directly.
 *     Thus, modules are loosely coupled.
 *
 *     A port (gate) manages the state of the connection.
 *     When an input port (gate) is connected to an output port (gate), both
 *     ports (gates) store the information about their peer ports (gates).
 *
 *     However, the management of ports (gates) and connectors significantly
 *     complicates the system, since users have to name and manage the ports
 *     (gates) and connectors.
 *     That is the reason why OPNET provides a GUI modeling tool and a set of
 *     APIs, and OMNET++ provides a separate NED modeling language, in order to
 *     make easier the naming and management burden.
 *
 *     OPNET only permits static connections among objects.
 *     In OMNET++, the NED describes static connections among objects, and APIs
 *     are provided to make runtime connections.
 *
 *     OPNET and OMNET++ takes a step even further that the ports (gates)
 *     accept any kind of packet (message).
 *     i.e., there is no explicit specification about what kind of information
 *     must be carried by a packet (message), and it leaves the users to guess,
 *     look for documents, or even read source codes.
 *     A specification actually exists, but it is just not stated explicitly.
 *
 *     The coupling among modules still remains, in an implicit and shadowy way.
 *     Even if packets (messages) can be delivered via ports (gates), that does
 *     not mean the modules are soundly connected with each other.
 *     Without an explicit specification, there lacks a compile-time or runtime
 *     sanity check for the correctness of the connections among modules.
 *     Thus, modules are not easily reused.
 *
 *     However, this does make it easier for OPNET and OMNET++ to implement
 *     inter-process communication between modules, since there is only one
 *     thing to implement, i.e., the serialization and deserialization of
 *     packets (messages).
 *
 * ### 3.2 NS3
 *     In NS3, there are two methods for objects to communicate with each other.
 *
 *     First, an object may hold a pointer to another object (or its interface),
 *     and directly invoke the methods on that object (interface).
 *     Second, an object may provide a callback site (event source), and let
 *     another object to connect a callback (event sink) to the site.
 *
 *     The first method is usually used to send packets downward along the
 *     protocol layers.
 *     And the second methods is usually used to forward packets upward along
 *     the protocol layers.
 *
 *     Since the first method is commonly used in NS3, the objects are more
 *     tightly coupled in NS3 than the modules in OPNET and OMNET++.
 *
 *     In order to connect a callback, an object obtains a pointer to the
 *     object that provides the callback site, and invokes its member functions
 *     to connect or disconnect the callback.
 *
 *     Since each callback site accepts only one callback, the management of
 *     connections is simple.
 *     That is, there is no need for a callback site to return a key to
 *     identify a connection, since there is no need to use a key to disconnect
 *     a specific connection.
 *     Thus, no extra information (keys) has to be saved by the object that
 *     connects a callback to a callback site.
 *
 *     By the way, in NS3, the codes that connect objects together are a mess,
 *     scattered and buried deeply.
 *
 * ### 3.3 NSFX
 *     In NSFX, an event model is specified.
 *     An event sink is only responsible to react to events.
 *     An event source provides an event, and allows multiple event sinks to
 *     connect and listen to the event.
 *     Since multiple event sinks may be connected to a single event source,
 *     and a single event sink may be connected to an event source multiple
 *     times, an event source provides a unique cookie value to identify each
 *     connection.
 *     The cookie value can be used to disconnect an event sink from the event
 *     source.
 *
 *     A cookie value does not identify an event sink, since when an event sink
 *     is connected multiple times to an event source, each connection has a
 *     different cookie value.
 *
 *     Cookie values are unique as regard to the same event source.
 *     Different event sources may provide the same cookie values to identify
 *     their own connections.
 *
 *     This approach is adopted from Microsoft COM.
 *
 *     There are three kind of objects attending the connection between
 *     an event source and an event sink.
 *
 *     1. An event source manages the connections (cookies and sinks) itself.
 *
 *     2. An event sink implementated by the users to react to the event.
 *
 *        An event sink does not provide any methods to manage a connection.
 *
 *     3. An object that manages the connections (cookeis and sinks).
 *
 *        When an event sink is connected to an event source, the event source
 *        provides a cookie value.
 *        The cookie value shall be managed by the object that holds the event
 *        sink and performs the connection.
 *
 *     In OMNET++, the keys are managed by gates, internally.
 *     Users focus upon the implementation of \c handleMessage(), which is
 *     a central point of message processing, and a pity that lots of \c if
 *     and \c switch have to be used there.
 *
 *     In OMNET++, when one gate is connected to another gate, both gates have
 *     to store the state of the connection.
 *     Since the library provides the implementation of gate, one gate can
 *     access the private resources of another gate, and stores the state of
 *     the connection for the peer gate upon connection.
 *     However, the mechanism in OMNET++ cannot be adopted in NSFX.
 *
 *     There is no connection object (port, gate, etc.) responsible for
 *     the management, since it just complicates the system.
 *     A separate connection object has to store the event source, the event
 *     sink, as well as the cookie value.
 *     To keep the connection valid, it has to occupy one reference count of
 *     each of the event source and the event sink.
 *     It is far more clean to let the object that holds the event sink manage
 *     the cookie value.
 */
template<class Proto = typename std::remove_pointer<void(void)>::type>
class IEventSink;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define the UID of <code>IEventSink<></code>.
 */
NSFX_DEFINE_CLASS_UID(IEventSink<>, "edu.uestc.nsfx.IEventSink");


////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Event
 * @brief The event sink interface (specialized class template).
 *
 * @tparam Ret  The return type of the callback method of the event sink.
 * @tparam Args The types of arguments of the callback method of the event sink.
 *
 * This class template provides template argument deduction the primary class
 * template <code>IEventSink<></code>.
 *
 * @see \c IEventSink (the primary class template)
 */
template<class Ret, class... Args>
class IEventSink<Ret(Args...)> :
    virtual public IObject
{
public:
    typedef Ret(Prototype)(Args...);

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    /**
     * @brief The callback method of the event sink.
     */
    virtual Ret Fire(Args... args) = 0;
};


////////////////////////////////////////
#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (3, (0, NSFX_MAX_ARITY, <nsfx/event/i-event-sink.h>))

# include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
// IEventSinkConcept.
/**
 * @ingroup Event
 * @brief The event sink interface concept.
 *
 * A class is an event sink if it satisfies the following conditions.
 * 1. It is the <code>IEventSink<void(void)></code> interface or it is derived
 *    from the \c IEventSink class template.
 * 2. It is has nested type \c Prototype that is a function type.
 * 3. It conforms to \c HasUidConcept.
 *
 * Since <code>IEventSink</code><code><></code> class template is derived from
 * \c IObject, the class also conforms to \c IObjectConcept.
 */
template<class ISink>
struct IEventSinkConcept
{
    BOOST_CONCEPT_ASSERT((HasUidConcept<ISink>));

    // Has a nested type: Prototype.
    typedef char yes[1];
    typedef char no[2];

    template<class T>
    static yes& HasPrototype(typename T::Prototype*);
    template<class >
    static no&  HasPrototype(...);
    BOOST_STATIC_CONSTANT(bool, hasPrototype =
        sizeof (HasPrototype<ISink>(nullptr)) == sizeof (yes));

    static_assert(hasPrototype,
                  "The type does not conform to IEventSinkConcept, since it "
                  "does not have a nested 'Prototype'.");

    // The nested Prototype is a function type.
    static_assert(std::is_function<typename ISink::Prototype>::value,
                  "The type does not conform to IEventSinkConcept, since it has "
                  "an invalid nested 'Prototype' that is not a function type.");

    // Is IEventSink<void(void)> or derived from IEventSink<> class template.
    typedef IEventSink<typename ISink::Prototype>  BaseType;
    static_assert(
        std::is_same<IEventSink<>, ISink>::value ||
        std::is_base_of<BaseType, ISink>::value,
        "The type does not conform to IEventSinkConcept since, it is neither "
        "IEventSink<void(void)>, nor derived from IEventSink<> class template.");
};


/**
 * @ingroup Event
 * @brief The event sink interface with prototype concept.
 *
 * 1. The \c ISink confoms to \c IEventSinkConcept.
 * 2. The nested \c Prototype of \c ISink is the same type as \c Proto.
 */
template<class ISink, class Proto>
struct IEventSinkPrototypeConcept
{
    BOOST_CONCEPT_ASSERT((IEventSinkConcept<ISink>));

    static_assert(std::is_same<typename ISink::Prototype, Proto>::value,
                  "The type does not conform to IEventSinkPrototypeConcept "
                  "since it has a matching 'Prototype'.");
};


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SINK_H__CE5ABEF5_7C7B_4D87_ABA6_27E053F3A8F1


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

// template<class Ret, class A0, class A1, ...>
// class IEventSink<Ret(A0, A1, ...)>
template<class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
class IEventSink<Ret(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))> :
    virtual public IObject
{
public:
    // typedef Ret(Prototype)(A0, A1, ...);
    typedef Ret(Prototype)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A));

    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    // virtual Ret Fire(A0 a0, A1 a1, ...) = 0;
    virtual Ret Fire(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, a)) = 0;
};


#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

