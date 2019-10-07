/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F
#define OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F


#include <nsfx/component/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/exception.h>
#include <boost/type_index.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <type_traits> // is_base_of
#include <functional>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ObjectBase
/**
 * @ingroup Component
 * @brief The base class for implementing `IObject`.
 *
 * ## 1. Introduction.
 * Usually, an object provides a reference count for lifetime management.
 * When an object wants to implement an interface, it may reuse an existing
 * object that has already implemented the interface.
 * The container creates an instance of the contained object, and delegates
 * the calls on the interface to the contained object.
 *
 * A container have to create *an extra layer of virtual function call*
 * to delegate the calls to the interface on the contained object.
 * i.e., the container derives from the pure abstract interface, and the
 * virtual functions on the interface call the virtual functions implemented
 * by the contained object.
 * It places extra burden upon coding, memory and CPU cycles.
 *
 * @verbatim
 * caller
 *    | virtual call
 *    V
 *    a method on the interface of the container
 *       | virtual call
 *       V
 *       the method on the interface of the contained object
 * @endverbatim
 *
 * The problem is that the container cannot expose the interfaces on the
 * contained object directly to the users.
 * Because the contained object cannot know or expose other interfaces on
 * the container object.
 * Also, from an interface on the contained object, a user cannot obtain
 * other interfaces exposed by the container.
 *
 * This is the primary form of interface implementation: a concrete class
 * inherits a pure abstract interface, and implements the methods on the
 * interface.
 * The main drawback of this approach is that, as the reuse depth goes deeper,
 * more layers of virtual function calls are formed.
 * Such indirection harms the performance of the software system.
 * And designers may be forced to take an effort to limit the reuse depth.
 *
 * Object aggregation allows a container (controller) object to expose
 * the interfaces on the contained (aggregated) object directly to the users.
 *
 * An interface exposed by an aggregated object *acts like* an interface
 * implemented by its controller.
 * A user cannot distinguish whether the interface is implemented by an
 * aggregated object or not.
 *
 * The key idea is to **separate the responsibility** of the aggregated object.
 * The aggregated object is responsible to implement interfaces, while the
 * lifetime management and navigability functions of `IObject` are delegated
 * to the `IObject` on its controller.
 * In order for the controller to query interfaces on the aggregated object,
 * a separate navigator object is responsible to expose the interfaces on the
 * aggregated object.
 *
 * ## 2. Lifetime management.
 * An aggregated object has the same lifetime as its controller.
 * i.e., aggregated objects and its controller share a single reference count.
 * The reference count is provided by the controller.
 * An aggregated object holds a pointer to the `IObject` interface on the
 * controller.
 * Calls to `IObject::AddRef()` and `IObject::Release()` are delegated to
 * the controller.
 *
 * ## 3. Interface navigation.
 * The interface exposed by an aggregated object is exposed directly to users.
 * This frees the controller from having to create an extra layer to access
 * the aggregated object indirectly, which saves codes, memory and CPU cycles.
 *
 * The `IObject` interface on an aggregated object must be able to navigate all
 * other interfaces exposed by the controller.
 * However, only the controller knows the set of interfaces it exposes.
 * So an aggregated object has to delegate calls to `IObject::QueryInterface()`
 * to its controller's `IObject::QueryInterface()`.
 * However, an aggregated object cannot distinguish who calls its
 * `IObject::QueryInterface()`, whether it is a user or its controller.
 * Therefore, the `IObject` on the aggregated object loses the ability to
 * navigate its own interfaces.
 *
 * To solve this problem, an separate *navigator object* is created.
 * It implements `IObject` that exposes the interfaces on the aggregated object.
 * The controller can use the navigator object to query the interfaces on the
 * aggregated object.
 * The controller never exposes this object to others.
 *
 * For a user-defined aggregable class, its instance should be a navigator,
 * not an aggregated object.
 * If it would be an aggregated object, then a user might mistakenly use
 * the object directly without using a navigator.
 * e.g., a controller would be able to create an aggregated object by passing
 * a pointer to its own `IObject`.
 * Then the controller would query an interface directly from the aggregated
 * object, which in turn queries the controller, causing an infinite call loop.
 * Nevertheless, an aggregable object should always be a navigator.
 * A user must not be able to create an aggregated object directly.
 * An aggregated object can only be exposed by its navigator.
 *
 * This creates a coding problem.
 * An object cannot be a navigator, and at the same time an aggregated object,
 * since they have different implementations of `IObject`.
 * Traditionally, a library provides a base class, and let users derive a child
 * class from it, and the child class provides implementations of interfaces.
 * However, such child class cannot be both an implementor and a navigator.
 * A navigator have to be a separate class, possibly provided by the library.
 * To prevent users from mis-using an aggregated class, the aggregated class
 * can be nested within the navigator class with a non-public access right.
 * The library can provide a navigator class template that has user-defined
 * aggregated class as a type parameter.
 *
 * The aggregated object is created alone with (by) the navigator, and exposed
 * to the controller via the navigator's `IObject`.
 * An aggregated object can be a member variable or smart pointer held by
 * the navigator, thus they share the same lifetime.
 * The navigator's `IObject::QueryInterface()` returns the navigator's own
 * `IObject` if `IObject` is queried.
 * If other interfaces are queried, the navigator exposes the interfaces on
 * the aggregated object.
 *
 * The user-defined class implements a non-public `InternalQueryInterface()`
 * member function.
 * The user-defined class derives from a specialized class template,
 * and let the class template hold the type of user-defined class.
 * This way, the class template can call the `InternalQueryInterface()`
 * member function implemented by the user-defined class.
 *
 * ## 5. Techniques.
 * ### 5.1 Template-based virtual function implmentation.
 * A base class provides necessary data and non-virtual functions.
 * A dervied class implements its virtual functions by calling the
 * appropriate non-virtual functions.
 *
 * ### 5.2 Union-based data compression.
 * When there are two pieces of data, and only one of them is used after
 * creation, them can be compressed into a union.
 * This is indeed the case here.
 * When an object is not aggregated, it has a reference count; when it is
 * aggregated, it has a pointer to the controller.
 * Only one piece of data is used after creation, so they can be packed into
 * a union.
 *
 * A non-aggregated object has a reference count.
 * An aggregated object has a pointer to the controller's `IObject`.
 * A poly object uses one of the data according to whether a valid pointer
 * to the controller is specified at creation.
 *
 * ## 6. `ObjectBase`.
 * `ObjectBase` provides necessary data and non-virtual functions for
 * implementing `IObject`.
 * It provides compressed data that can be a reference count or a controller
 * pointer.
 * It provides non-virtual functions to manipulate the data.
 *
 * User-defined classes can focus upon implementing dictated interfaces, and
 * do not implement `IObject` themselves (remain abstract).
 * The library provides `Object` and `AggObject` class templates that argument
 * the user-defined classes with a reference count or a controller pointer,
 * and implement "IObject" as appropriate.
 * `ObjectBase` is intended to be used by the class templates.
 * It is not intended to be constructed directly, thus its methods are not
 * public.
 *
 * Users can use `NSFX_INTERFACE_XXX()` macros to ease the implementation of
 * `InternalQueryInterface()`.
 *
 * Such design does add extra layers of virtual function calls for
 * `IObject::QueryInterface()`.
 * However, most calls to `IObject::QueryInterface()` happen at program or
 * component initialization (component wiring and event sink connection),
 * and it is not likely to cause performance problem.
 *
 * ## 7. Kinds of objects.
 * ### 7.1 `Object`
 * An `Object` has a dynamic lifetime, and is allocated on heap.
 * It owns a reference count itself.
 * When `IObject::Release()` is called, and its reference count reaches `0`, it is
 * deallocated automatically.
 * It is the most common kind of objects.
 *
 * ### 7.2 `StaticObject`
 * The allocation and deallocation are not always desired.
 * e.g., a singleton does not need a dynamic lifetime management, which
 * should be defined as a `static` object.
 * In such cases, the lifetime of the object is meaningless.
 * `StaticObject` can be used for such use cases.
 *
 * A `StaticObject` does not have a reference count.
 * To prevent memory leak, a `StaticObject` must be defined as a `static` variable.
 *
 * ### 7.3 `AggObject`
 * An `AggObject` is an aggregable object.
 * i.e., the navigator of an aggregated object.
 * It holds a pointer to its controller.
 * The `QueryInterface()` method returns the navigator itself if `IObject`
 * is queried, while delegates all other queries to its controller.
 * It must not be deallocated until the controller's lifetime ends.
 * It holds an aggregated object that shares the same reference count with
 * its controller.
 *
 * ### 7.4 `MemberAggObject`
 * The lifetime of an `AggObject` is quite similar to a member variable of
 * its controller.
 * Actually, a member variable of type `Ptr<IObject>` is used to manage
 * the lifetime of an `AggObject`.
 * Allocating an `AggObject` on heap is a good way to encapsulate/hide the
 * user-defined class away from the controller.
 * However, it is not efficient when the user-defined class is known to the
 * controller.
 * `MemberAggObject` can be used for such use cases.
 *
 * A `MemberAggObject` does not have a reference count.
 * A `MemberAggObject` must be defined as a member variable of a controller class.
 * The lifetime of a `MemberAggObject` is naturally the same as its controller.
 *
 * ### 7.5 `MutualObject`
 * One of the limitations of the component model is that a component cannot
 * expose the same interface multiple times with different implementations.
 *
 * For example, when a component wants to schedule multiple event sinks via
 * an event scheduler, it faces a problem that these event sinks share
 * the same interface (`IEventSink`) but have different implementations.
 *
 * The first problem is that a component cannot expose an event sink interface
 * for multiple times by aggregation, nor by inheritance.
 * A natural solution is to create separate objects (event sinks) on heap that
 * implement the event sink interface, and connect these event sinks to
 * the event sources.
 *
 * Another problem is that the component must have a lifetime as long as
 * the event sinks.
 * i.e., as long as an event source holds an event sink, the component must
 * not be deallocated.
 * The key is to let each event sink hold a reference count of the component.
 * i.e., let each event sink hold a `Ptr` that points to the component.
 * However, this is not efficient.
 *
 * In most cases, an event source does not query other interfaces from
 * an event sink.
 * Therefore, the navigability of an event sink is not important.
 * What is important is that an event sink shares the same lifetime
 * (reference count) of the component.
 * `MutualObject` can be used in such cases.
 *
 * A `MutualObject` shares the same reference count with its controller that
 * provides the `MutualObject` to other components.
 * However, the controller does not expose the interfaces on `MutualObject`, nor
 * does the `MutualObject` expose the interfaces on its controller.
 * They seem like two different components from the outside.
 *
 * Usually, one must not hold a pointer to a member variable of a dynamic
 * object, since the deallocation of the object invalidates the pointer, and
 * a dangling pointer is deadly.
 * However, a `MutualObject` can be defined as a member variable of the
 * controller, since a `MutualObject` shares the same lifetime with the
 * controller.
 *
 * The controller is free to provide to other components smart pointers that
 * point to the `MutualObject`, although it is a member variable.
 * The point is that a smart pointer to the `MutualObject` actually holds
 * a reference count of the controller.
 * Thus, as long as a smart pointer to the `MutualObject` exists, the controller
 * also exists.
 *
 * @see `NSFX_INTERFACE_MAP_BEGIN`,
 *      `NSFX_INTERFACE_ENTRY`,
 *      `NSFX_INTERFACE_AGGREGATED_ENTRY`,
 *      `NSFX_INTERFACE_MAP_END`.
 */
