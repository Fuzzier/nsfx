/**
 * @file
 *
 * @brief Test support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
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
#include <memory> // unique_ptr
#include <sstream>
#include <iomanip>
#include <locale> // locale, isprint
#include <type_traits> // integral_constant, is_same, common_type, is_integral


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 * @brief Define test tools.
 *
 * The following test tools are defined:
 * @code{.cpp}
 * NSFX_TEST_EXPECT(pred)                           // Truthful.
 * NSFX_TEST_EXPECT_EQ(actual, limit)               // Equal.
 * NSFX_TEST_EXPECT_NE(actual, limit)               // Not equal.
 * NSFX_TEST_EXPECT_LT(actual, limit)               // Less than.
 * NSFX_TEST_EXPECT_LE(actual, limit)               // Less equal.
 * NSFX_TEST_EXPECT_GT(actual, limit)               // Greater than.
 * NSFX_TEST_EXPECT_GE(actual, limit)               // Greater equal.
 * NSFX_TEST_EXPECT_AC(actual, limit, tolerance)    // Absolute closeness.
 * NSFX_TEST_EXPECT_RC(actual, limit, tolerance)    // Relative closeness.
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
 *
 * Users can also use `NSFX_TEST_MESSAGE()` to output messages.
 */


////////////////////////////////////////////////////////////////////////////////
NSFX_TEST_OPEN_NAMESPACE


template<class T, bool integral = std::is_integral<T>::value>
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
struct ValueFormatter<bool>
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
            typename std::integral_constant<size_t, sizeof (T)>::type());
    }

    std::string operator()(
        T value, typename std::integral_constant<size_t, 1>::type) const
    {
        std::ostringstream oss;
        if (std::isprint(value, std::locale()))
        {
            oss << value;
        }
        else
        {
            oss << ' ';
        }
        oss << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << (static_cast<size_t>(value) & 0xff)
            << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename std::integral_constant<size_t, 2>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << (static_cast<size_t>(value) & 0xffff)
            << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename std::integral_constant<size_t, 4>::type) const
    {
        std::ostringstream oss;
        oss << value << " (0x"
            << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
            << std::nouppercase << value << ")";
        return oss.str();
    }

    std::string operator()(
        T value, typename std::integral_constant<size_t, 8>::type) const
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

template<>
struct ValueFormatter<const char*>
{
    std::string operator()(const char* value) const
    {
        std::ostringstream oss;
        oss << "\"" << value << "\"";
        return oss.str();
    }
};

template<>
struct ValueFormatter<std::string>
{
    std::string operator()(const std::string& value) const
    {
        std::ostringstream oss;
        oss << "\"" << value << "\"";
        return oss.str();
    }
};

template<class T>
inline std::string FormatValue(T value)
{
    return ValueFormatter<T>()(value);
}


NSFX_TEST_CLOSE_NAMESPACE


// Define class templates.
#define NSFX_TEST_TOOL_FILENAME <nsfx/test/tool.h>
#include <nsfx/test/tool-iterate.h>


////////////////////////////////////////////////////////////////////////////////
// Message./*{{{*/
/**
 * @ingroup Test
 * @brief Output message.
 */
#define NSFX_TEST_MESSAGE()                                                     \
    /* Set the message for output. */                                           \
    if (bool go = true)                                                         \
    for (std::ostringstream oss; go;                                            \
         ::nsfx::test::runner::ShowMessage(oss.str()), go = false)              \
        oss
/*}}}*/


// Predicate./*{{{*/
/**
 * @ingroup Test
 * @brief Implement testing assertion.
 *
 * @param[in] TYPE  The tool type.
 * @param[in] LEVEL The tool level.
 * @param[in] DESC  The description.
 * @param[in] ACT   The actual value.
 *
 * The actual value is evaluated only once by the `checker`.
 * If the test assertion failed, the result is stored in the `checker`.
 */
