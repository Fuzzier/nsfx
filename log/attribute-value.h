/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B
#define ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B


#include <nsfx/log/config.h>
#include <typeinfo>
#include <memory>
#include <utility>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Logging
 * @brief A type dispatcher carries the \c type_info of the requested value
 *        type, and a user-defined callback to retrieve the value.
 *
 * \c AttributeValue objects use the \c TypeDispatcher object to deliver
 * the stored value to users.
 *
 * There're two uncertainties:
 * 1. The \c AttributeValue object must know which type is requested.
 * 2. The \c AttributeValue object must invoke the callback.
 *
 * 1. The \c AttributeValue object must know which type is requested.
 * 1) The \c TypeDispatcher object must expose a method \c GetTypeId() to
 *    allow the \c AttributeValue object to examine the requested type.
 * 2) The \c AttributeValue object cannot provide the value type for
 *    \c TypeDispatcher to specialize class/function templates.
 * 3) The \c TypeDispatcher object doesn't know the exact type either.
 *    To avoid \c GetTypeId() to be a virtual function, it just stores
 *    a <code>const std::type_info&</code> pointer.
 *
 * 2. The \c AttributeValue object must invoke the callback.
 * 1) After the \c AttributeValue object examined the
 *    <code>cosnt std::type_info&</code> pointer returned by \c GetTypeId(),
 *    it knows the requested type.
 * 2) The requested value type can be provided to the \c TypeDispatcher object.
 *    The \c TypeDispatcher object can use the value type to specialize
 *    class/function templates.
 *    That's why class template <code>CallbackWithValueType<></code> is defined.
 *    As the value type \c ValueT is known, the template can be specialized to
 *    class <code>CallbackWithValueType<ValueT></code>.
 * 3) However, the actual type of the user-defined callback will never be known
 *    by either the \c AttributeValue object the or \c TypeDispatcher object.
 *    A virtual function call is inevitable.
 *    That's why the virtual function \c Invoke(ValueT&) is defined for
 *    <code>CallbackWithValueType<ValueT></code>.
 */
class TypeDispatcher /*{{{*/
{
    // Callback interface./*{{{*/
private:
    class Callback /*{{{*/
    {
    public:
        virtual ~Callback(void) NSFX_NOEXCEPT {}
    }; // class Callback /*}}}*/

    template<class ValueT>
    class CallbackWithValueType : public Callback/*{{{*/
    {
    public:
        virtual ~CallbackWithValueType(void) NSFX_NOEXCEPT {}

        // Operations./*{{{*/
    public:
        virtual void Invoke(const ValueT& value) = 0;
        /*}}}*/
    }; // class CallbackWithValueType/*}}}*/

    /**
     * @brief The class that stores the user-defined callback to retrieve the
     *        value stored in an \c AttributeValue object.
     */
    template<class ValueT, class CallbackT>
    class CallbackImpl : public CallbackWithValueType<ValueT> /*{{{*/
    {
    public:
        CallbackImpl(const CallbackT& callback) :/*{{{*/
            callback_(callback)
        {
        }/*}}}*/

        CallbackImpl(CallbackT&& callback) :/*{{{*/
            callback_(std::move(callback))
        {
        }/*}}}*/

        virtual ~CallbackImpl(void) NSFX_NOEXCEPT {}

        // CallbackWithValueType<ValueT> operations./*{{{*/
    public:
        /**
         * @brief Invokes user-defined callback to deliver the value to user.
         */
        virtual void Invoke(const ValueT& value) NSFX_OVERRIDE/*{{{*/
        {
            callback_(value);
        }/*}}}*/

        /*}}}*/

        // Properties./*{{{*/
    private:
        /**
         * @brief The user-defined callable object to retrieve the value stored in
         *        an \c AttributeValue object.
         */
        CallbackT callback_;
        /*}}}*/
    }; // class CallbackImpl /*}}}*/
/*}}}*/

public:
    TypeDispatcher(void) :/*{{{*/
        typeid_(nullptr),
        callback_(nullptr)
    {
    }/*}}}*/

private:
    TypeDispatcher(const std::type_info& valueTypeId, /*{{{*/
                   const std::shared_ptr<Callback>& callback) :
        typeid_(&valueTypeId),
        callback_(callback)
    {
    }/*}}}*/

    TypeDispatcher(const std::type_info& valueTypeId, /*{{{*/
                   std::shared_ptr<Callback>&& callback) :
        typeid_(&valueTypeId),
        callback_(std::move(callback))
    {
    }/*}}}*/

    /**
     * @brief Need to access the constructor of \c TypeDispatcher.
     */
    template<class ValueT, class CallbackT>
    friend TypeDispatcher MakeTypeDispatcher(CallbackT&& callback);

