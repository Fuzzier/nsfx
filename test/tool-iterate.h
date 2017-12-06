/**
 * @file
 *
 * @brief Testing support for network simulation frameworks.
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

#include <nsfx/preprocessor/tuple-elem.h>


#if !defined (NSFX_TEST_TOOL_FILENAME)
# error NSFX_TEST_TOOL_ITERATE_ERROR: iteration filename is not defined.
#endif // !defined (NSFX_TEST_TOOL_FILENAME)

#if !defined (NSFX_TEST_TOOL_INFO)

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

#else // defined (NSFX_TEST_TOOL_INFO)

# define NSFX_TEST_TOOL_NUM_OPERANDS  NSFX_PP_TUPLE_ELEM(4, 0, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_OPERATOR      NSFX_PP_TUPLE_ELEM(4, 1, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_CHECKER       NSFX_PP_TUPLE_ELEM(4, 2, NSFX_TEST_TOOL_INFO)
# define NSFX_TEST_TOOL_MAKE_CHECKER  NSFX_PP_TUPLE_ELEM(4, 3, NSFX_TEST_TOOL_INFO)

# define NSFX_TEST_TOOL_IS_ITERATING  1
# include NSFX_TEST_TOOL_FILENAME
# undef  NSFX_TEST_TOOL_IS_ITERATING

# undef NSFX_TEST_TOOL_MAKE_CHECKER
# undef NSFX_TEST_TOOL_CHECKER
# undef NSFX_TEST_TOOL_OPERATOR
# undef NSFX_TEST_TOOL_NUM_OPERANDS

#endif // !defined (NSFX_TEST_TOOL_INFO)



