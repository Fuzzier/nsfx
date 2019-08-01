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
#include <nsfx/component/object.h>
#include <boost/type_index.hpp>
#include <boost/core/swap.hpp>
#include <type_traits> // true_type, false_type, is_same
#include <iostream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Ptr
/**
 * @ingroup Component
 * @brief Base class of \c Ptr.
 *
 * This class is intended to provide common methods to be used by \c Ptr,
 * thus its does not declare the member variable \c PtrBase::p_ to be private.
 *
 * @tparam T A type that conforms to \c IObjectConcept.
 *
 * \c std::true_type is used in methods when the operand pointer is of the
 * same type as \c T.
 * While \c std::false_type is used in methods when the operand pointer is of
 * a different type than \c T.
 */
template<class T>
class PtrBase/*{{{*/
{
private:
    template<class U>
    friend class PtrBase;

    BOOST_CONCEPT_ASSERT((IObjectConcept<T>));

    typedef PtrBase<T>  ThisType;

protected:
    PtrBase(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    PtrBase(T* p, bool addRef, std::true_type) :
        p_(p)
    {
        if (p_ && addRef)
        {
            p->AddRef();
        }
    }

    template<class U>
    PtrBase(U* p, bool addRef, std::false_type) :
        p_(nullptr)
    {
        BOOST_CONCEPT_ASSERT((HasUidConcept<T>));
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!std::is_same<T, U>::value, "");
        if (p)
        {
            try
            {
                p_ = static_cast<T*>(p->QueryInterface(uid_of<T>()));
                if (!p_)
                {
                    BOOST_THROW_EXCEPTION(
                        NoInterface() <<
                        QueriedInterfaceUidErrorInfo(uid_of<T>()));
                }
            }
            catch (NoInterface& e)
            {
                e << QueriedFromInterfaceErrorInfo(
                        boost::typeindex::type_id<U>().pretty_name())
                  << QueriedInterfaceErrorInfo(
                        boost::typeindex::type_id<T>().pretty_name());
                throw;
            }
            if (!addRef)
            {
                p->Release();
            }
        }
    }