    // Methods./*{{{*/
public:
    /**
     * @brief An \c AttributeValue object uses this function to check whether it
     *        can convert the its value into the requested type or not.
     */
    const std::type_info& GetTypeId(void) const/*{{{*/
    {
        NSFX_ASSERT(typeid_);
        return *typeid_;
    }/*}}}*/

    /**
     * @brief An \c AttributeValue object uses this function to deliver its
     *        stored value to the user.
     */
    template<class ValueT>
    void Dispatch(const ValueT& value) const/*{{{*/
    {
        CallbackWithValueType<ValueT>* callback =
            static_cast<CallbackWithValueType<ValueT>*>(callback_.get());
        callback->Invoke(value);
    }/*}}}*/

    /*}}}*/

    // Properties./*{{{*/
private:
    const std::type_info* typeid_;
    std::shared_ptr<Callback> callback_;
    /*}}}*/
}; // class TypeDispatcher /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Logging
 * @brief Make a \c TypeDispatcher object that carries a user-defined callback
 *        that retrieves the value stored in an \c AttributeValue object.
 *
 * The user-defined callback is a callable object with the signature:
 * <code>void Callback(const ValueType& value)</code>, where \c ValueType is
 * the type of the desired value.
 * The \c value passed to \c Callback() is only valid during the invocation,
 * and it's not guaranteed to remain valid outside \c Callback().
 * If the \c AttributeValue object doesn't support to convert its stored value
 * into the requested \c ValueType, the callback is not invoked.
 *
 * @tparam ValueT Users want the \c AttributeValue object to convert its stored
 *                value to this type.
 */
template<class ValueT, class CallbackT>
inline TypeDispatcher MakeTypeDispatcher(CallbackT&& callback)/*{{{*/
{
    return TypeDispatcher(
        typeid (ValueT),
        std::make_shared<TypeDispatcher::CallbackImpl<ValueT, CallbackT>>(
            std::forward<CallbackT>(callback)));
}/*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Logging
 * @brief Attribute value is carried by log records.
 *
 * \c AttributeValue object never stores references, it stores concreet values.
 */
class AttributeValue /*{{{*/
{
public:
    class Impl /*{{{*/
    {
    public:
        virtual ~Impl(void) NSFX_NOEXCEPT {}

        // Impl operations./*{{{*/
    public:
        virtual const std::type_info& GetTypeId(void) const NSFX_NOEXCEPT = 0;

        /**
         * @brief If the requested value type is supported, the user-defined
         *        callback is invoked.
         *
         * @param dispatcher The type dispatcher that carries the \c type_info
         *                   of the specified value type, and a user-defined
         *                   callback function to retrieve the value.
         *
         * @return Return \c true if the requested value type is supported and
         *         the user-defined callback is invoked. If the requested value
         *         type is not supported, then user-defined callback is not
         *         invoked, and this function returns \c false.
         */
        virtual bool Dispatch(const TypeDispatcher& dispatcher) = 0;
        /*}}}*/
    }; // class Impl /*}}}*/

    // Constructors./*{{{*/
public:
    AttributeValue(void) NSFX_NOEXCEPT {}

    AttributeValue(const std::shared_ptr<Impl>& impl) :/*{{{*/
        impl_(impl)
    {
    }/*}}}*/

    AttributeValue(std::shared_ptr<Impl>&& impl) :/*{{{*/
        impl_(std::move(impl))
    {
    }/*}}}*/

    /*}}}*/

    // Methods./*{{{*/
public:
    const std::type_info& GetTypeId(void) const/*{{{*/
    {
        NSFX_ASSERT(!!impl_);
        return impl_->GetTypeId();
    }/*}}}*/

    bool Dispatch(const TypeDispatcher& dispatcher) const/*{{{*/
    {
        NSFX_ASSERT(!!impl_);
        return impl_->Dispatch(dispatcher);
    }/*}}}*/

    /*}}}*/

    // Properties./*{{{*/
private:
    std::shared_ptr<Impl> impl_;
    /*}}}*/
}; // class Attribute /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @
 * @brief A default implemenation of \c AttributeValue::Impl.
 *
 * @tparam ValueT The value type.
 */
template<class ValueT>
class AttributeValueImpl : public AttributeValue::Impl /*{{{*/
{
    typedef ValueT ValueType;

    // Constructors./*{{{*/
public:
#if NSFX_HAS_VARIADIC_TEMPLATE
    template<class... Args>
    AttributeValueImpl(Args&&... args) :/*{{{*/
        value_(std::forward<Args>(args)...)
    {
    }/*}}}*/

#else // !(NSFX_HAS_VARIADIC_TEMPLATE)
    AttributeValueImpl(void) :/*{{{*/
        value_()
    {
    }/*}}}*/

