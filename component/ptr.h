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
#include <nsfx/component/i-object.h>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_index.hpp>
#include <utility>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Ptr
/**
 * @ingroup Component
 * @brief Base class of \c Ptr.
 *
 * This class is intended to provide common methods to be used by \c Ptr,
 * thus its does not declare the member variable \c PtrBase::p_ to be private.<br/>
 *
 * @tparam T A type that conforms to \c IObjectConcept.
 * @tparam nothrow_ Do not throw \c NoInterface when failed to query an interface
 *                  that is different from \c T.
 *
 * \c boost::true_type is used in methods when the operand pointer is of the
 * same type as \c T.<br/>
 * While \c boost::false_type is used in methods when the operand pointer is of
 * a different type than \c T.<br/>
 *
 * @throw \c NoInterface If \c nothrow_ is \c true, the exception is thrown when
 *                       an interface cannot be queried from the source pointer.<br/>
 *                       If \c takeRefCount argument of a method is \c true,
 *                       then the exception is thrown <b>before</b> the
 *                       reference count is taken.<br/>
 *                       After the exception is thrown, this smart pointer is
 *                       reset to \c nullptr, and the source pointer is intact.<br/>
 *                       <p>
 *                       Since the exception may be thrown at the constructor
 *                       of this base class, the child class \c Ptr may not
 *                       properly set the source smart pointer to \c nullptr.<br/>
 *                       So the exception has to be thrown before the reference
 *                       count is taken.<br/>
 *                       Otherwise, the reference count may be taken without
 *                       setting the source smart pointer to \c nullptr.
 */
template<class T, bool nothrow_ = true>
class PtrBase/*{{{*/
{
private:
    template<class U, bool>
    friend class PtrBase;

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

    typedef PtrBase<T>  ThisType;

protected:
    PtrBase(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    PtrBase(T* p, bool takeRefCount, boost::true_type) BOOST_NOEXCEPT :
        p_(p)
    {
        if (p_ && !takeRefCount)
        {
            p->AddRef();
        }
    }

    template<class U>
    PtrBase(U* p, bool takeRefCount, boost::false_type) BOOST_NOEXCEPT_IF(nothrow_) :
        p_(nullptr)
    {
        BOOST_CONCEPT_ASSERT((HasUuidConcept<T>));
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!boost::is_same<T, U>::value, "");
        if (p)
        {
            p_ = static_cast<T*>(p->QueryInterface(uuid_of<T>()));
            ThrowNoInterface<T, U>(boost::integral_constant<bool, nothrow_>::type());
            if (takeRefCount)
            {
                p->Release();
            }
        }
    }

    ~PtrBase(void) BOOST_NOEXCEPT
    {
        Reset();
    }

    void Reset(void) BOOST_NOEXCEPT
    {
        if (p_)
        {
            p_->Release();
            p_ = nullptr;
        }
    }

    void Reset(T* p, bool takeRefCount, boost::true_type) BOOST_NOEXCEPT
    {
        Reset();
        p_ = p;
        if (p_ && !takeRefCount)
        {
            p_->AddRef();
        }
    }

    template<class U>
    void Reset(U* p, bool takeRefCount, boost::false_type) BOOST_NOEXCEPT_IF(nothrow_)
    {
        BOOST_CONCEPT_ASSERT((HasUuidConcept<T>));
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!boost::is_same<T, U>::value, "");
        Reset();
        if (p)
        {
            p_ = static_cast<T*>(p->QueryInterface(uuid_of<T>()));
            ThrowNoInterface<T, U>(boost::integral_constant<bool, nothrow_>::type());
            if (takeRefCount)
            {
                p->Release();
            }
        }
    }

    bool IsSameObject(T* p, boost::true_type) const BOOST_NOEXCEPT
    {
        return (p_ == p);
    }

    template<class U>
    bool IsSameObject(U* p, boost::false_type) const BOOST_NOEXCEPT
    {
        BOOST_CONCEPT_ASSERT((HasUuidConcept<T>));
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!boost::is_same<T, U>::value, "");
        // Query the IObject interface.
        // Do not query other interfaces, since an object may not support them.
        PtrBase<IObject> lhs(p_, false, boost::is_same<IObject, T>::type());
        PtrBase<IObject> rhs(p, false, boost::is_same<IObject, U>::type());
        return (lhs.p_ == rhs.p_);
    }

    template<class T, class U>
    void ThrowNoInterface(/* nothrow */boost::true_type) BOOST_NOEXCEPT {}

    template<class T, class U>
    void ThrowNoInterface(/* throw */boost::false_type)
    {
        if (!p_)
        {
            BOOST_THROW_EXCEPTION(NoInterface()
                << QuerySourceTypeErrorInfo(boost::typeindex::type_id<U>().pretty_name())
                << QueryTargetTypeErrorInfo(boost::typeindex::type_id<T>().pretty_name())
            );
        }
    }

