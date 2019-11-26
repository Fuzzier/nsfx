/**
 * @file
 *
 * @brief Lexical comparison for class members.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-26
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef LEXICAL_COMPARE_H__A72E0F7E_AB10_4797_8C54_74B85F30B589
#define LEXICAL_COMPARE_H__A72E0F7E_AB10_4797_8C54_74B85F30B589


#include <nsfx/utility/config.h>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_binary_params.hpp>
#include <boost/preprocessor/iterate.hpp>

#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <type_traits> // enable_if


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
#if 0 /*{{{*/
/**
 * @ingroup Utility
 * @brief The traits of a pointer to member variable.
 *
 * @tparam P    The type of `ptmv`, which looks like `T C::*`,
 *              where `C` is the class type, and `T` is the value type.
 * @tparam ptmv The pointer to member variable.
 *
 * This traits class defines two types:
 * * `ClassType`
 * * `ValueType`: It **must** support `<` and `==` operators.
 *
 * This traits provides an overloaded function call operator to compare
 * the member variables of two instances of the class.
 * @code{.cpp}
 * int operator()(const C& lhs, const C& rhs) const;
 * @endcode
 * * If the member variable of `lhs` is smaller than that of `rhs`, `-1` is returned.
 * * If the member variable of `lhs` is equal to that of `rhs`, `0` is returned.
 * * If the member variable of `lhs` is greater than that of `rhs`, `1` is returned.
 */
template<class P, P ptmv>
class Ptmv;

