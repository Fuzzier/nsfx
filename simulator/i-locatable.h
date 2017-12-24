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

#ifndef I_LOCATABLE_H__7F711CD4_EFDD_4BF7_9580_9F76E722F67B
#define I_LOCATABLE_H__7F711CD4_EFDD_4BF7_9580_9F76E722F67B


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


/**
 * @ingroup Simulator.
 * @brief Geocentric coordinates.
 */
struct XYZ
{
    double x;
    double y;
    double z;
};


/**
 * @ingroup Simulator.
 * @brief Geographic coordinates.
 */
struct LLA
{
    double longitude;
    double latitude;
    double altitude;
};


////////////////////////////////////////////////////////////////////////////////
// ILocatable.
/**
 * @ingroup Simulator
 * @brief The locatable interface.
 */
class ILocatable :
    virtual public IObject
{
public:
    virtual ~ILocatable(void) BOOST_NOEXCEPT {}

    virtual XYZ GetXYZ(void) = 0;
    virtual LLA GetLLA(void) = 0;

};


NSFX_DEFINE_CALSS_UUID4(ILocatable, 0x7F711CD4, 0xEFDD, 0x4BF7, 0x95809F76E722F67BLL)


NSFX_CLOSE_NAMESPACE


#endif // I_LOCATABLE_H__7F711CD4_EFDD_4BF7_9580_9F76E722F67B

