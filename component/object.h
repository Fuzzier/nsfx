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
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <functional>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ObjectBase
/**
 * @ingroup Component
 * @brief The base class for implementing \c IObject.
 *
 * ## 1. Introduction.<br/>
 * Usually, an object provides a reference count for lifetime management.<br/>
 * When an object want to implement an interface, it may reuse an existing
 * object that has already implemented the interface.<br/>
 * The container creates an instance of the contained object, and delegates the
 * calls on the interface to the contained object.<br/>
 *
 * The problem is that the container cannot expose the interfaces on the
 * contained object directly to the users.<br/>
 * Because the contained object cannot know or expose other interfaces on the
 * container object.<br/>
 * Thus, from an interface on the contained object, a user cannot obtain other
 * interfaces exposed by the container.<br/>
 * Therefore, a container have to create <i>an extra layer of virtual function
 * call</i> to delegate the interface to the contained object.<br/>
 * i.e., the container derives from the pure abstract interface, and the virtual
 * functions on the interface call the virtual functions implemented by the
 * contained object.<br/>
 * It places extra burden upon coding, memory and CPU cycles.<br/>
 *
 * This is the primary form of interface implementation: a concrete class
 * inherits a pure abstract interface, and implements the methods on it.<br/>
 * The main drawback of this approach is that, as the reuse depth goes deeper,
 * more layers of virtual function calls are formed.<br/>
 * Such indirection harms the performance of the software system.<br/>
 * And designers have to take an effort to limit the reuse depth.<br/>
 *
 * Object aggregation allows a container (controller) object to expose the
 * interfaces on the contained (aggregated) object directly to the users.<br/>
 *
 * An interface exposed by an aggregated object <i>acts like</i> an interface
 * implemented by its controller.<br/>
 * A user cannot distinguish whether the interface is implemented by an
 * aggregated object or not.<br/>
 *
 * The key idea is to <b>separate the responsibility</b> of the aggregated
 * object.<br/>
 * The aggregated object is responsible to implement interfaces, while the
 * lifetime management and navigability functions of \c IObject are delegated
 * to the \c IObject on its controller.<br/>
 * In order for the controller to query interfaces on the aggregated object,
 * a separate navigator object is responsible to expose the interfaces on the
 * aggregated object.<br/>
 *
 * ## 2. Lifetime management.<br/>
 * An aggregated object has the same lifetime as its controller.<br/>
 * i.e., aggregated objects and its controller share a single reference count.<br/>
 * The reference count is provided by the controller.<br/>
 * An aggregated object holds a pointer to the \c IObject interface on the
 * controller.<br/>
 * Calls to \c IObject::AddRef() and \c IObject::Release() are delegated to the
 * controller.<br/>
 *
 * ## 3. Interface navigation.<br/>
 * The interface exposed by an aggregated object is exposed directly to users.<br/>
 * This frees the controller from having to create an extra layer to access the
 * aggregated object indirectly, which saves codes, memory and CPU cycles.<br/>
 *
 * The \c IObject interface on an aggregated object must be able to navigate all
 * other interfaces exposed by the controller.<br/>
 * However, only the controller knows the set of interfaces it exposes.<br/>
 * So an aggregated object has to delegate calls to \c IObject::QueryInterface()
 * to its controller's \c IObject::QueryInterface().<br/>
 * However, an aggregated object cannot distinguish who calls its
 * \c IObject::QueryInterface(), whether it is a user or its controller.<br/>
 * Therefore, the \c IObject on the aggregated object loses the ability to
 * navigate its own interfaces.<br/>
 *
 * To solve this problem, an separate <b>navigator object</b> is created.<br/>
 * It implements \c IObject that exposes the interfaces on the aggregated object.<br/>
 * The controller can use the navigator object to query the interfaces on the
 * aggregated object.<br/>
 * The controller never exposes this object to others.<br/>
 *
 * For a user-defined aggregable class, its instance should be a navigator, not
 * an aggregated object.<br/>
 * If it would be an aggregated object, then a user might mistakenly use the
 * object directly without using a navigator.<br/>
 * e.g., a controller would be able to create an aggregated object by passing a
 * pointer to its own \c IObject.<br/>
 * Then the controller would query an interface directly from the aggregated
 * object, which in turn queries the controller, causing an infinite call loop.<br/>
 * Nevertheless, an aggregable object should always be a navigator.<br/>
 * A user must not be able to create an aggregated object directly.<br/>
 * An aggregated object can only be exposed by its navigator.<br/>
 *
 * This creates a coding problem.<br/>
 * An object cannot be a navigator, and at the same time an aggregated object,
 * since they have different implementations of \c IObject.<br/>
 * Traditionally, a library provides a base class, and let users derive a child
 * class from it, and the child class provides implementations of interfaces.<br/>
 * However, such child class cannot be both an implementor and a navigator.<br/>
 * A navigator have to be a separate class, possibly provided by the library.<br/>
 * To prevent users from mis-using an aggregated class, the aggregated class can
 * be nested within the navigator class with a non-public access right.<br/>
 * The library can provide a navigator class template that has user-defined
 * aggregated class as a type parameter.<br/>
 *
 * The aggregated object is created alone with (by) the navigator, and exposed
 * to the controller via the navigator's \c IObject.<br/>
 * An aggregated object can be a member variable or smart pointer held by the
 * navigator, thus they share the same lifetime.<br/>
 * The navigator's \c IObject::QueryInterface() returns the navigator's own
 * \c IObject if \c IObject is queried.<br/>
 * If other interfaces are queried, the navigator exposes the interfaces on
 * the aggregated object.<br/>
 *
 * The user-defined class implements a non-public \c InternalQueryInterface()
 * member function.<br/>
 * The user-defined class derives from a specialized class template,
 * and let the class template hold the type of user-defined class.<br/>
 * This way, the class template can call the \c InternalQueryInterface() member
 * function implemented by the user-defined class.<br/>
 *
 * ## 5. Techniques.<br/>
 * ### 5.1 Template-based virtual function implmentation.
 * A base class provides necessary data and non-virtual functions.<br/>
 * A dervied class implements its virtual functions by calling the appropriate
 * non-virtual functions.<br/>
 *
 * ### 5.2 Union-based data compression.
 * When there are two pieces of data, and only one of them is used after
 * creation, them can be compressed into a union.<br/>
 * This is indeed the case here.
 * When an object is not aggregated, it has a reference count; when it is
 * aggregated, it has a pointer to the controller.
 * Only one piece of data is used after creation, so they can be packed into
 * a union.
 *
 * let one provide data, and the other virtual inherit from the former, and .
 *
 * A non-aggregated object has a reference count.<br/>
 * An aggregated object has a pointer to the controller's \c IObject.<br/>
 * A poly object uses one of the data according to whether a valid pointer to
 * the controller is specified at creation.<br/>
 *
 * ## 6. \c ObjectBase.<br/>
 * \c ObjectBase provides necessary data and non-virtual functions for
 * implementing \c IObject.<br/>
 * It provides compressed data that can be a reference count or a controller
 * pointer.<br/>
 * It provides non-virtual functions to manipulate the data.<br/>
 *
 * User-defined object classes can focus upon implementing dictated interfaces,
 * and do not implement \c IObject themselves (remain abstract).<br/>
 * The library provides <code>Object<></code> and <code>AggObject<></code>
 * class templates to envelope the user-defined classes, equipping user-defined
 * classes with a reference count or a controller pointer, and implementing
 * \c IObject as appropriate.<br/>
 * \c ObjectBase is intended to be used by the class templates.<br/>
 * It is not intended to be constructed directly, thus its methods are not
 * public.<br/>
 *
 * Users can use \c NSFX_INTERFACE_XXX() macros to ease the implementation of
 * \c NavigatorQueryInterface().<br/>
 *
 * Such design does add extra layers of virtual function calls for
 * \c IObject::QueryInterface().<br/>
 * However, most calls to \c IObject::QueryInterface() happen at program or
 * component initialization, and it is not likely to cause performance problem.
 *
 * 7. Kinds of objects.<br/>
 * 7.1 \c Object
 *     An \c Object has a dynamic lifetime, and is allocated on heap.<br/>
 *     It owns a reference count itself.<br/>
 *     When \c IObject::Release() is called, and its reference count reaches
 *     \c 0, it is deallocated automatically.<br/>
 *     It is the most common kind of objects.
 *
 * 7.2 \c StaticObject
 *     The allocation and deallocation are not always desired.<br/>
 *     e.g., a singleton does not need a dynamic lifetime management, which
 *     should be defined as a \c static object.<br/>
 *     In such cases, the lifetime of the object is meaningless.<br/>
 *     \c StaticObject can be used for such use cases.<br/>
 *     <p>
 *     A \c StaticObject does not have a reference count.<br/>
 *     To prevent memory leak, a \c StaticObject must be defined as a \c static
 *     variable.
 *
 * 7.3 \c AggObject
 *     An \c AggObject is an aggregable object.<br/>
 *     i.e., the navigator of an aggregated object.<br/>
 *     The \c AggObject owns a reference count itself.<br/>
 *     Thus, it can be allocated and deallocated dynamically.<br/>
 *     However, its reference count is held only by its controller.<br/>
 *     And it is deallocated when the controller's lifetime ends.<br/>
 *     It holds an aggregated object that shares the same reference count with
 *     its controller.<br/>
 *
 * 7.4 \c MemberObject
 *     The lifetime of an \c AggObject is quite similar to a member variable of
 *     its controller.<br/>
 *     Actually, a member variable of type <code>Ptr<IObject></code> is used to
 *     manage the lifetime of an \c AggObject.<br/>
 *     Allocating an \c AggObject on heap is a good way to encapsulate/hide the
 *     user-defined class away from the controller.<br/>
 *     However, it is not efficient when the user-defined class is known to the
 *     controller.<br/>
 *     \c MemberObject can be used for such use cases.
 *     <p>
 *     A \c MemberObject does not have a reference count.<br/>
 *     Except for this, it is the same as an \c AggObject.<br/>
 *     A \c MemberObject must be defined as a member variable of a controller
 *     class.<br/>
 *     Thus, the lifetime of a \c MemberObject is naturally the same as its
 *     controller.<br/>
 *
 * 7.5 \c LinkedObject
 *     The navigability is not always desired.<br/>
 *     One of the limitations of the component model is that a component cannot
 *     expose an interface multiple times with different implementations.<br/>
 *     e.g., when a component needs to provide multiple event sinks to various
 *     event sources, it faces a problem that these event sinks share the same
 *     interface but have different implementations.<br/>
 *     In such cases, the component cannot expose the event sinks by using the
 *     component model described so far.<br/>
 *     Another problem is that the component must have a lifetime as long as the
 *     event sinks.<br/>
 *     i.e., as long as an event source holds an event sink, the component must
 *     not be deallocated.<br/>
 *     <p>
 *     The key is to let each event sink hold a reference count of the component.<br/>
 *     This is can be done by letting each event sink hold a <code>Ptr<></code>
 *     that points to the component.<br/>
 *     However, this is not efficient.<br/>
 *     Usually, the navigability of an event sink is not important, as in most
 *     cases an event source does not query other interfaces from an event sink.<br/>
 *     \c LinkedObject can be used in such use cases.
 *     <p>
 *     A \c LinkedObject shares the same reference count with its controller,
 *     i.e., the component that provides the object to other components.<br/>
 *     However, the controller does not expose the interfaces on \c LinkedObject,
 *     and the \c LinkedObject does not expose the interfaces on its controller.<br/>
 *     They seem like two different components from outside, as the navigability
 *     is not provided.<br/>
 *     <p>
 *     Usually, one must not hold a pointer to a member variable of a dynamic
 *     object, since the deallocation of the object invalidates the pointer,
 *     and a dangling pointer is deadly.<br/>
 *     However, a \c LinkedObject can be defined as a member variable of the
 *     controller, since a \c LinkedObject shares the same reference count with
 *     the controller.<br/>
 *     The controller is free to provide to other components smart pointers that
 *     point to the \c LinkedObject, although it is a member variable.<br/>
 *     The point is that a smart pointer to the \c LinkedObject actually holds
 *     a reference count of the controller.<br/>
 *     Thus, as long as a smart pointer to the \c LinkedObject exists, the
 *     controller also exists.<br/>
 *
 *
 *
 * @see \c NSFX_INTERFACE_MAP_BEGIN,
 *      \c NSFX_INTERFACE_ENTRY,
 *      \c NSFX_INTERFACE_AGGREGATED_ENTRY,
 *      \c NSFX_INTERFACE_MAP_END.
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
    // void* InternalQueryInterface(const uuid& iid);

    refcount_t ControllerAddRef(void)
    {
        return controller_->AddRef();
    }

    refcount_t ControllerRelease(void)
    {
        return controller_->Release();
    }

    void* ControllerQueryInterface(const uuid& iid)
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


////////////////////////////////////////////////////////////////////////////////
// EnvelopableConcept
/**
 * @ingroup Component
 * @brief Envelopable concept.
 *
 * A class is envelopable if it satisfies the following conditions.<br/>
 * 1. It conforms to \c IObjectConcept.<br/>
 * 2. It has a non-private \c InternalQueryInterface() function.<br/>
 * 3. It is not derived from \c ObjectBase.<br/>
 */
