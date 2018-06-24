/**
 * @file
 *
 * @brief Variant support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__523314C2_8F78_400F_A670_D85217C35D0E
#define CONFIG_H__523314C2_8F78_400F_A670_D85217C35D0E


#include <nsfx/config.h>
#include <nsfx/interop/exception.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Interop
 * @defgroup Variant
 *
 * @brief Union-based type-safe value.
 *
 * # Motivation
 *   To enable interoperability, variant are provided.
 *
 *   A variant is a union-based type-safe value.
 *   It provides type-erasure feature, without resort to virtual inheritance.
 *
 *   The type-safety is guaranteed, since a variant has a single-type and a
 *   single-value during its lifetime.
 *   The type of a variant is described by a member variable \c vartype.
 *
 *   The set of supported types are limited.
 *   The library forbids to change the vartype of a variant
 *
 */


#endif // CONFIG_H__523314C2_8F78_400F_A670_D85217C35D0E

