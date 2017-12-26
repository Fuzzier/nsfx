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

#ifndef I_SCENARIO_H__3269A1E3_23F4_4002_A5ED_25B19485E574
#define I_SCENARIO_H__3269A1E3_23F4_4002_A5ED_25B19485E574


#include <nsfx/component/config.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/simulator/i-alarm.h>
#include <nsfx/simulator/i-event-scheduler.h>
#include <nsfx/network/i-channel.h>
#include <nsfx/network/i-node.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IScenario.
/**
 * @ingroup Network
 * @brief The scenario interface.
 *
 * A scenario holds all resources needed to run a simulation.<br/>
 * It holds a reference count, directly or indirectly, of every static object
 * within it, e.g., channels, nodes, protocol entities, etc.<br/>
 * It does not manage the lifetime of dynamic objects directly, such as packets.<br/>
 */
class IScenario :
    virtual public IObject
{
public:
    virtual ~IScenario(void) BOOST_NOEXCEPT {}

    virtual void Initialize(Ptr<IClock> clock,
                            Ptr<IAlarm> alarm,
                            Ptr<IEventScheduler> scheduler) = 0;

    virtual void AddChannel(Ptr<IChannel> channel) = 0;
    virtual uint32_t GetNumChannels(void) = 0;
    virtual IChannel GetChannel(uint32_t index) = 0;

    virtual void AddNode(Ptr<INode> channel) = 0;
    virtual uint32_t GetNumNodes(void) = 0;
    virtual INode* GetNode(uint32_t index) = 0;

    virtual void Run(void) = 0;
    virtual void RunUntil(const TimePoint& t) = 0;
    virtual void RunFor(const Duration& t) = 0;

}; // class IScenario


NSFX_DEFINE_CALSS_UUID4(IScenario, 0x58DB8889, 0x9270, 0x4F73, 0xAC86525152B53260LL);


NSFX_CLOSE_NAMESPACE


#endif // I_SCENARIO_H__3269A1E3_23F4_4002_A5ED_25B19485E574