template<class T>
struct EnvelopableConcept/*{{{*/
{
    static_assert(!boost::is_base_of<ObjectBase, T>::value,
                  "An envelopable class must not derive from ObjectBase.");

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

    BOOST_CONCEPT_USAGE(EnvelopableConcept)
    {
        HasNonPrivateInternalQueryInterface();
    }

    void HasNonPrivateInternalQueryInterface(void)
    {
        struct Child : T
        {
            // T has a private InternalQueryInterface().
            void* InternalQueryInterface(const uuid& iid)
            {
                return T::InternalQueryInterface(iid);
            }
        };

        Child* child = nullptr;
        void* iobj = child->InternalQueryInterface(uuid_of<IObject>());
    }

}; // class EnvelopableConcept /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Object
/**
 * @ingroup Component
 * @brief A non-aggregable object that must be allocated on heap.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * A non-aggretable object possesses a reference counter.<br/>
 * However, it does not hold a pointer to a controller, thus it cannot be
 * managed by a controller.<br/>
 *
 * An \c Object will deallocate itself when its reference count reaches \c 0.<br/>
 * Therefore, do not define an \c Object as a static variable or a member
 * variable.<br/>
 *
 * The \c Envelopable may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * ### When to use?
 *     \c Object can be used everywhere.
 * ### How to use?
 *     An \c Object must be allocated on heap.<br/>
 *     i.e., <code>Ptr<></code> shall be used to hold an \c Object.
 */
