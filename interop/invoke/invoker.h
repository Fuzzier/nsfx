/**
 * @file
 *
 * @brief Interoperability support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-21
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef INVOKER_H__98A4BBB1_75FE_47E8_A451_BA93B8AB65F5
#define INVOKER_H__98A4BBB1_75FE_47E8_A451_BA93B8AB65F5


#include <nsfx/interop/config.h>
#include <nsfx/interop/exception.h>
#include <nsfx/interop/variant/variant.h>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/insert_range.hpp>
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <tuple>
#include <initializer_list>
#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <type_traits> // conditional, is_same, decay, remove_cv, remove_pointer


NSFX_OPEN_NAMESPACE
namespace aux {


////////////////////////////////////////////////////////////////////////////////
// ArgumentAdaptor.
/**
 * @ingroup Interop
 * @brief Adaptor for argument variant.
 *
 * @tparam Arg  The parameter type.
 *
 * \c Arg is a pointer to a POD or C++ class, since \c byref is \c true.
 *
 * The underlying value held by a variant is a POD value.
 * However, a variant does not expose a pointer or reference to the underlying
 * value directly.
 *
 * The adaptor provides a dummy variable of \c Arg type.
 * Its address can to be passed to a method, and the returned value can be
 * assigned to the argument variant.
 *
 * Users call \c Get() to obtain a pointer to the dummy variable, and pass the
 * pointer to a method.
 * After the method returns, users call \c Finish() to copy the dummy variable
 * to argument variant.
 */
template<class Arg, bool byref = VariantTraits<Arg>::byref>
struct ArgumentAdaptor
{
    // Since 'byref' is true, 'Arg' must be a pointer.
    static_assert(std::is_pointer<Arg>::value, "Unexpected");

    typedef typename std::remove_cv<
                typename std::remove_pointer<
                    typename std::decay<Arg>::type
                >::type
            >::type  ValueType;

    ArgumentAdaptor(Variant* arg) :
        arg_(arg)
    {
        if (!arg)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgumentVariant() <<
                ErrorMessage("The argument must not be nullptr."));
        }
        // If the argument carries value, copy the value to dummy, so
        // the value is passed to the method via the dummy.
        if (arg_->GetType() == VariantTraits<ValueType>::type)
        {
            dummy_ = arg_->GetValue<ValueType>();
        }
    }

    Arg Get(void)
    {
        return &dummy_;
    }

    void Finish(void)
    {
        *arg_ = dummy_;
    }

    ValueType dummy_;
    Variant*  arg_;
};


/**
 * @ingroup Interop
 * @brief Adaptor for argument variant.
 *
 * @tparam Arg  The parameter type.
 *
 * \c Arg is a POD or C++ class, since \c byref is \c false.
 *
 * The underlying value held by a variant is a POD value.
 *
 * Users call \c Get() to obtain a variable of type \c Arg, and pass the
 * variable to a method.
 * After the method returns, users may call \c Finish() which does nothing in
 * this template specialization.
 */
template<class Arg>
struct ArgumentAdaptor<Arg, /*byref=*/false>
{
    typedef typename std::decay<Arg>::type  ValueType;

    ArgumentAdaptor(Variant* arg) :
        arg_(arg)
    {
        if (!arg)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgumentVariant() <<
                ErrorMessage("The argument must not be nullptr."));
        }
        if (arg->GetType() != VariantTraits<ValueType>::type)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgumentVariant() <<
                ErrorMessage("The vartype of the argument variant does not "
                             "match the parameter type."));
        }
    }

    Arg Get(void)
    {
        return arg_->GetValue<ValueType>();
    }

    void Finish(void)
    {
    }

    Variant*  arg_;
};


////////////////////////////////////////////////////////////////////////////////
// Invoker.
struct DoReturnTag {};
struct NoReturnTag {};


