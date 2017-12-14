/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-07
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B
#define PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B


#include <nsfx/component/config.h>
#include <nsfx/component/iobject.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Ptr
/**
 * @ingroup Component
 * @brief A smart pointer that manages the lifetime of an object.
 *
 * @tparam T An class that implements \c IObject.
 */
template<class T>
class Ptr/*{{{*/
{
public:
    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

public:
    Ptr(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    explicit Ptr(T* p) BOOST_NOEXCEPT :
        p_(p)
    {
        if (p_)
        {
            p_->AddRef();
        }
    }

    Ptr(const Ptr& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
        if (p_)
        {
            p_->AddRef();
        }
    }

    Ptr(Ptr&& rhs) BOOST_NOEXCEPT :
        p_(rhs.Detach())
    {
    }

    ~Ptr(void) BOOST_NOEXCEPT
    {
        Reset();
    }

    Ptr& operator=(T* rhs) BOOST_NOEXCEPT
    {
        if (p_ != rhs)
        {
            if (p_)
            {
                p_->Release();
            }
            p_ = rhs;
            if (p_)
            {
                p_->AddRef();
            }
        }
        return *this;
    }

    Ptr& operator=(const Ptr& rhs) BOOST_NOEXCEPT
    {
        return (*this = rhs.p_);
    }

    Ptr& operator=(Ptr&& rhs) BOOST_NOEXCEPT
    {
        if (p_ != rhs.p_)
        {
            if (p_)
            {
                p_->Release();
            }
            p_ = rhs.Detach();
        }
        return *this;
    }

    /**
     * @brief Query-copy constructor.
     *
     * @tparam U An interface type.
     *
     * This operator queries and manages a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    Ptr(const Ptr<U>& rhs) :
        p_(nullptr)
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        if (!!rhs)
        {
            p_ = static_cast<T*>(rhs->QueryInterface(uuid_of(p_)));
        }
    }

    /**
     * @brief Query-move contructor.
     *
     * @tparam U An interface type.
     *
     * This operator queries and manages a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    Ptr(Ptr<U>&& rhs) :
        p_(nullptr)
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        if (!!rhs)
        {
            p_ = static_cast<T*>(rhs->QueryInterface(uuid_of(p_)));
            rhs.Reset();
        }
    }

    /**
     * @brief Query-copy assignment.
     *
     * @tparam U An interface type.
     *
     * This operator queries and manages a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    Ptr& operator=(const Ptr<U>& rhs)
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        if (p_)
        {
            p_->Release();
            p_ = nullptr;
        }
        if (!!rhs)
        {
            p_ = static_cast<T*>(rhs->QueryInterface(uuid_of(p_)));
        }
        return *this;
    }

    /**
     * @brief Query-move assignment.
     *
     * @tparam U An interface type.
     *
     * This operator queries and manages a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    Ptr& operator=(Ptr<U>&& rhs)
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        if (p_)
        {
            p_->Release();
            p_ = nullptr;
        }
        if (!!rhs)
        {
            p_ = static_cast<T*>(rhs->QueryInterface(uuid_of(p_)));
            rhs.Reset();
        }
        return *this;
    }

    operator T*() const BOOST_NOEXCEPT
    {
        return p_;
    }

    T& operator*() const BOOST_NOEXCEPT
    {
        NSFX_ASSERT(p_);
        return *p_;
    }

    T* operator->() const BOOST_NOEXCEPT
    {
        NSFX_ASSERT(p_);
        return p_;
    }

    bool operator!() const BOOST_NOEXCEPT
    {
        return !p_;
    }

    bool operator==(const Ptr& rhs) const BOOST_NOEXCEPT
    {
        return (p_ == rhs.p_);
    }

    bool operator!=(const Ptr& rhs) const BOOST_NOEXCEPT
    {
        return (p_ != rhs.p_);
    }

    /**
     * @brief Equality comparison with querying.
     *
     * @tparam U An interface type.
     *
     * This operator queries a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.<br/>
     * Then, it performs comparison.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    bool operator==(const Ptr<U>& rhs) const
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        Ptr<T> tmp(rhs);
        return (tmp.p_ == rhs.p_);
    }

    /**
     * @brief Inequality comparison with querying.
     *
     * @tparam U An interface type.
     *
     * This operator queries a pointer of interface \c T from
     * the pointer \c rhs of interface \c U.<br/>
     * Then, it performs comparison.
     * <p>
     * It may throw exceptions from \c IObject::QueryInterface().
     */
    template<class U>
    bool operator!=(const Ptr<U>& rhs) const
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        Ptr<T> tmp(rhs);
        return (tmp.p_ != rhs.p_);
    }

    /**
     * @brief Release the managed interface and reset the pointer to \c nullptr.
     */
    void Reset(void) BOOST_NOEXCEPT
    {
        if (p_)
        {
            p_->Release();
            p_ = nullptr;
        }
    }

    /**
     * @brief Take the ownership of a pointer without calling \c IObject::AddRef().
     *
     * @param p Do not call \c IObject::Release() on the pointer after attached.
     */
    void Attach(T* rhs) BOOST_NOEXCEPT
    {
        if (p_)
        {
            p_->Release();
        }
        p_ = rhs;
    }

    /**
     * @brief Release the ownership of the pointer without calling \c IObject::Release().
     *
     * @return A pointer that is no longer managed by this smart pointer.<br/>
     *         User is responsible for calling \c IObject::Release() on the
     *         returned pointer when it's not used.
     */
    T* Detach(void) BOOST_NOEXCEPT
    {
        T* result = p_;
        p_ = nullptr;
        return result;
    }

private:
    T* p_;

}; // class Ptr /*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class T>
const uuid& uuid_of(const Ptr<T>& ) BOOST_NOEXCEPT
{
    return uuid_of<T>::GetUuid_();
}


NSFX_CLOSE_NAMESPACE


#endif // PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B

