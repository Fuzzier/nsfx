/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-22
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_COUNTDOWN_H__6E292462_142C_42B8_BD58_FC2B2D788419
#define I_COUNTDOWN_H__6E292462_142C_42B8_BD58_FC2B2D788419


#include <nsfx/all.h>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
class ICountdown :
    virtual public IObject
{
public:
    virtual ~ICountdown(void) BOOST_NOEXCEPT {}

    // Set the counter.
    virtual void SetCounter(uint32_t c) = 0;

    // Decrement the counter by one until it reaches zeros.
    virtual void Decrement(void) = 0;
};

NSFX_DEFINE_CLASS_UID(ICountdown, "edu.uestc.nsfx.tutorial.ICountdown");


#endif // I_COUNTDOWN_H__6E292462_142C_42B8_BD58_FC2B2D788419

