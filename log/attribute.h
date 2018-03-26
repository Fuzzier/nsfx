/**
 * @file
 *
 * @brief Log support for Network Simulation Frameworks.
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
#include <nsfx/log/attribute-value.h>
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
 * @brief The virtual base class of type-specific attribute classes.
 */
template<class T>
class IAttribute : public IAttributeBase
{
public:
    virtual ~IAttribute(void) BOOST_NOEXCEPT {}
    virtual const std::type_info& GetTypeId(void) = 0;
    virtual AttributeValue Get(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Attribute is used by log sources to genrate attribute values.
 *
 * \c Attribute object generates \c AttributeValue.
 */
class Attribute
{
public:
    Attribute(const std::shared_ptr<IAttribute>& attr) :
        attr_(attr)
    {
        if (!attr_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    Attribute(std::shared_ptr<IAttribute>&& attr) :
        attr_(std::move(attr))
    {
        if (!attr_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    // Methods.
public:
    const std::type_info& GetTypeId(void) const
    {
        BOOST_ASSERT(!!value_);
        return attr_->GetTypeId();
    }

    AttributeValue Get(void) const
    {
        return attr_->Get();
    }

    // Properties.
private:
    std::shared_ptr<IAttribute> attr_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Constant attribute.
 *
 * @tparam T Type of value.
 */
template<class T>
class ConstantAttribute : public IAttribute
{
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class... Args>
    ConstantAttribute(Args&&... args) :
        value_(MakeConstantAttributeValue<T>(std::forward<Args>(args)...))
    {}
#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ConstantAttribute(void) :
        value_(MakeConstantAttributeValue<T>())
    {}

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/attribute.h>, 0))

#include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~ConstantAttribute(void) {}

    virtual const std::type_info& GetTypeId(void) const NSFX_OVERRIDE
    {
        return typeid (T);
    }

    virtual AttributeValue Get(void) const NSFX_OVERRIDE
    {
        return value_;
    }

private:
    AttributeValue value_;
};


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Log
 * @brief Make a constant attribute.
 *
 * @tparam T    Type of value stored in the \c Attribute object.
 * @tparam Args Type of arguments to construct a \c T object.
 */
template<class T, class... Args>
inline Attribute MakeConstantAttribute(Args&&... args)
{
    return Attribute(
        std::shared_ptr<IAttribute>(
            std::make_shared<ConstantAttribute<T> >(
                std::forward<Args>(args)...)));
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/attribute.h>, 1))

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
// ConstantAttribute(A0&& a0, A1&& a1, ...) :
//     value_(MakeConstantAttributeValue<T>(std::forward<A0>(a0), std::forward<A1>(a1), ...))
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
ConstantAttribute(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a)) :
    value_(MakeConstantAttributeValue<T>(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )))
{}
#  endif // BOOST_PP_ITERATION() >= 1

# elif BOOST_PP_ITERATION_FLAGS() == 1

// template<class T, class A0, class A1, ...>
// inline Attribute MakeConstantAttribute(A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline Attribute MakeConstantAttribute(
        BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    // return Attribute(
    //     std::shared_ptr<IAttribute>(
    //         std::make_shared<ConstantAttribute<T> >(
    //             std::forward<A0>(a0), std::forward<A1>(a1), ...)));
    return Attribute(
        std::shared_ptr<IAttribute>(
            std::make_shared<ConstantAttribute<T> >(
                BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))));
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

