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

#ifndef SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548
#define SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-simulator.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/simulation/i-event-scheduler.h>
#include <nsfx/simulation/exception.h>
#include <nsfx/event/event.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class Simulator;


////////////////////////////////////////////////////////////////////////////////
// Simulator.
/**
 * @ingroup Simulator
 * @brief The simulator.
 *
 * This simulator provides a clock, and executes events in the scheduler.
 *
 * ## Interfaces
 * ### Uses
 * * \c IEventSchedulerUser
 * ### Provides
 * * \c IClock
 * * \c ISimulator
 * ### Events
 * * \c ISimulationBeginEvent
 * * \c ISimulationRunEvent
 * * \c ISimulationPauseEvent
 * * \c ISimulationEndEvent
 */
class Simulator :
    public IEventSchedulerUser,
    public IClock,
    public ISimulator
{
public:
    Simulator(void) BOOST_NOEXCEPT :
        initialized_(false),
        started_(false),
        paused_(true),
        beginEvent_(this),
        runEvent_(this),
        pauseEvent_(this),
        endEvent_(this)
    {
    }

    virtual ~Simulator(void) {}

    // IEventSchedulerUser /*{{{*/
    virtual void Use(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE
    {
        if (initialized_)
        {
            BOOST_THROW_EXCEPTION(
                IllegalMethodCall() <<
                ErrorMessage("Cannot change the scheduler after initialization."));
        }
        if (!scheduler)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        scheduler_ = scheduler;
        initialized_ = true;
    }

    /*}}}*/

    // IClock /*{{{*/
    virtual TimePoint Now(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return now_;
    }

    /*}}}*/

    // ISimulator /*{{{*/
    virtual void Run(void) NSFX_OVERRIDE
    {
        RunUntil(TimePoint::Max());
    }

    virtual void RunUntil(const TimePoint& t) NSFX_OVERRIDE
    {
        if (!initialized_)
        {
            BOOST_THROW_EXCEPTION(Uninitialized());
        }
        if (!scheduler_->GetNumEvents())
        {
            BOOST_THROW_EXCEPTION(SimulatorFinished());
        }
        CheckBeginOfSimulation();
        paused_ = false;
        FireSimulationRunEvent();
        // An external object can schedule events in its event sink.
        while (!paused_)
        {
            Ptr<IEventHandle> handle = scheduler_->GetNextEvent();
            if (!handle)
            {
                // End the loop when the scheduler is empty.
                break;
            }
            TimePoint t0 = handle->GetTimePoint();
            if (t0 > t)
            {
                // End the loop if the event is scheduled for a later time.
                break;
            }
            now_ = t0;
            // Can pause simulation.
            scheduler_->FireAndRemoveNextEvent();
        }
        paused_ = true;
        FireSimulationPauseEvent();
        CheckEndOfSimulation();
    }

    virtual void RunFor(const Duration& dt) NSFX_OVERRIDE
    {
        RunUntil(now_ + dt);
    }

    virtual void Pause(void) NSFX_OVERRIDE
    {
        paused_ = true;
    }

    void CheckBeginOfSimulation(void)
    {
        if (!started_)
        {
            started_ = true;
            FireSimulationBeginEvent();
        }
    }

    void CheckEndOfSimulation(void)
    {
        if (!scheduler_->GetNumEvents())
        {
            FireSimulationEndEvent();
        }
    }

    /*}}}*/

    // Events./*{{{*/
    void FireSimulationBeginEvent(void)
    {
        beginEvent_.GetImpl()->Fire();
    }

    void FireSimulationRunEvent(void)
    {
        runEvent_.GetImpl()->Fire();
    }

    void FireSimulationPauseEvent(void)
    {
        pauseEvent_.GetImpl()->Fire();
    }

    void FireSimulationEndEvent(void)
    {
        endEvent_.GetImpl()->Fire();
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(Simulator)
        NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
        NSFX_INTERFACE_ENTRY(IClock)
        NSFX_INTERFACE_ENTRY(ISimulator)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationBeginEvent, &beginEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationRunEvent,   &runEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationPauseEvent, &pauseEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationEndEvent,   &endEvent_)
    NSFX_INTERFACE_MAP_END()

private:
    TimePoint  now_;
    Ptr<IEventScheduler>  scheduler_;
    bool  initialized_;
    bool  started_;
    bool  paused_;

    MemberAggObject<Event<ISimulationBeginEvent>>  beginEvent_;
    MemberAggObject<Event<ISimulationRunEvent>>    runEvent_;
    MemberAggObject<Event<ISimulationPauseEvent>>  pauseEvent_;
    MemberAggObject<Event<ISimulationEndEvent>>    endEvent_;

}; // class Simulator


NSFX_REGISTER_CLASS(Simulator, "edu.uestc.nsfx.Simulator");


NSFX_CLOSE_NAMESPACE


#endif // SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548

