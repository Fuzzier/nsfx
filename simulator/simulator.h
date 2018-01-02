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
 * ### Interfaces:
 * * \c IEventSchedulerUser
 * * \c Clock
 * * \c ISimulator
 * * \c IEventScheduler
 */
class Simulator :
    public ISimulator,
    public IClock,
    public IEventSchedulerUser
{
private:
    class SimulatorSinks/*{{{*/
    {
        class SimulatorSink
        {
        public:
            SimulatorSink(Ptr<ISimulatorSink>&& sink, SimulatorEventType mask) :
                sink_(std::move(sink)),
                mask_(mask)
            {}

            void Fire(SimulatorEventType type)
            {
                if (mask_ & type)
                {
                    sink_->OnSimulatorEvent(type);
                }
            }

        private:
            Ptr<ISimulatorSink> sink_;
            SimulatorEventType  mask_;
        };

    public:
        // For ISimulatorSink /*{{{*/
        cookie_t Connect(Ptr<ISimulatorSink>&& sink, SimulatorEventType mask)
        {
            if (!sink)
            {
                BOOST_THROW_EXCEPTION(InvalidPointer());
            }
            if (!mask)
            {
                BOOST_THROW_EXCEPTION(InvalidArgument());
            }
            map_.emplace(++cookie_, SimulatorSink(std::move(sink), mask));
            return cookie_;
        }

        void Disconnect(cookie_t cookie) BOOST_NOEXCEPT
        {
            map_.erase(cookie_);
        }

        void Fire(SimulatorEventType type)
        {
            for (auto it = map_.begin(); it != map_.end(); ++it)
            {
                it->second.Fire(type);
            }
        }

        void Clear(void) BOOST_NOEXCEPT
        {
            map_.clear();
        }

        /*}}}*/

    private:
        cookie_t cookie_;
        unordered_map<cookie_t, SimulatorSink>  map_;
    };/*}}}*/

public:
    Simulator(void) BOOST_NOEXCEPT :
        initialized_(false),
        started_(false),
        finished_(false)
    {
    }

    virtual ~Simulator(void) BOOST_NOEXCEPT {}

    // IEventSchedulerUser /*{{{*/
    virtual void UseEventScheduler(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE
    {
        if (initialized_)
        {
            BOOST_THROW_EXCEPTION(CannotReinitialize());
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
        return t_;
    }

    /*}}}*/

    // ISimulator /*{{{*/
    virtual cookie_t Connect(Ptr<ISimulatorSink> sink,
                             SimulatorEventType mask) NSFX_OVERRIDE
    {
        return sinks_.Connect(std::move(sink), mask);
    }

    virtual void Disconnect(cookie_t cookie) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        sinks_.Disconnect(cookie);
    }

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
        sinks_.Fire(NSFX_SIMULATOR_EVENT_RUN);
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
            handle->Fire();
        }
        sinks_.Fire(NSFX_SIMULATOR_EVENT_PAUSE);
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
            sinks_.Fire(NSFX_SIMULATOR_EVENT_BEGIN);
            started_ = true;
        }
    }

    void CheckEndOfSimulation(void)
    {
        if (!scheduler_->GetNextEvent() && !finished_)
        {
            sinks_.Fire(NSFX_SIMULATOR_EVENT_END);
            sinks_.Clear();
            finished_ = true;
        }
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(Simulator)
        NSFX_INTERFACE_ENTRY(ISimulator)
        NSFX_INTERFACE_ENTRY(IClock)
        NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
    NSFX_INTERFACE_MAP_END()

private:
    TimePoint  t_;
    Ptr<IEventScheduler>  scheduler_;
    bool  initialized_;
    bool  started_;
    bool  finished_;
    SimulatorSinks sinks_;
}; // class Simulator


NSFX_DEFINE_CLASS_UUID4(Simulator, 0xC079AC9A, 0x0F83, 0x48F4, 0x82F354924DBBA46CLL);
NSFX_REGISTER_CLASS(Simulator);


NSFX_CLOSE_NAMESPACE


#endif // SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548

