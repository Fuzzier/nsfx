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

#ifndef I_ENTITY_H__BE6BB3B5_6187_455D_8D3E_D33BBA6E5069
#define I_ENTITY_H__BE6BB3B5_6187_455D_8D3E_D33BBA6E5069


#include <nsfx/network/config.h>
#include <nsfx/network/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IEntitySink.
class IEntitySink :
    virtual public IObject
{
public:
    virtual ~IEntitySink(void) BOOST_NOEXCEPT {}

    virtual void Send(Ptr<IPacket> packet) = 0;

}; // class IEntitySink


NSFX_DEFINE_CALSS_UUID4(IEntitySink, 0x8D412E63, 0x9745, 0x414A, 0xA5FEC3B5AE7EA6DALL);


////////////////////////////////////////////////////////////////////////////////
// IEntity.
class IEntity :
    virtual public IObject
{
public:
    virtual ~IEntity(void) BOOST_NOEXCEPT {}

    virtual void Initialize(IObject* node) = 0;

    virtual void Connect(int32_t port, IEntitySink* sink) = 0;
    virtual void Disconnect(int32_t port, IEntitySink* sink) = 0;

}; // class IEntity


NSFX_DEFINE_CALSS_UUID4(IEntity, 0xCAE60041, 0xEDA1, 0x44B7, 0xBD06B52E892DF521LL);


NSFX_CLOSE_NAMESPACE


#endif // I_ENTITY_H__BE6BB3B5_6187_455D_8D3E_D33BBA6E5069

