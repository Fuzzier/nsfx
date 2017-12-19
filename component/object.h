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
#define NSFX_NAVIGATOR_MAP_BEGIN()                                         \
    private:                                                               \
    void* NavigatorQueryInterface(const ::nsfx::uuid& iid) BOOST_NOEXCEPT  \
    {                                                                      \
        void* result = nullptr;                                            \
        if (iid == ::nsfx::uuid_of<IObject>())                             \
        {                                                                  \
            static_assert(                                                 \
                boost::is_base_of<                                         \
                    IObject,                                               \
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
#define NSFX_NAVIGATOR_ENTRY(intf)                                \
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
#define NSFX_NAVIGATOR_ENTRY_AGGREGATE(intf, navi)                     \
        else if (iid == ::nsfx::uuid_of<intf>())                       \
        {                                                              \
            result = static_cast<intf*>((navi)->QueryInterface(iid));  \
        }

#define NSFX_NAVIGATOR_MAP_END()  \
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
 * Usually, an object provides a reference count for lifetime management.<br/>
 * When an object want to implement an interface, it may reuse an existing
 * object that implementes the interface.<br/>
 * The container creates an instance of the contained object, and delegates the
 * calls on the interface to the contained object.<br/>
 *
 * The problem is that the container cannot expose the interfaces on the
 * contained object directly to the users.<br/>
 * Because the contained object cannot expose other interfaces on the container
 * object.<br/>
 * From an interface on the contained object, a user cannot query other
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
 * 1. Lifetime management.<br/>
 * An aggregated object has the same lifetime as its controller.<br/>
 * i.e., aggregated objects and its controller share a single reference count.<br/>
 * The reference count is ultimately provided by the outter-most controller.<br/>
 * An aggregated object holds a pointer to the \c IObject interface on the
 * controller.<br/>
 * Calls to \c IObject::AddRef() and \c IObject::Release() are delegated to the
 * controller.<br/>
 *
 * 2. Interface navigation.<br/>
 * The interface exposed by an aggregated object is exposed directly to users.<br/>
 * This frees the controller from having to create an extra layer to access the
 * aggregated object indirectly, which saves codes, memory and CPU cycles.<br/>
 *
 * The \c IObject interface on an aggregated object must be able to expose all
 * interfaces exposed by the controller.<br/>
 * Only the controller knows the set of interfaces it exposes.<br/>
 * So an aggregated object has to delegate calls to \c IObject::QueryInterface()
 * to its controller.<br/>
 * However, an aggregated object cannot distinguish whether the caller of
 * \c IObject::QueryInterface() is a user or its controller.<br/>
 * Therefore, the \c IObject on the aggregated object loses the ability to
 * navigate its own interfaces.<br/>
 *
 * To solve this problem, an separate <b>navigator object</b> must be created.<br/>
 * It implements \c IObject and exposes the interfaces on the aggregated object.<br/>
 * The controller uses the navigator object to query the interfaces on the
 * aggregated object.<br/>
 * The navigator object may not have a reference count, since it is only held by
 * the controller, and the controller will never expose this object to others.<br/>
 * If it ever has a reference count, the reference count shall always be \c 1.<br/>
 * When the controller is deleted, it is deleted along with the controller.<br/>
 * In practice, the navigator object is implemented by an nested class.<br/>
 *
 * To provide a uniform approach to support both non-aggregated and aggregated
 * objects, the navigator object is abstracted out.<br/>
 * \c ObjectBase provides a virtual function \c NavigatorQueryInterface().<br/>
 * A user-defined object implements \c NavigatorQueryInterface().<br/>
 * This virtual funtion exposes interfaces on an object.<br/>
 * Users can use \c NSFX_NAVIGATOR_XXX() macros to ease the implementation of
 * \c NavigatorQueryInterface().<br/>
 *
 * For a non-aggregated object, the navigator is itself.<br/>
 * Its \c IObject::QueryInterface() calls the \c NavigatorQueryInterface().<br/>
 *
 * For an aggregated object, the navigator is an object of a nested class.<br/>
 * The navigator holds a pointer to the aggregated object, and its
 * \c IObject::QueryInterface() calls the \c NavigatorQueryInterface() of the
 * aggregated object.<br/>
 *
 * Such design does add extra layers of virtual function calls for
 * \c IObject::QueryInterface().<br/>
 * However, most calls to \c IObject::QueryInterface() happen at program or
 * component initialization, and it is not likely to cause performance problem.
 *
 * @see \c NSFX_NAVIGATOR_MAP_BEGIN,
 *      \c NSFX_NAVIGATOR_ENTRY,
 *      \c NSFX_NAVIGATOR_ENTRY_AGGREGATE,
 *      \c NSFX_NAVIGATOR_MAP_END.
 */
class ObjectBase
{
public:
    virtual ~ObjectBase(void) BOOST_NOEXCEPT {}

    void* NavigatorQueryInterface(const uuid& iid) BOOST_NOEXCEPT = 0;

};


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
 * The derived object may implement \c ObjectBase::NavigatorQueryInterface() via
 * \c NSFX_NAVIGATOR_XXX() macros.
 */
template<class Derived>
class Object :/*{{{*/
    virtual public ObjectBase,
    virtual public IObject
{
protected:
    Object(void) BOOST_NOEXCEPT :
        refCount_(0)
    {
    }

    virtual ~Object(void) BOOST_NOEXCEPT
    {
        NSFX_ASSERT(!refCount_);
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
        ++refCount_;
        return refCount_;
    }

    /**
     * @brief Decrement the reference counter.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        refcount_t result = --refCount_;
        if (!refCount_)
        {
            delete this;
        }
        return result;
    }

    /**
     * @brief Queries an interface on the object.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return NavigatorQueryInterface(iid);
    }

    /*}}}*/

private:
    refcount_t refCount_;
}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregate-only object.
 *
 * @tparam Derived The derived class that is aggregate-only.
 *
 * The derived object may implement \c ObjectBase::NavigatorQueryInterface() via
 * \c NSFX_NAVIGATOR_XXX() macros.
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
            return obj_->NavigatorQueryInterface(iid);
        }

        /*}}}*/

        ObjectBase* obj_;
    }; // class Navigator /*}}}*/

protected:
    /**
     * @brief Construct an aggregated object.
     *
     * @param outer The controller object. Must <b>not</b> be \c nullptr.
     *
     * @throw NoAggregation When the controller object is \c nullptr.
     */
    AggObject(IObject* outer) :
        outer_(outer)
    {
        navi_.SetObject(this);
        if (!outer)
        {
            BOOST_THROW_EXCEPTION(NoAggregation());
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
        return outer_->AddRef();
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
        return outer_->Release();
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
        return outer_->QueryInterface(iid);
    }

    /*}}}*/

    IObject* GetNavigator(void) BOOST_NOEXCEPT
    {
        return &navi_;
    }

private:
    Navigator navi_;  /// The navigator.
    IObject* outer_;  /// The controller.

}; // class AggObject /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F