    template<class A1>
    AttributeValueImpl(A1&& a1) :/*{{{*/
        value_(
            std::forward<A1>(a1)
            )
    {
    }/*}}}*/

    template<class A1, class A2>
    AttributeValueImpl(A1&& a1, A2&& a2) :/*{{{*/
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3) :/*{{{*/
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4) :/*{{{*/
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5) :/*{{{*/
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5,
             class A6>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
                       A6&& a6) :
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5,
             class A6, class A7>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
                       A6&& a6, A7&& a7) :
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5,
             class A6, class A7, class A8>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
                       A6&& a6, A7&& a7, A8&& a8) :
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5,
             class A6, class A7, class A8, class A9>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
                       A6&& a6, A7&& a7, A8&& a8, A9&& a9) :
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8),
            std::forward<A9>(a9)
            )
    {
    }/*}}}*/

    template<class A1, class A2, class A3, class A4, class A5,
             class A6, class A7, class A8, class A9, class A10>
    AttributeValueImpl(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
                       A6&& a6, A7&& a7, A8&& a8, A9&& a9, A10&& a10) :
        value_(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8),
            std::forward<A9>(a9),
            std::forward<A10>(a10)
            )
    {
    }/*}}}*/

#endif // NSFX_HAS_VARIADIC_TEMPLATE
    /*}}}*/

    virtual ~AttributeValueImpl(void) NSFX_NOEXCEPT {}

    // AttributeValue::Impl operations./*{{{*/
public:
    virtual const std::type_info& GetTypeId(void) const NSFX_NOEXCEPT NSFX_OVERRIDE/*{{{*/
    {
        return typeid (ValueType);
    }/*}}}*/

    virtual bool Dispatch(const TypeDispatcher& dispatcher) NSFX_OVERRIDE/*{{{*/
    {
        bool result = false;
        if (dispatcher.GetTypeId() == typeid (ValueType))
        {
            dispatcher.Dispatch(value_);
            result = true;
        }
        return result;
    }/*}}}*/

    /*}}}*/

    // Properties./*{{{*/
private:
    ValueType value_;
    /*}}}*/
}; // class AttributeValueImpl /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// MakeAttributeValue()/*{{{*/.
#if NSFX_HAS_VARIADIC_TEMPLATE
/**
 * @ingroup Logging
 * @brief Make an attribute value.
 *
 * \c AttributeValue object never stores references, it stores concreet values.
 * The \c value is always copied/moved into the \c AttributeValue object.
 *
 * @tparam ValueT Type of value stored in the \c AttributeValue object.
 * @tparam Args Type of arguments to construct a \c ValueT object.
 */
template<class ValueT, class... Args>
inline AttributeValue MakeAttributeValue(Args&&... args) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<Args...>(args...)
        ));
}/*}}}*/

#else // !(NSFX_HAS_VARIADIC_TEMPLATE)
template<class ValueT>
inline AttributeValue MakeAttributeValue(void) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
        ));
}/*}}}*/

template<class ValueT, class A1>
inline AttributeValue MakeAttributeValue(A1&& a1) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5) /*{{{*/
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5,
         class A6>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
    A6&& a6)
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5,
         class A6, class A7>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
    A6&& a6, A7&& a7)
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5,
         class A6, class A7, class A8>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
    A6&& a6, A7&& a7, A8&& a8)
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5,
         class A6, class A7, class A8, class A9>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
    A6&& a6, A7&& a7, A8&& a8, A9&& a9)
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8),
            std::forward<A9>(a9)
        ));
}/*}}}*/

template<class ValueT, class A1, class A2, class A3, class A4, class A5,
         class A6, class A7, class A8, class A9, class A10>
inline AttributeValue MakeAttributeValue(A1&& a1, A2&& a2, A3&& a3, A4 &&a4, A5&& a5, /*{{{*/
    A6&& a6, A7&& a7, A8&& a8, A9&& a9, A10&& a10)
{
    return AttributeValue(
        std::make_shared<AttributeValueImpl<ValueT>>(
            std::forward<A1>(a1),
            std::forward<A2>(a2),
            std::forward<A3>(a3),
            std::forward<A4>(a4),
            std::forward<A5>(a5),
            std::forward<A6>(a6),
            std::forward<A7>(a7),
            std::forward<A8>(a8),
            std::forward<A9>(a9),
            std::forward<A10>(a10)
        ));
}/*}}}*/

#endif // NSFX_HAS_VARIADIC_TEMPLATE

/*}}}*/


////////////////////////////////////////////////////////////////////////////////
typedef std::unordered_map<std::string, AttributeValue> AttributeValues;


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B