#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
struct Invoker
{
public:
    /**
     * @brief Invoke a method.
     *
     * @tparam Obj   An interface type.
     * @tparam Ret   The type of return value.
     * @tparam Args  The types of arguments.
     *
     * @param[in]     o     A pointer to the interface.
     * @param[in]     ptmf  A pointer to member function of the interface.
     * @param[out]    ret   The return value.
     *                      It can be \c nullptr if the return value is not
     *                      required.
     * @param[in,out] args  The arguments.
     *                      It can be \c nullptr if the method has no parameters.
     */
    template<class Obj, class Ret, class... Args>
    static void Invoke(Obj* o, Ret (Obj::*ptmf)(Args...),
                       Variant* ret, size_t numArgs, Variant* args)
    {
        typedef typename std::conditional<std::is_same<Ret, void>::value,
                                          NoReturnTag, DoReturnTag>::type  R;

        typedef typename boost::mpl::insert_range<
                    boost::vector_c<size_t>,
                    0,
                    boost::mpl::range_c<size_t, 0, sizeof...(Args)>
                >::type  Indices;

        Invoke(o, ptmf, ret, numArgs, args, R(), Indices());
    }

private:
    template<class Obj, class Ret, class... Args, size_t... indices>
    static void Invoke(Obj* o, Ret (Obj::* ptmf)(Args...),
                       Variant* ret, size_t numArgs, Variant* args,
                       NoReturnTag, boost::mpl::vector_c<size_t, indices...>)
    {
        std::tuple<ArgumentAdaptor<Args>...>  aa =
            std::make_tuple<Args...>(&args[indices]...);
        (o->*ptmf)(std::get<indices>(aa).Get()...);
        (void)std::initializer_list<int>{
            ((void)std::get<indices>(aa.Finish()), 0)...};
    }

    template<class Obj, class Ret, class... Args, size_t... indices>
    static void Invoke(Obj* o, Ret (Obj::* ptmf)(Args...),
                       Variant* ret, size_t numArgs, Variant* args,
                       DoReturnTag, boost::mpl::vector_c<size_t, indices...>)
    {
        if (!ret)
        {
            Invoke(o, ptmf, ret, numArgs, args, NoReturnTag(),
                   boost::mpl::vector_c<size_t, indices...>);
        }
        else
        {
            std::tuple<ArgumentAdaptor<Args>...>  aa =
                std::make_tuple<Args...>(&args[indices]...);
            Ret r = (o->*ptmf)(std::get<indices>(aa).Get()...);
            (void)std::initializer_list<int>{
                ((void)std::get<indices>(aa.Finish()), 0)...};
            *ret = r;
        }
    }
};

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

struct Invoker
{

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/interop/invoke/invoker.h>, 0))
# include BOOST_PP_ITERATE()

};

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


} // namespace aux
NSFX_CLOSE_NAMESPACE


#endif // INVOKER_H__98A4BBB1_75FE_47E8_A451_BA93B8AB65F5


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

// state is (n, numArgs), PRED is (n < numArgs)
#define NSFX_PP_PRED(r, state)             \
    BOOST_PP_NOT_EQUAL(                    \
        BOOST_PP_TUPLE_ELEM(2, 0, state),  \
        BOOST_PP_TUPLE_ELEM(2, 1, state)   \
    )

// state: from (n, numArgs) to (n+1, numArgs)
#define NSFX_PP_NEXT(r, state)                           \
    (                                                    \
        BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 0, state)),  \
        BOOST_PP_TUPLE_ELEM(2, 1, state)                 \
    )

// ArgumentAdaptor<A##n>  aa##n(arg + n);
#define NSFX_PP_ARGUMENT_ADAPTOR_DECLARE(r, state)                       \
    ArgumentAdaptor<BOOST_PP_CAT(A,  BOOST_PP_TUPLE_ELEM(2, 0, state))>  \
                    BOOST_PP_CAT(aa, BOOST_PP_TUPLE_ELEM(2, 0, state))(  \
                        args + BOOST_PP_TUPLE_ELEM(2, 0, state));

// aa##n.Get()
#define NSFX_PP_ARGUMENT_ADAPTOR_GET(z, n, d)   aa ## n.Get()

