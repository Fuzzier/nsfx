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

#ifndef I_CHANNEL_H__3DE4C118_B5B3_4B73_9D99_836A0137D26E
#define I_CHANNEL_H__3DE4C118_B5B3_4B73_9D99_836A0137D26E


#include <nsfx/network/config.h>
#include <nsfx/network/i-object.h>


NSFX_OPEN_NAMESPACE


class IChannelSink /*{{{*/
{
public:
    virtual ~IChannelSink(void) BOOST_NOEXCEPT {}

public:
    virtual void StartTransmission(Ptr<IPacket> packet) = 0;
    virtual void EndTransmission(Ptr<IPacket> packet) = 0;

    virtual void StartReception(Ptr<IPacket> packet) = 0;
    virtual void EndReception(Ptr<IPacket> packet) = 0;

}; // class IChannelSink /*}}}*/


NSFX_DEFINE_CALSS_UUID4(IChannelSink, 0xD37C27F6, 0xE911, 0x4225, 0x96BB625D3688F261LL);


////////////////////////////////////////////////////////////////////////////////
// IChannel.
class IChannel :
    virtual public IObject
{
public:
    virtual ~IChannel(void) BOOST_NOEXCEPT {}

    virtual void Initialize(IObject* scenario) = 0;

    virtual void Connect(IChannelSink* sink) = 0;
    virtual void Disconnect(IChannelSink* sink) = 0;

}; // class IChannel


NSFX_DEFINE_CALSS_UUID4(IChannel, 0xCAE60041, 0xEDA1, 0x44B7, 0xBD06B52E892DF521LL);


NSFX_CLOSE_NAMESPACE


#endif // I_CHANNEL_H__3DE4C118_B5B3_4B73_9D99_836A0137D26E

