/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-28
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52
#define I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52


#include <nsfx/simulation/config.h>
#include <nsfx/event/i-event.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ISimulator;
class ISimulatorUser;

class ISimulationBeginEvent;
class ISimulationRunEvent;
class ISimulationPauseEvent;
class ISimulationEndEvent;

class ISimulationBeginEventSink;
class ISimulationRunEventSink;
class ISimulationPauseEventSink;
class ISimulationEndEventSink;


////////////////////////////////////////////////////////////////////////////////
// ISimulator.
/**
 * @ingroup Simulator
 * @brief A simulator.
 */
class ISimulator :
    virtual public IObject
{
public:
    virtual ~ISimulator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Run until there is no scheduled alarm.
     *
     * @throw NoScheduledEvent There are no events in the scheduler.
     *
     * @post The simulation time is the time point of the last fired event.
     *       <p>
     *       If the scheduler is empty, then no event is fired, and
     *       the simulation time is unchanged.
     */
    virtual void Run(void) = 0;

    /**
     * @brief Run until a specific time point.
     *
     * @throw NoScheduledEvent There are no events in the scheduler.
     *
     * @post If the simulation is *paused* via `Pause()`, the simulation time is
     *       the time point of the last fired event.
     *       <p>
     *       Otherwise, the simulation time advances to `t`, which *may* beyond
     *       the time point of the last fired event.
     */
    virtual void RunUntil(const TimePoint& t) = 0;

    /**
     * @brief Run for a specific duration.
     *
     * @throw NoScheduledEvent There are no events in the scheduler.
     *
     * @post If the simulation is *paused* via `Pause()`, the simulation time is
     *       the time point of the last fired event.
     *       <p>
     *       Otherwise, the simulation time advances for `dt`, which *may* beyond
     *       the time point of the last fired event.
     */
    virtual void RunFor(const Duration& dt) = 0;

    /**
     * @brief Pause the simulator.
     */
    virtual void Pause(void) = 0;

};


NSFX_DEFINE_CLASS_UID(ISimulator, "edu.uestc.nsfx.ISimulator");


////////////////////////////////////////////////////////////////////////////////
// Simulator events.
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ISimulationBeginEventSink, "edu.uestc.nsfx.ISimulationBeginEventSink",
    ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ISimulationRunEventSink, "edu.uestc.nsfx.ISimulationRunEventSink",
    ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ISimulationPauseEventSink, "edu.uestc.nsfx.ISimulationPauseEventSink",
    ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ISimulationEndEventSink, "edu.uestc.nsfx.ISimulationEndEventSink",
    ( void(void) ));

NSFX_DEFINE_EVENT_INTERFACE(
    ISimulationBeginEvent, "edu.uestc.nsfx.ISimulationBeginEvent",
    ISimulationBeginEventSink);
NSFX_DEFINE_EVENT_INTERFACE(
    ISimulationRunEvent, "edu.uestc.nsfx.ISimulationRunEvent",
    ISimulationRunEventSink);
NSFX_DEFINE_EVENT_INTERFACE(
    ISimulationPauseEvent, "edu.uestc.nsfx.ISimulationPauseEvent",
    ISimulationPauseEventSink);
NSFX_DEFINE_EVENT_INTERFACE(
    ISimulationEndEvent, "edu.uestc.nsfx.ISimulationEndEvent",
    ISimulationEndEventSink);


////////////////////////////////////////////////////////////////////////////////
// ISimulatorUser.
NSFX_DEFINE_USER_INTERFACE(
    ISimulatorUser, "edu.uestc.nsfx.ISimulatorUser",
    ISimulator);


NSFX_CLOSE_NAMESPACE


#endif // I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52

