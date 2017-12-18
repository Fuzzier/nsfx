/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PLATFORM_H__5676E3A6_D0C1_4BE1_A5E6_D7924C5CCCE0
#define PLATFORM_H__5676E3A6_D0C1_4BE1_A5E6_D7924C5CCCE0


#include <nsfx/component/config.h>
#include <nsfx/component/object.h>
#include <nsfx/component/exception.h>
#include <nsfx/simulator/clock.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Platform
/**
 * @ingroup Simulator
 * @brief The simulator platform (non-aggregatable, singleton).
 */
class Platform :
    virtual public Object
{
public:
    virtual ~Platform(void) {}

private:
    IClock* clock_;  /// The system wall clock.
      /// The system scheduler.
};


NSFX_DEFINE_CLASS_UUID4(Platform, 0x5676E3A6, 0xD0C1, 0x4BE1, 0xA5E6D7924C5CCCE0LL);


NSFX_CLOSE_NAMESPACE


#endif // PLATFORM_H__5676E3A6_D0C1_4BE1_A5E6_D7924C5CCCE0

