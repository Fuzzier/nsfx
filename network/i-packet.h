/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-26
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_PACKET_H__F0088BCC_D04A_4CBA_8443_AF4D430E55F6
#define I_PACKET_H__F0088BCC_D04A_4CBA_8443_AF4D430E55F6


#include <nsfx/component/config.h>
#include <nsfx/network/i-scenario.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IPacket.
class IPacket :
    virtual public IObject
{
public:
    virtual ~IPacket(void) BOOST_NOEXCEPT {}

    virtual void Initialize(IObject* node) = 0;

    virtual void Connect(IChannel* channel) = 0;

}; // class IPacket


NSFX_DEFINE_CALSS_UUID4(IPacket, 0xCC3BD8BF, 0x8A58, 0x42AE, 0x88C412FB7A9CB79BLL);


NSFX_CLOSE_NAMESPACE


#endif // I_PACKET_H__F0088BCC_D04A_4CBA_8443_AF4D430E55F6

