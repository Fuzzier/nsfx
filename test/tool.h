/**
 * @file
 *
 * @brief Test support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TEST_TOOL_H__8DBFCA65_0486_449A_8A5C_7BDE42EEDB70
#define TEST_TOOL_H__8DBFCA65_0486_449A_8A5C_7BDE42EEDB70


#include <nsfx/test/config.h>
#include <nsfx/test/tool-type.h>
#include <nsfx/test/tool-level.h>
#include <nsfx/test/result.h>
#include <nsfx/test/case.h>
#include <nsfx/test/suite.h>
#include <nsfx/test/runner.h>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/common_type.hpp>
#include <sstream>
#include <iomanip>


/**
 * @ingroup Test
 *
 * @brief Define test tools.
 *
 * The following test tools are defined.
 * @code
 * NSFX_TEST_EXPECT(pred)                           // Truthful.
 * NSFX_TEST_EXPECT_EQ(actual, limit)               // Equal.
 * NSFX_TEST_EXPECT_NE(actual, limit)               // Not equal.
 * NSFX_TEST_EXPECT_LT(actual, limit)               // Less than.
 * NSFX_TEST_EXPECT_LE(actual, limit)               // Less equal.
 * NSFX_TEST_EXPECT_GT(actual, limit)               // Greater than.
 * NSFX_TEST_EXPECT_GE(actual, limit)               // Greater equal.
 * NSFX_TEST_EXPECT_AC(actual, limit, tolerance)    // absolute closeness.
 * NSFX_TEST_EXPECT_RC(actual, limit, tolerance)    // relative closeness.
 *
 * NSFX_TEST_ASSERT(pred)
 * NSFX_TEST_ASSERT_EQ(actual, limit)
 * NSFX_TEST_ASSERT_NE(actual, limit)
 * NSFX_TEST_ASSERT_LT(actual, limit)
 * NSFX_TEST_ASSERT_LE(actual, limit)
 * NSFX_TEST_ASSERT_GT(actual, limit)
 * NSFX_TEST_ASSERT_GE(actual, limit)
 * NSFX_TEST_ASSERT_AC(actual, limit, tolerance)
 * NSFX_TEST_ASSERT_RC(actual, limit, tolerance)
 * @endcode
 */

template<class T, bool integral = boost::is_integral<T>::value>
struct ValueFormatter
{
    std::string operator()(T value) const
    {
        std::ostringstream oss;
        oss << std::setprecision(9) << value;
        return oss.str();
    }
};

template<>
struct ValueFormatter<bool, true>
{
    std::string operator()(bool value) const
    {
        std::ostringstream oss;
        oss << std::boolalpha << value;
        return oss.str();
    }
};

template<class T>
struct ValueFormatter<T, true>
{
    std::string operator()(T value) const
    {
        return operator()(
            value,
            typename boost::integral_constant<size_t, sizeof (T)>::type());
    }

    std::string operator()(
        T value, typename boost::integral_constant<size_t, 1>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << (static_cast<ptrdiff_t>(value) & 0xff)
            << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename boost::integral_constant<size_t, 2>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << (static_cast<ptrdiff_t>(value) & 0xffff)
            << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename boost::integral_constant<size_t, 4>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << value << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename boost::integral_constant<size_t, 8>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << value << ")";
        return oss.str();
    }

};

template<class T>
struct ValueFormatter<T*, false>
{
    std::string operator()(T* value) const
    {
        std::ostringstream oss;
        oss << "0x" << std::setw(sizeof (T*) * 2) << std::setfill('0')
            << std::hex << std::nouppercase << value;
        return oss.str();
    }
};

template<class T>
inline std::string TestFormatValue(T value)
{
    return ValueFormatter<T>()(value);
}

// Define class templates.
#define NSFX_TEST_TOOL_FILENAME <nsfx/test/tool.h>
#include <nsfx/test/tool-iterate.h>


////////////////////////////////////////////////////////////////////////////////
// Predicate./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implement testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 *
 * @internal
 */
#define NSFX_TEST_PREDICATE_IMPL(TYPE, LEVEL, DESC, ACT)                        \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakePredicateChecker(ACT);                   \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(),                                                   \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT(actual)                                                \
    NSFX_TEST_PREDICATE_IMPL(                                                   \
        ::nsfx::test::ToolType::PREDICATE,                                      \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual, actual)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT(actual)                                                \
    NSFX_TEST_PREDICATE_IMPL(                                                   \
        ::nsfx::test::ToolType::PREDICATE,                                      \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual, actual)
