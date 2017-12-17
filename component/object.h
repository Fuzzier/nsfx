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


#define NSFX_OBJECT_MAP()                                       \
    void* QueryInterfaceImpl(const uuid& iid) BOOST_NOEXCEPT    \
    {                                                           \
        void* result = nullptr;                                 \
        if (false)                                              \
        {                                                       \
        }                                                       \

#define NSFX_OBJECT_INTERFACE_ENTRY(type)       \
        else if (iid == uuid_of<type>())        \
        {                                       \
            AddRef();                           \
            result = static_cast<type*>(this);  \
        }

#define NSFX_OBJECT_INTERFACE_ENTRY_AGGREGATE(type)                 \
        else if (result = outer_->QueryInterface(uuid_of<type>()))  \
        {                                                           \
            AddRef();                                               \
            result = static_cast<type*>(this);                      \
        }

#define NSFX_END_DECLARE_INTERFACE()  \
    return result;                    \
    }


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Object
/**
 * @ingroup Component
 * @brief A non-aggregatable object.
 *
 * A non-aggregatable object possesses a reference counter.
 */
class Object : public virtual IObject/*{{{*/
{
protected:
    Object(void) BOOST_NOEXCEPT :
        refCount_(1)
    {
    }

    virtual ~Object(void)
    {
        NSFX_ASSERT(!refCount_);
    }

    /**
     * @brief Increment the reference counter.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual size_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        ++refCount_;
    }

    /**
     * @brief Decrement the reference counter.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual size_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        uint32_t result = --refCount_;
        if (!result)
        {
            delete this;
        }
        return result;
    }

    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        void * result = nullptr;
        if (iid == uuid_of<IObject>())
        {
            result = static_cast<IObject*>(this);
        }
        return result;
    }

protected:
    uint32_t refCount_;
}; // class Object /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// AggObject
/**
 * @ingroup Component
 * @brief An aggregatable object.
 *
 * An aggregatable object does <b>not</b> possess a reference counter.<br/>
 * Instead, its lifetime is managed by an outer object (controller).
 */
class AggObject : public virtual IObject/*{{{*/
{
protected:
    /**
     * @brief Construct an aggregated object.
     *
     * @param outer The controller object. Must <b>not</b> be \c nullptr.
     *
     * @throw InvalidPointer When the controller object is \c nullptr.
     */
    Object(IObject* outer) :
        outer_(outer)
    {
        if (!outer)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer);
        }
    }

    virtual ~Object(void)
    {
    }

    /**
     * @brief Increment the reference counter.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual size_t AddRef(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return outer_->AddRef();
    }

    /**
     * @brief Decrement the reference counter.
     *
     * The call is delegated to the controller.
     *
     * @remarks It is a \c final method that cannot be overridden.
     */
    virtual size_t Release(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return outer_->Release();
    }

    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        void* result = outer_->QueryInterface(iid);
        return result;
    }

protected:
    IObject* outer_;
}; // class AggObject /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // OBJECT_H__DB70B5FF_F35B_4309_B62C_EDD8AE30522F