#define NSFX_TEST_PREDICATE_IMPL(TYPE, LEVEL, DESC, ACT)                        \
    /* If the testing is not stopped.  */                                       \
    if (!::nsfx::test::runner::GetStopFlag())                                   \
    /* If the actual value is false.   */                                       \
    for (auto checker = ::nsfx::test::MakePredicateChecker(ACT);                \
         !checker.Done();                                                       \
    /* Construct and commit a result.  */                                       \
         checker.CommitResult(                                                  \
            TYPE, LEVEL, DESC, __FUNCTION__, __FILE__, __LINE__))               \
    /* Set the message for the result. */                                       \
        checker.GetStream()

/**
 * @ingroup Test
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


// Compare./*{{{*/
/**
 * @ingroup Test
 * @brief Implements testing assertion.
 *
 * @param[in] TYPE  The tool type.
 * @param[in] LEVEL The tool level.
 * @param[in] DESC  The description.
 * @param[in] ACT   The actual value.
 * @param[in] LIM   The expected value.
 * @param OP        The name of the operator.
 *                  * `Equal`
 *                  * `NotEqual`
 *                  * `LessThan`
 *                  * `LessEqual`
 *                  * `GreaterThan`
 *                  * `GreaterEqual`
 */
#define NSFX_TEST_COMPARE_IMPL(TYPE, LEVEL, DESC, ACT, LIM, OP)                 \
    /* If the testing is not stopped.  */                                       \
    if (!::nsfx::test::runner::GetStopFlag())                                   \
    /* If the actual value is false.   */                                       \
    for (auto checker = ::nsfx::test::Make ##OP## Checker((ACT), (LIM));        \
         !checker.Done();                                                       \
    /* Construct and commit a result.  */                                       \
         checker.CommitResult(                                                  \
            TYPE, LEVEL, DESC, __FUNCTION__, __FILE__, __LINE__))               \
    /* Set the message for the result. */                                       \
        checker.GetStream()

// Equal.
/**
 * @ingroup Test
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_EQ(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::EQ,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " == " #limit, actual, limit,                                   \
        Equal)

/**
 * @ingroup Test
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_EQ(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::EQ,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " == " #limit, actual, limit,                                   \
        Equal)

// Not equal.
/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_NE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::NE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " != " #limit, actual, limit,                                   \
        NotEqual)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_NE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::NE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " != " #limit, actual, limit,                                   \
        NotEqual)


// Less than.
/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_LT(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::LT,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " < " #limit, actual, limit,                                    \
        LessThan)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_LT(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::LT,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " < " #limit, actual, limit,                                    \
        LessThan)


// Less equal.
/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_LE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::LE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " <= " #limit, actual, limit,                                   \
        LessEqual)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_LE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::LE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " <= " #limit, actual, limit,                                   \
        LessEqual)


// Greater than.
/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_GT(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::GT,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " > " #limit, actual, limit,                                    \
        GreaterThan)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_GT(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::GT,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " > " #limit, actual, limit,                                    \
        GreaterThan)


// Greater equal.
/**
 * @ingroup Test
 *
 * @brief Expected testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_GE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::GE,                                             \
        ::nsfx::test::ToolLevel::EXPECT,                                        \
        #actual " >= " #limit, actual, limit,                                   \
        GreaterEqual)

/**
 * @ingroup Test
 *
 * @brief Required testing assertion.
 *
 * @param actual Must be insertable to C++ stream.
 * @param limit  Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_GE(actual, limit)                                      \
    NSFX_TEST_COMPARE_IMPL(                                                     \
        ::nsfx::test::ToolType::GE,                                             \
        ::nsfx::test::ToolLevel::ASSERT,                                        \
        #actual " >= " #limit, actual, limit,                                   \
        GreaterEqual)
/*}}}*/


// Closeness./*{{{*/
/**
 * @ingroup Test
 * @brief Implements testing assertion.
 *
 * @param[in] TYPE  The tool type.
 * @param[in] LEVEL The tool level.
 * @param[in] DESC  The description.
 * @param[in] ACT   The actual value.
 * @param[in] LIM   The expected value.
 * @param[in] TOL   The tolerance value.
 * @param OP        The name of the operator.
 *                  * `AbsoluteCloseness`
 *                  * `RelativeCloseness`
 */
