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
 *
 * A component is an object that provides and uses various interfaces.
 * The behavior of a component is defined by the interfaces it provides and uses.
 *
 * ==================================
 * Component, object and interface
 * ==================================
 *
 * An interface always represents the object that implements it.
 * A component can be an aggregation of objects.
 * The relationship between a component and an interface can be either **is-a** or
 * **has-a**.
 * **is-a** means the component is the object that implements the interface.
 * **has-a** means the component aggregates an object that implements the interface.
 *
 * @verbatim
 * interface
 *    | implemented by
 *    V
 *    object
 *       | aggregated into
 *       V
 *       component
 *          | provides/uses
 *          V
 *          interface
 * @endverbatim
 *
 * For convenience, a user can **always** assume that a component
 * **is** an `IObject`, and **has** other interfaces.
 * i.e., a component implements `IObject` itself, and all other interfaces are
 * implemented by internally aggregated objects.
 *
 * When a unique object implements an interface, the component can expose the
 * interface directly.
 * When multiple objects implements the same interface, the component can
 * expose an enumerator interface for users to obtain the required objects.
 *
 * ======================
 * Interface navigability
 * ======================
 *
 * ## How to obtain different interfaces on a component?
 *
 * ### Use concrete class and downcast
 * The primary form of interface implementation is to create a concrete class
 * that inherits from pure abstract bases, and implements the pure virtual
 * functions.
 * Such classes can be large and heavy, since a class inherits from all
 * interfaces it exposes, and presents to users a huge table of virtual
 * functions, along with a set of non-virtual functions.
 * The code of the class becomes hard to read or understand.
 * It becomes unclear how interfaces are cohesive with each other, but they
 * are just coupled tightly together within a class.
 *
 * Another key problem is that this approach lacks **navigability**.
 * If one holds a pointer to the object, one can safely *upcast* it to one of
 * the interfaces it derives from.
 * However, if one holds a pointer to an interface on the object, one have
 * to *downcast* the interface to the object, then downcast to obtain
 * the other interfaces on that object.
 *
 * The key is that interfaces are generally independent of each other.
 * i.e., they do not know whether they are exposed by the same component.
 * The users generally have to know the concrete type of the object and use
 * member functions or type casts to obtain the interfaces.
 * When other objects want to use the object, they either have to know its
 * concrete type in order to obtain its interfaces, or the users have to
 * extract and provide all necessary interfaces of the object.
 *
 * It is very inconvenient and error prone.
 * It emphasizes upon inheritance, and discourages encapsulation.
 * Microsoft Component Model (COM) offers practical methods to solve these
 * problems.
 *
 * ### Hide objects behind a navigator interface
 * A dictated navigator interface `IObject` is defined to query interfaces
 * on a component.
 * The method `IObject::QueryInterface()` is defined for this purpose.
 * The `IObject` interface can be considered as the component itself.
 * There is no need to know the concrete type of the object behind an
 * interface, and the usage of downcast (the dependency upon inheritance)
 * is completely eliminated.
 *
 * Object factories can be used to obtain the `IObject` interface on components,
 * which completely hides their implementations.
 * The users can query the required interfaces in a safe way, as exception
 * can be thrown when an interface is not supported.
 *
 * ## Make all interfaces navigable
 * All interfaces inherit from the `IObject` interface, and provide navigability
 * for the component.
 *
 * Virtual inheritance has to be used, since all interfaces extends the
 * `IObject` interface.
 *
 * ===================
 * Lifetime management
 * ===================
 *
 * Only deterministic methods are considered.
 * A garbage collector is not considered right now.
 *
 * Please note that, if an object is created by the library, it must be
 * deallocated by the means provided by the library.
 * e.g., one cannot create an object via `CreateObject()` function, and
 * deallocate the object via `delete`.
 *
 * ## Implicit transfer of responsibility
 * Transfer the responsibility of deallocating an object when a raw-pointer
 * is transferred.
 *
 * This approach must **never** be used.
 * Use `std::unique_ptr` to enforce the transfer of responsibility.
 *
 * ## Creator-based management
 * When an object is created, the creator is responsible to deallocate the
 * object.
 * The creator must not be deallocated until the created object is no longer
 * used.
 * It requires a careful pre-design of the software system to properly layout
 * and manage the objects.
 * An allocated object has one and only one allocator that is responsible to
 * deallocate it.
 * The allocation relationships must form directed trees, where the vertices
 * are objects, and an edge starts from an allocator to an allocated.
 * This way, a clear deallocation path is formed so that no object is
 * deallocated more than once or leaked.
 * Therefore, hierarchical organization is commonly used in this management
 * scheme.
 *
 * ### Reference counting
 * Use a reference count to manage object lifetime, either intrusively or
 * non-intrusively.
 * It eliminates the need to transfer the responsibility of deallocation,
 * and formulates a distributed lifetime management scheme.
 * Users can use `std::shared_ptr` and `std::weak_ptr` for non-intrusive reference
 * counting, or `boost::intrusive_ptr` for intrusive reference counting.
 *
 * The use of non-intrusive reference count provided by a smart pointer also
 * has a problem.
 * To manage the lifetime of *different* interfaces on a single object,
 * a smart pointer must either know the concrete type of the object, or
 * have a way to access the reference count.
 * If the interface held by a smart pointer doesn't provide lifetime
 * management itself, the smart pointer has to store a variable or function
 * pointer to access the reference count (cost memory).
 * It may even have to query a dictated lifetime manager interface at runtime
 * (cost CPU cycles).
 *
 * To solve the problem, similar to Microsoft COM, every interface must provide
 * methods to manage the reference count of the component.
 * Virtual inheritance has to be used.
 *
 * `IObject::AddRef()` and `IObject::Release()` are defined for this purpose.
 * And class template `Ptr` is provided as smart pointers to manage the reference
 * count of a component.
 *
 * Reference counting has a problem of cyclic reference counting, since
 * deferencing is passive.
 * i.e., the intent to deallocate an object is not given to all holders of
 * the object.
 * It can be solved by actively/explicitly notify every client of a component
 * to release the reference count they hold.
 *
 * ### Transfer of reference counts across functions.
 * When a pointer to an object is passed into a function call, the caller
 * must guarantee that the object remains valid until the function returns.
 * Usually, the caller increments the reference count by one before calling
 * the function.
 *
 * Should that one reference count be transferred to the callee?
 * Microsoft COM rules specify that there is no transferring, and the caller
 * is responsible to release this reference count.
 * Microsoft RCW (Runtime Caller Wrapper) rules specify that this reference
 * count is transferred, and the callee is responsible to release this
 * reference count.
 *
 * When a pointer to an object is passed out of a function call, the callee
 * must guarantee that the caller obtains a valid object.
 * Usually, the callee has to increment the reference count by one, and
 * transfer that one reference count to the caller.
 *
 * The intention whether to transfer a reference count must be stated
 * explicitly.
 * -# If a **smart pointer** is used, a reference count is transferred across
 *    the function call.
 * -# If a **non-void raw pointer** is used, a reference count is **not** transferred
 *    across the function call.
 * -# If a **void raw pointer** is used, a reference count is transferred out of
 *    the function call.
 *
 * The 3rd case is only applied to methods that perform interface acquisition
 * and object creation.
 * Since a virtual function cannot have multiple return types, and function
 * templates cannot be mixed with virtual funtions, the return type has to
 * be `void*`, instead of a pointer to a specific interface.
 * Examples are `IObject::QueryInterface()` and `IFactory::CreateObject()`.
 * Such cases are handled by library-provided smart pointers and template
 * functions (e.g., `CreateObject()`), and should not bother the users.
 * User-defined methods should avoid using void raw pointers as return values.
 *
 * The 2nd case can be applied when a container object holds a reference count
 * of an internal object that **shares the same lifetime** as the container,
 * a **free pointer** to the internal object can be returned as a non-void raw
 * pointer.
 * It is user's responsibility to ensure that the container remains valid.
 *
 * ## Conclusion.
 * Performance and flexibility are conflicting aspects.
 * To improve performance, more responsibilities are shifted to the users.
 * And the code becomes more error-prone, since the users will almost always
 * forget certain oral rules sometime somewhere.
 *
 * The library is not performance hunger.
 * Try not to shift burdens to users.
 * Intrusive reference counting is welcome.
 * Objects are hidden behind interfaces exposed by components.
 *
 * ==========================
 * Interface identifier (IID)
 * ==========================
 *
 * An interface is `IObject` or an interface that extends `IObject`.
 * An interface **must** have a UID (unique identifier).
 * The UID of an interface is also called an IID (interface identifier).
 *
 * An IID is a human-readable string literal that describes the hierarchical
 * namespaces where the interface is defined, as well as the name of the
 * interface.
 * The general format of UID is `"<organization>.<module>.<name>"`.
 * e.g., `"edu.uestc.nsfx.IObject"` is the IID of `IObject`.
 *
 * ## Define an interface identifier
 * The library provides several tools to associate an interface with a UID.
 * In general, the class template `NsfxClassTraits` is specialized for
 * an interface, which provides a static member function that returns the IID
 * of the interface.
 *
 * ## Obtain an interface identifier
 * Once associated, users no longer have to write the whole IID.
 * Instead, users can use `uid_of<IXxx>()` to obtain the IID of
 * the interface `IXxx`.
 *
 * ## Query an interface via IID
 * The IID is passed to `IObject::QueryInterface()` to obtain the interface.
 *
 * ===============
 * Component class
 * ===============
 * A component class is the C++ class that implements the `IObject` interface.
 *
 * ## Gray-box reuse
 * If the type (C++ class) of the component class is visible, users can reuse
 * the class as a gray-box.
 * i.e., users can access not only the interfaces provided by the component
 * class, but also all public members of it.
 *
 * A gray-box reusable component class **can** have non-default constructors.
 * Gray-box reuse is used to implement a component.
 * Many components provided by the library are gray-box reusable.
 *
 * ## Black-box reuse
 * In black-box reuse scheme, users do not know the type of the component class.
 * Instead, the `IObject` interface is given to users, and users can query
 * required interfaces on the component.
 * The component class is presented to users in binary form.
 *
 * A black-box reusable component class **must** have a default constructor.
 *
 * ## Class identifier (CID)
 * Since users have no access to the concrete type of a component, there must
 * be an alternative way for the users to identify the component class and
 * create component instances.
 *
 * A black-box reusable component class **must** have a UID.
 * The UID that identifies an component class is also called a CID (class
 * identifier).
 * A CID is a string that has a similar format as an IID.
 *
 * To allow users to create a component, the writer of a component class must
 * give the component class a CID.
 * The library provides a class registry.
 * The class factory is registered to the class registry, and the CID is used
 * as the index of the class factory.
 *
 * Users use a CID to find the class factory, and create component instances
 * via the class factory.
 *
 * ## Class factory
 * The library adopts abstract class factory pattern.
 * The `IClassFactory` interface offers a method `IClassFactory::Create()`
 * that allows users to create a component instance via a CID.
 * If the component instance is to be aggregated, a non-null controller pointer
 * must also be provided.
 *
 * The library provides a class template `ClassFactory` that implements a default
 * class factory.
 * The default class factory uses the default constructor of the component
 * class to create a component instance.
 * Therefore, a component instance created by a class factory is usually
 * *uninitialized*.
 *
 * ## Class registry
 * The writer of a component class **must** register a class factory to
 * the class registry to enable black-box reuse.
 *
 * The library provides `IClassRegistry` interface, and implements the interface
 * by `ClassRegistry` class.
 * The class registry is a singleton.
 *
 * The the writer of a component class use `NSFX_REGISTER_CLASS()` macro to
 * register the component class with a CID.
 *
 * Users call `CreateObject<IXxx>()` function template to create a component
 * instance, and obtain a pointer to `IXxx` interface of the component.
 *
 * ==============
 * Initialization
 * ==============
 *
 * When a component is created, it is usually uninitialized.
 * For a component to initialize, it must acquire all required resources.
 * For example, the required interfaces have to be provided to the component,
 * and the configuration parameters have to be assigned to the component.
 *
 * When an uninitialized component is used, it **shall** throw `Uninitialized`
 * exception.
 *
 * There are two initialization methods.
 *
 * ## Explicit initialization
 * A component exposes `IInitializable` interface.
 * The interface has a single method `Initialize()`.
 * When the required resources have been assigned to the component, users
 * call `IInitializable::Initialize()` to initialize the component.
 *
 * If there are insufficient resources, or there are conflicting data, the
 * component **shall** throw `CannotInitialize` exception.
 *
 * ## Lazy initialization
 * A component does not expose `IInitializable` interface.
 * Instead, when the required resources are assigned, the component checks
 * whether it has acquired all resources.
 * If so, the component performs initialization automatically.
 *
 * ======
 * Wiring
 * ======
 *
 * The behavior of a component is defined by the interfaces it provides and uses.
 * A component that provides interfaces is a *provider* or *server*.
 * A component that uses interfaces is a *user* or *client*.
 * A client *uses* interfaces on its providers.
 * A provider *provides* interfaces to the clients.
 * Such action is called *wiring*.
 * Therefore, wiring is part of initialization.
 *
 * A component class can be registered via `NSFX_REGISTER_CLASS()`.
 * An uninitialized component of the class can be created via `CreateObject()`.
 * The component cannot initialize itself, since it does not and cannot know
 * which components are providing interfaces to it.
 * The component that does wiring is called a *composer*.
 *
 * ## Wiring in TinyOS
 * TinyOS provides a method to wire components together.
 * In TinyOS, wiring is function linking.
 * That is, linking a called function to its calling sites.
 *
 * TinyOS provides an intuitive way of thinking.
 * A component is viewed as a chip (integrated circuit) with many pins (legs).
 * A wire is a metal line that connects the pins of chips.
 *
 * @verbatim
 *  --------   wire   --------
 * |        |--------|        |
 * | chip 1 |        | chip 2 |
 * |        |---+----|        |
 *  --------    |     --------
 *              |
 *          --------
 *         |        |
 *         | chip 3 |
 *         |        |
 *          --------
 * @endverbatim
 *
 * Although it does not fully reflect the component programming paradigm in
 * C++, it provides a good direction.
 * To support bi-directional communications, TinyOS introduces an interface
 * model that contains both commands and events.
 *
 * In TinyOS, for performance reasons, wiring is performed at compile time.
 * It does no support runtime wiring.
 *
 * ## Models of wiring
 * Runtime wiring can be more complicated due to constrants.
 * There are two kinds of design models.
 *
 * ### Model 1: component-aware wiring
 * When an interface is provided to a component (a client), the client assumes
 * that a component (a server) is provided.
 * Thus, the client can query any interface provided by the server.
 *
 * ### Model 2: interface-only wiring
 * The channel does not make any assumption about the component that provides
 * the used interface.
 * Thus, it must not query other interfaces from it.
 * This is the method used in TinyOS.
 *
 * ## The convention of wiring
 * If a component uses an `IObject` interface, it is a component-aware wiring.
 * If a component uses a specific interface, it is an interface-only wiring.
 *
 * Please note that, component-aware wiring can sometimes be mimiced by
 * providing a large interface that aggregates some/all interfaces of the
 * component.
 * It is not recommended to do so, as an interface shall focus upon a single
 * task to improve cohesion, while making a large interface that aggregates
 * several independent interfaces together is dull.
 *
 * ## The *User* interfaces
 * To initialize the object, the class must provide means for wiring.
 * However, a component can only be manipulated via its interfaces.
 * i.e., it must provide interfaces that allow the composer to provide the
 * required interfaces for the client.
 *
 * For each interface, an associated *User* interface can be defined.
 * For example, for an interface `IClock`, an associated interface `IClockUser`
 * can be defined.
 * It has a single method `Use(Ptr<IClock>)` that allows a composer to provide
 * a clock to its user.
 * If a component requires a clock, it shall expose the interface to acquire
 * a clock.
 *
 * When a component exposes a `IXxxUser` interface, it *requires* an `IXxx` interface
 * to behave correctly.
 * That is, it cannot perform its task before the interface is provided.
 * It has a strong dependency upon the required interface.
 *
 * This is a different relationship between an event source and event sinks.
 * An event does not depend upon event sinks.
 *
 * A component can expose a combination of *User* interfaces.
 * The composer is responsible to query the interfaces that are used by
 * the components being wired.
 *
 * @verbatim
 * provider component        user component
 *    | exposes                 | exposes
 *    V                         V
 *    IXxx --- provided to ---> IXxxUser
 *                  A
 *                  |
 *          composer component
 * @endverbatim
 *
 * =====
 * Event
 * =====
 *
 * The event model specifies how callbacks are used.
 *
 * ## All-in-one event.
 * The practice of providing an all-in-one event sink interface can be quite
 * frustrating.
 *
 * @verbatim
 * IXxxAllInOneEventSink
 *    | defines
 *    V
 *    OnEvent1(args), OnEvent2(args), ...
 *       | implemented by
 *       V
 *       event sink component
 * @endcode
 * @endverbatim
 *
 * In that way, an event sinks are expected to implement all callback methods
 * of the event.
 * Thus, an event sink has to listen to all events, even if some of the events
 * are not interested.
 *
 * ## Many-to-one event.
 * A workaround is to define an event sink interface with a single callback
 * method, along with a set of data that specify the properties of the event.
 *
 * @verbatim
 * IXxxManyToOneEventSink
 *    | defines
 *    V
 *    OnEvent(eventKind, args)
 *       | implemented by
 *       V
 *       event sink component
 * @endverbatim
 *
 * This approach is not elegant, since users have to use `if-else` or `switch-case`
 * to filter and catch the events.
 * Event processing is not in a one-to-one way, but in a many-to-one way.
 * i.e., the callback is a centralized hotspot of conditional branches.
 *
 * Another problem is that, the designers have to unify the arguments for
 * different kind of events.
 *
 * ## One-to-one event.
 * To maximize flexibility, the granularity of event sink interfaces shall
 * be minimized.
 * i.e., each event sink interface exposes only one event, and contains only
 * one callback method.
 *
 * @verbatim
 * IXxxEventSink
 *    | defines
 *    V
 *    Fire(args)
 *       | implemented by
 *       V
 *       event sink component
 * @endverbatim
 *
 * This way, users can connect to any combination of events as they want.
 * It consumes more memory, but provides a cleaner way to connect to and
 * process events.
 *
 * ### Event and sinks.
 * The responsibility of an event sink interface is to provide a virtual
 * callback method for the event sinks (listeners) to implement.
 *
 * On the other hand, for each event sink interface, there is an associated
 * event interface.
 * The responsibility of an event interface is to let an event sink connect
 * to the event source.
 *
 * An event interface provides two methods, `Connect()` and `Disconnect()`.
 * An event source component exposes multiple event interfaces.
 * The event sink components exposes event sink interfaces.
 * The connection between an event source and an event sink is performed by
 * a composer component.
 *
 * @verbatim
 * IXxxEventSink                IXxxEvent
 *    | defines                    | defines
 *    V                            V
 *    Fire(args)                   Connect(Ptr<IXxxEventSink>), Disconnect()
 *       | implemented by             | exposed by
 *       V                            V
 *       event sink component         event source component
 *              |                     A
 *              |                     |
 *              ---- connected to -----
 *                        A
 *                        |
 *                composer component
 * @endverbatim
 *
 * ==========
 * Connection
 * ==========
 *
 * A component depends upon the required interfaces.
 * The coupling between a component and its required interfaces is tight.
 * Wiring is the realization of this kind of dependency.
 *
 * Besides, the event model specifies a loosely coupled relationship between
 * an event source and the event sinks.
 * An event source shall work without event sinks, so even if no one processes
 * the signalled event, the event source shall not go wrong.
 * Also, an event sink shall work without event sources, so even if no events
 * are signalled, the event sink shall not go wrong.
 *
 * However, the connection between two components can involve more than one
 * interface, and the event model is usually not enough to deal with this kind
 * of connection.
 *
 * For example, multiple nodes can connect to a channel.
 * The channel must know the location of the nodes in order to calculate the
 * proprogation delay and pathloss.
 * Each transmitter must offer an event interface that transmits signals to
 * the channel.
 * Each receiver must also offer an event sink interface to receive signals.
 * Therefore, the connection involves at least three interfaces that must be
 * provided as a bundle that represent a single node.
 *
 * Thus, the channel can provide a connection interface like this:
 * @verbatim
 * class IChannelConnection : virtual public IObject
 * {
 *     virtual ~IChannelConnection(void) {}
 *     cookie_t Connect(Ptr<ILocatable>,
 *                      Ptr<ISignalTxEvent>,
 *                      Ptr<ISignalRxEventSink>) = 0;
 *     void Disconnect(cookie_t cookie) = 0;
 * };
 * @endverbatim
 *
 * The nodes and channel are loosely coupled, as a node can operate without
 * connecting to a channel, and a channel can operate with no nodes connecting
 * to it.
 *
 * Connection is a more general kind of loosely coupled relationship among
 * components, compared to the event model.
 * Unlike the event model, there is no *Sink* interface for a connection.
 * @verbatim
 * interfaces                   IXxxConnection
 *       | implemented by       | defines
 *       |                      V
 *       |                      Connect(interfaces), Disconnect()
 *       |                            | exposed by
 *       V                            V
 *       connecting component         connectable component
 *              |                     A
 *              |                     |
 *              ---- connected to -----
 *                        A
 *                        |
 *                composer component
 * @endverbatim
 *
 * A composer is not only responsible for initializing components (including
 * wiring), but also connecting components together.
 *
 * =============
 * Disposability
 * =============
 *
 * Disposability means that a component can be dynamically unplugged from
 * a system of inter-wired and inter-connected components at runtime.
 *
 * ## Key problem
 * The key problem of disposability is how to prevent functionally dependent
 * components from malfunctioning when a component is disposed.
 * A functionally dependent component is a client component that **requires**
 * the interfaces of provided by the disposed component.
 * That is, the key problem is how to **de-wiring** of components.
 *
 * The whole matter of error avoidance, detection and solution usually
 * extends out of the scope the library itself.
 * However, the library must provide guidelines to support disposability to
 * prevent fatal runtime errors, while the prevention of other kinds of errors
 * are out of the scope of the library.
 *
 * ## Disposal strategy
 * The disposing process involves disconnection and de-wiring of components.
 *
 * Since a de-wired component does not acquire enough resources to work, it
 * cannot fulfill any commands or react to any events issued by the connected
 * peers.
 * Therefore, disconnection **shall** be performed before de-wiring.
 *
 * The key difficulty of disposability is that a component within the system
 * often lacks the information about which components are connecting to it or
 * using it.
 * Thus, to perform disposal in a purely distributed way is not an easy task.
 * For example, when a component is disposed, it may notify all of its client
 * components, and these components may react by disposing themselves
 * successively.
 * However, if the programmer just want to replace the disposed component with
 * another component, it becomes an over-reaction.
 *
 * Therefore, the disposal of components often requires a dictated strategy
 * that is executed by a dictated component, which is usually the composer
 * component or some other components that has enough information to do so.
 * A component that supports disposing itself shall be *conservative* that it
 * only releases the resources held by itself.
 *
 * A disposal strategy often has spatial, temporal and procedural aspects.
 * The spatial aspect determines the boundary of disposal.
 * The boundary of disposal is the set of components involved in a disposal.
 * It identifies which components are to be disconnected or de-wired.
 * The temporal aspect determines the time span of disposal.
 * Usually, during disposal, the system is temporarily at an unworking state,
 * and does not perform any other tasks.
 * The procedural aspect involves the actions performed before, during and
 * after the disposal.
 *
 * ### Batch disposal
 * The disposal of components can be performed in batch, i.e., a sub-system of
 * components is disposed.
 * Usually, the sub-system is a *dependency closure*, i.e., no other components
 * are functionally dependent upon the sub-system of components.
 * The sub-system is loosely coupled with other sub-systems via connection.
 *
 * For example, when a node is to be removed from a network, first it is
 * disconnected from the channel, then the components within the node are
 * disposed and deallocated.
 *
 * ### Substitution
 * If a provider component is disposed, and the client components still work,
 * then one must provide the client components a substitution in replacement
 * of the disposed component.
 * The task may be performed by the composer component that had created and
 * wired them.
 *
 * ### Distributed disposal (not supported)
 * Purely distributed disposal is not easy, as it requires a component to
 * have full information about the connections and dependency that it involves.
 * Thus, it is not formally supported by the library.
 *
 * #### Method 1: IDisposeEvent
 * One way is to let a component provide an `IDisposeEvent` interface, and
 * every client of the component listens to the event.
 * When a component is about to dispose, it fires the event to notify its
 * clients.
 * However, by the nature of event, its clients may or may not listen to
 * the event.
 * The requirement may be enforced by letting the component use an
 * `IDisposeEventSink` interface, and let each client provide the interface.
 * This is also semantically ill, as the component uses undetermined times of
 * a single interface `IDisposeEventSink`.
 *
 * #### Method 2: Make every command interface an event sink
 * Another way is to make every command interface an event sink.
 * When a client component wants to use a command interface, it provides
 * an event interface.
 *
 * This method seemingly decouples the provider and client more thoroughly.
 * When the provider is disposed, it disconnects the sink interface from
 * the event of its clients.
 * Each client is notified about the event of disconnection.
 * After the disconnection, when client fires the event, there is natually
 * no sink to perform the task, and there is not dangling pointer.
 *
 * However, it does not make enough distinction between tight and loose
 * coupling that exist objectively among objects.
 * For example, to query a clock about the current time, a component have
 * to fire an event, and obtain the current time returned by the clock.
 * The problem is that, for an event, there can be no or mutliple clocks
 * that react to the event, which would be semantically ill.
 * The tight coupling that the component depends upon a single clock still
 * exists, which is **not** captured by a pure event-based model.
 *
 * The library does not make every interface an event.
 * The seemingly loosely coupled design framework would create an illusion
 * that the "provides/requires" relationship can be eliminated from among
 * components.
 * On the contrary, such design framework actually fails to capture the
 * relationship.
 * the relationships among components are objective, and good programming
 * frameworks shall reveal and make them evident, instead of making them vague.
 *
 * ## Rules of disposal
 * A component exposes `IDisposable` interface, which has a single method
 * `Dispose()`, which **must** perform the following actions:
 * * Become uninitialized and unconfigured.
 * * Cancel all scheduled events.
 * * Disconnect all event connections made by itself.
 * * Dispose all objects created by itself.
 * * Release all used interfaces.
 *
 * There are several aspects to notify:
 * * A component **do not** release or deallocate itself during disposal.
 * * Disposal **shall** be performed by an independent (neither connected nor wired)
 *   component.
 *
 * For example, when an event source invokes an event sink, the event sink
 * must be careful if it wants to dispose the event source, as the event
 * source may still be executing the loop to invoke event sinks.
 *
 * ===================
 * Non-virtual backend
 * ===================
 *
 * Sometimes, to improve performance, it is desired to define a class with
 * *non-virtual* methods that can be reused to either implement the pure-virtual
 * interfaces, or as a building block to implement larger non-virtual classes.
 *
 * The depth of aggregation **must** be as small as possible, since each level of
 * aggregation introduces an extra layer of virtual function calls, and
 * increases the overhead of lifetime management and interface inquiry.
 * For example, assigning a smart pointer needs to increase the reference
 * counter of the object, and several layers of `AddRef()` have to be invoked.
 *
 * In all situations, one **shall** prevent frequent interface inquiry, event
 * connection and disconnection.
 * The rule of thumb is to query interfaces during initialization, and connect
 * events before startup.
 * When smart pointers are passed, use `move` semantic whenever possible.
 *
 * However, such non-virtual class is almost *impossible* to design, since it
 * cannot avoid the dependency upon the component model, and it must deal with
 * lifetime management and support interface inquiry.
 *
 * ## Dependency
 *
 * Dependent objects **must** keep their lifetimes consistent.
 *
 * When a dependent object is destructed, the dependency **must** be ended to
 * prevent other objects from accessing the object that does no longer exist.
 *
 * One of the methods to end dependency is to release smart pointers.
 * However, dependency can be complex.
 *
 * ### Event scheduling
 *
 * To schedule an event, a class provides an event sink and pass it to the
 * scheduler.
 * Usually, the event sink holds an object pointer and a pointer to its member
 * function.
 *
 * When the object is destructed, make sure the event handle is *cancelled*.
 * By doing this, the event sink is *released*, and the dependency is ended.
 * Since the event sink is released from the event handle, the event handle
 * will no longer invoke the member function of the object.
 * Thus, the event sink does not have to be aggregated.
 *
 * ### Event sink
 *
 * The processing of an event can be done by providing a public member function.
 * It can be encapsulated in an event sink by an outer object and provided to
 * other components.
 *
 * To prevent the event sink from using the object after the object has been
 * destructed, make sure the event sink is *disconnected* from the event source.
 * However, the object **may not** hold a smart pointer to the event source.
 * It is **not** possible to ensure the disconnection by the object itself.
 * Thus, an event sink object **must** be aggregated with the outer object.
 *
 * ### Event source
 *
 * An event is provided to allow event sinks to connect and disconnect.
 * An object uses the event to notify the event sinks.
 *
 * When the object is destructed, nothing needs to be done to the event.
 * Even if the event sources are still held by other components, since the
 * object has been destructed, the event will never be fired.
 * Also, it is still safe to connect and disconnect event sinks, as the event
 * is a complete object.
 *
 * If the outer object wants to expose the event, it can expose it as is.
 * However, the exposed event has a *non-standard* behavior, as the event **cannot
 * not** provide other interfaces of the object.
 * Thus, the event does not have to be aggregated.
 *
 * ### Other interfaces
 *
 * If the object **have to** provide interfaces, it have to be a component itself.
 *
 * =========================
 * Component organization
 * =========================
 *
 * The components are organized into a hierarchical structure.
 *
 * ## Root component
 * Each hierarchy has a root component.
 * There can be multiple hierarchies in a program.
 *
 * ## Naming a component
 * A component **can** have a name.
 *
 * ## Access a component
 * A component can be accessed by its hierarchical full path.
 *
 * =============
 * Reflection
 * =============
 *
 * A component can expose meta-data about itself.
 *
 * Meta-data of a component
 * * CID of the component class
 * * IID of the provided interfaces
 *
 * Type-neutral invocation is mainly aimed to enable cross-language
 * inter-operability (scripting language, GUI, etc.).
 *
 * The library provides `IObjectMetaData` interface to provide metadata.
 *
 * ================
 * Interoperability
 * ================
 *
 * Interoperability is a allow users to invoke the methods of interfaces on
 * a component in a type-neutral way.
 *
 * ## Internal to C++
 * It is fairly easy to invoke a method of an interface on a component in C++.
 * * Interfaces (pure abstract classes) are always visible to users.
 * * Users can always obtain the `IObject` interface of a component.
 * * Users can always obtain other interfaces via the `IObject` interface.
 *
 * ## Outside of C++
 * Type-neutral invocability is mainly aimed to enable cross-language
 * inter-operability (scripting language, GUI, etc.).
 *
 * A separate interface can be provided.
 * e.g., `IDispatch`.
 *
 */


NSFX_OPEN_NAMESPACE

/**
 * @ingroup Component
 * @brief The type of cookie.
 *
 * A cookie is obtained when connecting a event sink to an event source, and
 * can be used to disconnect the sink from the event source later.
 */
typedef uint32_t  cookie_t;

NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B