protected:
    T* p_;
}; // class PtrBase /*}}}*/

/**
 * @ingroup Component
 * @brief A smart pointer that manages the lifetime of an object.
 *
 * @tparam T A type that conforms to \c IObjectConcept.
 * @tparam nothrow_ Do not throw \c NoInterface when failed to query an interface
 *                  that is different from \c T.
 *
 * @remarks If the pointer is constructed from or assigned to a pointer of a
 *          different type, then the template parameter \c T must also conform
 *          to \c HasUuidConcept.
 */
template<class T = IObject, bool nothrow_ = true>
class Ptr : private PtrBase<T, nothrow_>/*{{{*/
{
private:
    template<class U, bool>
    friend class Ptr;

    typedef PtrBase<T, nothrow_>  BaseType;

    // Constructors./*{{{*/
public:
    /**
     * @brief Construct a null smart pointer.
     */
    Ptr(void) BOOST_NOEXCEPT
    {
    }

    explicit Ptr(nullptr_t) BOOST_NOEXCEPT
    {
    }

    /**
     * @brief Construct a smart pointer from a pointer without taking its reference count.
     *
     * @param p  A pointer to the object that is to be managed by the smart
     *           pointer.
     */
    explicit Ptr(T* p) BOOST_NOEXCEPT :
        BaseType(p, false, boost::is_same<T, T>::type())
    {
    }

