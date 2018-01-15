/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
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


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-simulator.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/simulator/i-event-scheduler.h>
#include <nsfx/simulator/exception.h>
#include <nsfx/event/event.h>
#include <nsfx/component/i-disposable.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class Simulator;

#define NSFX_CID_Simulator  NSFX_UUID_OF(::nsfx::Simulator)


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
 * * \c Clock
 * * \c ISimulator
 * * \c IDisposable
 * ### Events
 * * \c ISimulationBeginEvent
 * * \c ISimulationRunEvent
 * * \c ISimulationPauseEvent
 * * \c ISimulationEndEvent
 */
class Simulator :
    public IEventSchedulerUser,
    public IClock,
    public ISimulator,
    public IDisposable
{
public:
    Simulator(void) BOOST_NOEXCEPT :
        initialized_(false),
        started_(false),
        finished_(false),
        beginEvent_(this),
        runEvent_(this),
        pauseEvent_(this),
        endEvent_(this)
    {
    }

    virtual ~Simulator(void) BOOST_NOEXCEPT {}

    // IEventSchedulerUser /*{{{*/
    virtual void UseEventScheduler(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE
    {
        if (!scheduler)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (scheduler_)
        {
            BOOST_THROW_EXCEPTION(CannotReinitialize());
        }
        scheduler_ = scheduler;
        initialized_ = true;
    }

    /*}}}*/

    // IClock /*{{{*/
    virtual TimePoint Now(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return t_;
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
        if (finished_)
        {
            BOOST_THROW_EXCEPTION(SimulatorFinished());
        }
        CheckBeginOfSimulation();
        SignalSimulationRunEvent();
        // An external object can schedule events in its event sink.
        while (true)
        {
            Ptr<IEventHandle> handle = scheduler_->GetNextEvent();
            if (!handle)
            {
                break;
            }
            TimePoint t0 = handle->GetTimePoint();
            if (t0 > t)
            {
                break;
            }
            t_ = t0;
            handle->Signal();
        }
        SignalSimulationPauseEvent();
        CheckEndOfSimulation();
    }

    virtual void RunFor(const Duration& dt) NSFX_OVERRIDE
    {
        RunUntil(t_ + dt);
    }

    void CheckBeginOfSimulation(void)
    {
        if (!started_)
        {
            SignalSimulationBeginEvent();
            started_ = true;
        }
    }

    void CheckEndOfSimulation(void)
    {
        if (!scheduler_->GetNextEvent() && !finished_)
        {
            SignalSimulationEndEvent();
            finished_ = true;
        }
    }

    /*}}}*/

    // IDisposable/*{{{*/
    virtual void Dispose(void) NSFX_OVERRIDE
    {
        t_           = TimePoint();
        initialized_ = false;
        scheduler_   = nullptr;
        started_     = false;
        finished_    = false;
        beginEvent_.GetEnveloped()->Dispose();
        runEvent_.GetEnveloped()->Dispose();
        pauseEvent_.GetEnveloped()->Dispose();
        endEvent_.GetEnveloped()->Dispose();
    }
    /*}}}*/

    // Events./*{{{*/
    void SignalSimulationBeginEvent(void)
    {
        beginEvent_.GetEnveloped()->Visit(
            [] (ISimulationBeginEventSink* sink){ sink->Fire(); });
    }

    void SignalSimulationRunEvent(void)
    {
        runEvent_.GetEnveloped()->Visit(
            [] (ISimulationRunEventSink* sink){ sink->Fire(); });
    }

    void SignalSimulationPauseEvent(void)
    {
        pauseEvent_.GetEnveloped()->Visit(
            [] (ISimulationPauseEventSink* sink){ sink->Fire(); });
    }

    void SignalSimulationEndEvent(void)
    {
        endEvent_.GetEnveloped()->Visit(
            [] (ISimulationEndEventSink* sink){ sink->Fire(); });
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(Simulator)
        NSFX_INTERFACE_ENTRY(ISimulator)
        NSFX_INTERFACE_ENTRY(IClock)
        NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
        NSFX_INTERFACE_ENTRY(IDisposable)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationBeginEvent, &beginEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationRunEvent,   &runEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationPauseEvent, &pauseEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ISimulationEndEvent,   &endEvent_)
    NSFX_INTERFACE_MAP_END()

private:
    TimePoint  t_;
    Ptr<IEventScheduler>  scheduler_;
    bool  initialized_;
    bool  started_;
    bool  finished_;

    AggObject<Event<ISimulationBeginEvent>, false>  beginEvent_;
    AggObject<Event<ISimulationRunEvent>,   false>  runEvent_;
    AggObject<Event<ISimulationPauseEvent>, false>  pauseEvent_;
    AggObject<Event<ISimulationEndEvent>,   false>  endEvent_;

}; // class Simulator


NSFX_DEFINE_CLASS_UUID4(Simulator, 0xC079AC9A, 0x0F83, 0x48F4, 0x82F354924DBBA46CLL);
NSFX_REGISTER_CLASS(Simulator);


NSFX_CLOSE_NAMESPACE


#endif // SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548