// aa##n.Finish();
#define NSFX_PP_ARGUMENT_ADAPTOR_FINISH(r, state)                \
    BOOST_PP_CAT(aa, BOOST_PP_TUPLE_ELEM(2, 0, state)).Finish();


# if BOOST_PP_ITERATION_FLAGS() == 0

public:
    // template<class Obj, class Ret, class A0, class A1, ...>
    template<class Obj, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
    // static void Invoke(Obj* o, Ret (Obj::*ptmf)(A0, A1, ...),
    //                    Variant* ret, Variant* args)
    static void Invoke(Obj* o, Ret (Obj::*ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)),
                       Variant* ret, size_t numArgs, Variant* args)
    {
        typedef typename std::conditional<std::is_same<Ret, void>::value,
                                          NoReturnTag, DoReturnTag>::type  R;
        Invoke(o, ptmf, ret, numArgs, args, R());
    }

private:
    // template<class Obj, class Ret, class A0, class A1, ...>
    template<class Obj, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
    // static void Invoke(Obj* o, Ret (Obj::*ptmf)(A0, A1, ...),
    //                    Variant* ret, size_t numArgs, Variant* args,
    //                    NoReturnTag)
    static void Invoke(Obj* o, Ret (Obj::*ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)),
                       Variant* ret, size_t numArgs, Variant* args,
                       NoReturnTag)
    {
        // ArgumentAdaptor<A0>  a0;
        // ArgumentAdaptor<A1>  a1;
        // ...
        BOOST_PP_FOR((0, BOOST_PP_ITERATION()), NSFX_PP_PRED, NSFX_PP_NEXT,
                     NSFX_PP_ARGUMENT_ADAPTOR_DECLARE);
        // (o->*ptmf)(a0.Get(), a1.Get(), ...);
        (o->*ptmf)(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_ARGUMENT_ADAPTOR_GET, ));
        // a0.Finish();
        // a1.Finish();
        // ...
        BOOST_PP_FOR((0, BOOST_PP_ITERATION()), NSFX_PP_PRED, NSFX_PP_NEXT,
                     NSFX_PP_ARGUMENT_ADAPTOR_FINISH);
    }

    // template<class Obj, class Ret, class A0, class A1, ...>
    template<class Obj, class Ret  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                        BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
    // static void Invoke(Obj* o, Ret (Obj::*ptmf)(A0, A1, ...),
    //                    Variant* ret, size_t numArgs, Variant* args,
    //                    DoReturnTag)
    static void Invoke(Obj* o, Ret (Obj::*ptmf)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)),
                       Variant* ret, size_t numArgs, Variant* args,
                       DoReturnTag)
    {
        if (!ret)
        {
            Invoke(o, ptmf, ret, numArgs, args, NoReturnTag());
        }
        else
        {
            // ArgumentAdaptor<A0>  a0;
            // ArgumentAdaptor<A1>  a1;
            // ...
            BOOST_PP_FOR((0, BOOST_PP_ITERATION()), NSFX_PP_PRED, NSFX_PP_NEXT,
                         NSFX_PP_ARGUMENT_ADAPTOR_DECLARE);
            // Ret r = (o->*ptmf)(a0.Get(), a1.Get(), ...);
            Ret r = (o->*ptmf)(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_ARGUMENT_ADAPTOR_GET, ));
            // a0.Finish();
            // a1.Finish();
            // ...
            BOOST_PP_FOR((0, BOOST_PP_ITERATION()), NSFX_PP_PRED, NSFX_PP_NEXT,
                         NSFX_PP_ARGUMENT_ADAPTOR_FINISH);
            *ret = r;
        }
    }

# endif // BOOST_PP_ITERATION_FLAGS() == x

#undef NSFX_PP_ARGUMENT_ADAPTOR_FINISH
#undef NSFX_PP_ARGUMENT_ADAPTOR_GET
#undef NSFX_PP_ARGUMENT_ADAPTOR_DECLARE
#undef NSFX_PP_NEXT
#undef NSFX_PP_PRED

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

