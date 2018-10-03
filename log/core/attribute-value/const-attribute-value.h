/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
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

#ifndef CONST_ATTRIBUTE_VALUE_H__0C4005ED_3C86_4457_9AFD_0425E0DC5D93
#define CONST_ATTRIBUTE_VALUE_H__0C4005ED_3C86_4457_9AFD_0425E0DC5D93


#include <nsfx/log/config.h>
#include <nsfx/log/core/attribute-value/attribute-value.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <utility> // move, forward


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Constant attribute value.
 *
 * @tparam T Type of stored value.
 */
template<class T>
class ConstantAttributeValue :
    public ITypedAttributeValue<T>
{
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class... Args>
    ConstantAttributeValue(Args&&... args) :
        value_(std::forward<Args>(args)...)
    {}
#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ConstantAttributeValue(void) {}

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/core/attribute-value/const-attribute-value.h>, 0))

#include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~ConstantAttributeValue(void) {}

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
 * @tparam Args Type of arguments to construct a \c T object.
 */
template<class T, class... Args>
inline AttributeValue MakeConstantAttributeValue(Args&&... args)
{
    return AttributeValue(
        std::shared_ptr<ITypedAttributeValue<T> >(
            std::make_shared<ConstantAttributeValue<T> >(
                std::forward<Args>(args)...)));
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/core/attribute-value/const-attribute-value.h>, 1))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_LOG_CLOSE_NAMESPACE


#endif // CONST_ATTRIBUTE_VALUE_H__0C4005ED_3C86_4457_9AFD_0425E0DC5D93


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0

#  if BOOST_PP_ITERATION() >= 1
// template<class A0, class A1, ...>
// ConstantAttributeValue(A0&& a0, A1&& a1, ...) :
//     value_(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
ConstantAttributeValue(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a)) :
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
        std::shared_ptr<ITypedAttributeValue<T> >(
            std::make_shared<ConstantAttributeValue<T> >(
                BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))));
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

