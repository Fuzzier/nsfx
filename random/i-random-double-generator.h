/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-23
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_RANDOM_DOUBLE_GENERATOR_H__6338F0D0_6AED_4695_92C3_23C03DA74596
#define I_RANDOM_DOUBLE_GENERATOR_H__6338F0D0_6AED_4695_92C3_23C03DA74596


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A continuous uniform random number generator.
 *
 * This random number generator produces double precision floating-point values
 * according to a uniform distribution.
 */
class IRandomDoubleGenerator :
    virtual public IObject
{
public:
    virtual ~IRandomDoubleGenerator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual double Generate(void) = 0;

    /**
     * @brief The minimum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual double GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual double GetMaxValue(void) = 0;

    /**
     * @brief The entropy estimate for the generated random numbers.
     *
     * The value is expressed on a base-2 scale, with a value between \c 0 and
     * <code>std::numeric_limits<double>::digits</code>.
     *
     * @return For a deterministic number generator, the return value is always
     *         \c 0.
     *         <p>
     *         For a non-deterministic random number generator, the return value
     *         may be positive.
     */
    virtual double GetEntropy(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IRandomDoubleGenerator,
                      "edu.uestc.nsfx.IRandomDoubleGenerator");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IRandomDoubleGeneratorUser, "edu.uestc.nsfx.IRandomDoubleGeneratorUser",
    IRandomDoubleGenerator);


NSFX_CLOSE_NAMESPACE


#endif // I_RANDOM_DOUBLE_GENERATOR_H__6338F0D0_6AED_4695_92C3_23C03DA74596

