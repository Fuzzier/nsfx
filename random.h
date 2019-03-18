/**
 * @file
 *
 * @brief Testing support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-27
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RANDOM_H__B4E5C3DA_BCD7_45DD_967D_1C6A539E69E5
#define RANDOM_H__B4E5C3DA_BCD7_45DD_967D_1C6A539E69E5


#include <nsfx/random/config.h>

// Distributions.
#include <nsfx/random/distribution/i-uniform-uint32-distribution.h>
#include <nsfx/random/distribution/i-uniform-int32-distribution.h>
#include <nsfx/random/distribution/i-uniform-uint64-distribution.h>
#include <nsfx/random/distribution/i-uniform-int64-distribution.h>
#include <nsfx/random/distribution/i-uniform-double-distribution.h>
#include <nsfx/random/distribution/i-uniform-float-distribution.h>

#include <nsfx/random/distribution/i-bernoulli-distribution.h>
#include <nsfx/random/distribution/i-binomial-distribution.h>
#include <nsfx/random/distribution/i-geometric-distribution.h>
#include <nsfx/random/distribution/i-negative-binomial-distribution.h>

#include <nsfx/random/distribution/i-poisson-distribution.h>
#include <nsfx/random/distribution/i-exponential-distribution.h>
#include <nsfx/random/distribution/i-gamma-distribution.h>
#include <nsfx/random/distribution/i-weibull-distribution.h>
#include <nsfx/random/distribution/i-extreme-value-distribution.h>
#include <nsfx/random/distribution/i-beta-distribution.h>
#include <nsfx/random/distribution/i-laplace-distribution.h>

#include <nsfx/random/distribution/i-normal-distribution.h>
#include <nsfx/random/distribution/i-lognormal-distribution.h>
#include <nsfx/random/distribution/i-chi-squared-distribution.h>
#include <nsfx/random/distribution/i-cauchy-distribution.h>
#include <nsfx/random/distribution/i-fisher-f-distribution.h>
#include <nsfx/random/distribution/i-student-t-distribution.h>

#include <nsfx/random/distribution/i-discrete-distribution.h>
#include <nsfx/random/distribution/i-piecewise-constant-distribution.h>
#include <nsfx/random/distribution/i-piecewise-linear-distribution.h>

#include <nsfx/random/distribution/i-triangle-distribution.h>

// Pseudo-random number generators.
#include <nsfx/random/engine/splitmix-engine.h>
#include <nsfx/random/engine/xoroshiro-engine.h>
#include <nsfx/random/engine/xoshiro-engine.h>

// Random number generators.
#include <nsfx/random/i-random-distribution-generator.h>

#include <nsfx/random/i-random-uint32-generator.h>
#include <nsfx/random/i-random-uint64-generator.h>
#include <nsfx/random/i-random-double-generator.h>

#include <nsfx/random/i-pseudo-random-engine.h>

#include <nsfx/random/pseudo-random-generator.h>


#endif // RANDOM_H__B4E5C3DA_BCD7_45DD_967D_1C6A539E69E5