class ObjectBase/*{{{*/
{
protected:
    ObjectBase(void) BOOST_NOEXCEPT :
        controller_(nullptr)
    {
    }

    ObjectBase(IObject* controller) BOOST_NOEXCEPT :
        controller_(controller)
    {
    }

    refcount_t InternalAddRef(void) BOOST_NOEXCEPT
    {
        refcount_t result = ++refCount_;
        return result;
    }

    refcount_t InternalRelease(void) BOOST_NOEXCEPT
    {
        refcount_t result = --refCount_;
        return result;
    }

    // Defined via NSFX_INTERFACE_XXX() macros.
    // void* InternalQueryInterface(const Uid& iid);

    refcount_t ControllerAddRef(void)
    {
        return controller_->AddRef();
    }

    refcount_t ControllerRelease(void)
    {
        return controller_->Release();
    }

    void* ControllerQueryInterface(const Uid& iid)
    {
        return controller_->QueryInterface(iid);
    }

private:
    union
    {
        refcount_t refCount_;   /// The reference count.
        IObject*   controller_; /// The controller.
    };
}; // class ObjectBase /*}}}*/


struct InterfaceTag  {};
struct AggregatedTag {};


////////////////////////////////////////////////////////////////////////////////
// ObjectImplConcept
/**
 * @ingroup Component
 * @brief ObjectImpl concept.
 *
 * A class conforms to the concept if it satisfies the following conditions.
 * 1. It conforms to `IObjectConcept`.
 * 2. It has a non-private `InternalQueryInterface()` function.
 * 3. It is not derived from `ObjectBase`.
 */