    PtrBase(Object<T>* p, bool addRef) :
        p_(nullptr)
    {
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    PtrBase(StaticObject<T>* p, bool addRef) :
        p_(nullptr)
    {
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    PtrBase(AggObject<T>* p, bool addRef) :
        p_(nullptr)
    {
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    PtrBase(MemberAggObject<T>* p, bool addRef) :
        p_(nullptr)
    {
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    PtrBase(MutualObject<T>* p, bool addRef) :
        p_(nullptr)
    {
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    ~PtrBase(void)
    {
        Reset();
    }

    void Reset(void)
    {
        if (p_)
        {
            p_->Release();
            p_ = nullptr;
        }
    }

    void Reset(T* p, bool addRef, std::true_type)
    {
        Reset();
        p_ = p;
        if (p_ && addRef)
        {
            p_->AddRef();
        }
    }

    template<class U>
    void Reset(U* p, bool addRef, std::false_type)
    {
        BOOST_CONCEPT_ASSERT((HasUidConcept<T>));
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!std::is_same<T, U>::value, "");
        Reset();
        if (p)
        {
            try
            {
                p_ = static_cast<T*>(p->QueryInterface(uid_of<T>()));
                if (!p_)
                {
                    BOOST_THROW_EXCEPTION(
                        NoInterface() <<
                        QueriedInterfaceUidErrorInfo(uid_of<T>()));
                }
            }
            catch (NoInterface& e)
            {
                e << QueriedFromInterfaceErrorInfo(
                        boost::typeindex::type_id<U>().pretty_name())
                  << QueriedInterfaceErrorInfo(
                        boost::typeindex::type_id<T>().pretty_name());
                throw;
            }
            if (!addRef)
            {
                p->Release();
            }
        }
    }

    void Reset(Object<T>* p, bool addRef)
    {
        Reset();
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    void Reset(StaticObject<T>* p, bool addRef)
    {
        Reset();
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    void Reset(AggObject<T>* p, bool addRef)
    {
        Reset();
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    void Reset(MemberAggObject<T>* p, bool addRef)
    {
        Reset();
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    void Reset(MutualObject<T>* p, bool addRef)
    {
        Reset();
        if (p)
        {
            p_ = p->GetImpl();
            if (addRef)
            {
                p_->AddRef();
            }
        }
    }

    bool IsSameObject(T* p, std::true_type) const BOOST_NOEXCEPT
    {
        return (p_ == p);
    }

    template<class U>
    bool IsSameObject(U* p, std::false_type) const
    {
        BOOST_CONCEPT_ASSERT((IObjectConcept<U>));
        static_assert(!std::is_same<T, U>::value, "");
        // Query the IObject interface.
        // Do not query other interfaces, since an object may not support them.
        PtrBase<IObject> lhs(p_, true, std::is_same<IObject, T>::type());
        PtrBase<IObject> rhs(p,  true, std::is_same<IObject, U>::type());
        return (lhs.p_ == rhs.p_);
    }

protected:
    T* p_;
}; // class PtrBase /*}}}*/

/**
 * @ingroup Component
 * @brief A smart pointer that manages the lifetime of an object.
 *
 * @tparam T A type that conforms to \c IObjectConcept.
 *
 * @remarks If the smart pointer is constructed from or assigned to a pointer of
 *          a different type, then the template parameter \c T must also conform
 *          to \c HasUidConcept.
 *          Thus, the smart pointer is able to query an interface of type \c T
 *          from the source pointer.
 *
 * # Strong exception safety.
 *  If the smart point fails to query the interface, \c NoInterface is thrown.
 *  The smart pointer is put into an empty state.
 *  However, the source pointer is intact.
 *  Especially, the reference count of the source pointer is intact.
 *  Thus, users are still <b>responsible to free the source pointer</b> to
 *  prevent memory leak, if the source pointer is not a smart pointer.
 *  <p>
 *  Caution must be taken when writing such code:
 *  @code
 *  Ptr<I>  p(new C);
 *  @endcode
 *  <p>
 *  If \c C is different than \c I, then the smart pointer \c p will query
 *  an interface of type \c I from the newly allocated instance of type \c C.
 *  However, if the query fails, there is a <i>memory leak</i> that the
 *  instance of \c C is not deallocated.
 *  <p>
 *  The above code is safe only if the user can make sure that \c C supplies
 *  an interface of \c I.
 *  Otherwise, the following more secure code shall be used:
 *  @code
 *  C* c = new C;
 *  try
 *  {
 *      Ptr<I>  p(c);
 *  }
 *  catch (NoInterface& )
 *  {
 *      delete c;
 *      throw;
 *  }
 *  @endcode
 *  <p>
 *  Generally, it is safe to write:
 *  @code
 *  Ptr<C>        c(new C);
 *  Ptr<IObject>  o(new C);
 *  @endcode
 */
template<class T = IObject>
class Ptr : private PtrBase<T>/*{{{*/
{
private:
    template<class U>
    friend class Ptr;

    typedef PtrBase<T>  BaseType;

    // Constructors./*{{{*/
public:
    /**
     * @brief Construct a null smart pointer.
     */
    Ptr(void) BOOST_NOEXCEPT
    {
    }

    Ptr(nullptr_t) BOOST_NOEXCEPT
    {
    }

    /**
     * @brief Construct a smart pointer from a pointer without taking its reference count.
     *
     * @param p  A pointer to the object that is to be managed by the smart
     *           pointer.
     */
    Ptr(T* p) :
        BaseType(p, true, std::is_same<T, T>::type())
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
    Ptr(U* p) :
        BaseType(p, true, std::is_same<T, U>::type())
    {
    }

    /**
     * @brief Construct a smart pointer from a pointer.
     *
     * @param p  A pointer to the object that is to be managed by the smart
     *           pointer.
     * @param addRef If \c false, the pointer \c p gives up one reference
     *               count to this smart pointer.
     *
     * @remarks When \c U is not the same type as \c T, a pointer of type \c T*
     *          is queried from the pointer \c p, and may result in \c nullptr.
     *          If \c addRef is \c false at that time, then the pointer
     *          \c p loses one reference count, and the smart pointer under
     *          construction holds also no reference count.
     *          The object may be leaked.
     */
    template<class U>
    Ptr(U* p, bool addRef) :
        BaseType(p, addRef, std::is_same<T, U>::type())
    {
    }

    Ptr(const Ptr& rhs) :
        BaseType(rhs.p_, true, std::is_same<T, T>::type())
    {
    }

    template<class U>
    Ptr(const Ptr<U>& rhs) :
        BaseType(rhs.p_, true, std::is_same<T, U>::type())
    {
    }

    Ptr(Ptr<T>&& rhs) :
        BaseType(rhs.p_, false, std::is_same<T, T>::type())
    {
        rhs.p_ = nullptr;
    }

    template<class U>
    Ptr(Ptr<U>&& rhs) :
        BaseType(rhs.p_, false, std::is_same<T, U>::type())
    {
        rhs.p_ = nullptr;
    }

    Ptr(Object<T>* p) :
        BaseType(p, true)
    {
    }

    Ptr(StaticObject<T>* p) :
        BaseType(p, true)
    {
    }

    Ptr(AggObject<T>* p) :
        BaseType(p, true)
    {
    }

    Ptr(MemberAggObject<T>* p) :
        BaseType(p, true)
    {
    }

    Ptr(MutualObject<T>* p) :
        BaseType(p, true)
    {
    }

    Ptr(Object<T>* p, bool addRef) :
        BaseType(p, addRef)
    {
    }

    Ptr(StaticObject<T>* p, bool addRef) :
        BaseType(p, addRef)
    {
    }

    Ptr(AggObject<T>* p, bool addRef) :
        BaseType(p, addRef)
    {
    }

    Ptr(MemberAggObject<T>* p, bool addRef) :
        BaseType(p, addRef)
    {
    }

    Ptr(MutualObject<T>* p, bool addRef) :
        BaseType(p, addRef)
    {
    }

    /*}}}*/

    // Operators./*{{{*/
    Ptr& operator=(nullptr_t)
    {
        BaseType::Reset();
        return *this;
    }

    template<class U>
    Ptr& operator=(U* rhs)
    {
        BaseType::Reset(rhs, true, std::is_same<T, U>::type());
        return *this;
    }

    Ptr& operator=(const Ptr& rhs)
    {
        BaseType::Reset(rhs.p_, true, std::is_same<T, T>::type());
        return *this;
    }

    template<class U>
    Ptr& operator=(const Ptr<U>& rhs)
    {
        BaseType::Reset(rhs.p_, true, std::is_same<T, U>::type());
        return *this;
    }

    Ptr& operator=(Ptr&& rhs)
    {
        BaseType::Reset(rhs.p_, false, std::is_same<T, T>::type());
        rhs.p_ = nullptr;
        return *this;
    }

    template<class U>
    Ptr& operator=(Ptr<U>&& rhs)
    {
        BaseType::Reset(rhs.p_, false, std::is_same<T, U>::type());
        rhs.p_ = nullptr;
        return *this;
    }

    Ptr& operator=(Object<T>* rhs)
    {
        BaseType::Reset(rhs, true);
        return *this;
    }

    Ptr& operator=(StaticObject<T>* rhs)
    {
        BaseType::Reset(rhs, true);
        return *this;
    }

    Ptr& operator=(AggObject<T>* rhs)
    {
        BaseType::Reset(rhs, true);
        return *this;
    }

    Ptr& operator=(MemberAggObject<T>* rhs)
    {
        BaseType::Reset(rhs, true);
        return *this;
    }

    Ptr& operator=(MutualObject<T>* rhs)
    {
        BaseType::Reset(rhs, true);
        return *this;
    }

    T& operator*() const BOOST_NOEXCEPT
    {
        BOOST_ASSERT_MSG(p_, "Cannot dereference a Ptr<> that is nullptr.");
        return *p_;
    }

    T* operator->() const BOOST_NOEXCEPT
    {
        BOOST_ASSERT_MSG(p_, "Cannot dereference a Ptr<> that is nullptr.");
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
        return BaseType::IsSameObject(rhs, std::is_same<T, T>::type());
    }

    /**
     * @brief Test whether the pointers refer to the <b>same</b> object.
     */
    template<class U>
    bool operator==(U* rhs) const
    {
        return BaseType::IsSameObject(rhs, std::is_same<T, U>::type());
    }

    template<class U>
    bool operator==(const Ptr<U>& rhs)
    {
        return BaseType::IsSameObject(rhs.p_, std::is_same<T, U>::type());
    }

    /**
     * @brief Test whether the pointer refers to a <b>different</b> object.
     */
    bool operator!=(T* rhs) const BOOST_NOEXCEPT
    {
        return !BaseType::IsSameObject(rhs, std::is_same<T, T>::type());
    }

    /**
     * @brief Test whether the pointer refers to a <b>different</b> object.
     */
    template<class U>
    bool operator!=(U* rhs) const
    {
        return !BaseType::IsSameObject(rhs, std::is_same<T, U>::type());
    }

    template<class U>
    bool operator!=(const Ptr<U>& rhs)
    {
        return !BaseType::IsSameObject(rhs.p_, std::is_same<T, U>::type());
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
    void Reset(void)
    {
        BaseType::Reset();
    }

    /**
     * @brief Reset to the pointer \c p without taking its reference count.
     */
    void Reset(T* p)
    {
        BaseType::Reset(p, true, std::is_same<T, T>::type());
    }

    /**
     * @brief Reset to the pointer \c p.
     */
    void Reset(T* p, bool addRef)
    {
        BaseType::Reset(p, addRef, std::is_same<T, T>::type());
    }

    /**
     * @brief Reset to the pointer \c p without taking its reference count.
     */
    template<class U>
    void Reset(U* p)
    {
        BaseType::Reset(p, true, std::is_same<T, U>::type());
    }

    /**
     * @brief Resets to the pointer \c p.
     */
    template<class U>
    void Reset(U* p, bool addRef)
    {
        BaseType::Reset(p, addRef, std::is_same<T, U>::type());
    }

    void Reset(Object<T>* p)
    {
        BaseType::Reset(p, true);
    }

    void Reset(StaticObject<T>* p)
    {
        BaseType::Reset(p, true);
    }

    void Reset(AggObject<T>* p)
    {
        BaseType::Reset(p, true);
    }

    void Reset(MemberAggObject<T>* p)
    {
        BaseType::Reset(p, true);
    }

    void Reset(MutualObject<T>* p)
    {
        BaseType::Reset(p, true);
    }

    void Reset(Object<T>* p, bool addRef)
    {
        BaseType::Reset(p, addRef);
    }

    void Reset(StaticObject<T>* p, bool addRef)
    {
        BaseType::Reset(p, addRef);
    }

    void Reset(AggObject<T>* p, bool addRef)
    {
        BaseType::Reset(p, addRef);
    }

    void Reset(MemberAggObject<T>* p, bool addRef)
    {
        BaseType::Reset(p, addRef);
    }

    void Reset(MutualObject<T>* p, bool addRef)
    {
        BaseType::Reset(p, addRef);
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
        boost::swap(p_, p);
    }

    void swap(Ptr& rhs) BOOST_NOEXCEPT
    {
        boost::swap(p_, rhs.p_);
    }

    /*}}}*/

}; // class Ptr /*}}}*/


template<class T, class U>
inline bool
operator==(T* lhs, const Ptr<U>& rhs)
{
    return rhs.operator==(lhs);
}

template<class T, class U>
inline bool
operator!=(T* lhs, const Ptr<U>& rhs)
{
    return rhs.operator!=(lhs);
}

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
// Enables boost::hash<Ptr<T>>.
/**
 * @ingroup Component
 * @brief Hash value of a smart pointer.
 */
template<class T>
inline size_t hash_value(const Ptr<T>& p) BOOST_NOEXCEPT
{
    return boost::hash<T*>()(p.Get());
}

template<class CharT, class TraitsT, class T>
inline std::basic_ostream<CharT, TraitsT>&
operator<<(std::basic_ostream<CharT, TraitsT>& os, const Ptr<T>& rhs)
{
    return os << "0x" << rhs.Get();
}


////////////////////////////////////////////////////////////////////////////////
template<class T>
struct IsPtr : std::false_type {};

template<class T>
struct IsPtr<Ptr<T>> : std::true_type {};


NSFX_CLOSE_NAMESPACE


#endif // PTR_H__D7EB912E_B221_4596_844E_FD6191C0945B