////////////////////////////////////////
template<class C, class T, T C::* ptmv>
class Ptmv<T C::*, ptmv>
{
public:
    typedef C  ClassType;
    typedef T  ValueType;
    int operator()(const C& lhs, const C& rhs) const BOOST_NOEXCEPT
    {
        if (lhs.*ptmv < rhs.*ptmv)
        {
            return -1;
        }
        else if (lhs.*ptmv == rhs.*ptmv)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 *
 * @code{.cpp}
 * class A
 * {
 *     int i;
 * };
 * A a = { 0 };
 * A b = { 1 };
 *
 * typedef PTMV_SEQ1(A, i) tag;
 * int result = LexicalCompare(a, b, tag()); // result = -1;
 * @endcode
 */
#define PTMV_SEQ1(C, mv1)                         \
    ::boost::mpl::list<                           \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>> \

/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 * @param mv2 The member variable.
 *
 * @code{.cpp}
 * class A
 * {
 * public:
 *     int i;
 *     double j;
 * };
 *
 * A a = { 1, 2.2 };
 * A b = { 1, 2.0 };
 *
 * typedef PTMV_SEQ2(A, i, j)  tag;
 * int result = LexicalCompare(a, b, tag()); // result = 1;
 * @endcode
 */
#define PTMV_SEQ2(C, mv1, mv2)                    \
    ::boost::mpl::list<                           \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>, \
        ::nsfx::Ptmv<decltype(&C::mv2), &C::mv2>> \

/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 * @param mv2 The member variable.
 * @param mv3 The member variable.
 *
 * See `PTMV_SEQ2` for code samples.
 */
#define PTMV_SEQ3(C, mv1, mv2, mv3)               \
    ::boost::mpl::list<                           \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>, \
        ::nsfx::Ptmv<decltype(&C::mv2), &C::mv2>, \
        ::nsfx::Ptmv<decltype(&C::mv3), &C::mv3>> \

/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 * @param mv2 The member variable.
 * @param mv3 The member variable.
 * @param mv4 The member variable.
 *
 * See `PTMV_SEQ2` for code samples.
 */
#define PTMV_SEQ4(mv1, mv2, mv3, mv4)             \
    ::boost::mpl::list<                           \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>, \
        ::nsfx::Ptmv<decltype(&C::mv2), &C::mv2>, \
        ::nsfx::Ptmv<decltype(&C::mv3), &C::mv3>, \
        ::nsfx::Ptmv<decltype(&C::mv4), &C::mv4>> \

/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 * @param mv2 The member variable.
 * @param mv3 The member variable.
 * @param mv4 The member variable.
 * @param mv5 The member variable.
 *
 * See `PTMV_SEQ2` for code samples.
 */
#define PTMV_SEQ5(mv1, mv2, mv3, mv4, mv5)        \
    ::boost::mpl::list<                           \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>, \
        ::nsfx::Ptmv<decltype(&C::mv2), &C::mv2>, \
        ::nsfx::Ptmv<decltype(&C::mv3), &C::mv3>, \
        ::nsfx::Ptmv<decltype(&C::mv4), &C::mv4>, \
        ::nsfx::Ptmv<decltype(&C::mv5), &C::mv5>> \

/**
 * @ingroup Utility
 * @brief Defines a type that reprents a sequence of `Ptmv` traits.
 *
 * @param C   The class.
 * @param mv1 The member variable.
 * @param mv2 The member variable.
 * @param mv3 The member variable.
 * @param mv4 The member variable.
 * @param mv5 The member variable.
 * @param mv6 The member variable.
 *
 * See `PTMV_SEQ2` for code samples.
 */
#define PTMV_SEQ6(C, mv1, mv2, mv3, mv4, mv5, mv6) \
    ::boost::mpl::list<                            \
        ::nsfx::Ptmv<decltype(&C::mv1), &C::mv1>,  \
        ::nsfx::Ptmv<decltype(&C::mv2), &C::mv2>,  \
        ::nsfx::Ptmv<decltype(&C::mv3), &C::mv3>,  \
        ::nsfx::Ptmv<decltype(&C::mv4), &C::mv4>,  \
        ::nsfx::Ptmv<decltype(&C::mv5), &C::mv5>,  \
        ::nsfx::Ptmv<decltype(&C::mv6), &C::mv6>>  \

////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief Lexical comparison of a sequence of class members.
 *
 * @tparam T       The class type.
 * @tparam PtmvSeq The sequence of `Ptmv`.
 *                 It can be defined by `PTMV_SEQn()` macros.
 *
 * @param[in] lhs     The instance of the class.
 * @param[in] rhs     The instance of the class.
 * @param[in] PtmvSeq The sequence of `Ptmv`.
 *
 * @return The class members are compared one after another, until a member
 *         is different, and the result of the last comparison is returned.
 *         * `-1` is returned, if the member of `lhs` is smaller than that of `rhs`.
 *         *  `0` is returned, if the member of `lhs` is equal to that of `rhs`.
 *         *  `1` is returned, if the member of `lhs` is greater than that of `rhs`.
 */
template<class T, class PtmvSeq>
typename std::enable_if<(boost::mpl::size<PtmvSeq>::value == 1), int>::type
LexicalCompare(const T& lhs, const T& rhs, PtmvSeq)
{
    typedef typename boost::mpl::front<PtmvSeq>::type P1;
    return P1()(lhs, rhs);
}

template<class T, class PtmvSeq>
typename std::enable_if<(boost::mpl::size<PtmvSeq>::value > 1), int>::type
LexicalCompare(const T& lhs, const T& rhs, PtmvSeq)
{
    typedef typename boost::mpl::front<PtmvSeq>::type P1;
    int result = P1()(lhs, rhs);
    if (!result)
    {
        typedef typename boost::mpl::pop_front<PtmvSeq>::type  Next;
        result = LexicalCompare(lhs, rhs, Next());
    }
    return result;
}
#endif // 0 /*}}}*/

////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief Lexical comparison of a sequence of class members.
 *
 * @tparam C       The class.
 * @tparam T       The type of the member variable.
 *
 * @param[in] lhs  The instance of the class.
 * @param[in] rhs  The instance of the class.
 * @param[in] ptmv The poiner to the member variable.
 *
 * @return * `-1` is returned, if the member of `lhs` is smaller than that of `rhs`.
 *         *  `0` is returned, if the member of `lhs` is equal to that of `rhs`.
 *         *  `1` is returned, if the member of `lhs` is greater than that of `rhs`.
 */
template<class C, class T>
int LexicalCompare(const C& lhs, const C& rhs, T C::* ptmv)
{
    int result = 1;
    if (lhs.*ptmv < rhs.*ptmv)
    {
        return -1;
    }
    else if (lhs.*ptmv == rhs.*ptmv)
    {
        result = 0;
    }
    return result;
}

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

/**
 * @ingroup Utility
 * @brief Lexical comparison of a sequence of class members.
 *
 * @tparam C The class.
 * @tparam T The type of the member variable.
 * @tparam U The types of the remaining member variables.
 *
 * @param[in] lhs   The instance of the class.
 * @param[in] rhs   The instance of the class.
 * @param[in] ptmv  The poiner to the member variable of the most significance.
 * @param[in] ptmvs The poiners to the member variables of lesser significance.
 *
 * @return The class members are compared one after another, until a member
 *         is different, and the result of the last comparison is returned.
 *         * `-1` is returned, if the member of `lhs` is smaller than that of `rhs`.
 *         *  `0` is returned, if the member of `lhs` is equal to that of `rhs`.
 *         *  `1` is returned, if the member of `lhs` is greater than that of `rhs`.
 */
template<class C, class T, class...U>
int LexicalCompare(const C& lhs, const C& rhs, T C::* ptmv, U C::*...ptmvs)
{
    int result = LexicalCompare(lhs, rhs, ptmv);
    if (result == 0)
    {
        result = LexicalCompare(lhs, rhs, ptmvs...);
    }
    return result;
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (3, (2, NSFX_MAX_ARITY, <nsfx/utility/lexical-compare.h>))

# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_CLOSE_NAMESPACE


#endif // LEXICAL_COMPARE_H__A72E0F7E_AB10_4797_8C54_74B85F30B589


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

// template<class C, class T, class...U>
// int LexicalCompare(const C& lhs, const C& rhs, T C::* ptmv, U C::*...ptmvs)
template<class C, class T, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_ITERATION(), class U)>
int LexicalCompare(const C& lhs, const C& rhs, T C::* ptmv,
       BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(BOOST_PP_ITERATION(), U, C::* ptmv))
{
    int result = LexicalCompare(lhs, rhs, ptmv);
    if (result == 0)
    {
        // result = LexicalCompare(lhs, rhs, ptmvs...);
        result = LexicalCompare(lhs, rhs,
                    BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_ITERATION(), ptmv));
    }
    return result;
}

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