template<class T>
struct ObjectImplConcept/*{{{*/
{
    static_assert(!std::is_base_of<ObjectBase, T>::value,
                  "An ObjectImpl class must not derive from ObjectBase.");

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

    BOOST_CONCEPT_USAGE(ObjectImplConcept)
    {
        HasNonPrivateInternalQueryInterface();
    }

    void HasNonPrivateInternalQueryInterface(void)
    {
        struct Child : T
        {
            // T has a private InternalQueryInterface().
            void* InternalQueryInterface(const Uid& iid)
            {
                return T::InternalQueryInterface(iid);
            }
        };

        Child* child = nullptr;
        void* iobj = child->InternalQueryInterface(uid_of<IObject>());
    }

}; // class ObjectImplConcept /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Object
/**
 * @ingroup Component
 * @brief A non-aggregable object that must be allocated on heap.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * A non-aggretable object possesses a reference counter.
 * However, it does not hold a pointer to a controller, thus it cannot be
 * managed by a controller.
 *
 * An `Object` will deallocate itself when its reference count reaches `0`.
 * Therefore, do not define an `Object` as a static variable or a member
 * variable.
 *
 * The `ObjectImpl` **must** define `InternalQueryInterface()` via `NSFX_INTERFACE_XXX()`
 * macros.
 *
 * ### When to use?
 *     The `Object` can be used everywhere.
 * ### How to use?
 *     An `Object` must be allocated on heap.
 *     i.e., `Ptr` shall be used to hold an `Object`.
 */
