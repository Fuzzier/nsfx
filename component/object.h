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
#include <boost/type_traits/is_base_of.hpp>


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
 * 4. Poly object.<br/>
 * A poly object supports both non-aggregated and aggregated usage.<br/>
 *
 * The navigator object always exposes the interfaces on the user-defined
 * object, no matter whether it is aggregated or not.<br/>
 * The key difference is on the lifetime management.<br/>
 * For this purpose, another lifetime manager object is defined to virtualize
 * the lifetime management.<br/>
 *
 * The user-defined object delegates its own \c IObject::AddRef() and
 * \c IObject::Release() to the lifetime manager object.<br/>
 * The lifetime manager object holds a reference count, and provides
 * \c IObject::AddRef() and \c IObject::Release() that virtualize current usage.<br/>
 * If a controller is specified at the creation of the user-defined object,
 * the lifetime manager delegates the lifetime management to the controller;
 * Otherwise, the lifetime manager provides implementation for the lifetime
 * management of the user-defined object.<br/>
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
    // void* InternalQueryInterface(const uuid& iid) BOOST_NOEXCEPT;

    refcount_t ControllerAddRef(void) BOOST_NOEXCEPT
    {
        return controller_->AddRef();
    }

    refcount_t ControllerRelease(void) BOOST_NOEXCEPT
    {
        return controller_->Release();
    }

    void* ControllerQueryInterface(const uuid& iid) BOOST_NOEXCEPT
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
 * 2. It has a public \c InternalQueryInterface() function.<br/>
 * 3. It is default constructible.<br/>
 * 4. It is not derived from \c ObjectBase.<br/>
 */
template<class T>
struct EnvelopableConcept/*{{{*/
{
    static_assert(!boost::is_base_of<ObjectBase, T>::value,
                  "An envelopable class must not derive from ObjectBase.");

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

    BOOST_CONCEPT_USAGE(EnvelopableConcept)
    {
        HasNonPrivateStaticInternalQueryInterface();
    }

    void HasNonPrivateStaticInternalQueryInterface(void)
    {
        struct Child : T
        {
            // T is default constructible.
            Child(void) {}

            // T has a private InternalQueryInterface().
            void* InternalQueryInterface(const uuid& iid) BOOST_NOEXCEPT
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
 * @brief A non-aggregable object.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 * @tparam autoDelete If \c true, the object is automatically deleted when its
 *                    reference count is decremented to \c 0.<br/>
 *                    The object must be allocated on heap.<br/>
 *                    Otherwise, the object can be placed on stack or as a
 *                    concrete member variable of a class.
 *
 * A non-aggretable object possesses a reference counter.<br/>
 * However, it does not hold a pointer to a controller, thus it cannot be
 * managed by a controller.
 */
template<class Envelopable, bool autoDelete = true>
class Object NSFX_FINAL :/*{{{*/
    public Envelopable,
    // Use private inherit as the methods are only used internally.
    private ObjectBase
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
    BOOST_DEFAULTED_FUNCTION(Object(void) BOOST_NOEXCEPT, {});

    virtual ~Object(void) BOOST_NOEXCEPT {}

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = autoDelete ? InternalAddRef() : 1;
        return result;
    }

    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = autoDelete ? InternalRelease() : 1;
        if (autoDelete && !result)
        {
            delete this;
        }
        return result;
    }

    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return InternalQueryInterface(iid);
    }

    /*}}}*/

}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregable object that is the navigator of an aggregated object.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 * @tparam autoDelete If \c true, the navigator is automatically deleted when
 *                    its reference count is decremented to \c 0.<br/>
 *                    The object must be allocated on heap.<br/>
 *                    Otherwise, the object can be placed on stack or as a
 *                    concrete member variable in the controller class.
 *
 * The derived object may implement \c ObjectBase::InternalQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 *
 * @remarks The aggregated object has the same lifetime as the navigator.<br/>
 *          So the navigator also has the same lifetime as the controller.<br/>
 *          Therefore, the controller must <b>not</b> deallocate the navigator
 *          until the controller itself is deallocated.
 */
template<class Envelopable, bool autoDelete = true>
class AggObject NSFX_FINAL :/*{{{*/
    public IObject,
    // Use private inherit as the methods are only used internally.
    private ObjectBase
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

    class Aggregated NSFX_FINAL :/*{{{*/
        public Envelopable,
        public ObjectBase
    {
    public:
        Aggregated(IObject* controller) :
            ObjectBase(controller)
        {
            if (!controller)
            {
                BOOST_THROW_EXCEPTION(BadAggregation());
            }
        }

        virtual ~Aggregated(void) BOOST_NOEXCEPT {}

        // IObject./*{{{*/
        virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return ControllerAddRef();
        }

        virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return ControllerRelease();
        }

        /**
         * @brief Exposes interfaces implemented by the aggregated object.
         */
        virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return ControllerQueryInterface(iid);
        }

        /*}}}*/

    }; // class Aggregated /*}}}*/

public:
    AggObject(IObject* controller) :
        agg_(controller)
    {
    }

    virtual ~AggObject(void) BOOST_NOEXCEPT {}

    // IObject./*{{{*/
public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = autoDelete ? InternalAddRef() : 1;
        return result;
    }

    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = autoDelete ? InternalRelease() : 1;
        if (autoDelete && !result)
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
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == uuid_of<IObject>())
        {
            result = static_cast<IObject*>(this);
            autoDelete ? InternalAddRef() : 1;
        }
        else
        {
            result = agg_.InternalQueryInterface(iid);
        }
        return result;
    }

    /*}}}*/

private:
    Aggregated agg_;  /// The aggregated object.

}; // class AggObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Type traits./*{{{*/
template<class T>
struct IsNonAggregated: boost::false_type {};

template<class T, bool autoDelete>
struct IsNonAggregated<Object<T, autoDelete> > : boost::true_type {};


template<class T>
struct IsAggregated : boost::false_type {};

template<class T, bool autoDelete>
struct IsAggregated<AggObject<T, autoDelete> > : boost::true_type {};

/*}}}*/


NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// NSFX_INTERFACE_XXX() macros./*{{{*/
/**
 * @ingroup Component
 * @brief Begin the interface map.
 *
 * @param ThisClass The class that defines the interface map.<br/>
 *                  It is the class that implements or aggregates interfaces.
 *
 * @remarks The macros changes the access rights to \c protected.
 */
#define NSFX_INTERFACE_MAP_BEGIN(ThisClass)                               \
  private:                                                                \
    typedef ThisClass  ThisClass_;                                        \
    template<class T, bool autoDelete> friend class ::nsfx::AggObject;    \
  protected:                                                              \
    void* InternalQueryInterface(const ::nsfx::uuid& iid) BOOST_NOEXCEPT  \
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
 * @brief Expose an interface implemented by the object.
 *
 * @param Intf The type of the interface. It should <b>not</b> be \c IObject.
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
 * @param Intf The type of the interface. It should <b>not</b> be \c IObject.
 * @param navi A pointer to the \c IObject interface on the navigator object.<br/>
 *             It must be accessible within \c ThisClass_.
 */
#define NSFX_INTERFACE_AGGREGATED_ENTRY(Intf, navi)                 \
        else if (iid == ::nsfx::uuid_of<Intf>())                    \
        {                                                           \
            result = (navi)->QueryInterface(iid);                   \
        }

/**
 * @ingroup Component
 * @brief End the interface map.
 */
#define NSFX_INTERFACE_MAP_END()  \
        return result;            \
    }

/*}}}*/


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F