template<class Envelopable>
class Object NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public Envelopable
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    Object(Args&&... args) :
        Envelopable(std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    Object(void) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~Object(void) BOOST_NOEXCEPT {}

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

    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return InternalQueryInterface(iid);
    }

    /*}}}*/

    // Methods.
    Envelopable* GetEnveloped(void)
    {
        return static_cast<Envelopable*>(this);
    }

}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A non-aggregable object that must be used as a static variable.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * A \c StaticObject does not have a reference count.<br/>
 * Thus, do not allocate a \c StaticObject on heap, since <code>Ptr<></code>
 * cannot deallocate it automatically.<br/>
 * It must be defined as a static variable to prevent memory leak.<br/>
 * e.g. a singleton can be defined as a static \c StaticObject, and there is
 * no need to allocate it on heap.<br/>
 *
 * The \c Envelopable may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * ### When to use?
 *     \c StaticObject can be used when an object has a static lifetime.
 * ### How to use?
 *     A \c StaticObject must be defined as a \c static variable.
 * ### Benefits.
 *     It saves a <code>Ptr<></code> to manage lifetime.<br/>
 *     There is no need to allocate on heap.
 */
template<class Envelopable>
class StaticObject NSFX_FINAL :/*{{{*/
    public Envelopable
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    StaticObject(Args&&... args) :
        Envelopable(std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    StaticObject(void) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~StaticObject(void) BOOST_NOEXCEPT {}

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

    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return InternalQueryInterface(iid);
    }

    /*}}}*/

    // Methods.
    Envelopable* GetEnveloped(void)
    {
        return static_cast<Envelopable*>(this);
    }

}; // class StaticObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A aggregated object that is internally used by \c MemberObject and \c AggObject.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * Users cannot use this class directly.
 *
 * @internal
 */
