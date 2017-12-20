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
#include <nsfx/component/iobject.h>
#include <nsfx/component/exception.h>
#include <boost/type_traits.hpp>


// #if defined(NSFX_MSVC)
// // Allow 'this' pointer to be used in constructor initialization list.
// #pragma warning(disable: 4355)
// #endif // defined(NSFX_MSVC)


////////////////////////////////////////////////////////////////////////////////
// Macros.
/**
 * @brief Expose an interface implemented by the object.
 *
 * @remarks The macros changes the access rights to \c private.
 */
#define NSFX_INTERFACE_MAP_BEGIN()                                         \
    public:                                                                \
    void* InnerQueryInterface(const ::nsfx::uuid& iid) BOOST_NOEXCEPT      \
    {                                                                      \
        void* result = nullptr;                                            \
        if (iid == ::nsfx::uuid_of<IObject>())                             \
        {                                                                  \
            static_assert(                                                 \
                boost::is_base_of<                                         \
                    ::nsfx::IObject,                                       \
                    boost::remove_cv_ref<decltype(*this)>::type            \
                >::value,                                                  \
                "Cannot expose an unimplemented interface");               \
            result = static_cast<IObject*>(this);                          \
        }

/**
 * @brief Expose an interface implemented by the object.
 *
 * @param intf The type of the interface. It must <b>not</b> be \c IObject.
 */
#define NSFX_INTERFACE_ENTRY(intf)                                \
        else if (iid == ::nsfx::uuid_of<intf>())                  \
        {                                                         \
            static_assert(                                        \
                boost::is_base_of<                                \
                    intf,                                         \
                    boost::remove_cv_ref<decltype(*this)>::type   \
                >::value,                                         \
                "Cannot expose an unimplemented interface");      \
            result = static_cast<intf*>(this);                    \
        }

/**
 * @brief Expose an interface implemented by an aggregated object.
 *
 * @param intf The type of the interface. It must <b>not</b> be \c IObject.
 * @param iobj The \c IObject interface on the navigator object.
 */
#define NSFX_INTERFACE_ENTRY_AGGREGATE(intf, navi)                     \
        else if (iid == ::nsfx::uuid_of<intf>())                       \
        {                                                              \
            result = static_cast<intf*>((navi)->QueryInterface(iid));  \
        }

#define NSFX_INTERFACE_MAP_END()  \
        if (result)               \
        {                         \
            AddRef();             \
        }                         \
        return result;            \
    }


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ObjectBase
/**
 * @ingroup Component
 * @brief The base class for implementing \c IObject.
 *
 * 1. Introduction.
 * Usually, an object provides a reference count for lifetime management.<br/>
 * When an object want to implement an interface, it may reuse an existing
 * object that has implemented the interface.<br/>
 * The container creates an instance of the contained object, and delegates the
 * calls on the interface to the contained object.<br/>
 *
 * The problem is that the container cannot expose the interfaces on the
 * contained object directly to the users.<br/>
 * Because the contained object cannot expose other interfaces on the container
 * object.<br/>
 * i.e., from an interface on the contained object, a user cannot query other
 * interfaces exposed by the container object.<br/>
 * Therefore, a container have to create an extra layer of virtual function call
 * to delegate the interface to the contained object.<br/>
 * It places extra burden upon coding, memory and CPU cycles.<br/>
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
 * The aggregated object is responsible to implement interfaces, the lifetime
 * management and navigability functions of \c IObject are delegated to the
 * \c IObject on its controller.<br/>
 * In order for the controller to query interfaces on the aggregated object,
 * a separate navigator object is responsible to expose the interfaces on the
 * aggregated object.<br/>
 *
 * 2. Lifetime management.<br/>
 * An aggregated object has the same lifetime as its controller.<br/>
 * i.e., aggregated objects and its controller share a single reference count.<br/>
 * The reference count is ultimately provided by the outter-most controller.<br/>
 * An aggregated object holds a pointer to the \c IObject interface on the
 * controller.<br/>
 * Calls to \c IObject::AddRef() and \c IObject::Release() are delegated to the
 * controller.<br/>
 *
 * 3. Interface navigation.<br/>
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
 * To solve this problem, an separate <b>navigator object</b> must be created.<br/>
 * It implements \c IObject and exposes the interfaces on the aggregated object.<br/>
 * The controller can use the navigator object to query the interfaces on the
 * aggregated object.<br/>
 * The navigator object may not have a reference count, since it is only held by
 * the controller, and the controller will never expose this object to others.<br/>
 * The navigator object can be a member variable hold an aggregated object, thus
 * share the same lifetime of the aggregated object.<br/>
 * If it ever has a reference count, the reference count can always be \c 1.<br/>
 * In practice, the navigator object can be implemented by an nested class of
 * the aggregated class.<br/>
 *
 * To provide a uniform approach to support both non-aggregated and aggregated
 * objects, the navigator object is abstracted out.<br/>
 * \c ObjectBase provides a virtual function \c NavigatorQueryInterface().<br/>
 * The user-defined object must implement \c NavigatorQueryInterface().<br/>
 * This virtual funtion exposes the interfaces on the user-defined object.<br/>
 *
 * For a user-defined non-aggregated object, the navigator is itself.<br/>
 * Its \c IObject::QueryInterface() calls the \c NavigatorQueryInterface().<br/>
 *
 * For a user-defined aggregated object, the navigator is an object of a nested
 * class.<br/>
 * The navigator object holds a pointer to the aggregated object, and its
 * \c IObject::QueryInterface() calls the \c NavigatorQueryInterface()
 * implemented by the aggregated object.<br/>
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
 * 5. Template-based virtual function implmentation.<br/>
 * A base class provides necessary data and non-virtual functions.<br/>
 * A virtual function calls the appropriate non-virtual functions.<br/>
 *
 * 5.1 Data
 * A non-aggregated object has a reference count.<br/>
 * An aggregated object has a pointer to the controller's \c IObject.<br/>
 * A poly object uses one of the data according to whether a valid pointer to
 * the controller is specified at creation.<br/>
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
 *      \c NSFX_INTERFACE_ENTRY_AGGREGATE,
 *      \c NSFX_INTERFACE_MAP_END.
 */
