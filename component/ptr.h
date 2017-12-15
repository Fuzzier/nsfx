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
#include <utility>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Ptr
/**
 * @ingroup Component
 * @brief A smart pointer that manages the lifetime of an object.
 *
 * @tparam T A type that conforms to \c IObjectConcept.
 */
template<class T>
class Ptr/*{{{*/
{
public:
    template<class U>
    friend class Ptr;

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

public:
    Ptr(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    Ptr(T* p, bool addRef) BOOST_NOEXCEPT :
        p_(p)
    {
        if (p_ && addRef)
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

    /**
     * @brief Manages a pointer, and calls \c IObject::AddRef().
     *
     * @param rhs The pointer still holds a reference count of the object.
     */
    Ptr& operator=(T* rhs) BOOST_NOEXCEPT
    {
        if (p_ != rhs)
        {
            Reset();
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
            Reset();
            p_ = rhs.Detach();
        }
        return *this;
    }

    /**
     * @brief Query-copy constructor.
     *
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries and manages a pointer of type \c T from
     * the pointer \c rhs of type \c U.
     */
    template<class U>
    Ptr(const Ptr<U>& rhs) BOOST_NOEXCEPT :
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
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries and manages a pointer of type \c T from
     * the pointer \c rhs of type \c U.
     */
    template<class U>
    Ptr(Ptr<U>&& rhs) BOOST_NOEXCEPT :
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
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries and manages a pointer of type \c T from
     * the pointer \c rhs of type \c U.
     */
    template<class U>
    Ptr& operator=(const Ptr<U>& rhs) BOOST_NOEXCEPT
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
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries and manages a pointer of type \c T from
     * the pointer \c rhs of type \c U.
     */
    template<class U>
    Ptr& operator=(Ptr<U>&& rhs) BOOST_NOEXCEPT
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

#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
    explicit operator bool() const BOOST_NOEXCEPT
    {
        return !!p_;
    }
#else // defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
private:
    bool ToBool(void) const BOOST_NOEXCEPT { return !!p_; }
public:
    typedef bool (Ptr::* BoolType)(void) const BOOST_NOEXCEPT;
    operator BoolType() const BOOST_NOEXCEPT
    {
        return !!p_ ? &Ptr::ToBool : nullptr;
    }
#endif // !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)

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
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries a pointer of type \c T from
     * the pointer \c rhs of type \c U.<br/>
     * Then, it performs comparison.
     */
    template<class U>
    bool operator==(const Ptr<U>& rhs) const BOOST_NOEXCEPT
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        Ptr<T> tmp(rhs);
        return (tmp.p_ == rhs.p_);
    }

    /**
     * @brief Inequality comparison with querying.
     *
     * @tparam U A type that conforms to \c IObjectConcept.
     *
     * This operator queries a pointer of type \c T from
     * the pointer \c rhs of type \c U.<br/>
     * Then, it performs comparison.
     */
    template<class U>
    bool operator!=(const Ptr<U>& rhs) const BOOST_NOEXCEPT
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        Ptr<T> tmp(rhs);
        return (tmp.p_ != rhs.p_);
    }

    /**
     * @brief Get the stored pointer without calling \c IObject::AddRef().
     */
    T* Get(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    /**
     * @brief Releases the managed object and resets to \c nullptr.
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
     * @brief Resets to pointer \c p, and calls \c IObject::AddRef().
     */
    void Reset(T* p) BOOST_NOEXCEPT
    {
        Reset();
        p_ = p;
        if (p_)
        {
            p_->AddRef();
        }
    }

    /**
     * @brief Resets to pointer \c p.
     */
    void Reset(T* p, bool addRef) BOOST_NOEXCEPT
    {
        Reset();
        p_ = p;
        if (p_ && addRef)
        {
            p_->AddRef();
        }
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

    void swap(T*& p) BOOST_NOEXCEPT
    {
        std::swap(p_, p);
    }

    void swap(Ptr& rhs) BOOST_NOEXCEPT
    {
        std::swap(p_, rhs.p_);
    }

    template<class U>
    friend void swap(Ptr<U>& lhs, Ptr<U>& rhs) BOOST_NOEXCEPT;
    template<class U>
    friend void swap(T*& lhs, Ptr<U>& rhs) BOOST_NOEXCEPT;
    template<class U>
    friend void swap(Ptr<U>& lhs, T*& rhs) BOOST_NOEXCEPT;

private:
    T* p_;

}; // class Ptr /*}}}*/


template<class T>
inline void swap(Ptr<T>& lhs, Ptr<T>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

template<class T>
inline void swap(T*& lhs, Ptr<T>& rhs) BOOST_NOEXCEPT
{
    rhs.swap(lhs);
}

template<class T>
inline void swap(Ptr<T>& lhs, T*& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


////////////////////////////////////////////////////////////////////////////////
// Enables boost::hash<Ptr<T> >.
template<class T>
inline size_t hash_value(const Ptr<T>& p) BOOST_NOEXCEPT
{
    return boost::hash<uintptr_t>(p.Get());
}


////////////////////////////////////////////////////////////////////////////////
// Extend uuid_of() to work for Ptr<T>./*{{{*/
template<class T>
const uuid& uuid_of(const Ptr<T>& ) BOOST_NOEXCEPT
{
    return uuid_of<T>::GetUuid_();
}
/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B

