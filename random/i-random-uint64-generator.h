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

#ifndef I_RANDOM_UINT64_GENERATOR_H__821ED707_415B_49B8_AA8C_C0C4E9352CB0
#define I_RANDOM_UINT64_GENERATOR_H__821ED707_415B_49B8_AA8C_C0C4E9352CB0


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A discrete uniform random number generator.
 *
 * This random number generator produces 64-bit unsigned integer values
 * according to a uniform distribution.
 */
class IRandomUint64Generator :
    virtual public IObject
{
public:
    virtual ~IRandomUint64Generator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Generate a new random number.
     */
    virtual uint64_t Generate(void) = 0;

    /**
     * @brief The minimum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual uint64_t GetMinValue(void) = 0;

    /**
     * @brief The maximum value that can be potentially generated.
     *
     * The return value <b>must</b> be the same during the lifetime of the
     * random engine.
     */
    virtual uint64_t GetMaxValue(void) = 0;

    /**
     * @brief The entropy estimate for the generated random numbers.
     *
     * The value is expressed on a base-2 scale, with a value between \c 0 and
     * <i>log2(GetMaxValue()+1)</i>
     * (which is equal to <code>std::numeric_limits<unsigned int>::digits</code>).
     *
     * @return For a pseudo-random (deterministic) number generator, the value
     *         returned is always \c 0.
     *         <p>
     *         For a non-deterministic random number generator, the value
     *         returned may be positive.
     */
    virtual double GetEntropy(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IRandomUint64Generator,
                      "edu.uestc.nsfx.IRandomUint64Generator");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IRandomUint64GeneratorUser, "edu.uestc.nsfx.IRandomUint64GeneratorUser",
    IRandomUint64Generator);


NSFX_CLOSE_NAMESPACE


#endif // I_RANDOM_UINT64_GENERATOR_H__821ED707_415B_49B8_AA8C_C0C4E9352CB0

