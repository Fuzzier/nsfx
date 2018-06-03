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

#include <boost/preprocessor/tuple/elem.hpp>


#if !defined(NSFX_TEST_TOOL_FILENAME)
# error NSFX_TEST_TOOL_ITERATE_ERROR: iteration filename is not defined.
#endif // !defined(NSFX_TEST_TOOL_FILENAME)

#if !defined(NSFX_TEST_TOOL_INFO)

// Part 1: Define a tuple that provides necessary information.
//         Each time a tuple is defined, include Part 2 to define
//         class templates.
// (number of operands, operator, class, maker, tool type, impl, expect, assert)
# define NSFX_TEST_TOOL_INFO  (1, !!, PredicateChecker, MakePredicateChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, ==, EqualChecker, MakeEqualChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, !=, NotEqualChecker, MakeNotEqualChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, < , LessThanChecker, MakeLessThanChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, <=, LessEqualChecker, MakeLessEqualChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, > , GreaterThanChecker, MakeGreaterThanChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (2, >=, GreaterEqualChecker, MakeGreaterEqualChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (3, 0 , AbsoluteClosenessChecker, MakeAbsoluteClosenessChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# define NSFX_TEST_TOOL_INFO  (3, 1 , RelativeClosenessChecker, MakeRelativeClosenessChecker)
# include <nsfx/test/tool-iterate.h>
# undef  NSFX_TEST_TOOL_INFO

# undef NSFX_TEST_TOOL_FILENAME

#else // if defined(NSFX_TEST_TOOL_INFO)

// Part 2: Extract information from the tuple.
//         Then include the macro template file to define class templates.
# define NSFX_TEST_TOOL_NUM_OPERANDS  BOOST_PP_TUPLE_ELEM(4, 0, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_OPERATOR      BOOST_PP_TUPLE_ELEM(4, 1, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_CHECKER       BOOST_PP_TUPLE_ELEM(4, 2, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_MAKE_CHECKER  BOOST_PP_TUPLE_ELEM(4, 3, NSFX_TEST_TOOL_INFO)

# define NSFX_TEST_TOOL_IS_ITERATING  1
# include NSFX_TEST_TOOL_FILENAME
# undef  NSFX_TEST_TOOL_IS_ITERATING

# undef NSFX_TEST_TOOL_MAKE_CHECKER
# undef NSFX_TEST_TOOL_CHECKER
# undef NSFX_TEST_TOOL_OPERATOR
# undef NSFX_TEST_TOOL_NUM_OPERANDS

#endif // !defined(NSFX_TEST_TOOL_INFO)