/*}}}*/


// Equal./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_EQUAL_IMPL(TYPE, LEVEL, DESC, ACT, LIM)                       \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeEqualChecker((ACT), (LIM));              \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_EQ(actual, limit)                                      \
    NSFX_TEST_EQUAL_IMPL(                                                       \
        ::nsfx::test::ToolType::EQ,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " == " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_EQ(actual, limit)                                      \
    NSFX_TEST_EQUAL_IMPL(                                                       \
        ::nsfx::test::ToolType::EQ,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " == " #limit, actual, limit)
/*}}}*/


// Not equal./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_NOT_EQUAL_IMPL(TYPE, LEVEL, DESC, ACT, LIM)                   \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeNotEqualChecker((ACT), (LIM));           \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_NE(actual, limit)                                      \
    NSFX_TEST_NOT_EQUAL_IMPL(                                                   \
        ::nsfx::test::ToolType::NE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " != " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_NE(actual, limit)                                      \
    NSFX_TEST_NOT_EQUAL_IMPL(                                                   \
        ::nsfx::test::ToolType::NE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " != " #limit, actual, limit)
/*}}}*/


// Less than./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_LESS_THAN_IMPL(TYPE, LEVEL, DESC, ACT, LIM)                   \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeLessThanChecker((ACT), (LIM));           \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_LT(actual, limit)                                      \
    NSFX_TEST_LESS_THAN_IMPL(                                                   \
        ::nsfx::test::ToolType::LT,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " < " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_LT(actual, limit)                                      \
    NSFX_TEST_LESS_THAN_IMPL(                                                   \
        ::nsfx::test::ToolType::LT,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " < " #limit, actual, limit)
/*}}}*/


// Less equal./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_LESS_EQUAL_IMPL(TYPE, LEVEL, DESC, ACT, LIM)                  \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeLessEqualChecker((ACT), (LIM));          \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_LE(actual, limit)                                      \
    NSFX_TEST_LESS_EQUAL_IMPL(                                                  \
        ::nsfx::test::ToolType::LE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " <= " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_LE(actual, limit)                                      \
    NSFX_TEST_LESS_EQUAL_IMPL(                                                  \
        ::nsfx::test::ToolType::LE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " <= " #limit, actual, limit)
/*}}}*/


// Greater than./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_GREATER_THAN_IMPL(TYPE, LEVEL, DESC, ACT, LIM)                \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeGreaterThanChecker((ACT), (LIM));        \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_GT(actual, limit)                                      \
    NSFX_TEST_GREATER_THAN_IMPL(                                                \
        ::nsfx::test::ToolType::GT,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " > " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_GT(actual, limit)                                      \
    NSFX_TEST_GREATER_THAN_IMPL(                                                \
        ::nsfx::test::ToolType::GT,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " > " #limit, actual, limit)
/*}}}*/


// Greater equal./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 *
 * @internal
 */
#define NSFX_TEST_GREATER_EQUAL_IMPL(TYPE, LEVEL, DESC, ACT, LIM)               \
    /* If the testing is not stopped.  */                                       \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)\
    /* If the actual value is false.   */                                       \
    for (auto pred = ::nsfx::test::MakeGreaterEqualChecker((ACT), (LIM));       \
         go && !pred; go = false)                                               \
    /* Construct and commit a result.  */                                       \
    for (::nsfx::test::Result result(                                           \
            TYPE, LEVEL, DESC,                                                  \
            pred.GetActual(), pred.GetLimit(),                                  \
            __FUNCTION__, __FILE__, __LINE__); go;                              \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)  \
    /* Set the message for the result. */                                       \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)  \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_GE(actual, limit)                                      \
    NSFX_TEST_GREATER_EQUAL_IMPL(                                               \
        ::nsfx::test::ToolType::GE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " >= " #limit, actual, limit)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_GE(actual, limit)                                      \
    NSFX_TEST_GREATER_EQUAL_IMPL(                                               \
        ::nsfx::test::ToolType::GE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " >= " #limit, actual, limit)
/*}}}*/


// Absolute closeness./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 * @param TOL  tolerance value.
 *
 * @internal
 */
#define NSFX_TEST_ABSOLUTE_CLOSENESS_IMPL(TYPE, LEVEL, DESC, ACT, LIM, TOL)             \
    /* If the testing is not stopped.  */                                               \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)        \
    /* If the actual value is false.   */                                               \
    for (auto pred = ::nsfx::test::MakeAbsoluteClosenessChecker((ACT), (LIM), (TOL));   \
         go && !pred; go = false)                                                       \
    /* Construct and commit a result.  */                                               \
    for (::nsfx::test::Result result(                                                   \
            TYPE, LEVEL, DESC,                                                          \
            pred.GetActual(), pred.GetLimit(), pred.GetTolerance(),                     \
            __FUNCTION__, __FILE__, __LINE__); go;                                      \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)          \
    /* Set the message for the result. */                                               \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)          \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_AC(actual, limit, tolerance)                           \
        NSFX_TEST_ABSOLUTE_CLOSENESS_IMPL(                                      \
            ::nsfx::test::ToolType::AC,                                         \
            ::nsfx::test::ToolLevel::EXPECT,                                    \
            "|" #actual " - " #limit "| <= " #tolerance,                        \
            actual, limit, tolerance)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_AC(actual, limit, tolerance)                           \
        NSFX_TEST_ABSOLUTE_CLOSENESS_IMPL(                                      \
            ::nsfx::test::ToolType::AC,                                         \
            ::nsfx::test::ToolLevel::ASSERT,                                    \
            "|" #actual " - " #limit "| <= " #tolerance,                        \
            actual, limit, tolerance)
/*}}}*/


// Relative closeness./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Implements testing assertion.
 *
 * @param TYPE tool type.
 * @param LEVEL tool level.
 * @param DESC description.
 * @param ACT  actual value.
 * @param LIM  expected value.
 * @param TOL  tolerance value.
 *
 * @internal
 */
#define NSFX_TEST_RELATIVE_CLOSENESS_IMPL(TYPE, LEVEL, DESC, ACT, LIM, TOL)             \
    /* If the testing is not stopped.  */                                               \
    for (bool go = true; go && !::nsfx::test::runner::GetStopFlag(); go = false)        \
    /* If the actual value is false.   */                                               \
    for (auto pred = ::nsfx::test::MakeRelativeClosenessChecker((ACT), (LIM), (TOL));   \
         go && !pred; go = false)                                                       \
    /* Construct and commit a result.  */                                               \
    for (::nsfx::test::Result result(                                                   \
            TYPE, LEVEL, DESC,                                                          \
            pred.GetActual(), pred.GetLimit(), pred.GetTolerance(),                     \
            __FUNCTION__, __FILE__, __LINE__); go;                                      \
            ::nsfx::test::runner::CommitResult(std::move(result)), go = false)          \
    /* Set the message for the result. */                                               \
    for (std::ostringstream oss; go; result.SetMessage(oss.str()), go = false)          \
        oss

/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_RC(actual, limit, tolerance)                           \
        NSFX_TEST_ABSOLUTE_CLOSENESS_IMPL(                                      \
            ::nsfx::test::ToolType::RC,                                         \
            ::nsfx::test::ToolLevel::EXPECT,                                    \
            "|" #actual " - " #limit "| <= " #limit " * " #tolerance,           \
            actual, limit, tolerance)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_RC(actual, limit, tolerance)                           \
        NSFX_TEST_RELATIVE_CLOSENESS_IMPL(                                      \
            ::nsfx::test::ToolType::RC,                                         \
            ::nsfx::test::ToolLevel::ASSERT,                                    \
            "|" #actual " - " #limit "| <= " #limit " * " #tolerance,           \
            actual, limit, tolerance)
/*}}}*/


#endif // TEST_TOOL_H__8DBFCA65_0486_449A_8A5C_7BDE42EEDB70


#if defined (NSFX_TEST_TOOL_IS_ITERATING)/*{{{*/

NSFX_TEST_OPEN_NAMESPACE

# if (NSFX_TEST_TOOL_NUM_OPERANDS == 1)

// Checker
template<class Actual>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
public:
    NSFX_TEST_TOOL_CHECKER(const Actual& actual) :
        actual_(actual)
    {
        result_ = NSFX_TEST_TOOL_OPERATOR actual_;
    }

    // Methods.
public:
    std::string GetActual(void) const
    {
        return TestFormatValue((actual_));
    }

    std::string GetLimit(void) const
    {
        return "true";
    }

    bool GetResult(void) const
    {
        return result_;
    }

    operator bool() const
    {
        return result_;
    }


    // Properties.
private:
    const Actual& actual_;
    bool result_;

}; /*}}}*/

// MakeChecker
template<class Actual>
inline NSFX_TEST_TOOL_CHECKER<Actual>
NSFX_TEST_TOOL_MAKE_CHECKER(const Actual& actual)
{
    return NSFX_TEST_TOOL_CHECKER<Actual>(actual);
}

#elif (NSFX_TEST_TOOL_NUM_OPERANDS == 2)

// Checker
template<class Actual, class Limit>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
public:
    NSFX_TEST_TOOL_CHECKER(const Actual& actual, const Limit& limit) :
        actual_(actual),
        limit_(limit)
    {
        result_ = (actual_ NSFX_TEST_TOOL_OPERATOR limit_);
    }

    // Methods.
public:
    std::string GetActual(void) const
    {
        return TestFormatValue((actual_));
    }

    std::string GetLimit(void) const
    {
        return TestFormatValue((limit_));
    }

    bool GetResult(void) const
    {
        return result_;
    }

    operator bool() const
    {
        return result_;
    }


    // Properties.
private:
    const Actual& actual_;
    const Limit& limit_;
    bool result_;

};/*}}}*/

// MakeChecker
template<class Actual, class Limit>
inline NSFX_TEST_TOOL_CHECKER<Actual, Limit>
NSFX_TEST_TOOL_MAKE_CHECKER(const Actual& actual, const Limit& limit)
{
    return NSFX_TEST_TOOL_CHECKER<Actual, Limit>(actual, limit);
}

#elif (NSFX_TEST_TOOL_NUM_OPERANDS == 3)

// Checker
template<class Actual, class Limit, class Tol>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
public:
    NSFX_TEST_TOOL_CHECKER(const Actual& actual, const Limit& limit, const Tol& tol) :
        actual_(actual),
        limit_(limit),
        tol_(tol)
    {
# if (NSFX_TEST_TOOL_OPERATOR == 0) // Absolute closeness
        result_ = !((tol_ < actual_ - limit_) || (tol_ < limit_ - actual_));
# else // !(NSFX_TEST_TOOL_OPERATOR == 0) // Relative closeness
        result_ = !((tol_ * limit_ < actual_ - limit_ ) ||
                    (tol_ * limit_ < limit_  - actual_));
# endif // (NSFX_TEST_TOOL_OPERATOR == n)
    }

    // Methods.
public:
    std::string GetActual(void) const
    {
        return TestFormatValue((actual_));
    }

    std::string GetLimit(void) const
    {
        return TestFormatValue((limit_));
    }

    /**
     * @brief Get the absolute tolerance value.
     */
    std::string GetTolerance(void) const
    {
        typedef typename boost::common_type<Limit, Tol>::type  CommonType;
# if (NSFX_TEST_TOOL_OPERATOR == 0) // Absolute closeness
        return TestFormatValue((tol_));
# else // !(NSFX_TEST_TOOL_OPERATOR == 0) // Relative closeness
        return TestFormatValue((limit_ * tol_));
# endif // (NSFX_TEST_TOOL_OPERATOR == n)
    }

    bool GetResult(void) const
    {
        return result_;
    }

    operator bool() const
    {
        return result_;
    }


    // Properties.
private:
    const Actual& actual_;
    const Limit& limit_;
    const Tol& tol_;
    bool result_;

}; /*}}}*/

// MakeChecker
template<class Actual, class Limit, class Tol>
inline NSFX_TEST_TOOL_CHECKER<Actual, Limit, Tol>
NSFX_TEST_TOOL_MAKE_CHECKER(const Actual& actual, const Limit& limit, const Tol& tol)
{
    return NSFX_TEST_TOOL_CHECKER<Actual, Limit, Tol>(actual, limit, tol);
}

#endif // (NSFX_TEST_TOOL_NUM_OPERANDS == n)

NSFX_TEST_CLOSE_NAMESPACE

#endif // defined (NSFX_TEST_TOOL_IS_ITERATING)/*}}}*/

