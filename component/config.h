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


/**
 * @defgroup Component
 * @brief Support for component-based programming.
 *
 * Depends upon Exception module.
 */


/**
 * @page Component Component-based programming
 * A component is an object that provides and uses various interfaces.
 * The behavior of a component is defined by the interfaces it provides and uses.
 *
 * ===============================
 * Component, object and interface
 * ===============================
 *
 *   An interface always represents the object that implements it.
 *   A component can be an aggregation of objects.
 *   The relationship between a component and an interface can be either
 *   <b>`is-a'</b> or <b>`has-a'</b>.
 *   `is-a' means the component is the object that implements the interface.
 *   `has-a' means the component aggregates an object that implements the
 *   interface.
 *   @code
 *   interface
 *      | implemented by
 *      V
 *      object
 *         | aggregated into
 *         V
 *         component
 *            | provides/uses
 *            V
 *            interface
 *   @endcode
 *
 *   For convenience, a user can <b>always</b> assume that a component
 *   `is' an \c IObject, and `has' other interfaces.
 *   i.e., a component implements \c IObject itself, and all other interfaces
 *   are implemented by internally aggregated objects.
 *
 *   When a unique object implements an interface, the component can expose the
 *   interface directly.
 *   When multiple objects implements the same interface, the component can
 *   expose an enumerator interface for users to obtain the required objects.
 *
 * ======================
 * Interface navigability
 * ======================
 *
 * ## 1. How to obtain different interfaces on a component?
 *
 * ### 1.1 Use concrete class and downcast
 *     The primary form of interface implementation is to create a concrete
 *     class that inherits from pure abstract bases, and implements the methods.
 *     Such classes can be large and heavy, since a class inherits from all
 *     interfaces it exposes, and presents to users a huge table of virtual
 *     functions, along with a set of non-virtual functions.
 *     The code of the class becomes hard to read or understand.
 *     It becomes unclear how interfaces are cohesive with each other, but they
 *     are just coupled tightly together within a class.
 *
 *     Another key problem is that this approach lacks <b>navigability</b>.
 *     If one holds a pointer to the object, one can safely upcast it to one of
 *     the interfaces it derives from.
 *     However, if one holds a pointer to an interface on the object, one have
 *     to downcast the interface to the object, then downcast to obtain the
 *     other interfaces on that object.
 *
 *     The key is that interfaces are generally not aware of each other.
 *     i.e., they do not know whether they are on the same component or not.
 *     The users generally have to know the concrete type of the object and use
 *     member functions or type casts to obtain the interfaces.
 *     When other objects want to use the object, they either have to know its
 *     concrete type, or the provider have to extract and pass all necessary
 *     interfaces.
 *
 *     It is very inconvenient and error prone.
 *     It emphasizes upon inheritance, and discourages encapsulation.
 *     Microsoft Component Model (COM) offers practical methods to solve these
 *     problems.
 *
 * ### 1.2 Hide objects behind a navigator interface
 *     A dictated navigator interface \c IObject is defined to query interfaces
 *     on a component.
 *     The method \c IObject::QueryInterface() is defined for this purpose.
 *     The \c IObject interface can be considered as the component itself.
 *     There is no need to know the concrete type of the object behind an
 *     interface, and the usage of downcast (the dependency upon inheritance)
 *     is completely eliminated.
 *
 *     Object factories can be used to obtain the \c IObject interface on
 *     components, which completely hides their implementations.
 *     The users can query the required interfaces in a safe way, as an
 *     exception is thrown when an interface is not supported.
 *
 * ## 2. Make all interfaces navigable
 *    All interfaces inherit from the \c IObject interface, and provide
 *    navigability for the component.
 *
 *    Virtual inheritance has to be used, since all interfaces extends the
 *    \c IObject interface.
 *
 * ==========================
 * Interface identifier (IID)
 * ==========================
 *
 * An interface is \c IObject or an interface that extends \c IObject.
 * An interface <b>must</b> have a UID (unique identifier) that identifies it.
 * The UID that identifies an interface is the IID (interface identifier) of
 * the interface.
 *
 *   An IID is a human-readable string literal that describes the hierarchical
 *   namespaces where the interface is defined, and the name of the interface.
 *   The general format of UID is <code>"<organization>.<module>.<name>"</code>.
 *   e.g., <code>"edu.uestc.nsfx.IObject"</code> is the IID of \c IObject.
 *
 * ## Define an interface identifier
 *    The library provides several tools to associate an interface with a UID.
 *    In general, the class template <code>NsfxClassTraits<></code> is
 *    specialized for the interface, which provides a static member function
 *    that returns the IID of the interface.
 *
 * ## Obtain an interface identifier
 *    Once associated, users no longer have to write the whole IID.
 *    Instead, users can use <code>uid_of<IXxx>()</code> to obtain the IID of
 *    the interface \c IXxx.
 *
 * ## Query an interface via IID
 *   The IID is passed to \c IObject::QueryInterface() to obtain the interface.
 *
 * ===============
 * Component class
 * ===============
 *
 * A component class is the C++ class that implements the \c IObject interface.
 *
 * ## Gray-box reuse
 *    If the type (C++ class) of the component class is visible, users can
 *    reuse the class as a gray-box.
 *    i.e., users can access not only the interfaces provided by the component
 *    class, but also all public members of it.
 *
 *    A gray-box reusable component class <b>can</b> have non-default
 *    constructors.
 *    Gray-box reuse is used to implement a component.
 *    Many components provided by the library are gray-box reusable.
 *
 * ## Black-box reuse
 *    In black-box reuse scheme, users do not know the type of the component
 *    class.
 *    The component class is presented to users in binary form.
 *    Users can create components of the class, and reuse the component via
 *    its interfaces.
 *
 *    A black-box reusable component class <b>must</b> have a default
 *    constructor.
 *
 * ### General requirement
 *     To allow users to access a component class, the writer of a component
 *     class must give the class an identifier, and register a class factory to
 *     a name service (a class registry) provided by the library.
 *
 *     Users query the name service for the class factory, and create components
 *     via the class factory.
 *
 * ### Class identifier (CID)
 *     A component class <b>should</b> have a UID that identifies it.
 *     The UID that identifies an component class is the CID (class identifier)
 *     of the component class.
 *     A CID is a string that has a similar format as an IID.
 *
 * ### Class factory
 *     The library adopts abstract class factory pattern.
 *     The \c IClassFactory interface offers a method \c IClassFactory::Create()
 *     that allows users to create a component via a CID.
 *     If the component is aggregated, a non-null controller pointer must be
 *     also specified.
 *
 *     The library provides a class template <code>ClassFactory<C></code> that
 *     implements a default class factory.
 *     The default class factory uses the default constructor of the component
 *     class to create a component.
 *
 * ### Class registry
 *     The writer of a component class <b>must</b> register a class factory
 *     to the class registry to enable black-box reuse.
 *
 *     The library provides \c IClassRegistry interface, and implements the
 *     interface by \c ClassRegistry class.
 *     The class registry is a singleton.
 *
 *     The the writer of a component class use \c NSFX_REGISTER_CLASS() macro
 *     to register the component class with a CID.
 *
 *     Users call <code>CreateObject<IXxx>()</code> function template to create
 *     a component.
 *
 * ======================
 * Component organization
 * ======================
 *
 * The components are organized into a hierarchical structure.
 *
 * ## Root component
 *    Each hierarchy has a root component.
 *    There can be multiple hierarchies in a program.
 *
 * ## Naming a component
 *    A component <b>should</b> have a name.
 *
 * ## Access a component
 *    A component can be accessed by its hierarchical full path.
 *
 * ==========
 * Reflection
 * ==========
 *
 * A component can expose meta-data about itself.
 *
 * ## Meta-data of a component
 *    * CID of the component class
 *    * IID of the provided interfaces
 *
 *    The library do not provide tools to invoke the methods of interfaces in a
 *    type-neutral way.
 *    The reasons are as follows.
 *    * Interfaces are always visible to users.
 *    * Users can always obtain the \c IObject interface of a component.
 *    * Users can always obtain other interfaces via the \c IObject interface.
 *
 *    Type-neutral invocation is mainly aimed to enable cross-language
 *    inter-operability (scripting language, GUI, etc.).
 *
 * ## Providing meta-data
 *    The library provides \c IObjectMetaData interface.
 *
 * ================
 * Interoperability
 * ================
 *
 * Interoperability is a allow users to invoke the methods of interfaces on
 * a component in a type-neutral way.
 *
 * ## Internal to C++
 *    It is fairly easy to invoke a method of an interface on a component in C++.
 *    * Interfaces (pure abstract classes) are always visible to users.
 *    * Users can always obtain the \c IObject interface of a component.
 *    * Users can always obtain other interfaces via the \c IObject interface.
 *
 * ## Outside of C++
 *    Type-neutral invocability is mainly aimed to enable cross-language
 *    inter-operability (scripting language, GUI, etc.).
 *
 *    A separate interface can be provided.
 *    e.g., \c IDispatch.
 *
 * ===================
 * Lifetime management
 * ===================
 *
 *   Only deterministic methods are considered.
 *   A garbage collector is not considered right now.
 *
 *   Please note that, if an object is created by the library, it must be freed
 *   by means provided by the library.
 *   e.g., one cannot create an object via \c CreateObject() function, and free
 *   the object via \c delete.
 *
 * ## 2.1 Implicit transfer of responsibility
 *    Transfer the responsibility of deallocating an object when a raw-pointer
 *    is transferred.
 *
 *    This approach must <b>never</b> be used.
 *    Use \c std::unique_ptr, \c std::shared_ptr or \c boost::intrusive_ptr
 *    to enforce the transfer of responsibility.
 *
 * ## 2.2 Creator-based management
 *    When an object is created, the creator is responsible to deallocate the
 *    object.
 *    The creator must not be deallocated until the created object is no longer
 *    used.
 *    It requires a careful pre-design of the software system to properly
 *    layout and manage the objects.
 *    An allocated object has one and only one allocator that is responsible
 *    to deallocate it.
 *    The allocation relationships must form directed trees, where the vertices
 *    are objects, and an edge starts from an allocator to an allocated.
 *    This way, a clear deallocation path is formed so that no object is
 *    deallocated more than once or leaked.
 *    Therefore, hierarchical organization is commonly used in this management
 *    scheme.
 *
 * ### 2.3 Reference counting
 *     Use a reference count to manage object lifetime, either intrusively or
 *     non-intrusively.
 *     It eliminates the need to transfer the responsibility of deallocation,
 *     and formulates a distributed lifetime management scheme.
 *     Users can use \c std::shared_ptr and \c std::weak_ptr for non-intrusive
 *     reference counting.
 *
 *     The use of non-intrusive reference count provided by a smart pointer also
 *     has a problem.
 *     To manage the lifetime of <i>different</i> interfaces on a single object,
 *     a smart pointer must either know the concrete type of the object, or
 *     have a way to access the reference count.
 *     If the interface held by a smart pointer doesn't provide lifetime
 *     management itself, the smart pointer has to store a variable or function
 *     pointer to access the reference count (cost memory).
 *     It may even have to query a dictated lifetime manager interface at
 *     runtime (cost CPU cycles).
 *
 *     To solve the problem, similar to Microsoft COM, every interface must
 *     provide methods to manage the reference count of the component.
 *     Virtual inheritance has to be used.
 *
 *     \c IObject::AddRef() and \c IObject::Release() are defined for this
 *     purpose.
 *     And class template <code>Ptr<></code> is provided as smart pointers to
 *     manage the reference count of a component.
 *
 *     Reference counting has a problem of cyclic reference counting, since
 *     deferencing is passive.
 *     i.e., the intent to deallocate an object is not given to all holders of
 *     the object.
 *     It can be solved by actively/explicitly notify every client of a component
 *     to release the reference count they hold.
 *
 * ### 2.4. Transfer of reference counts across functions.
 *     When a pointer to an object is passed into a function call, the caller
 *     must guarantee that the object remains valid until the function returns.
 *     Usually, the caller increments the reference count by one before calling
 *     the function.
 *
 *     Should that one reference count be transferred to the callee?
 *     Microsoft COM rules specify that there is no transferring, and the caller
 *     is responsible to release this reference count.
 *     Microsoft RCW (Runtime Caller Wrapper) rules specify that this reference
 *     count is transferred, and the callee is responsible to release this
 *     reference count.
 *
 *     When a pointer to an object is passed out of a function call, the callee
 *     must guarantee that the caller obtains a valid object.
 *     Usually, the callee has to increment the reference count by one, and
 *     transfer that one reference count to the caller.
 *
 *     The intention whether to transfer a reference count must be stated
 *     explicitly.
 *     -# If a <b>smart pointer</b> is used, a reference count is transferred
 *        across the function call.
 *     -# If a <b>non-void raw pointer</b> is used, a reference count is <b>not</b>
 *        transferred across the function call.
 *     -# If a <b>void raw pointer</b> is used, a reference count is transferred
 *        out of the function call.
 *
 *     The 3rd case is only applied to methods that perform interface
 *     acquisition and object creation.
 *     Since a virtual function cannot have multiple return types, and function
 *     templates cannot be mixed with virtual funtions, the return type has to
 *     be \c void*, instead of a pointer to a specific interface.
 *     Examples are \c IObject::QueryInterface() and \c IFactory::CreateObject().
 *     Such cases are handled by library-provided smart pointers and template
 *     functions (e.g., \c CreateObject()), and should not bother the users.
 *     User-defined methods should avoid using void raw pointers as return
 *     values.
 *
 *     The 2nd case can be applied when a container object holds a reference
 *     count of an internal object that <b>shares the same lifetime</b> as the
 *     container, a <b>free pointer</b> to the internal object can be returned
 *     as a non-void raw pointer.
 *     It is user's responsibility to ensure that the container remains valid.
 *     Usually, the user holds a smart pointer to the container to keep it valid.
 *
 * ## 3. Conclusion.
 *    Performance and flexibility are conflicting aspects.
 *    To improve performance, more responsibilities are shifted to the users.
 *    And the code becomes more error-prone, since the users will almost always
 *    forget certain oral rules sometime somewhere.
 *
 *    The library is not performance hunger.
 *    Try not to shift burdens to the users.
 *    Intrusive reference counting is welcome.
 *    Objects are hidden behind interfaces exposed by components.
 *
 * ======
 * Wiring
 * ======
 *
 *   TinyOS provides practical methods to wire components together.
 *
 *   A component usually depends upon other components to work.
 *   A component that provides interfaces is a <b>provider</b> or <b>server</b>.
 *   A component that uses/requires interfaces is a <b>user</b> or <b>client</b>.
 *   A client <b>uses</b> interfaces on its providers.
 *   A provider <b>provides</b> interfaces for the clients.
 *   The action to associate a client with its provides is called <b>wiring</b>.
 *
 *   A component class can be registered via \c NSFX_REGISTER_CLASS().
 *   An uninitialized component of the class can be created via \c CreateObject().
 *   The component cannot initialize itself, since it does not and can not know
 *   what components are wired to it.
 *   The component that does wiring is called a <b>weaver</b>.
 *
 *   To initialize the object, the class must provide means for wiring.
 *   However, such a component can only be manipulated via its interfaces.
 *   i.e., it must provide interfaces that allow the weaver component to pass
 *   in the required interfaces on the providers.
 *
 * ## The <i>'User'</i> interfaces.
 *    Each interface should define an associated <i>`User'</i> interface.
 *    e.g., for an interface \c IClock, an associated interface \c IClockUser
 *    should be defined, and it has a single method \c Use(Ptr<IClock>) that
 *    allows a weaver component to provide a clock to its user.
 *    If a component uses a clock, it should expose the interface to acquire
 *    a clock.
 *
 *    A component can expose a combination of <i>`User'</i> interfaces.
 *    A wiring component is responsible to query the interfaces that are used by
 *    the components being wired.
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
 * =====
 * Event
 * =====
 *
 * ## All-in-one event.
 *    The practice of providing an all-in-one event sink interface becomes quite
 *    frustrating.
 *    @code
 *    IXxxAllInOneEventSink
 *       | defines
 *       V
 *       OnEvent1(args), OnEvent2(args), ...
 *          | implemented by
 *          V
 *          event sink component
 *    @endcode
 *
 *    In that way, users are expected to implement all callback methods on the
 *    interface, thus have to listen to all events, even if not all of the events
 *    are interested.
 *
 * ## Many-to-one event.
 *    A workaround is to define an event sink interface with a single callback
 *    method, along with a set of bit flags that identify the events.
 *    Users can connect to interested events by giving the event source a bit
 *    mask, and the callback carries a bit flag that identifies the event.
 *    @code
 *    IXxxManyToOneEventSink
 *       | defines
 *       V
 *       OnEvent(eventKind, args)
 *          | implemented by
 *          V
 *          event sink component
 *    @endcode
 *
 *    This approach is not elegant, since users have to use 'if-else' or
 *    'switch-case' to filter and catch the events.
 *    Event processing is not in a one-to-one way, but in a many-to-one way.
 *    i.e., the callback is a centralized hot spot of conditional branches.
 *
 *    Another problem is that, the designers have to unify the arguments for
 *    different events.
 *
 * ## One-to-one event.
 *    To maximize flexibility, the granularity of event sink interfaces must be
 *    minimized.
 *    i.e., each event sink interface exposes only one event, and contains only
 *    one callback method.
 *    @code
 *    IXxxEventSink
 *       | defines
 *       V
 *       Fire(args)
 *          | implemented by
 *          V
 *          event sink component
 *    @endcode
 *
 *    This way, users can connect to any combination of events as they want.
 *    It consumes more memory, but provides a cleaner way to connect and process
 *    events.
 *
 * ### Event and sinks.
 *     The responsibility of an event sink interface is to provide a virtual
 *     callback method for the event sinks (listeners) to implement.
 *
 *     On the other hand, for each event sink interface, there is an associated
 *     event interface.
 *     The responsibility of an event interface is to let an event sink connect
 *     to the event source.
 *
 *     An event interface provides two methods, <code>Connect()</code> and
 *     <code>Disconnect</code>.
 *     An event source component exposes a combination of event interfaces.
 *     The event sink components query the event interfaces, and connect their
 *     event sinks.
 *
 *     An event sink component exposes a <i>'User'</i> interface to use the
 *     event source component.
 *     The wiring component provides the event source to the event sink.
 *     It is the event sink's responsibility to perform the connection.
 *     @code
 *     IXxxEventSink                IXxxEvent
 *        | defines                    | defines
 *        V                            V
 *        Fire(args)                   Connect(Ptr<IXxxEventSink>), Disconnect()
 *           | implemented by             | exposed by
 *           V                            V
 *           event sink component         event source component
 *           (user) ---- connects to ---> (provider)
 *     @endcode
 *
 * ### Expose <i>`User'</i>, instead of <i>`Sink'</i>.
 *     A component shall not expose a <i>`Sink'</i> interface directly.
 *     Instead, it shall expose a <i>`User'</i> interface.
 *     When an interface is obtained from the <i>`User'</i> interface, it can
 *     query the <i>'Event'</i> interface of interest, and offer an event sink
 *     <i>internally</i>, and connect the event sink to the interface.
 *     A wiring component does not take care of the connections of sinks.
 *
 * =============
 * Disposability
 * =============
 *
 * Disposability means that one can dynamically plug a component into and
 * unplug the component from the system of inter-connected components.
 *
 * ## Key problem
 *    The key problem of disposability is how to prevent functionally dependent
 *    components from malfunctioning when a component is disposed.
 *    A functionally dependent component is a client component that <b>requires</b>
 *    the interfaces of the disposed provider component.
 *    <p>
 *    e.g., a MAC layer component requires a clock and a scheduler to work.
 *    However, without a physical layer or a network layer, it can run without
 *    fatal error, even if it cannot receive nor send any packet.
 *    Although, it can be considered as a configuration error in some situations.
 *    Whether a component is required is determined by the requirement of the
 *    system.
 *    <p>
 *    The whole matter of error avoidance, detection and solution usually
 *    extends out of the scope the library itself.
 *    However, the library must provide means to support disposability to
 *    prevent fatal runtime errors, while the prevention of other kinds of
 *    errors are out of the scope of the library.
 *
 * ## Disposal strategy
 *    The disposing process often involves a sub-system of components that have
 *    functional dependencies.
 *    The key difficulty of disposability is that a single component within the
 *    sub-system often lacks a sufficiently large and far sight to conduct the
 *    disposing process.
 *    The reasons is that a component is to be reused, but it is hard foretell
 *    the requirements of the system where the component is to be reused, and
 *    the component does not know how it is wired with other components.
 *    <p>
 *    To allow disposal in a distributed way is not an easy task.
 *    e.g., when a component is disposed, it may notify all of its client
 *    components, and these components may dispose successively.
 *    However, if the programmer just want to replace the disposed component
 *    with another component, this becomes an over-reaction.
 *    Therefore, the disposal of components often requires a dictated strategy
 *    that is executed by a dictated component (usually the weaver component).
 *    A component that supports disposal shall be conservative that it only
 *    releases the resources held by itself.
 *    <p>
 *    A disposal strategy often has spatial, temporal and procedural aspects.
 *    The spatial aspect determines the boundary of disposal.
 *    The boundary of disposal is the set of components involved in a disposal.
 *    The temporal aspect determines the time span of disposal, permanent or
 *    temporary.
 *    e.g., a component is permanently removed from the system; or the system
 *    is temporarily missing a component, and the disposed component is
 *    substituted by another component right away, so the functionally dependent
 *    components are working as usual.
 *    The procedural aspect involves the actions performed before, during and
 *    after the disposal.
 *    e.g., the reactions performed by a client component when a functionally
 *    dependent component disconnects from it.
 *    Usually, they shall do nothing in the reaction.
 *
 * ### Batch disposal
 *     The disposal of components can be performed in batch, i.e., a sub-system
 *     of components is disposed.
 *     Usually, the sub-system is a dependency closure, i.e., no other
 *     components are functionally dependent on the sub-system of components.
 *     The client components within the sub-system do not have to perform
 *     special actions upon the disconnection of the provider component, since
 *     the client components will be disposed along with it.
 *     <p>
 *     For a sub-system of components, if all other components only listen to
 *     the events of the component, the sub-system can be considered as a
 *     dependency closure.
 *     <p>
 *     e.g., a node is disposed from a network.
 *
 * ### Substitution
 *     If a provider component is disposed, and the client components still
 *     runs, then one should give the client components a substitution in
 *     replacement of the disposed component.
 *     The task <b>should</b> be performed by their parent component, who
 *     allocated and wired them.
 *
 * ## Rules of disposal
 *    A disposing component does:
 *    * Disconnect from all events.
 *    * Release all objects it holds.
 *    <p>
 *    The component does not:
 *    * Disconnect the event sinks.
 *
 * ### Disconnect from all events
 *     The disposing component disconnects from all events.
 *     It also cancels all scheduled events.
 *
 * ### Release all objects
 *     The disposing component releases a reference count of each object, and
 *     reset the pointer to \c nullptr.
 *
 * ### Disconnect no event sinks
 *     The disposing component does not disconnect any event sinks.
 *
 *     The correct approaches include:
 *     * Dispose the client components.
 *       The client components disconnect from and release the disposing
 *       component.
 *     * Provide the client components with another component in substitution
 *       of the disposing component.
 *       The client components disconnect from and release the disposing
 *       component, and connect to the substitution component.
 *
 * ### Notices
 *     The library does not make every interface an event.
 *
 *     In certain software systems, all interfaces are events.
 *     For exaple, OMNET++ uses such approach.
 *
 *     A provider implements a command interface as an event sink, and all
 *     clients use the interface by sending events that signals the event sink
 *     to perform certain tasks.
 *
 *     The problem still exists that one cannot just dispose the provider by
 *     disconnect it from all events of its clients.
 *     Since the clients still hold pointers to the disposed provider, the
 *     pointer becomes a dangling pointer; or if the pointer holds a reference
 *     count of the provider, the provider is not deallocated, and there is
 *     memory/resource leak.
 *
 * ## Dangling pointer prevention
 *    When one wants to dispose a component, the
 *    its client components <b>must</b> be notified, and discard the dangling
 *    pointer that points to the interface on the disposed component.
 *    <p>
 *    It is passive when a client component uses a command interface of a
 *    provider component.
 *    Since the provider does not know the set of its client, it cannot be
 *    disposed at will.
 *
 * ### Method 1: IDisposeEvent
 *     One way is to let every component provide an IDisposeEvent interface,
 *     and every client of the component listens to the event.
 *     When a component is about to dispose, it fires the event to notify its
 *     clients.
 *     This method relies upon programmers to remember to connect to and
 *     process the event.
 *
 * ### Method 2: Make every command interface an event sink
 *     Another way is to make every command interface an event sink.
 *     When a client component wants to use a command interface, it provides
 *     an event interface.
 *     Its parent component wires the provider and the client.
 *     i.e., the provider connects its event sink to the event of the client.
 *     When the client wants the provider to perform the command, it fires the
 *     event to notify the provider.
 *     <p>
 *     This method decouples the provider and client more thoroughly.
 *     When the provider is disposed, it disconnects the sink interface from
 *     the event of its clients.
 *     Each client is notified about the event of disconnection.
 *     After the disconnection, when client fires the event, there is natually
 *     no sink to perform the task, and there is not dangling pointer.
 *
 */

NSFX_OPEN_NAMESPACE


NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B