    /**
     * @brief Construct a smart pointer from a pointer without taking its reference count.
     *
     * @tparam U A type that conforms to \c IObjectConcept.
     * @param p  A pointer to the object that is to be managed by the smart
     *           pointer.
     */
    template<class U>
    explicit Ptr(U* p) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value)) :
        BaseType(p, false, boost::is_same<T, U>::type())
    {
    }

    /**
     * @brief Construct a smart pointer from a pointer.
     *
     * @param p  A pointer to the object that is to be managed by the smart
     *           pointer.
     * @param takeRefCount If \c true, the pointer \c p gives up one reference
     *                     count that it held, and may no longer manages the
     *                     lifetime of the object.
     *
     * @remarks When \c U is not the same type as \c T, a pointer of type \c T*
     *          is queried from the pointer \c p, and may result in \c nullptr.<br/>
     *          If \c takeRefCount is \c true at that time, then the pointer
     *          \c p loses one reference count, and the smart pointer under
     *          construction holds also no reference count.<br/>
     *          The user may lose the object completely.
     */
    template<class U>
    Ptr(U* p, bool takeRefCount) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value)) :
        BaseType(p, takeRefCount, boost::is_same<T, U>::type())
    {
    }

    Ptr(const Ptr& rhs) BOOST_NOEXCEPT :
        BaseType(rhs.p_, false, boost::is_same<T, T>::type())
    {
    }

    template<class U, bool nothrow2>
    Ptr(const Ptr<U, nothrow2>& rhs) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value)) :
        BaseType(rhs.p_, false, boost::is_same<T, U>::type())
    {
    }

    Ptr(Ptr<T>&& rhs) BOOST_NOEXCEPT :
        BaseType(rhs.p_, true, boost::is_same<T, T>::type())
    {
        rhs.p_ = nullptr;
    }

    template<class U, bool nothrow2>
    Ptr(Ptr<U, nothrow2>&& rhs) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value)) :
        BaseType(rhs.p_, true, boost::is_same<T, U>::type())
    {
        rhs.p_ = nullptr;
    }

    /*}}}*/

    // Operators./*{{{*/
    Ptr& operator=(nullptr_t) BOOST_NOEXCEPT
    {
        BaseType::Reset();
        return *this;
    }

    template<class U>
    Ptr& operator=(U* rhs) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value))
    {
        BaseType::Reset(rhs, false, boost::is_same<T, U>::type());
        return *this;
    }

    Ptr& operator=(const Ptr& rhs) BOOST_NOEXCEPT
    {
        BaseType::Reset(rhs.p_, false, boost::is_same<T, T>::type());
        return *this;
    }

    template<class U, bool nothrow2>
    Ptr& operator=(const Ptr<U, nothrow2>& rhs) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value))
    {
        BaseType::Reset(rhs.p_, false, boost::is_same<T, U>::type());
        return *this;
    }

    Ptr& operator=(Ptr&& rhs) BOOST_NOEXCEPT
    {
        BaseType::Reset(rhs.p_, true, boost::is_same<T, T>::type());
        rhs.p_ = nullptr;
        return *this;
    }

    template<class U, bool nothrow2>
    Ptr& operator=(Ptr<U, nothrow2>&& rhs) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value))
    {
        BaseType::Reset(rhs.p_, true, boost::is_same<T, U>::type());
        rhs.p_ = nullptr;
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
public:
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

    /**
     * @brief Test whether the pointers refer to the <b>same</b> object.
     */
    bool operator==(T* rhs) const BOOST_NOEXCEPT
    {
        return BaseType::IsSameObject(rhs, boost::is_same<T, T>::type());
    }

    /**
     * @brief Test whether the pointers refer to the <b>same</b> object.
     */
    template<class U>
    bool operator==(U* rhs) const BOOST_NOEXCEPT
    {
        return BaseType::IsSameObject(rhs, boost::is_same<T, U>::type());
    }

    template<class U, bool nothrow2>
    bool operator==(const Ptr<U, nothrow2>& rhs) BOOST_NOEXCEPT
    {
        return BaseType::IsSameObject(rhs.p_, boost::is_same<T, U>::type());
    }

    /**
     * @brief Test whether the pointer refers to a <b>different</b> object.
     */
    bool operator!=(T* rhs) const BOOST_NOEXCEPT
    {
        return !BaseType::IsSameObject(rhs, boost::is_same<T, T>::type());
    }

    /**
     * @brief Test whether the pointer refers to a <b>different</b> object.
     */
    template<class U>
    bool operator!=(U* rhs) const BOOST_NOEXCEPT
    {
        return !BaseType::IsSameObject(rhs, boost::is_same<T, U>::type());
    }

    template<class U, bool nothrow2>
    bool operator!=(const Ptr<U, nothrow2>& rhs) BOOST_NOEXCEPT
    {
        return !BaseType::IsSameObject(rhs.p_, boost::is_same<T, U>::type());
    }

    /*}}}*/

    // Methods./*{{{*/
    /**
     * @brief Obtain a raw pointer.
     *
     * @remarks The returned pointer does <b>not</b> hold a reference count to
     *          release.
     */
    T* Get(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    /**
     * @brief Release the held reference count and reset to \c nullptr.
     */
    void Reset(void) BOOST_NOEXCEPT
    {
        BaseType::Reset();
    }

    /**
     * @brief Reset to the pointer \c p without taking its reference count.
     */
    template<class U>
    void Reset(U* p) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value))
    {
        BaseType::Reset(p, false, boost::is_same<T, U>::type());
    }

    /**
     * @brief Resets to the pointer \c p.
     */
    template<class U>
    void Reset(U* p, bool takeRefCount) BOOST_NOEXCEPT_IF(nothrow_ || (boost::is_same<T, U>::value))
    {
        BaseType::Reset(p, takeRefCount, boost::is_same<T, U>::type());
    }

    /**
     * @brief Detach the stored raw pointer.
     *
     * @remarks The returned pointer <b>does</b> hold a reference count.
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

    /*}}}*/

}; // class Ptr /*}}}*/


template<class T, class U, bool nothrowU>
inline bool
operator==(T* lhs, const Ptr<U, nothrowU>& rhs) BOOST_NOEXCEPT
{
    return rhs.operator==(lhs);
}

template<class T, class U, bool nothrowU>
inline bool
operator!=(T* lhs, const Ptr<U, nothrowU>& rhs) BOOST_NOEXCEPT
{
    return rhs.operator!=(lhs);
}

template<class T, bool nothrowT>
inline void swap(Ptr<T, nothrowT>& lhs, Ptr<T, nothrowT>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

template<class T, bool nothrowT>
inline void swap(T*& lhs, Ptr<T, nothrowT>& rhs) BOOST_NOEXCEPT
{
    rhs.swap(lhs);
}

template<class T, bool nothrowT>
inline void swap(Ptr<T, nothrowT>& lhs, T*& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}


////////////////////////////////////////////////////////////////////////////////
// Enables boost::hash<Ptr<T> >.
/**
 * @ingroup Component
 * @brief Hash value of a smart pointer.
 */
template<class T>
inline size_t hash_value(const Ptr<T>& p) BOOST_NOEXCEPT
{
    return boost::hash<T*>()(p.Get());
}


NSFX_CLOSE_NAMESPACE


#endif // PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B

