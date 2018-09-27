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

#ifndef I_PSEUDO_RANDOM_ENGINE_H__8A081D7A_9B63_4C6A_8631_294E868F25D3
#define I_PSEUDO_RANDOM_ENGINE_H__8A081D7A_9B63_4C6A_8631_294E868F25D3


#include <nsfx/random/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A pseudo-random number engine.
 */
class IPseudoRandomEngine
{
public:
    virtual ~IPseudoRandomEngine(void) BOOST_NOEXCEPT {}

    /**
     * @brief Re-initialize the pseudo-random number generator by a seed value.
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

};

NSFX_DEFINE_CLASS_UID(IPseudoRandomEngine,
                      "edu.uestc.nsfx.IPseudoRandomEngine");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IPseudoRandomEngineUser, "edu.uestc.nsfx.IPseudoRandomEngineUser",
    IPseudoRandomEngine);


NSFX_CLOSE_NAMESPACE


#endif // I_PSEUDO_RANDOM_ENGINE_H__8A081D7A_9B63_4C6A_8631_294E868F25D3

