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

#ifndef GET_TYPE_NAME_H__79585B07_812D_4048_9BFB_B4081ACFDC87
#define GET_TYPE_NAME_H__79585B07_812D_4048_9BFB_B4081ACFDC87


#include <nsfx/interop/config.h>
#include <nsfx/interop/variant/variant-traits.h>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <type_traits> // decay, conditional, is_same


NSFX_OPEN_NAMESPACE
namespace aux {


////////////////////////////////////////////////////////////////////////////////
// GetTypeName()
/**
 * @ingroup Interop
 * @brief Get the name of the type.
 *
 * @tparam Sequence  A BOOST MPL type sequence that holds parameter types.
 * @tparam index     The index of the type element in \c Sequence.
 */
template<class Sequence, size_t index>
inline const char* GetTypeName(void) BOOST_NOEXCEPT
{
    static_assert(boost::mpl::is_sequence<Sequence>::value,
                  "The 'Sequence' type must be a BOOST MPL sequence.");
    static_assert(index < boost::mpl::size<Sequence>::value,
                  "Cannot access the type in the Sequence, "
                  "since the index is out of bound.");
    typedef typename
        boost::mpl::at<Sequence, boost::mpl::size_t<index>>::type  Type;
    return VariantTraits<Type>::GetTypeName();
}

////////////////////////////////////////////////////////////////////////////////
// GetTypeNameTable
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
/**
 * @ingroup Interop
 * @brief A table of functions that provide type name.
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
struct GetTypeNameTable;

template<class Sequence, size_t... indices>
struct GetTypeNameTable<Sequence, boost::mpl::vector_c<size_t, indices...>>
{
    typedef const char* (* FunctionType)(void) BOOST_NOEXCEPT;
    static const FunctionType Get(size_t index)
    {
        static const FunctionType table[] = {
            &GetTypeName<Sequence, indices>...
        };
        BOOST_ASSERT_MSG(0 <= index && sizeof (table) / sizeof (*table),
                         "The index is out-of-bound");
        return table[index];
    }
}

#else // defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<class Sequence, size_t size = boost::mpl::size<Sequence>::value>
struct GetTypeNameTable;

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/interop/invoke/get-type-name.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


} // namespace aux
NSFX_CLOSE_NAMESPACE


#endif // GET_TYPE_NAME_H__79585B07_812D_4048_9BFB_B4081ACFDC87


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING) /*{{{*/

# define NSFX_PP_GET_TYPE_NAME(z, n, d)  &GetTypeName<Sequence, n>

# if BOOST_PP_ITERATION_FLAGS() == 0

template<class Sequence>
struct GetTypeNameTable<Sequence, BOOST_PP_ITERATION()>
{
    typedef const char* (* FunctionType)(void) BOOST_NOEXCEPT;
    static const FunctionType Get(size_t index)
    {
        static const FunctionType table[] = {
            BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), NSFX_PP_GET_TYPE_NAME, )
        };
        BOOST_ASSERT_MSG(0 <= index && sizeof (table) / sizeof (*table),
                         "The index is out-of-bound");
        return table[index];
    }
};

# endif // BOOST_PP_ITERATION_FLAGS() == x

#undef NSFX_PP_GET_TYPE_NAME

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