template<class ObjectImpl>
class Object NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public ObjectImpl
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    Object(Args&&... args) :
        ObjectImpl(std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    Object(void) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~Object(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Object(const Object& ));
    BOOST_DELETED_FUNCTION(Object& operator=(const Object& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = InternalAddRef();
        return result;
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = InternalRelease();
        if (!result)
        {
            delete this;
        }
        return result;
    }

    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return ObjectImpl::InternalQueryInterface(iid);
    }

    /*}}}*/

    // Methods.
    ObjectImpl* GetImpl(void) BOOST_NOEXCEPT
    {
        return static_cast<ObjectImpl*>(this);
    }

}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A non-aggregable object that must be used as a static variable.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * A `StaticObject` does not have a reference count.
 * Thus, do not allocate a `StaticObject` on heap, since `Ptr` cannot deallocate
 * it automatically.
 * It must be defined as a static variable to prevent memory leak.
 * e.g. a singleton can be defined as a static `StaticObject`, and there is
 * no need to allocate it on heap.
 *
 * The `ObjectImpl` **must** define `InternalQueryInterface()` via `NSFX_INTERFACE_XXX()`
 * macros.
 *
 * ### When to use?
 *     The `StaticObject` can be used when an object has a static lifetime.
 * ### How to use?
 *     A `StaticObject` must be defined as a `static` variable.
 * ### Benefits.
 *     It saves a `Ptr` to manage lifetime.
 *     There is no need to allocate on heap.
 */
template<class ObjectImpl>
class StaticObject NSFX_FINAL :/*{{{*/
    public ObjectImpl
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    StaticObject(Args&&... args) :
        ObjectImpl(std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    StaticObject(void) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~StaticObject(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(StaticObject(const StaticObject& ));
    BOOST_DELETED_FUNCTION(StaticObject& operator=(const StaticObject& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return 1;
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return 1;
    }

    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return ObjectImpl::InternalQueryInterface(iid);
    }

    /*}}}*/

    // Methods.
    ObjectImpl* GetImpl(void) BOOST_NOEXCEPT
    {
        return static_cast<ObjectImpl*>(this);
    }

}; // class StaticObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A aggregated object used internally by `MemberAggObject` and `AggObject`.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * Users cannot use this class directly.
 */
template<class ObjectImpl>
class Aggregated NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public ObjectImpl
{
private:
    template<class ObjectImpl2> friend class AggObject;
    template<class ObjectImpl2> friend class MemberAggObject;

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    Aggregated(IObject* controller, Args&&... args) :
        ObjectBase(controller),
        ObjectImpl(std::forward<Args>(args)...)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    Aggregated(IObject* controller) :
        ObjectBase(controller)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
    }

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 2))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~Aggregated(void) {}

// Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Aggregated(const Aggregated& ));
    BOOST_DELETED_FUNCTION(Aggregated& operator=(const Aggregated& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerAddRef();
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerRelease();
    }

    /**
     * @brief Exposes interfaces implemented by the aggregated object.
     */
    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerQueryInterface(iid);
    }

    /*}}}*/

    // Make the protected ObjectImpl::InternalQueryInterface() a public member
    // of Aggregated class.
    void* InternalQueryInterface(const Uid& iid)
    {
        return ObjectImpl::InternalQueryInterface(iid);
    }

}; // class Aggregated /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregable object that must be allocated on heap.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * The aggregated object has the same lifetime as the navigator.
 * So the navigator also has the same lifetime as the controller.
 * Therefore, the controller **must not** deallocate the navigator until
 * the controller itself is deallocated.
 *
 * A controller shall define a member varaible of type `Ptr<IObject>` to hold
 * an `AggObject`, i.e., the navigator.
 * This smart pointer is never offer to other objects.
 * And it is deallocated along with the controller.
 *
 * The `ObjectImpl` **must** define `InternalQueryInterface()` via `NSFX_INTERFACE_XXX()`
 * macros.
 *
 * ### When to use?
 *     The `AggObject` can be used by a controller to aggregate an object.
 * ### How to use?
 *     An `AggObject` must be allocated on heap.
 *     The controller shall define a member variable of type `Ptr<IObject>` or
 *     `Ptr<AggObject<ObjectImpl>>` to hold the allocated `AggObject`.
 *     (The rule is that smart pointer that holds the allocated `AggObject`
 *     does NOT query any of its interfaces except for `IObject`.)
 *     This pointer must not be given to other objects.
 *     The `AggObject` lives until the controller is destructed.
 *     The controller must expose the interfaces on `ObjectImpl` as its own
 *     interfaces.
 * ### Benefits.
 *     The interfaces on an aggregated object are exposed directly by the
 *     controller.
 *
 * @verbatim
 * aggregable object (navigator, Ptr<IObject>)
 *    | exposes
 *    V
 *    interfaces on the aggregated object
 *                         A
 * controller              |
 *    | exposes            | aggregates a subset of
 *    V                    |
 *    interfaces on the controller
 * @endverbatim
 *
 * @remarks The interfaces on an aggregated object must be queried from its
 * navigator.
 * The interfaces on the controller shall be queried from 'this' pointer.
 * The rule of thumb is that a controller shall never query an interface
 * from an *aggregated interface*.
 *
 * @remarks The following example shows a pit-fall when trying to obtain
 * interfaces on an aggregated object or on a controller.
 * @code{.cpp}
 * // MyObject exposes IXxx, IYyy and IZzz
 * struct MyObject : IXxx, IYyy, IZzz
 * {
 *     NSFX_INTERFACE_MAP_BEGIN(MyObject)
 *         NSFX_INTERFACE_ENTRY(IXxx)
 *         NSFX_INTERFACE_ENTRY(IYyy)
 *         NSFX_INTERFACE_ENTRY(IZzz)
 *     NSFX_INTERFACE_MAP_END()
 *     ...
 * };
 *
 * // Controller also exposes IXxx and IYyy
 * struct Controller : IXxx
 * {
 *     Ptr<IObject>  agg_;
 *
 *     NSFX_INTERFACE_MAP_BEGIN(Controller)
 *         NSFX_INTERFACE_ENTRY(IXxx)
 *         NSFX_INTERFACE_AGGREGATED_ENTRY(IYyy, agg_)
 *     NSFX_INTERFACE_MAP_END()
 *
 *     void Initialize(void)
 *     {
 *         agg_ = CreateObject<IObject>(cid_of_MyObject, this);
 *
 *         // The IXxx on MyObject
 *         Ptr<IXxx> x_on_MyObject(agg_);
 *
 *         // The IZzz on MyObject
 *         Ptr<IZzz> z_on_MyObject(agg_);
 *
 *         // The aggregated IYyy on both MyObject and Controller
 *         // However, IYyy shall be considered as an interface on
 *         // Controller.
 *         Ptr<IYyy> y(agg_);
 *
 *         // Cannot query IZzz from y !
 *         // It is salvaged as 'NoInterface' exception is thrown.
 *         Ptr<IZzz> z(y);
 *
 *         // Pit-fall !!!
 *         // This is he IXxx on Controller, not the IXxx on MyObject.
 *         // Since y is the IYyy on MyObject, it is not obvious which
 *         // IXxx is queried.
 *         Ptr<IXxx> x_not_on_MyObject(y);
 *
 *         // The recommended usage
 *         Ptr<IXxx> x_on_Controller(this);
 *     }
 * };
 * @endcode
 */
