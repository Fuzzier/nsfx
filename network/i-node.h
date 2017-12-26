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

#ifndef I_NODE_H__F59C14F7_9389_49DC_9BB3_F7E72368DA10
#define I_NODE_H__F59C14F7_9389_49DC_9BB3_F7E72368DA10


#include <nsfx/component/config.h>
#include <nsfx/component/object.h>
#include <nsfx/component/exception.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/simulator/i-alarm.h>
#include <nsfx/simulator/i-event-scheduler.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// INode.
class INode :
    virtual public IObject
{
public:
    virtual ~INode(void) BOOST_NOEXCEPT {}

    virtual void AddDevice(Ptr<IDevice> device) = 0;
    virtual uint32_t GetNumDevices(void) = 0;
    virtual IDevice* GetDevice(uint32_t index) = 0;

}; // class Scenario


NSFX_DEFINE_CALSS_UUID4(INode, 0x3AFD6B78, 0x6961, 0x4BF7, 0x9ADE9F715AA3B3F0LL);


NSFX_CLOSE_NAMESPACE


#endif // I_NODE_H__F59C14F7_9389_49DC_9BB3_F7E72368DA10

