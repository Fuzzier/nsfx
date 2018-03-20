/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    date
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE
#define ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE


#include <nsfx/log/config.h>
#include <nsfx/component/exception.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <typeinfo>
#include <memory>
#include <utility>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The virtual base class of type-specific attribute value interfaces.
 */
class IAttributeValueBase
{
public:
    virtual ~IAttributeValueBase(void) BOOST_NOEXCEPT {}
    virtual const std::type_info& GetTypeId(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The virtual base class of type-specific attribute value classes.
 */
template<class T>
class IAttributeValue : public IAttributeValueBase
{
public:
    virtual ~IAttributeValue(void) BOOST_NOEXCEPT {}
    virtual const std::type_info& GetTypeId(void) NSFX_OVERRIDE = 0;
    virtual const T& Get(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Attribute value is carried by log records.
 *
 * \c AttributeValue object never stores references, it stores concreet values.
 */
class AttributeValue
{
public:
    template<class T>
    AttributeValue(const std::shared_ptr<IAttributeValue<T> >& value) :
        value_(value)
    {
        if (!value_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    template<class T>
    AttributeValue(std::shared_ptr<IAttributeValue<T> >&& value) :
        value_(std::move(value))
    {
        if (!value_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    // Methods.
public:
    const std::type_info& GetTypeId(void) const
    {
        BOOST_ASSERT(!!value_);
        return value_->GetTypeId();
    }

    template<class T>
    const T& Get(void) const
    {
        if (value_->GetTypeId() != typeid (T))
        {
            BOOST_THROW_EXCEPTION(
                IllegalMethodCall() <<
                ErrorMessage("Cannot access the log attribute value, since "
                             "the requested type mismatches the type of value."));
        }
        return static_cast<IAttributeValue<T>*>(value_.get())->Get();
    }

    // Properties.
private:
    std::shared_ptr<IAttributeValueBase> value_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Constant attribute value.
 *
 * @tparam T Type of stored value.
 */
template<class T>
class ConstantAttributeValue : public IAttributeValue<T>
{
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class... Args>
    ConstantAttributeValue(Args&&... args) :
        value_(std::forward<Args>(args)...)
    {}
#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ConstantAttributeValue(void) {}

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/attribute-value.h>, 0))

#include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~ConstantAttributeValue(void) {}

    virtual const std::type_info& GetTypeId(void) NSFX_OVERRIDE
    {
        return typeid (T);
    }

    virtual const T& Get(void) NSFX_OVERRIDE
    {
        return value_;
    }

private:
    T value_;
};


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Log
 * @brief Make a constant attribute value.
 *
 * @tparam T    Type of value stored in the \c AttributeValue object.
 * @tparam Args Type of arguments to construct a \c ValueT object.
 */
template<class T, class... Args>
inline AttributeValue MakeConstantAttributeValue(Args&&... args)
{
    return AttributeValue(
        std::shared_ptr<IAttributeValue<T> >(
            std::make_shared<ConstantAttributeValue<T> >(
                std::forward<Args>(args)...)));
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/attribute-value.h>, 1))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0

#  if BOOST_PP_ITERATION() >= 1
// template<class A0, class A1, ...>
// ConstantAttributeValue(A0&& a0, A1&& a1, ...) :
//     value_(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
ConstantAttributeValue(
    BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a)) :
    value_(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{}
#  endif // BOOST_PP_ITERATION() >= 1

# elif BOOST_PP_ITERATION_FLAGS() == 1

// template<class T, class A0, class A1, ...>
// inline AttributeValue MakeConstantAttributeValue(A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline AttributeValue MakeConstantAttributeValue(
        BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    // return AttributeValue(
    //     std::shared_ptr<IAttributeValue<T> >(
    //         std::make_shared<ConstantAttributeValue<T> >(
    //             std::forward<A0>(a0), std::forward<A1>(a1), ...)));
    return AttributeValue(
        std::shared_ptr<IAttributeValue<T> >(
            std::make_shared<ConstantAttributeValue<T> >(
                BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))));
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

