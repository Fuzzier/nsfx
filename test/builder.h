/**
 * @file
 *
 * @brief Test support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef BUILDER_H__7EEBC505_46D4_49A0_AC0B_2942076331D8
#define BUILDER_H__7EEBC505_46D4_49A0_AC0B_2942076331D8


#include <nsfx/test/config.h>
#include <nsfx/test/case.h>
#include <nsfx/test/suite.h>
#include <nsfx/test/runner.h>


////////////////////////////////////////////////////////////////////////////////
// NullFixture /*{{{*/
NSFX_TEST_OPEN_NAMESPACE


/**
 * @ingroup Test
 *
 * @brief The default test fixture.
 *
 * @internal
 */
class NullFixture
{
}; // class NullFixture


NSFX_TEST_CLOSE_NAMESPACE
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLES./*{{{*/
/**
 * @ingroup Test
 * @brief The active test suite in current namespace.
 * @internal
 *
 * Its purpose is to temporarily override the variable value in a new namespace,
 * and provide a stack-like mechanism that restores the variable value when
 * exiting the current namespace.
 * In different namespaces, the name of the variable remains the same, which
 * makes the life much easier to implement an easy-to-use way to construct
 * nesting test suites and cases. Especially, abitrary level of test suites and
 * cases can be provided.
 *
 * The value is not updated at runtime, so don't access it at runtime.
 * Use \c Runner::getActiveCase() to access the currently running test case,
 * and use \c Case::getSuite() to access the test suite that contains the case.
 */
static ::nsfx::test::Suite* NSFX_ACTIVE_TEST_SUITE =
        ::nsfx::test::runner::getMasterSuite();

/**
 * @ingroup Test
 * @brief The active test fixture in current namespace.
 * @internal
 *
 * Its purpose is to temporarily override the typedef in a new namespace,
 * and provide a stack-like mechanism that restores the typedef when
 * exiting the current namespace.
 * In different namespaces, the name of the typedef remains the same, which
 * makes the life much easier to implement an easy-to-use way to construct
 * test suites and cases. Especially, abitrary level of test suites and cases
 * can be provided.
 *
 * Anytime, use \c NSFX_TEST_SUITE_F(name, F) to use custom fixture \c F.
 * Anytime, use \c NSFX_TEST_CASE_F(name, F) to use custom fixture \c F.
 * The above two macros doesn't override \c NSFX_ACTIVE_FIXTURE in their
 * container suite (restored via stack-like mechanism).
 */
typedef ::nsfx::test::NullFixture NSFX_ACTIVE_FIXTURE;
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// MACROS./*{{{*/
/**
 * @ingroup Test
 *
 * @brief Define test suite with a fixture.
 * @internal
 *
 * @remarks This construct doesn't combine test suites with the same name
 *          in different translation units.
 */
#define NSFX_TEST_SUITE_IMPL(name, suite_name, FixtureType)                     \
    namespace suite_name                                                        \
    {                                                                           \
        typedef FixtureType NSFX_ACTIVE_FIXTURE;                                \
        /* Add a child test suite by defining a static variable.     */         \
        /* If the test suite already exists, the existing test suite */         \
        /* is obtained.                                              */         \
        /* It's suffice to support combining multiple test suites    */         \
        /* with the same identity from different translation units.  */         \
        static ::nsfx::test::Suite* NSFX_ACTIVE_TEST_SUITE_0 =                  \
                    NSFX_ACTIVE_TEST_SUITE->addSuite(#name);                    \
        /* Override the NSFX_ACTIVE_TEST_SUITE in the outer namespace.  */      \
        /* The test cases defined in this namespace enjoy the overrided */      \
        /* NSFX_ACTIVE_TEST_SUITE.                                      */      \
        static ::nsfx::test::Suite* NSFX_ACTIVE_TEST_SUITE =                    \
                    NSFX_ACTIVE_TEST_SUITE_0;                                   \
    }                                                                           \
    /* Reopen namespace to let user define test suites and cases in braces. */  \
    namespace suite_name

/**
 * @ingroup Test
 *
 * @brief Define a test suite without changing the active fixture.
 */
#define NSFX_TEST_SUITE(name)                                                   \
    NSFX_TEST_SUITE_IMPL(name, S_##name, NSFX_ACTIVE_FIXTURE)

/**
 * @ingroup Test
 *
 * @brief Define a test suite while specifying the active fixture.
 */
#define NSFX_TEST_SUITE_F(name, FixtureType)                                    \
    NSFX_TEST_SUITE_IMPL(name, S_##name, FixtureType)

/**
 * @ingroup Test
 *
 * @brief Define a test case with a fixture.
 * @internal
 */
#define NSFX_TEST_CASE_IMPL(name, case_name, register_name, FixtureType)        \
    /* A test case with a fixture. */                                           \
    struct case_name : public FixtureType                                       \
    {                                                                           \
        void run(void);                                                         \
    };                                                                          \
    /* Register a test case by defining a static variable. */                   \
    static struct register_name                                                 \
    {                                                                           \
        register_name(void)                                                         \
        {                                                                       \
            /* Use lambda expression to encourage inlining. */                  \
            NSFX_ACTIVE_TEST_SUITE->addCase(#name, [] { case_name().run(); });  \
        }                                                                       \
    } NSFX_TEST_REGISTER_##case_name;                                                         \
    /* Let user define test runner in braces. */                                \
    void case_name::run(void)

/**
 * @ingroup Test
 *
 * @brief Define a test case with the active fixture.
 *
 * @code
 * NSFX_TEST_CASE(case1)
 * {
 *   int i = 0;
 *   NSFX_TEST_ASSERT_EQ(i, 0);
 * }
 * @endcode
 */
#define NSFX_TEST_CASE(name)                                                    \
    NSFX_TEST_CASE_IMPL(name, C_##name, R_##name, NSFX_ACTIVE_FIXTURE)

/**
 * @ingroup Test
 *
 * @brief Define a test case with the speficied fixture.
 *
 * @code
 * struct MyFixture
 * {
 *   MyFixture(void) :
 *     i(new int(0))
 *   {
 *   }
 *   ~MyFixture(void)
 *   {
 *     delete i;
 *   }
 *   int* i;
 * };
 *
 * NSFX_TEST_CASE_F(case1, MyFixture)
 * {
 *   // Access public and protected members of fixture directly.
 *   NSFX_TEST_ASSERT_EQ(i, 0);
 * }
 * @endcode
 */
#define NSFX_TEST_CASE_F(name, F)                                               \
    NSFX_TEST_CASE_IMPL(name, C_##name, R_##name, F)

/*}}}*/


#endif // BUILDER_H__7EEBC505_46D4_49A0_AC0B_2942076331D8

