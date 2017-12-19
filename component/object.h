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
 * @param intf The type of the interface.
 */
#define NSFX_BEGIN_INTERFACE_OBJECT_MAP()                              \
    public:                                                            \
    void* InnerQueryInterface(const ::nsfx::uuid& iid) BOOST_NOEXCEPT  \
    {                                                                  \
        void* result = nullptr;                                        \
        if (iid == ::nsfx::uuid_of<IObject>())                         \
        {                                                              \
            static_assert(                                             \
                boost::is_base_of<                                     \
                    IObject,                                           \
                    boost::remove_cv_ref<decltype(*this)>::type        \
                >::value,                                              \
                "Cannot expose an unimplemented interface");           \
            result = static_cast<IObject*>(this);                      \
        }

/**
 * @brief Expose an interface implemented by the object.
 *
 * @param intf The type of the interface.
 */
#define NSFX_INTERFACE_OBJECT_ENTRY(intf)                         \
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
 * @param intf The type of the interface.
 * @param iobj The \c IObject interface of the aggregated object.
 */
#define NSFX_INTERFACE_OBJECT_ENTRY_AGGREGATE(intf, iobj)              \
        else if (iid == ::nsfx::uuid_of<intf>())                       \
        {                                                              \
            result = static_cast<intf*>((iobj)->QueryInterface(iid));  \
        }

#define NSFX_END_INTERFACE_OBJECT_MAP()  \
        if (result)                      \
        {                                \
            AddRef();                    \
        }                                \
        return result;                   \
    }


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ObjectBase
/**
 * @ingroup Component
 * @brief The base class for implementing \c IObject.
 *
 * 1. Object aggregation.<br/>
 * 1.1 Non-aggregated object.<br/>
 * The object provides a reference count, and implements all interfaces.<br/>
 * Thus, \c IObject::AddRef() and \c IObject::Release() operate upon its own
 * reference count.<br/>
 * When the object provides a queried interface, the interface must be managed
 * by its own reference count.<br/>
 *
 * 1.2 Aggregated object.<br/>
 * An interface queried from an aggregated object <i>acts like</i> an interface
 * implemented by its controller.<br/>
 * A user cannot distinguish whether the interface is implemented by an
 * aggregated object or not.<br/>
 *
 * 1.2.1 Lifetime management.<br/>
 * An aggregated object has the same lifetime as the controller.<br/>
 * i.e., aggregated objects and their controllers share a single instance of
 * reference count.<br/>
 * The reference count is ultimately provided by a single outter-most controller.<br/>
 * An aggregated object holds a pointer to the outter-most controller.<br/>
 * \c IObject::AddRef() and \c IObject::Release() are delegated to the
 * outter-most controller.<br/>
 *
 * 1.2.2 Interface navigation.<br/>
 * The interface queried from an aggregated object is provided directly to users.<br/>
 * This frees the controller from having to create an extra layer to access the
 * aggregated object indirectly, which saves codes, memory and CPU cycles.<br/>
 *
 * When users query an interface from the controller, and the controller itself
 * doesn't have the interface, it delegates the query to the aggregated objects
 * by calling their \c IObject::QueryInterface() method.<br/>
 * The returned interface must be able to navigate all interfaces of the
 * controller.<br/>
 * Thus, when users query an interface from an aggregated object, the aggregated
 * object would delegate the query to its controller.<br/>
 * It would create an infinite call loop when a non-supported interface is
 * queried.<br/>
 *
 * The key is that the aggregated object has dual \c IObject implementations.<br/>
 * One is for \c IObject interface, the other for other interfaces.<br/>
 * Users can only see a single implementation of \c IObject interface that is
 * implemented by the controller.<br/>
 * The implementation of \c IObject of an aggregated object is never exposed to
 * users.<br/>
 * While other interfaces of an aggregated object are exposed to users.<br/>
 *
 * The \c IObject interface of an aggregated object is held by the controller,
 * which exposes all other interfaces of the aggregated object.<br/>
 * The controller use this implementation to query other interfaces from the
 * aggregated object.<br/>
 * This implementation also manages its own reference count.<br/>
 *
 * For other interfaces of an aggregated object, the implementation of
 * \c IObject is delegated directly to the controller's implementation.<br/>
 *
 * In practice, \c IObject of an aggregated object is implemented by an nested
 * class.<br/>
 * While other interfaces share a common implemention of \c IObject that
 * delegates to the controller.<br/>
 */
class ObjectBase
{
public:
    virtual ~ObjectBase(void) BOOST_NOEXCEPT {}

    virtual void* InnerQueryInterface(const uuid& iid) BOOST_NOEXCEPT = 0;

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
 * The derived object may implement \c ObjectBase::InnerQueryInterface() via
 * \c NSFX_INTERFACE_OBJECT_XXX() macros.
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
        if (!result)
        {
            delete this;
        }
        return result;
    }

    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return InnerQueryInterface(iid);
    }

    /*}}}*/

protected:
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
 * The derived object may implement \c ObjectBase::InnerQueryInterface() via
 * \c NSFX_INTERFACE_OBJECT_XXX() macros.
 */
template<class Derived>
class AggObject :/*{{{*/
    virtual public ObjectBase,
    virtual public IObject
{
private:
    /**
     * @brief A nested class that implements \c IObject for the aggregated object.
     *
     * This \c IObject is held by the controller to query interfaces exposed
     * by the derived aggregated object.
     */
    class IObjectImpl : virtual public IObject /*{{{*/
    {
    public:
        IObjectImpl(void) BOOST_NOEXCEPT :
            obj_(nullptr)
        {
        }

        virtual ~IObjectImpl(void) BOOST_NOEXCEPT {}

        void SetObjectBase(ObjectBase* obj) BOOST_NOEXCEPT
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
            return obj_->InnerQueryInterface(iid);
        }

        /*}}}*/

        ObjectBase* obj_;
    };/*}}}*/

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
        iobj_.SetObjectBase(this);
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
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return outer_->QueryInterface(iid);
    }

    /*}}}*/

    IObject* GetInner(void) BOOST_NOEXCEPT
    {
        return &iobj_;
    }

protected:
    IObjectImpl iobj_; /// The implementation of \c IObject.
    IObject* outer_;   /// The controller.

}; // class AggObject /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F

