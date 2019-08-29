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

#ifndef MAKE_LOG_VALUE_H__6A6E62EA_3F19_418F_BDA2_53A5C8D7F97C
#define MAKE_LOG_VALUE_H__6A6E62EA_3F19_418F_BDA2_53A5C8D7F97C


#include <nsfx/log/config.h>
#include <nsfx/log/log-value.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <utility> // forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief A value-based log value.
 *
 * @tparam T Type of stored value.
 */
template<class T>
class ValueBasedLogValue :
    public ITypedLogValue<T>
{
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class... Args>
    ValueBasedLogValue(Args&&... args) :
        value_(std::forward<Args>(args)...)
    {}

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    ValueBasedLogValue(void) {}

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/make-log-value.h>, 0))

#include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~ValueBasedLogValue(void) {}

    virtual T Get(void) NSFX_OVERRIDE
    {
        return value_;
    }

private:
    T value_;
};


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief A functor-based log value.
 *
 * @tparam T       The type of stored value.
 * @tparam Functor The type of the functor.
 */
template<class T, class Functor>
class FunctorBasedLogValue :
    public ITypedLogValue<T>
{
    static_assert(!std::is_reference<Functor>::value,
                  "The functor must not be a reference.");

public:
    template<class F>
    FunctorBasedLogValue(F&& functor) :
        functor_(std::forward<F>(functor))
    {}

    virtual ~FunctorBasedLogValue(void) {}

    virtual T Get(void) NSFX_OVERRIDE
    {
        return functor_();
    }

private:
    Functor functor_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a functor-based log value.
 *
 * @tparam T       The type of the stored value.
 * @tparam Functor The type of the functor.
 */
template<class T, class Functor>
inline LogValue MakeLogValue(Functor&& functor)
{
    typedef FunctorBasedLogValue<T, Functor>  Impl;
    return LogValue(new Impl(std::forward<Functor>(functor)));
}


////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Log
 * @brief Make a log value that stores a constant value.
 *
 * @tparam T    The type of stored value.
 * @tparam Args The types of arguments to construct a \c T object.
 */
template<class T, class... Args>
inline LogValue MakeConstantLogValue(Args&&... args)
{
    typedef ValueBasedLogValue<T>  Impl;
    return LogValue(new Impl(std::forward<Args>(args)...));
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

#define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/log/make-log-value.h>, 1))

#include BOOST_PP_ITERATE()

#endif // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_CLOSE_NAMESPACE


#endif // MAKE_LOG_VALUE_H__6A6E62EA_3F19_418F_BDA2_53A5C8D7F97C


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

# if BOOST_PP_ITERATION_FLAGS() == 0

#  if BOOST_PP_ITERATION() >= 1
// template<class A0, class A1, ...>
// ValueBasedLogValue(A0&& a0, A1&& a1, ...) :
//     value_(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
ValueBasedLogValue(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a)) :
    value_(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{}
#  endif // BOOST_PP_ITERATION() >= 1

# elif BOOST_PP_ITERATION_FLAGS() == 1

// template<class T, class A0, class A1, ...>
// inline LogValue MakeConstantLogValue(A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline LogValue MakeConstantLogValue(
            BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    // return LogValue(
    //         new ValueBasedLogValue<T>(
    //             std::forward<A0>(a0), std::forward<A1>(a1), ...));
    return LogValue(
            new ValueBasedLogValue<T>(
                BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, )));
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

