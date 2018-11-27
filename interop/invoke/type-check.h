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

#ifndef TYPE_CHECK_H__D8C48D3B_6433_43BA_85E1_79B22643BE55
#define TYPE_CHECK_H__D8C48D3B_6433_43BA_85E1_79B22643BE55


#include <nsfx/interop/config.h>
#include <nsfx/interop/exception.h>
#include <nsfx/interop/variant/variant.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <type_traits> // decay, conditional, is_same


NSFX_OPEN_NAMESPACE
namespace aux {


////////////////////////////////////////////////////////////////////////////////
// CheckReturnType()
/**
 * @ingroup Interop
 * @brief Check the return type.
 *
 * @tparam Ret  The return type.
 * @param[in] ret  The return value. It must not be \c nullptr.
 */
template<class Ret>
inline void CheckReturnType(const Variant* ret)
{
    typedef typename std::decay<Ret>::type  RetType;

    // If the variant is not empty.
    if (res && ret->GetType() != VT_EMPTY)
    {
        // // The variant must be a reference.
        // if (!(ret.GetType() & VT_BYREF))
        // {
        //     BOOST_THROW_EXCEPTION(
        //         InvalidReturnVariant() <<
        //         ErrorMessage("The variant of the return value is not "
        //                      "a reference."));
        // }
        // Is Ptr<Intf>?
        // else
        if (ret->GetType() != VariantTraits<RetType>::type)
        {
            BOOST_THROW_EXCEPTION(
                InvalidReturnVariant() <<
                ErrorMessage("The type of the variant mismatches "
                             "the type of the return value."));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// CheckArgumentType()
/**
 * @ingroup Interop
 * @brief Check the argument type.
 *
 * @tparam Sequence  A BOOST MPL type sequence that holds parameter types.
 * @tparam index     The index of the type element in \c Sequence.
 *
 * @param[in] arg  The argument. It must not be \c nullptr.
 */
template<class Sequence, size_t index>
inline void CheckArgumentType(const Variant* arg)
{
    typedef typename
        boost::mpl::at<Sequence, boost::mpl::size_t<index>>::type  Arg;
    typedef typename std::decay<Arg>::type  ArgType;
    if (arg->GetType() != VariantTraits<ArgType>::type)
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgumentVariant() <<
            InvalidArgumentIndex(index) <<
            ErrorMessage("The type of the variant mismatches "
                         "the type of the argument."));
    }
}


////////////////////////////////////////////////////////////////////////////////
// CheckArgumentTypeTable
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Interop
 * @brief A table of functions that provide type check.
 *
 * @tparam Sequence  A BOOST MPL type sequence that holds parameter types.
 * @tparam indices   The indices of all elements in \c Sequence.
 *
 * This class provides a static member function \c Get() which returns
 * a function pointer.
 * The function pointer can be invoked to obtain the name of the type.
 */
template<class Sequence, class Indices =
        typename boost::mpl::insert_range<
            boost::mpl::vector_c<size_t>,
            0,
            boost::mpl::range_c<size_t, 0, boost::mpl::size<Sequence>::value>
        >::type>
struct CheckArgumentTypeTable;

template<class Sequence, size_t... indices>
struct CheckArgumentTypeTable<Sequence, boost::mpl::vector_c<size_t, indices...>>
{
    typedef const char* (* FunctionType)(const Variant* arg) BOOST_NOEXCEPT;
    static const FunctionType Get(size_t index)
    {
        static const FunctionType table[] = {
            &CheckArgumentType<Sequence, indices>...
        };
        BOOST_ASSERT_MSG(0 <= index && sizeof (table) / sizeof (*table),
                         "The index is out-of-bound");
        return table[index];
    }
}

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<class Sequence, size_t size = boost::mpl::size<Sequence>::value>
struct CheckArgumentTypeTable;

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/interop/invoke/type-check.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
// CheckTypes()
/**
 * @ingroup Interop
 * @brief Check the types of return value and arguments.
 *
 * @tparam Ret       The return type.
 * @tparam Sequence  A BOOST MPL type sequence that holds parameter types.
 *
 * @param[in] ret      The return value.
 * @param[in] numArgs  The number of arguments.
 * @param[in] args     The array of arguments.
 */
template<class Ret, class Sequence>
inline void CheckAllTypes(const Variant* ret, size_t numArgs, const Variant* args)
{
    // Check the return value.
    CheckReturnType<Ret>(ret);

    // Check the number of arguments.
    if (numArgs != boost::mpl::size<Sequence>::value)
    {
        BOOST_THROW_EXCEPTION(
            InvalidNumberOfArguments() <<
            ExpectedNumberOfArguments(boost::mpl::size<Sequence>::value) <<
            SuppliedNumberOfArguments(numArgs) <<
            ErrorMessage("Invalid number of the arguments."));
    }

    // Check the type of each argument.
    for (size_t i = 0; i < boost::mpl::size<Sequence>::value; ++i)
    {
        typedef CheckArgumentTypeTable<Sequence>  TableType;
        typename TableType::FunctionType  checker = TableType::Get(i);
        (*checker)(&args[i]);
    }
}


} // namespace aux
NSFX_CLOSE_NAMESPACE


#endif // TYPE_CHECK_H__D8C48D3B_6433_43BA_85E1_79B22643BE55


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

# define NSFX_PP_CHECK_ARGUMENT_TYPE(z, n, d)  &CheckArgumentType<Sequence, n>

# if BOOST_PP_ITERATION_FLAGS() == 0

template<class Sequence>
struct CheckArgumentTypeTable<Sequence, BOOST_PP_ITERATION()>
{
    typedef const char* (* FunctionType)(const Variant* arg) BOOST_NOEXCEPT;
    static const FunctionType Get(size_t index)
    {
        static const FunctionType table[] = {
            BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), NSFX_PP_CHECK_ARGUMENT_TYPE, )
        };
        BOOST_ASSERT_MSG(0 <= index && sizeof (table) / sizeof (*table),
                         "The index is out-of-bound");
        return table[index];
    }
};

# endif // BOOST_PP_ITERATION_FLAGS() == x

#undef NSFX_PP_CHECK_ARGUMENT_TYPE

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