#define NSFX_TEST_CLOSENESS_IMPL(TYPE, LEVEL, DESC, ACT, LIM, TOL, OP)          \
    /* If the testing is not stopped.  */                                       \
    if (!::nsfx::test::runner::GetStopFlag())                                   \
    /* If the actual value is false.   */                                       \
    for (auto checker = ::nsfx::test::Make ##OP## Checker((ACT), (LIM), (TOL)); \
         !checker.Done();                                                       \
    /* Construct and commit a result.  */                                       \
         checker.CommitResult(                                                  \
            TYPE, LEVEL, DESC, __FUNCTION__, __FILE__, __LINE__))               \
    /* Set the message for the result. */                                       \
        checker.GetStream()

// Absolute closeness.
/**
 * @ingroup Test
 * @brief Expected testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_AC(actual, limit, tolerance)                           \
        NSFX_TEST_CLOSENESS_IMPL(                                               \
            ::nsfx::test::ToolType::AC,                                         \
            ::nsfx::test::ToolLevel::EXPECT,                                    \
            "|" #actual " - " #limit "| <= " #tolerance,                        \
            actual, limit, tolerance,                                           \
            AbsoluteCloseness)

/**
 * @ingroup Test
 * @brief Required testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_AC(actual, limit, tolerance)                           \
        NSFX_TEST_CLOSENESS_IMPL(                                               \
            ::nsfx::test::ToolType::AC,                                         \
            ::nsfx::test::ToolLevel::ASSERT,                                    \
            "|" #actual " - " #limit "| <= " #tolerance,                        \
            actual, limit, tolerance,                                           \
            AbsoluteCloseness)

// Relative closeness.
/**
 * @ingroup Test
 * @brief Expected testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_EXPECT_RC(actual, limit, tolerance)                           \
        NSFX_TEST_CLOSENESS_IMPL(                                               \
            ::nsfx::test::ToolType::RC,                                         \
            ::nsfx::test::ToolLevel::EXPECT,                                    \
            "|" #actual " - " #limit "| <= " #limit " * " #tolerance,           \
            actual, limit, tolerance,                                           \
            RelativeCloseness)

/**
 * @ingroup Test
 * @brief Required testing assertion.
 *
 * @param actual    Must be insertable to C++ stream.
 * @param limit     Must be insertable to C++ stream.
 * @param tolerance Must be insertable to C++ stream.
 */
#define NSFX_TEST_ASSERT_RC(actual, limit, tolerance)                           \
        NSFX_TEST_CLOSENESS_IMPL(                                               \
            ::nsfx::test::ToolType::RC,                                         \
            ::nsfx::test::ToolLevel::ASSERT,                                    \
            "|" #actual " - " #limit "| <= " #limit " * " #tolerance,           \
            actual, limit, tolerance,                                           \
            RelativeCloseness)
/*}}}*/


#endif // TEST_TOOL_H__8DBFCA65_0486_449A_8A5C_7BDE42EEDB70


#if defined (NSFX_TEST_TOOL_IS_ITERATING)/*{{{*/

NSFX_TEST_OPEN_NAMESPACE

# if (NSFX_TEST_TOOL_NUM_OPERANDS == 1)

// Checker
template<class Actual>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
    struct Data
    {
        template<class A>
        Data(A&& actual) :
            actual_(std::forward<A>(actual))
        {}

        template<class Desc>
        void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                          const char* func, const char* file, size_t lineno)
        {
            runner::CommitResult(
                std::unique_ptr<Result>(new Result(
                    type, level, std::forward<Desc>(desc),
                    FormatValue(actual_),
                    func, file, lineno, oss_.str())));
        }

        std::ostream& GetStream(void) BOOST_NOEXCEPT
        {
            return oss_;
        }

        Actual actual_;
        std::ostringstream oss_;
    };

