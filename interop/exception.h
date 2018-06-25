/**
 * @file
 *
 * @brief Exception support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-24
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__101BD45F_7684_4913_8438_F87EC6288DC8
#define EXCEPTION_H__101BD45F_7684_4913_8438_F87EC6288DC8


#include <nsfx/interop/config.h>
#include <nsfx/exception/exception.h>


NSFX_OPEN_NAMESPACE


/**
 * @ingroup Exception
 * @brief The supplied type mismatches the type of the variant.
 */
struct VariantTypeMismatch : Exception {};

/**
 * @ingroup Exception
 * @brief Invalid return value.
 */
struct InvalidReturnVariant : Exception {};

struct InvalidArgumentVariant : Exception {};

struct InvalidNumberOfArguments : Exception {};

typedef boost::error_info<struct tag_ArgumentIndex, size_t>  ArgumentIndex;
typedef boost::error_info<struct tag_ExpectedNumberOfArguments, size_t>  ExpectedNumberOfArguments;
typedef boost::error_info<struct tag_SuppliedNumberOfArguments, size_t>  SuppliedNumberOfArguments;


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__101BD45F_7684_4913_8438_F87EC6288DC8