template<class Envelopable>
class Aggregated NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public Envelopable
{
private:
    template<class Envelopable> friend class AggObject;
    template<class Envelopable> friend class MemberObject;

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    Aggregated(IObject* controller, Args&&... args) :
        ObjectBase(controller),
        Envelopable(std::forward<Args>(args)...)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(
                BadAggregation() << ControllerErrorInfo(controller));
        }
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    Aggregated(IObject* controller) :
        ObjectBase(controller)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(
                BadAggregation() << ControllerErrorInfo(controller));
        }
    }

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 2))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~Aggregated(void) BOOST_NOEXCEPT {}

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
    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        return ControllerQueryInterface(iid);
    }

    /*}}}*/

    // Make the protected Envelopable::InternalQueryInterface() a member of
    // Aggregated class.
    void* InternalQueryInterface(const uuid& iid)
    {
        return Envelopable::InternalQueryInterface(iid);
    }

}; // class Aggregated /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregable object that must be allocated on heap.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * The aggregated object has the same lifetime as the navigator.<br/>
 * So the navigator also has the same lifetime as the controller.<br/>
 * Therefore, the controller must <b>not</b> deallocate the navigator until
 * the controller itself is deallocated.<br/>
 *
 * A controller shall define a member varaible of type <code>Ptr<IObject></code>
 * to hold an \c AggObject, i.e., the navigator.<br/>
 * This smart pointer is never offer to other objects.<br/>
 * And it is deallocated along with the controller.<br/>
 *
 * The \c Enveloped may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * ### When to use?
 *     \c AggObject can be used by a controller to aggregate an object.
 * ### How to use?
 *     An \c AggObject must be allocated on heap.<br/>
 *     The controller shall define a member variable of type <code>Ptr<IObject></code>
 *     to hold the allocated \c AggObject.<br/>
 *     This pointer must not be given to other objects.<br/>
 *     The \c AggObject lives until the controller is destructed.
 * ### Benefits.
 *     The interfaces on an aggregated object can be exposed directly by the
 *     controller.<br/>
 */
