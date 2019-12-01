/**
 * @file
 *
 * @brief No-op.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-12-01
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef NOP_H__76A85040_F783_4FAB_B2AB_CB4FAF65096B
#define NOP_H__76A85040_F783_4FAB_B2AB_CB4FAF65096B


#include <nsfx/utility/config.h>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

/**
 * @ingroup Utility
 * @{
 */

NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
/**
 * @brief No-op.
 *
 * A functor that does nothing.
 */
template<class = void, class...>
class nop
{
public:
    template<class...A>
    void operator()(A&&...) const BOOST_NOEXCEPT {}
};

#else // !(!defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES))

template<class T0 = void, class T1 = void, class T2 = void,
         class T3 = void, class T4 = void, class T5 = void>
class nop
{
public:
    void operator()(void) const BOOST_NOEXCEPT {}

# define BOOST_PP_ITERATION_PARAMS_1  (3, (1, NSFX_MAX_ARITY, <nsfx/utility/nop.h>))
# include BOOST_PP_ITERATE()

};

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // NOP_H__76A85040_F783_4FAB_B2AB_CB4FAF65096B


#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

// template<class A0, class A1, ...>
// void operator()(A0&& a0, A1&& a1, ...) const BOOST_NOEXCEPT
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
void operator()(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, && a))
    const BOOST_NOEXCEPT
{}

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/
