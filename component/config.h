/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B
#define CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>


/**
 * @defgroup Component
 * @brief Support for component-based programming.
 *
 * Depends upon Exception module.
 *
 * A component is an object that exposes various interfaces.<br/>
 *
 * # Component, object and interface.<br/>
 *   An interface always represents the object that implements it.<br/>
 *   A component can be an aggregation of objects.<br/>
 *   The relationship between a component and an interface can be either
 *   <b>`is-a'</b> or <b>`has-a'</b>.<br/>
 *   `is-a' means the component is the object that implements the interface.<br/>
 *   `has-a' means the component aggregates an object that implements the
 *   interface.<br/>
 *   @code
 *   interface
 *      | implemented by
 *      V
 *      object
 *         | aggregated into
 *         V
 *         compoment
 *            | exposes
 *            V
 *            interface
 *   @endcode
 *
 *   For convenience, a user can <b>always</b> assume that a component
 *   `is' an \c IObject, and `has' other interfaces.<br/>
 *   i.e., a component implements \c IObject itself, and all other interfaces
 *   are implemented by internally aggregated objects.<br/>
 *
 *   When a unique object implements an interface, the component can expose the
 *   interface directly.<br/>
 *   When multiple objects implements the same interface, the component can
 *   expose an enumerator interface for users to obtain the required objects.<br/>
 *
 * # The component model.
 * ## 1. How to obtain different interfaces on a component?<br/>
 *
 * ### 1.1 Use concrete class and downcast.<br/>
 *     The primary form of interface implementation is to create a concrete
 *     class that inherits from pure abstract bases, and implements the methods.<br/>
 *     Such classes can be large and heavy, since a class inherits from all
 *     interfaces it exposes, and presents to users a huge table of virtual
 *     functions, along with a set of non-virtual functions.<br/>
 *     The code of the class becomes hard to read or understand.<br/>
 *     It becomes unclear how interfaces are cohesive with each other, but they
 *     are just coupled tightly together within a class.<br/>
 *
 *     Another key problem is that this approach lacks <b>navigability</b>.<br/>
 *     If one holds a pointer to the object, one can safely upcast it to one of
 *     the interfaces it derives from.<br/>
 *     However, if one holds a pointer to an interface on the object, one have
 *     to downcast the interface to the object, then downcast to obtain the
 *     other interfaces on that object.<br/>
 *
 *     The key is that interfaces are generally not aware of each other.<br/>
 *     The users generally have to know the concrete type of the object and use
 *     member functions or type casts to obtain the interfaces.<br/>
 *     When other objects want to use the object, they either have to know its
 *     concrete type, or the provider have to extract and pass all necessary
 *     interfaces.<br/>
 *
 *     It is very inconvenient and error prone.<br/>
 *     It emphasizes upon inheritance, and discourages encapsulation.<br/>
 *     Microsoft Component Model (COM) offers practical methods to solve these
 *     problems.<br/>
 *
 * ### 1.2 Hide objects behind a navigator interface.<br/>
 *     A dictated navigator interface \c IObject is defined to query interfaces
 *     on a component.<br/>
 *     The method \c IObject::QueryInterface() is defined for this purpose.<br/>
 *     The navigator interface can be considered as the component itself.<br/>
 *     There is no need to know the concrete type of the object behind an
 *     interface, and the usage of downcast (the dependency upon inheritance)
 *     is completely eliminated.<br/>
 *     Object factories can be used to obtain the navigator interface on
 *     components, which completely hides their implementations.<br/>
 *     The users can query the required interfaces in a safe way, as an
 *     exception is thrown when an interface is not supported.<br/>
 *
 * ### 1.3 Make all interfaces navigable.<br/>
 *     All interfaces inherit from the navigator interface, and provide
 *     navigability around the component.<br/>
 *     Virtual inheritance has to be used, as all interfaces extends the root
 *     navigator interface.<br/>
 *
 * ## 2. How to manage object lifetime?<br/>
 *    Only deterministic methods are considered.<br/>
 *    A garbage collector is not considered right now.<br/>
 *
 * ### 2.1 Implicit transfer of responsibility.<br/>
 *     Transfer the responsibility of deallocating an object when a raw-pointer
 *     is transferred.<br/>
 *
 *     This approach must <b>never</b> be used.<br/>
 *     Use \c std::unique_ptr to enforce the transfer of responsibility.<br/>
 *
 * ### 2.2 Creator-based management.<br/>
 *     When an object is created, the creator is responsible to deallocate the
 *     object.<br/>
 *     The creator must not be deallocated until the created object is no longer
 *     used.<br/>
 *     It requires a careful pre-design of the software system to properly
 *     layout and manage the objects.<br/>
 *     An allocated object has one and only one allocator that is responsible
 *     to deallocate it.<br/>
 *     The allocation relationships must form directed trees, where the vertices
 *     are objects, and an edge starts from an allocator to an allocated.<br/>
 *     This way, a clear deallocation path is formed so that no object is
 *     deallocated more than once or leaked.<br/>
 *
 * ### 2.3 Reference counting.<br/>
 *     Use a reference count to manage object lifetime, either intrusively or
 *     non-intrusively.<br/>
 *     It eliminates the need to transfer the responsibility of deallocation,
 *     and formulates a distributed lifetime management scheme.<br/>
 *     Users can use \c std::shared_ptr and \c std::weak_ptr for non-intrusive
 *     reference counting.<br/>
 *
 *     The use of non-intrusive reference count provided by a smart pointer also
 *     has a problem.<br/>
 *     To manage the lifetime of <i>different</i> interfaces on a single object,
 *     a smart pointer must either know the concrete type of the object, or
 *     have a way to access the reference count.<br/>
 *     If the interface held by a smart pointer doesn't provide lifetime
 *     management itself, the smart pointer has to store a variable or function
 *     pointer to access the reference count (cost memory).<br/>
 *     It may even have to query a dictated lifetime manager interface at
 *     runtime (cost CPU cycles).<br/>
 *
 *     To solve the problem, similar to Microsoft COM, every interface must
 *     provide methods to manage the reference count of the component.<br/>
 *     Virtual inheritance has to be used.<br/>
 *
 *     \c IObject::AddRef() and \c IObject::Release() are defined for this
 *     purpose.<br/>
 *     And class template <code>Ptr<></code> is provided as smart pointers to
 *     manage the reference count of a component.<br/>
 *
 *     Reference counting has a problem of cyclic reference counting, since
 *     deferencing is passive.<br/>
 *     i.e., the intent to deallocate an object is not given to all holders of
 *     the object.<br/>
 *     It can be solved by actively/explicitly notify every user of a component
 *     to release the reference count they hold.<br/>
 *
 * ### 2.4. Transfer of reference counts across functions.<br/>
 *     When a pointer to an object is passed into a function call, the caller
 *     must guarantee that the object remains valid until the function returns.<br/>
 *     Usually, the caller increments the reference count by one before calling
 *     the function.<br/>
 *
 *     Should that one reference count be transferred to the callee?<br/>
 *     Microsoft COM rules specify that there is no transferring, and the caller
 *     is responsible to release this reference count.<br/>
 *     Microsoft RCW (Runtime Caller Wrapper) rules specify that this reference
 *     count is transferred, and the callee is responsible to release this
 *     reference count.<br/>
 *
 *     When a pointer to an object is passed out of a function call, the callee
 *     must guarantee that the caller obtains a valid object.<br/>
 *     Usually, the callee has to increment the reference count by one, and
 *     transfer that one reference count to the caller.<br/>
 *
 *     The intention whether to transfer a reference count must be stated
 *     explicitly.<br/>
 *     -# If a <b>smart pointer</b> is used, a reference count is transferred
 *        across the function call.<br/>
 *     -# If a <b>non-void raw pointer</b> is used, a reference count is <b>not</b>
 *        transferred across the function call.<br/>
 *     -# If a <b>void raw pointer</b> is used, a reference count is transferred
 *        out of the function call.<br/>
 *
 *     The 3rd case is only applied to methods that perform interface
 *     acquisition and object creation.<br/>
 *     Since a virtual function cannot have multiple return types, and function
 *     templates cannot be mixed with virtual funtions, the return type has to
 *     be \c void*, instead of a pointer to a specific interface.<br/>
 *     Examples are \c IObject::QueryInterface() and \c IFactory::CreateObject().<br/>
 *     Such cases are handled by library-provided smart pointers and template
 *     functions (e.g., \c CreateObject()), and should not bother the users.<br/>
 *     User-defined methods should avoid using void raw pointers as return
 *     values.<br/>
 *
 *     The 2nd case can be applied when a container object holds a reference
 *     count of an internal object that <b>shares the same lifetime</b> as the
 *     container, a <b>free pointer</b> to the internal object can be returned
 *     as a non-void raw pointer.<br/>
 *     It is user's responsibility to ensure that the container remains valid.<br/>
 *
 * ## 3. Conclusion.<br/>
 *    Performance and flexibility are conflicting aspects.<br/>
 *    To improve performance, more responsibilities are shifted to the users.<br/>
 *    And the code becomes more error-prone, since the users will almost always
 *    forget certain oral rules sometime somewhere.<br/>
 *
 *    The library is not performance hunger.<br/>
 *    Try not to shift burdens to the users.<br/>
 *    Intrusive reference counting is welcome.<br/>
 *    Objects are hidden behind interfaces exposed by components.<br/>
 *
 * # The wiring model.<br/>
 *   TinyOS provides practical methods to wire components together.<br/>
 *
 *   A component usually depends upon other components to work.<br/>
 *   The former component is called a <b>user</b>, and the latters are called
 *   <b>providers</b>.<br/>
 *   A user <b>uses</b> interfaces on the providers.<br/>
 *   A provider <b>provides</b> interfaces to the users.<br/>
 *   The action to associate a user with its provides is called <b>wiring</b>.<br/>
 *
 *   An object class can be registered via \c NSFX_REGISTER_CLASS().<br/>
 *   An uninitialized object of the class can be created via \c CreateObject().<br/>
 *   To initialize the object, the class must provide means for wiring.<br/>
 *
 *   However, an object can only be manipulated via its interfaces.<br/>
 *   i.e., it must provide interfaces that allow a wiring component to pass in
 *   the required interfaces on the providers.<br/>
 *
 * ## The <i>'User'</i> interfaces.
 *    Each interface should define an associated <i>`User'</i> interface.<br/>
 *    e.g., for an interface \c IClock, an associated interface \c IClockUser
 *    should be defined, and it has a single method \c Use(Ptr<IClock>) that
 *    allows a wiring component to provide a clock to its user.<br/>
 *    If a component uses a clock, it should expose the interface to acquire
 *    a clock.<br/>
 *
 *    A component can expose a combination of <i>`User'</i> interfaces.<br/>
 *    A wiring component is responsible to query the interfaces that are used by
 *    the components being wired.<br/>
 *
 *    @code
 *    provider component        user component
 *       | exposes                 | exposes
 *       V                         V
 *       IXxx --- provided to ---> IXxxUser
 *                     A
 *                     |
 *              wiring component
 *    @endcode
 *
 * # The event model.
 * ## All-in-one event.
 *   The practice of providing an all-in-one event sink interface becomes quite
 *   frustrating.<br/>
 *   @code
 *   IXxxAllInOneEventSink
 *      | defines
 *      V
 *      OnEvent1(args), OnEvent2(args), ...
 *         | implemented by
 *         V
 *         event sink component
 *   @endcode
 *
 *   In that way, users are expected to implement all callback methods on the
 *   interface, thus have to listen to all events, even if not all of the events
 *   are interested.<br/>
 *
 * ## Many-to-one event.
 *   A workaround is to define an event sink interface with a single callback
 *   method, along with a set of bit flags that identify the events.<br/>
 *   Users can connect to interested events by giving the event source a bit
 *   mask, and the callback carries a bit flag that identifies the event.<br/>
 *   @code
 *   IXxxManyToOneEventSink
 *      | defines
 *      V
 *      OnEvent(eventKind, args)
 *         | implemented by
 *         V
 *         event sink component
 *   @endcode
 *
 *   This approach is not elegant, since users have to use 'if-else' or
 *   'switch-case' to filter and catch the events.<br/>
 *   Event processing is not in a one-to-one way, but in a many-to-one way.<br/>
 *   i.e., the callback is a centralized hot spot of conditional branches.<br/>
 *
 *   Another problem is that, the designers have to unify the arguments for
 *   different events.<br/>
 *
 * ## One-to-one event.
 *   To maximize flexibility, the granularity of event sink interfaces must be
 *   minimized.<br/>
 *   i.e., each event sink interface exposes only one event, and contains only
 *   one callback method.<br/>
 *   @code
 *   IXxxEventSink
 *      | defines
 *      V
 *      Fire(args)
 *         | implemented by
 *         V
 *         event sink component
 *   @endcode
 *
 *   This way, users can connect to any combination of events as they want.<br/>
 *   It consumes more memory, but provides a cleaner way to connect and process
 *   events.<br/>
 *
 * ### Event and sinks.
 *    The responsibility of an event sink interface is to provide a virtual
 *    callback method for the event sinks (listeners) to implement.<br/>
 *
 *    On the other hand, for each event sink interface, there is an associated
 *    event interface.<br/>
 *    The responsibility of an event interface is to let an event sink connect
 *    to the event source.<br/>
 *
 *    An event interface provides two methods, <code>Connect()</code> and
 *    <code>Disconnect</code>.<br/>
 *    An event source component exposes a combination of event interfaces.<br/>
 *    The event sink components query the event interfaces, and connect their
 *    event sinks.<br/>
 *
 *    An event sink component exposes a <i>'User'</i> interface to use the
 *    event source component.<br/>
 *    The wiring component provides the event source to the event sink.<br/>
 *    It is the event sink's responsibility to perform the connection.<br/>
 *    @code
 *    IXxxEventSink                IXxxEvent
 *       | defines                    | defines
 *       V                            V
 *       Fire(args)                   Connect(Ptr<IXxxEventSink>), Disconnect()
 *          | implemented by             | exposed by
 *          V                            V
 *          event sink component         event source component
 *          (user) ---- connects to ---> (provider)
 *    @endcode
 *
 * ### Expose <i>`User'</i>, instead of <i>`Sink'</i>.
 *     A component shall not expose a <i>`Sink'</i> interface directly.<br/>
 *     Instead, it shall expose a <i>`User'</i> interface.<br/>
 *     When an interface is obtained from the <i>`User'</i> interface, it can
 *     query the <i>'Event'</i> interface of interest, and offer an event sink
 *     <i>internally</i>, and connect the event sink to the interface.<br/>
 *     A wiring component does not take care of the connections of sinks.<br/>
 *
 */


NSFX_OPEN_NAMESPACE


/**
 * @ingroup Component
 * @brief The type of reference count.
 */
typedef ptrdiff_t  refcount_t;


/**
 * @ingroup Component
 * @brief The type of cookie.
 *
 * A cookie is obtained when connecting a event sink to an event source, and
 * can be used to disconnect the sink from the event source later.
 */
typedef size_t  cookie_t;


NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B