class ObjectBase/*{{{*/
{
public:
    ObjectBase(void) BOOST_NOEXCEPT :
        controller_(nullptr)
    {
    }

    ObjectBase(IObject* controller) BOOST_NOEXCEPT :
        controller_(controller)
    {
    }

    virtual ~ObjectBase(void) BOOST_NOEXCEPT {}

    refcount_t InnerAddRef(void) BOOST_NOEXCEPT
    {
        refcount_t result = ++refCount_;
        return result;
    }

    refcount_t InnerRelease(void) BOOST_NOEXCEPT
    {
        refcount_t result = --refCount_;
        if (!refCount_)
        {
            delete this;
        }
        return result;
    }

    virtual void* InnerQueryInterface(const uuid& iid) BOOST_NOEXCEPT = 0;

    refcount_t OuterAddRef(void) BOOST_NOEXCEPT
    {
        return controller_->AddRef();
    }

    refcount_t OuterRelease(void) BOOST_NOEXCEPT
    {
        return controller_->Release();
    }

    void* OuterQueryInterface(const uuid& iid) BOOST_NOEXCEPT
    {
        return controller_->QueryInterface(iid);
    }

private:
    union
    {
        refcount_t refCount_; /// The reference count.
        IObject*   controller_;    /// The controller.
    };
}; // class ObjectBase /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Object
/**
 * @ingroup Component
 * @brief A non-aggregable object.
 *
 * A non-aggretable object possesses a reference counter.<br/>
 * However, it does not hold a pointer to a controller, thus it cannot be
 * managed by a controller.
 *
 * The derived object may implement \c ObjectBase::InnerQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 */
template<class Derived>
class Object :/*{{{*/
    virtual public ObjectBase,
    virtual public IObject
{
protected:
    Object(void) BOOST_NOEXCEPT
    {
    }

    virtual ~Object(void) BOOST_NOEXCEPT
    {
    }

    // IObject./*{{{*/
public:
    /**
     * @brief Increment the reference counter.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return InnerAddRef();
    }

    /**
     * @brief Decrement the reference counter.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return InnerRelease();
    }

    /**
     * @brief Queries an interface on the object.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return InnerQueryInterface(iid);
    }

    /*}}}*/

}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregate-only object.
 *
 * @tparam Derived The derived class that is aggregate-only.
 *
 * The derived object may implement \c ObjectBase::InnerQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 */