template<class ObjectImpl>
class AggObject NSFX_FINAL :/*{{{*/
    public IObject,
    private ObjectBase // Use private inherit as the methods are only used internally.
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    AggObject(IObject* controller, Args&&... args) :
        agg_(controller, std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    AggObject(IObject* controller) :
        agg_(controller) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 3))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~AggObject(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(AggObject(const AggObject& ));
    BOOST_DELETED_FUNCTION(AggObject& operator=(const AggObject& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = InternalAddRef();
        return result;
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = InternalRelease();
        if (!result)
        {
            delete this;
        }
        return result;
    }

    /**
     * @brief Query an interface from the navigator.
     *
     * @return If the `iid` is the UID of `IObject`, return the `IObject`
     *         on the navigator itself.
     *         Otherwise, return the interface on the aggregated object.
     */
    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == uid_of<IObject>())
        {
            result = static_cast<IObject*>(this);
            InternalAddRef();
        }
        else
        {
            result = agg_.InternalQueryInterface(iid);
        }
        return result;
    }

    /*}}}*/

    // Methods.
    ObjectImpl* GetImpl(void) BOOST_NOEXCEPT
    {
        return static_cast<ObjectImpl*>(&agg_);
    }

private:
    Aggregated<ObjectImpl> agg_;  /// The aggregated object.

}; // class AggObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// MemberAggObject
/**
 * @ingroup Component
 * @brief An aggregable object that must be used as a member variable of its controller.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * A member aggregable object does not have a reference count.
 * Thus, **do not** allocate a `MemberAggObject` on heap, since `Ptr` cannot deallocate
 * it automatically.
 * It must be defined as a member variable of the controller to prevent memory
 * leak.
 *
 * The `ObjectImpl` **must** define `InternalQueryInterface()` via `NSFX_INTERFACE_XXX()`
 * macros.
 *
 * ### When to use?
 *     The `MemberAggObject` can be used by a controller to aggregate an object.
 * ### How to use?
 *     A `MemberAggObject` must be defined as a member variable of
 *     the controller class.
 *     The controller must expose the interfaces on `ObjectImpl` as its own
 *     interfaces.
 * ### Benefits.
 *     The interfaces on an aggregated object are exposed directly by the
 *     controller.
 *     A `MemberAggObject` naturally has the same lifetime as the controller,
 *     which is similar to an `AggObject`.
 *     It saves a `Ptr` to manage lifetime.
 *     There is no need to allocate on heap.
 */
template<class ObjectImpl>
class MemberAggObject NSFX_FINAL :/*{{{*/
    public IObject
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    MemberAggObject(IObject* controller, Args&&... args) :
        agg_(controller, std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    MemberAggObject(IObject* controller) :
        agg_(controller) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 4))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~MemberAggObject(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(MemberAggObject(const MemberAggObject& ));
    BOOST_DELETED_FUNCTION(MemberAggObject& operator=(const MemberAggObject& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return 1;
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return 1;
    }

    /**
     * @brief Query an interface from the navigator.
     *
     * @return If the `iid` is the UID of `IObject`, return the `IObject`
     *         on the navigator itself.
     *         Otherwise, return the interface on the aggregated object.
     */
    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == uid_of<IObject>())
        {
            result = static_cast<IObject*>(this);
        }
        else
        {
            result = agg_.InternalQueryInterface(iid);
        }
        return result;
    }

    /*}}}*/

    // Methods.
    ObjectImpl* GetImpl(void) BOOST_NOEXCEPT
    {
        return static_cast<ObjectImpl*>(&agg_);
    }