template<class Envelopable>
class AggObject NSFX_FINAL :/*{{{*/
    public IObject,
    private ObjectBase // Use private inherit as the methods are only used internally.
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

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

    virtual ~AggObject(void) BOOST_NOEXCEPT {}

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
     * @return If the \c iid is the uuid of \c IObject, return the \c IObject
     *         on the navigator itself.<br/>
     *         Otherwise, return the interface on the aggregated object.
     */
    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == uuid_of<IObject>())
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
    Envelopable* GetEnveloped(void)
    {
        return static_cast<Envelopable*>(&agg_);
    }

private:
    Aggregated<Envelopable> agg_;  /// The aggregated object.

}; // class AggObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// MemberObject
/**
 * @ingroup Component
 * @brief An aggregable object that must be used a member variable of its constroller.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * A member aggregable object does not have a reference count.<br/>
 * Thus, do not allocate a \c MemberObject on heap, since <code>Ptr<></code>
 * cannot deallocate it automatically.<br/>
 * It must be defined as a member variable of the controller to prevent memory
 * leak.<br/>
 *
 * The \c Envelopable may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * ### When to use?
 *     \c MemberObject can be used by a controller to aggregate an object.
 * ### How to use?
 *     A \c MemberObject must be defined as a member variable of the controller.<br/>
 * ### Benefits.
 *     The interfaces on an aggregated object can be exposed directly by the
 *     controller.<br/>
 *     A \c MemberObject naturally has the same lifetime as the controller,
 *     which is similar to an \c AggObject.<br/>
 *     It saves a <code>Ptr<></code> to manage lifetime.<br/>
 *     There is no need to allocate on heap.
 */
