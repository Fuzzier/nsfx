/**
 * @file
 *
 * @brief Test support for network simulation frameworks.
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
 * @brief Test support.
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
 */


////////////////////////////////////////////////////////////////////////////////
#define NSFX_TEST_OPEN_NAMESPACE    \
    NSFX_OPEN_NAMESPACE             \
    namespace test {


#define NSFX_TEST_CLOSE_NAMESPACE   \
    } /* namespace test */          \
    NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__F2E2C329_1A2E_4899_AC64_C9EABF65BFC6

