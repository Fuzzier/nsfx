/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-24
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_MOVABLE_H__8DA30A2E_ECFE_4D80_97D0_89CEA77792D0
#define I_MOVABLE_H__8DA30A2E_ECFE_4D80_97D0_89CEA77792D0


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/simulator/i-locatable.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IMovable.
/**
 * @ingroup Simulator
 * @brief The movable interface.
 */
class IMovable :
    virtual public IObject,
    virtual public ILocatable
{
public:
    virtual ~IMovable(void) BOOST_NOEXCEPT {}

    // ILocatable.
    virtual XYZ GetXYZ(void) NSFX_OVERRIDE = 0;
    virtual LLA GetLLA(void) NSFX_OVERRIDE = 0;

    // IMovable.
    virtual void SetXYZ(const XYZ& coord) = 0;
    virtual void SetLLA(const LLA& coord) = 0;

};


NSFX_DEFINE_CALSS_UUID4(x, 0x8DA30A2E, 0xECFE, 0x4D80, 0x97D089CEA77792D0LL)


NSFX_CLOSE_NAMESPACE


#endif // I_MOVABLE_H__8DA30A2E_ECFE_4D80_97D0_89CEA77792D0

