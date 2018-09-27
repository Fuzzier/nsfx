/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_QUASI_RANDOM_NUMBER_GENERATOR_H__EC15EC28_DDE1_4D9E_B27B_44CE0FDA2D4F
#define I_QUASI_RANDOM_NUMBER_GENERATOR_H__EC15EC28_DDE1_4D9E_B27B_44CE0FDA2D4F


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A quasi-random number engine.
 */
class IQuasiRandomEngine
{
public:
    virtual ~IQuasiRandomEngine(void) BOOST_NOEXCEPT {}

    /**
     * @brief Re-initialize the quasi-random number generator by a seed value.
     *
     * @param[in] value The seed value.
     */
    virtual void Seed(uint64_t value) = 0;

    /**
     * @brief Advances the internal state by \c z notches.
     *
     * This function operations as if \c Generate() was called \c z times,
     * but without generating any numbers in the process.
     *
     * @param[in] z The number of advances.
     */
    virtual void Discard(uint64_t z) = 0;

    /**
     * @brief The dimension of the quasi-random number generator.
     */
    virtual uint32_t GetDimension(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IQuasiRandomEngine,
                      "edu.uestc.nsfx.IQuasiRandomEngine");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IQuasiRandomEngineUser, "edu.uestc.nsfx.IQuasiRandomEngineUser",
    IQuasiRandomEngine);


NSFX_CLOSE_NAMESPACE


#endif // I_QUASI_RANDOM_NUMBER_GENERATOR_H__EC15EC28_DDE1_4D9E_B27B_44CE0FDA2D4F