template<class Derived>
class AggObject :/*{{{*/
    virtual public ObjectBase,
    virtual public IObject
{
private:
    /**
     * @brief The interface navigator of the aggregated object.
     *
     * This \c IObject is held by the controller to query the interfaces exposed
     * by \c Derived.
     */
    class Navigator :/*{{{*/
        virtual public IObject
    {
    public:
        Navigator(void) BOOST_NOEXCEPT :
            obj_(nullptr)
        {
        }

        virtual ~Navigator(void) BOOST_NOEXCEPT {}

        // Set after construction.
        // This prevents the warning that 'this' pointer is accessed at
        // constructor initialization list.
        void SetObject(ObjectBase* obj) BOOST_NOEXCEPT
        {
            obj_ = obj;
        }

        // IObject./*{{{*/
        /**
         * @brief Dummy.
         */
        virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return 1;
        }

        /**
         * @brief Dummy.
         */
        virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return 1;
        }

        /**
         * @brief Exposes interfaces implemented by the aggregated object.
         */
        virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return obj_->OuterQueryInterface(iid);
        }

        /*}}}*/

        ObjectBase* obj_;
    }; // class Navigator /*}}}*/

protected:
    /**
     * @brief Construct an aggregated object.
     *
     * @param controller The controller object. Must <b>not</b> be \c nullptr.
     *
     * @throw BadAggregation When the controller object is \c nullptr.
     */
    AggObject(IObject* controller) :
        ObjectBase(controller)
    {
        navi_.SetObject(this);
        if (!controller)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
    }

    virtual ~AggObject(void) BOOST_NOEXCEPT
    {
    }

    // IObject./*{{{*/
public:
    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return OuterAddRef();
    }

    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return OuterRelease();
    }

    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return OuterQueryInterface(iid);
    }

    /*}}}*/

    static IObject* CreateInstance(const uuid& iid)
    {
        if (iid != uuid_of<IObject>())
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
        Derived* p = new Derived;
        if (!p)
        {
            BOOST_THROW_EXCEPTION(OutOfMemory());
        }
        return &(p->navi_);
    }

private:
    Navigator navi_;  /// The navigator.

}; // class AggObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// PolyObject
/**
 * @ingroup Component
 * @brief An object that can be either non-aggregated or aggregated.
 *
 * @tparam Derived The derived class that is aggregate-only.
 *
 * The derived object may implement \c ObjectBase::InnerQueryInterface() via
 * \c NSFX_INTERFACE_XXX() macros.
 */
template<class Derived>
class PolyObject :/*{{{*/
    virtual public ObjectBase,
    virtual public IObject
{
private:
    /**
     * @brief The interface navigator of the aggregated object.
     *
     * This \c IObject is held by the controller to query the interfaces exposed
     * by \c Derived.
     */
    class Navigator :/*{{{*/
        virtual public IObject
    {
    public:
        Navigator(void) BOOST_NOEXCEPT :
            obj_(nullptr)
        {
        }

        virtual ~Navigator(void) BOOST_NOEXCEPT {}

        // Set after construction.
        // This prevents the warning that 'this' pointer is accessed at
        // constructor initialization list.
        void SetObject(ObjectBase* obj) BOOST_NOEXCEPT
        {
            obj_ = obj;
        }

        // IObject./*{{{*/
        /**
         * @brief Dummy.
         */
        virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return 1;
        }

        /**
         * @brief Dummy.
         */
        virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return 1;
        }

        /**
         * @brief Exposes interfaces implemented by the aggregated object.
         */
        virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
        {
            return InnerQueryInterface(iid);
        }

        /*}}}*/

        ObjectBase* obj_;
    }; // class Navigator /*}}}*/


protected:
    /**
     * @brief Construct an aggregated object.
     *
     * @param controller The controller object. Must <b>not</b> be \c nullptr.
     *
     * @throw BadAggregation When the controller object is \c nullptr.
     */
    PolyObject(IObject* controller) :
        ObjectBase(controller)
    {
        navi_.SetObject(this);
        if (!controller)
        {
            // TODO
        }
    }

    virtual ~PolyObject(void) BOOST_NOEXCEPT
    {
    }

    // IObject./*{{{*/
public:
    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return addRef_(this);
    }

    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return release_(this);
    }

    /**
     * @brief Shared implementation for other interfaces of \c Derived.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return queryInterface_(this, iid);
    }

    /*}}}*/

    IObject* GetNavigator(void) BOOST_NOEXCEPT
    {
        return &navi_;
    }

    typedef refcount_t (* AddRefType)(IObject* obj);
    typedef refcount_t (* ReleaseType)(IObject* obj);
    typedef void* (* QueryInterfaceType)(IObject* obj, const uuid& iid);

private:
    Navigator navi_;  /// The navigator.

    AddRefType  addRef_;
    ReleaseType release_;
    QueryInterfaceType queryInterface_;

}; // class PolyObject /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F