template<class Envelopable>
class MemberObject NSFX_FINAL :/*{{{*/
    public IObject
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    MemberObject(IObject* controller, Args&&... args) :
        agg_(controller, std::forward<Args>(args)...) {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    MemberObject(IObject* controller) :
        agg_(controller) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 4))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~MemberObject(void) BOOST_NOEXCEPT {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(MemberObject(const MemberObject& ));
    BOOST_DELETED_FUNCTION(MemberObject& operator=(const MemberObject& ));

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
     * @return If the \c iid is the uuid of \c IObject, return the \c IObject
     *         on the navigator itself.<br/>
     *         Otherwise, return the interface on the aggregated object.
     */
    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == uuid_of<IObject>())
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
    Envelopable* GetEnveloped(void)
    {
        return static_cast<Envelopable*>(&agg_);
    }

private:
    Aggregated<Envelopable> agg_;  /// The aggregated object.

}; // class MemberObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// LinkedObject
/**
 * @ingroup Component
 * @brief An object that can be exposed by its constroller with a separate set of interfaces.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * A linked object shares the same reference count as its controller.<br/>
 * It can be defined as a member variable of the controller.<br/>
 *
 * The \c Envelopable may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * ### When to use?
 *     \c LinkedObject can be used by a controller to provide an object that
 *     exposes a separate set of interfaces.<br/>
 * ### How to use?
 *     A \c LinkedObject can be defined as a member variable of the controller.<br/>
 *     The controller can provide to other objects smart pointers that point to
 *     the \c LinkedObject.<br/>
 *     The controler must not expose the interfaces on a \c LinkedObject as its
 *     own interfaces.
 * ### Benefits.
 *     The controller lives as long as other objects hold smart pointers to
 *     the \c LinkedObject.
 *     It saves a <code>Ptr<></code> to manage lifetime.<br/>
 *     There is no need to allocate on heap.
 */
template<class Envelopable>
class LinkedObject NSFX_FINAL :/*{{{*/
    private ObjectBase, // Use private inherit as the methods are only used internally.
    public Envelopable
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class...Args>
    LinkedObject(IObject* controller, Args&&... args) :
        ObjectBase(controller),
        Envelopable(std::forward<Args>(args)...)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(
                BadAggregation() << ControllerErrorInfo(controller));
        }
    }

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    LinkedObject(IObject* controller) :
        ObjectBase(controller)
    {
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(
                BadAggregation() << ControllerErrorInfo(controller));
        }
    }

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/component/object.h>, 5))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~LinkedObject(void) BOOST_NOEXCEPT {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(LinkedObject(const LinkedObject& ));
    BOOST_DELETED_FUNCTION(LinkedObject& operator=(const LinkedObject& ));

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
     * @brief Query an interface from the navigator.
     *
     * @return The interface on the aggregated object.
     */
    virtual void* QueryInterface(const uuid& iid) NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = Envelopable::InternalQueryInterface(iid);
        return result;
    }

    /*}}}*/

    // Methods.
    Envelopable* GetEnveloped(void)
    {
        return static_cast<Envelopable*>(this);
    }

}; // class LinkedObject /*}}}*/


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// NSFX_INTERFACE_XXX() macros./*{{{*/
/**
 * @ingroup Component
 * @brief Begin an interface map.
 *
 * @param ThisClass The class that defines the interface map.<br/>
 *                  It is the class that implements or aggregates interfaces.
 *
 * @remarks The macros changes the access rights to \c protected.
 */