private:
    Aggregated<ObjectImpl> agg_;  /// The aggregated object.

}; // class MemberAggObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// MutualObject
/**
 * @ingroup Component
 * @brief A seemingly separate object that can be used a member variable of its controller.
 *
 * @tparam ObjectImpl A class that conforms to `ObjectImplConcept`.
 *
 * The `ObjectImpl` **must** define `InternalQueryInterface()` via `NSFX_INTERFACE_XXX()`
 * macros.
 *
 * ### When to use?
 *     The `MutualObject` can be used by a controller to provide an object that
 *     exposes a separate set of interfaces.
 * ### How to use?
 *     A `MutualObject` can be defined as a member variable of the controller
 *     The controller can provide to other objects smart pointers that point to
 *     the `MutualObject`.
 *     The controler must not expose the interfaces on a `MutualObject` as its
 *     own interfaces.
 * ### Benefits.
 *     The controller lives as long as other objects hold smart pointers to
 *     the `MutualObject`.
 *     It saves a `Ptr` to manage lifetime, if the `MutualObject` is defined as
 *     a member variable.
 *     There is no need to allocate on heap.
 */
template<class ObjectImpl>
class MutualObject NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public ObjectImpl
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    MutualObject(IObject* controller, Args&&... args) :
        ObjectBase(controller),
        ObjectImpl(std::forward<Args>(args)...)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    MutualObject(IObject* controller) :
        ObjectBase(controller)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
    }

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 5))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~MutualObject(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(MutualObject(const MutualObject& ));
    BOOST_DELETED_FUNCTION(MutualObject& operator=(const MutualObject& ));

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerAddRef();
    }

    virtual refcount_t Release(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerRelease();
    }

    /**
     * @brief Exposes interfaces.
     *
     * The interfaces on the controller is not exposed.
     */
    virtual void* QueryInterface(const Uid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return ObjectImpl::InternalQueryInterface(iid);
    }

    /*}}}*/

    // Methods.
    ObjectImpl* GetImpl(void) BOOST_NOEXCEPT
    {
        return static_cast<ObjectImpl*>(this);
    }

}; // class MutualObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief A visitor to visit the interface map.
 *
 * Implements `InternalQueryInterface()`.
 */
class InternalQueryInterfaceVisitor
{
public:
    InternalQueryInterfaceVisitor(const Uid& iid, void*& result) :
        iid_(iid),
        result_(result)
    {}

    template<class Intf>
    bool operator()(const Uid& iid, Intf* intf, InterfaceTag)
    {
        bool found = (iid_ == iid);
        if (found)
        {
            intf->AddRef();
            result_ = intf;
        }
        return found;
    }

    bool operator()(const Uid& iid, IObject* navi, AggregatedTag)
    {
        bool found = (iid_ == iid);
        if (found)
        {
            result_ = navi->QueryInterface(iid);
        }
        return found;
    }

private:
    const Uid& iid_;
    void*& result_;
};


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// NSFX_INTERFACE_XXX() macros./*{{{*/
/**
 * @ingroup Component
 * @brief Begin an interface map.
 *
 * @param ThisClass The class that defines the interface map.
 *                  It is the class that implements or aggregates interfaces.
 *
 * @remarks The macros defines a public member function template
 * `InternalVisitInterfaceMap()` for the component class.
 * The template parameter `Visitor` must be a functor class that supports
 * tag-based function dispatch.
 *
 * @remarks For example,
 * @code{.cpp}
 * struct Visitor
 * {
 *     // @param[in] iid  The UID of the interface Intf.
 *     // @param[in] intf The pointer to the interface.
 *     template<class Intf>
 *     bool operator()(const Uid& iid, Intf* intf, InterfaceTag);
 *
 *     // @param[in] iid  The UID of the aggregated interface.
 *     // @param[in] navi The pointer to the navigator.
 *     bool operator()(const Uid& iid, IObject* navi, AggregatedTag);
 * };
 * @endcode
 *
 * @remarks The functor (visitor) returns `false` to conitnue the visiting.
 * Or it returns `true` to terminate the visiting.
 *
 * @remarks The funtion template returns the return value of visitor.
 * i.e., the function template returns `true` if the visiting is terminated
 * by the visitor.
 * Otherwise, it returns `false`.
 */