public:
    template<class A>
    NSFX_TEST_TOOL_CHECKER(A&& actual)
    {
        if (!(NSFX_TEST_TOOL_OPERATOR actual))
        {
            // Store the result of the evaluation if the test assertion failed.
            data_ = std::unique_ptr<Data>(new Data(std::forward<A>(actual)));
        }
    }

    NSFX_TEST_TOOL_CHECKER(NSFX_TEST_TOOL_CHECKER&& rhs) BOOST_NOEXCEPT :
        data_(std::move(rhs.data_)) {}

    bool Done(void) const BOOST_NOEXCEPT
    {
        return !data_;
    }

    std::ostream& GetStream(void) const BOOST_NOEXCEPT
    {
        return data_->GetStream();
    }

    template<class Desc>
    void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                      const char* func, const char* file, size_t lineno)
    {
        data_->CommitResult(type, level, std::forward<Desc>(desc),
                            func, file, lineno);
        data_ = nullptr;
    }

private:
    std::unique_ptr<Data> data_; ///< Available only if the test assertion failed.
                                 ///< Allocated on heap to be less stack-consuming.
}; /*}}}*/

// MakeChecker
template<class Actual>
inline NSFX_TEST_TOOL_CHECKER<typename std::decay<Actual>::type>
NSFX_TEST_TOOL_MAKE_CHECKER(Actual&& actual)
{
    typedef typename std::decay<Actual>::type  A;
    return NSFX_TEST_TOOL_CHECKER<A>(std::forward<Actual>(actual));
}

#elif (NSFX_TEST_TOOL_NUM_OPERANDS == 2)

// Checker
template<class Actual, class Limit>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
    struct Data
    {
        template<class A, class L>
        Data(A&& actual, L&& limit) :
            actual_(std::forward<A>(actual)),
            limit_(std::forward<L>(limit))
        {}

        template<class Desc>
        void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                          const char* func, const char* file, size_t lineno)
        {
            runner::CommitResult(
                std::unique_ptr<Result>(new Result(
                    type, level, std::forward<Desc>(desc),
                    FormatValue(actual_), FormatValue(limit_),
                    func, file, lineno, oss_.str())));
        }

        std::ostream& GetStream(void) BOOST_NOEXCEPT
        {
            return oss_;
        }

        Actual actual_;
        Limit  limit_;
        std::ostringstream oss_;
    };

public:
    template<class A, class L>
    NSFX_TEST_TOOL_CHECKER(A&& actual, L&& limit)
    {
        if (!(actual NSFX_TEST_TOOL_OPERATOR limit))
        {
            data_ = std::unique_ptr<Data>(new Data(
                std::forward<A>(actual),
                std::forward<L>(limit)));
        }
    }

    NSFX_TEST_TOOL_CHECKER(NSFX_TEST_TOOL_CHECKER&& rhs) BOOST_NOEXCEPT :
        data_(std::move(rhs.data_)) {}

    bool Done(void) const BOOST_NOEXCEPT
    {
        return !data_;
    }

    std::ostream& GetStream(void) const BOOST_NOEXCEPT
    {
        return data_->GetStream();
    }

    template<class Desc>
    void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                      const char* func, const char* file, size_t lineno)
    {
        data_->CommitResult(type, level, std::forward<Desc>(desc),
                            func, file, lineno);
        data_ = nullptr;
    }

private:
    std::unique_ptr<Data> data_; ///< Available only if the test assertion failed.
                                 ///< Allocated on heap to be less stack-consuming.

};/*}}}*/

// MakeChecker
template<class Actual, class Limit>
inline NSFX_TEST_TOOL_CHECKER<typename std::decay<Actual>::type,
                              typename std::decay<Limit>::type>
NSFX_TEST_TOOL_MAKE_CHECKER(Actual&& actual, Limit&& limit)
{
    typedef typename std::decay<Actual>::type A;
    typedef typename std::decay<Limit>::type  L;
    return NSFX_TEST_TOOL_CHECKER<A, L>(std::forward<Actual>(actual),
                                        std::forward<Limit>(limit));
}

