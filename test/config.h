/**
 * @file
 *
 * @brief Test support for network simulation Framework.
 *
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-26
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__F2E2C329_1A2E_4899_AC64_C9EABF65BFC6
#define CONFIG_H__F2E2C329_1A2E_4899_AC64_C9EABF65BFC6


#include <nsfx/config.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Test
 *
 * @brief Test library.
 *
 * Tests are organized in a tree structure.
 * The root of the tree is a master test suite.
 * A test suite contains test suites and test cases.
 * A test case is a leaf of the tree, i.e.,
 * test cases cannot contain test suites or cases.
 *
 * In a test case, users invoke test assertions to check the results of
 * their code.
 * If a test assertion fails, a test result is generated.
 * The test results are logged.
 *
 * @remarks The test library is made because of the convient trailing streaming.
 *          For example,
 *          @code
 *          NSFX_TEST_EXPECT_EQ(a, b) << "trailing " << "message";
 *          @endcode
 *          The test tools in BOOST test library lack such feature.
 */


////////////////////////////////////////////////////////////////////////////////
#define NSFX_TEST_OPEN_NAMESPACE    \
    NSFX_OPEN_NAMESPACE             \
    namespace test {


#define NSFX_TEST_CLOSE_NAMESPACE   \
    } /* namespace test */          \
    NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__F2E2C329_1A2E_4899_AC64_C9EABF65BFC6