#define NSFX_INTERFACE_MAP_BEGIN(ThisClass)                                   \
    private:                                                                  \
        typedef ThisClass  ThisClass_;                                        \
    public:                                                                   \
        template<class Visitor>                                               \
        bool InternalVisitInterfaceMap(Visitor&& visitor)                     \
        {                                                                     \
            bool result = true;                                               \
            do                                                                \
            {                                                                 \
                static_assert(                                                \
                    ::std::is_base_of<::nsfx::IObject, ThisClass_>::value,    \
                    "Cannot expose an unimplemented interface");              \
                if (visitor(::nsfx::uid_of<::nsfx::IObject>(),                \
                            static_cast<::nsfx::IObject*>(this),              \
                            ::nsfx::InterfaceTag()))                          \
                {                                                             \
                    break;                                                    \
                }

/**
 * @ingroup Component
 * @brief Expose an interface implemented by the `ThisClass` that defines the interface map.
 *
 * @param Intf The type of the interface.
 *             `ThisClass` that defines the interface map must derive from `Intf`.
 *             It **should not** be `IObject`.
 */
#define NSFX_INTERFACE_ENTRY(Intf)                                 \
                static_assert(                                     \
                    ::std::is_base_of<Intf, ThisClass_>::value,    \
                    "Cannot expose an unimplemented interface");   \
                if (visitor(::nsfx::uid_of<Intf>(),                \
                            static_cast<Intf*>(this),              \
                            ::nsfx::InterfaceTag()))               \
                {                                                  \
                    break;                                         \
                }

/**
 * @ingroup Component
 * @brief Expose an interface implemented by an aggregated object.
 *
 * @param Intf The type of the interface.
 *             It **should not** be "IObject".
 * @param navi It must be a **raw** poiner to an aggregable object that exposes
 *             the `Intf` interface.
 *             i.e., the object implements `IObject`, and `Intf` can be queried
 *             from the object.
 *             The object must be accessible within the component class.
 */
#define NSFX_INTERFACE_AGGREGATED_ENTRY(Intf, navi)               \
                if (visitor(::nsfx::uid_of<Intf>(),               \
                            static_cast<::nsfx::IObject*>(navi),  \
                            ::nsfx::AggregatedTag()))             \
                {                                                 \
                    break;                                        \
                }

/**
 * @ingroup Component
 * @brief End an interface map.
 *
 * Uses `InternalQueryInterfaceVisitor` functor class to implement
 * `InternalQueryInterface()`.
 *
 * @remarks The macros changes the access rights to `protected`.
 */
#define NSFX_INTERFACE_MAP_END()                                      \
                result = false;                                       \
            }                                                         \
            while (false);                                            \
            return result;                                            \
        } /* bool InternalVisitInterfaceMap() */                      \
    protected:                                                        \
        void* InternalQueryInterface(const ::nsfx::Uid& iid)          \
        {                                                             \
            void* result = nullptr;                                   \
            InternalVisitInterfaceMap(                                \
                ::nsfx::InternalQueryInterfaceVisitor(iid, result));  \
            if (!result)                                              \
            {                                                         \
                BOOST_THROW_EXCEPTION(                                \
                    ::nsfx::NoInterface()                             \
                    << ::nsfx::QueriedClassErrorInfo(                 \
                        ::boost::typeindex::type_id<ThisClass_>()     \
                            .pretty_name())                           \
                    << ::nsfx::QueriedInterfaceUidErrorInfo(iid)      \
                );                                                    \
            }                                                         \
            return result;                                            \
        }

/*}}}*/


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F


#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0

// template<class A0, class A1, ...>
// Object(A0&& a0, A1&& a1, ...)) :
//     ObjectImpl(std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
Object(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectImpl(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 1

// template<class A0, class A1, ...>
// StaticObject(A0&& a0, A1&& a1, ...)) :
//     ObjectImpl(std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
StaticObject(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectImpl(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 2

// template<class A0, class A1, ...>
// Aggregated(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     ObjectBase(controller),
//     ObjectImpl(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
Aggregated(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectBase(controller),
    ObjectImpl(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{
    if (!controller)
    {
        BOOST_THROW_EXCEPTION(BadAggregation());
    }
}

# elif BOOST_PP_ITERATION_FLAGS() == 3

// template<class A0, class A1, ...>
// AggObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     agg_(controller, std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
AggObject(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    agg_(controller, BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 4

// template<class A0, class A1, ...>
// MemberAggObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     agg_(controller, std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
MemberAggObject(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    agg_(controller, BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 5

// template<class A0, class A1, ...>
// MutualObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     ObjectBase(controller),
//     ObjectImpl(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
MutualObject(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectBase(controller),
    ObjectImpl(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{
    if (!controller)
    {
        BOOST_THROW_EXCEPTION(BadAggregation());
    }
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