#elif (NSFX_TEST_TOOL_NUM_OPERANDS == 3)

// Checker
template<class Actual, class Limit, class Tol>
class NSFX_TEST_TOOL_CHECKER /*{{{*/
{
    struct Data
    {
        template<class A, class L, class T>
        Data(A&& actual, L&& limit, T&& tol) :
            actual_(std::forward<A>(actual)),
            limit_(std::forward<L>(limit)),
            tol_(std::forward<T>(tol))
        {}

        template<class Desc>
        void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                          const char* func, const char* file, size_t lineno)
        {
            runner::CommitResult(
                std::unique_ptr<Result>(new Result(
                    type, level, std::forward<Desc>(desc),
                    FormatValue(actual_), FormatValue(limit_),
# if (NSFX_TEST_TOOL_OPERATOR == 0) // Absolute closeness
                    FormatValue(tol_),
# else // !(NSFX_TEST_TOOL_OPERATOR == 0) // Relative closeness
                    FormatValue(limit_* tol_),
# endif // (NSFX_TEST_TOOL_OPERATOR == n)
                    func, file, lineno, oss_.str())));
        }

        std::ostream& GetStream(void) BOOST_NOEXCEPT
        {
            return oss_;
        }

        Actual actual_;
        Limit  limit_;
        Tol    tol_;
        std::ostringstream oss_;
    };

public:
    template<class A, class L, class T>
    NSFX_TEST_TOOL_CHECKER(A&& actual, L&& limit, T&& tol)
    {
# if (NSFX_TEST_TOOL_OPERATOR == 0) // Absolute closeness
        if ((tol < actual - limit) || (tol < limit - actual))
# else // !(NSFX_TEST_TOOL_OPERATOR == 0) // Relative closeness
        if ((limit * tol < actual - limit ) ||
            (limit * tol < limit  - actual))
# endif // (NSFX_TEST_TOOL_OPERATOR == n)
        {
            data_ = std::unique_ptr<Data>(new Data(
                std::forward<A>(actual),
                std::forward<L>(limit),
                std::forward<T>(tol)));
        }
    }

    NSFX_TEST_TOOL_CHECKER(NSFX_TEST_TOOL_CHECKER&& rhs) BOOST_NOEXCEPT :
        data_(std::move(rhs.data_)) {}

    bool Done(void) const BOOST_NOEXCEPT
    {
        return !data_;
    }

    std::ostream& GetStream(void) const BOOST_NOEXCEPT
    {
        return data_->GetStream();
    }

    template<class Desc>
    void CommitResult(ToolType type, ToolLevel level, Desc&& desc,
                      const char* func, const char* file, size_t lineno)
    {
        data_->CommitResult(type, level, std::forward<Desc>(desc),
                            func, file, lineno);
        data_ = nullptr;
    }

private:
    std::unique_ptr<Data> data_; ///< Available only if the test assertion failed.
                                 ///< Allocated on heap to be less stack-consuming.

}; /*}}}*/

// MakeChecker
template<class Actual, class Limit, class Tol>
inline NSFX_TEST_TOOL_CHECKER<typename std::decay<Actual>::type,
                              typename std::decay<Limit>::type,
                              typename std::decay<Tol>::type>
NSFX_TEST_TOOL_MAKE_CHECKER(Actual&& actual, Limit&& limit, Tol&& tol)
{
    typedef typename std::decay<Actual>::type A;
    typedef typename std::decay<Limit>::type  L;
    typedef typename std::decay<Tol>::type    T;
    return NSFX_TEST_TOOL_CHECKER<A, L, T>(std::forward<Actual>(actual),
                                           std::forward<Limit>(limit),
                                           std::forward<Tol>(tol));
}

#endif // (NSFX_TEST_TOOL_NUM_OPERANDS == n)

NSFX_TEST_CLOSE_NAMESPACE

#endif // defined (NSFX_TEST_TOOL_IS_ITERATING)/*}}}*/