#define NSFX_INTERFACE_MAP_BEGIN(ThisClass)                               \
  private:                                                                \
    typedef ThisClass  ThisClass_;                                        \
  protected:                                                              \
    void* InternalQueryInterface(const ::nsfx::uuid& iid)                 \
    {                                                                     \
        void* result = nullptr;                                           \
        if (iid == ::nsfx::uuid_of<IObject>())                            \
        {                                                                 \
            static_assert(                                                \
                ::boost::is_base_of<::nsfx::IObject, ThisClass_>::value,  \
                "Cannot expose an unimplemented interface");              \
            result = static_cast<IObject*>(this);                         \
            AddRef();                                                     \
        }

/**
 * @ingroup Component
 * @brief Expose an interface implemented by the \c ThisClass that defines the interface map.
 *
 * @param Intf The type of the interface.<br/>
 *             \c ThisClass that defines the interface map must derive from
 *             \c Intf.<br/>
 *             It should <b>not</b> be \c IObject.
 */
#define NSFX_INTERFACE_ENTRY(Intf)                               \
        else if (iid == ::nsfx::uuid_of<Intf>())                 \
        {                                                        \
            static_assert(                                       \
                ::boost::is_base_of<Intf, ThisClass_>::value,    \
                "Cannot expose an unimplemented interface");     \
            result = static_cast<Intf*>(this);                   \
            AddRef();                                            \
        }

/**
 * @ingroup Component
 * @brief Expose an interface implemented by an aggregated object.
 *
 * @param Intf The type of the interface.<br/>
 *             It should <b>not</b> be \c IObject.
 * @param navi A pointer to an object that exposes the \c Intf interface.<br/>
 *             i.e., the objet implements \c IObject, and \c Intf can be queried
 *             from the object.
 */
#define NSFX_INTERFACE_AGGREGATED_ENTRY(Intf, navi)                 \
        else if (iid == ::nsfx::uuid_of<Intf>())                    \
        {                                                           \
            result = (navi)->QueryInterface(iid);                   \
        }

/**
 * @ingroup Component
 * @brief End an interface map.
 */
#define NSFX_INTERFACE_MAP_END()                                              \
        if (!result)                                                          \
        {                                                                     \
            BOOST_THROW_EXCEPTION(                                            \
                ::nsfx::NoInterface()                                         \
                << ::nsfx::QuerySourceClassErrorInfo(                         \
                    ::boost::typeindex::type_id<ThisClass_>().pretty_name())  \
                << ::nsfx::QueryTargetIidErrorInfo(iid)                       \
            );                                                                \
        }                                                                     \
        return result;                                                        \
    }

/*}}}*/


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F


#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

#define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0

// template<class A0, class A1, ...>
// Object(A0&& a0, A1&& a1, ...)) :
//     Envelopable(std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
Object(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    Envelopable(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 1

// template<class A0, class A1, ...>
// StaticObject(A0&& a0, A1&& a1, ...)) :
//     Envelopable(std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
StaticObject(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    Envelopable(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 2

// template<class A0, class A1, ...>
// AggObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     ObjectBase(controller),
//     Envelopable(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
Aggregated(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectBase(controller),
    Envelopable(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{
    if (!controller)
    {
        BOOST_THROW_EXCEPTION(BadAggregation() << ControllerErrorInfo(controller));
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
// MemberObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     agg_(controller, std::forward<A0>(a0), std::forward<A1>(a1), ...) {}
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
MemberObject(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    agg_(controller, BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )) {}

# elif BOOST_PP_ITERATION_FLAGS() == 5

// template<class A0, class A1, ...>
// LinkedObject(IObject* controller, A0&& a0, A1&& a1, ...)) :
//     ObjectBase(controller),
//     Envelopable(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
LinkedObject(IObject* controller, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a)) :
    ObjectBase(controller),
    Envelopable(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{
    if (!controller)
    {
        BOOST_THROW_EXCEPTION(
            BadAggregation() << ControllerErrorInfo(controller));
    }
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

#undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

