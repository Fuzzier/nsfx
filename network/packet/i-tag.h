/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TAG_H__9AFD9FF4_A048_455E_8569_D90DD7604F6E
#define TAG_H__9AFD9FF4_A048_455E_8569_D90DD7604F6E


#include <nsfx/network/config.h>
#include <boost/type_index.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <utility> // forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The type-neutral tag interface.
 */
class ITag
{
public:
    virtual ~ITag(void) BOOST_NOEXCEPT {}

    virtual const boost::typeindex::type_info& GetTypeId(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The type-specific tag.
 *
 * @tparam T  The type of the underlying value of the tag.
 */
template<class T>
class TypedTag :
    public ITag
{
public:
    typedef typename boost::decay<T>::type  ValueType;
    static_assert(boost::is_same<T, ValueType>::value,
                  "Invalid value type for a packet tag.");

    // Xtructors.
public:
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class... Args>
    TypedTag(Args&&... args) :
        value_(std::forward<Args>(args)...)
    {}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    TypedTag(void) {}

# define BOOST_PP_ITERATION_PARAMS_1  (4, (1, NSFX_MAX_ARITY, <nsfx/network/packet/i-tag.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    virtual ~TypedTag(void) {}

    // Non-copyable.
public:
    BOOST_DELETED_FUNCTION(TypedTag(const TypedTag&));
    BOOST_DELETED_FUNCTION(TypedTag& operator=(const TypedTag&));

public:
    virtual const boost::typeindex::type_info& GetTypeId(void) NSFX_OVERRIDE NSFX_FINAL
    {
        return boost::typeindex::type_id<ValueType>().type_info();
    }

    const ValueType& GetValue(void) const BOOST_NOEXCEPT
    {
        return value_;
    }

private:
    ValueType  value_;
};


NSFX_CLOSE_NAMESPACE


#endif // TAG_H__9AFD9FF4_A048_455E_8569_D90DD7604F6E


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

////////////////////////////////////////
# if BOOST_PP_ITERATION_FLAGS() == 0
// template<class A0, class A1, ...>
// TypedTag(A0&& a0, A1&& a1, ...) :
//     value_(std::forward<A0>(a0), std::forward<A1>(a1), ...)
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
TypedTag(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a)) :
    value_(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ))
{}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

