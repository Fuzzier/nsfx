/**
 * @file
 *
 * @brief Log support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONST_ATTRIBUTE_H__826F8621_57E1_4FCB_8779_88E23409377D
#define CONST_ATTRIBUTE_H__826F8621_57E1_4FCB_8779_88E23409377D


#include <nsfx/log/config.h>
#include <nsfx/log/core/attribute/attribute.h>
#include <nsfx/log/core/attribute-value/const-attribute-value.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <memory>  // shared_ptr
#include <utility> // move, forward


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief An attribute that stores a constant attribute value.
 *
 * @tparam T Type of value.
 */
template<class T>
class ConstantAttribute :
    public IAttribute
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

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/core/attribute/const-attribute.h>, 0))

#include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~ConstantAttribute(void) {}

    virtual AttributeValue GetValue(void) NSFX_OVERRIDE
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
            std::make_shared<ConstantAttribute<T>>(
                std::forward<Args>(args)...)));
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/core/attribute/const-attribute.h>, 1))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_LOG_CLOSE_NAMESPACE


#endif // CONST_ATTRIBUTE_H__826F8621_57E1_4FCB_8779_88E23409377D


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
    //         std::make_shared<ConstantAttribute<T>>(
    //             std::forward<A0>(a0), std::forward<A1>(a1), ...)));
    return Attribute(
        std::shared_ptr<IAttribute>(
            std::make_shared<ConstantAttribute<T>>(
                BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))));
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

