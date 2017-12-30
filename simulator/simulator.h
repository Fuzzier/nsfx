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
#include <nsfx/simulator/i-alarm.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Simulator.
/**
 * @ingroup Simulator
 * @brief The simulator interface.
 *
 * ### Provides:
 * \c IClock, \c IAlarm, \c ISimulator.
 */
class Simulator :
    public ISimulator,
    public IClock,
    public IAlarm
{
private:
    class Alarm/*{{{*/
    {
    public:
        Alarm(void) BOOST_NOEXCEPT :
            simulator_(nullptr),
            pending_(false)
        {}

        void Wire(Simulator* simulator) BOOST_NOEXCEPT
        {
            simulator_ = simulator;
        }

        // For IAlarm /*{{{*/
        void Connect(Ptr<IAlarmSink> sink)
        {
            if (!sink)
            {
                BOOST_THROW_EXCEPTION(InvalidPointer());
            }
            if (sink_)
            {
                BOOST_THROW_EXCEPTION(ConnectionLimit());
            }
            sink_ = std::move(sink);
        }

        void Disconnect(void) BOOST_NOEXCEPT
        {
            Cancel();
            sink_.Reset();
        }

        void ScheduleAt(const TimePoint& t)
        {
            if (!sink_)
            {
                BOOST_THROW_EXCEPTION(NotConnected());
            }
            if (t < simulator_->Simulator::Now())
            {
                BOOST_THROW_EXCEPTION(InvalidArgument());
            }
            t_ = t;
            pending_ = true;
        }

        void ScheduleIn(const Duration& dt)
        {
            if (!sink_)
            {
                BOOST_THROW_EXCEPTION(NotConnected());
            }
            if (dt < Duration(0))
            {
                BOOST_THROW_EXCEPTION(InvalidArgument());
            }
            t_ = simulator_->Simulator::Now() + dt;
            pending_ = true;
        }

        bool IsPending(void) BOOST_NOEXCEPT
        {
            return pending_;
        }

        void Cancel(void) BOOST_NOEXCEPT
        {
            pending_ = false;
        }

        /*}}}*/

        TimePoint GetTimePoint(void) BOOST_NOEXCEPT
        {
            return t_;
        }

        void Fire(void)
        {
            pending_ = false;
            sink_->OnAlarm();
        }

    private:
        Simulator* simulator_;
        Ptr<IAlarmSink> sink_;
        TimePoint t_;
        bool pending_;
    }; // class Alarm

    /*}}}*/

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
    Simulator(void) :
        started_(false),
        finished_(false)
    {
        alarm_.Wire(this);
    }

    virtual ~Simulator(void) BOOST_NOEXCEPT {}

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
        CheckBeginOfSimulation();
        sinks_.Fire(NSFX_SIMULATOR_EVENT_RUN);
        // An external object can schedule alarms in its alarm sink.
        while (alarm_.IsPending() && alarm_.GetTimePoint() <= t)
        {
            FireAlarm();
        }
        sinks_.Fire(NSFX_SIMULATOR_EVENT_PAUSE);
        CheckEndOfSimulation();
    }

    virtual void RunFor(const Duration& dt) NSFX_OVERRIDE
    {
        RunUntil(t_ + dt);
    }

    /**
     * @pre <code>alarm_->IsPending() == true</code>
     * @post \c t_ is updated to the time point of the scheduled alarm.
     */
    void FireAlarm(void)
    {
        t_ = alarm_.GetTimePoint();
        alarm_.Fire();
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
        if (!alarm_.IsPending() && !finished_)
        {
            sinks_.Fire(NSFX_SIMULATOR_EVENT_END);
            sinks_.Clear();
            finished_ = true;
        }
    }

    /*}}}*/

    // IClock/*{{{*/
    virtual TimePoint Now(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return t_;
    }

    /*}}}*/

    // IAlarm /*{{{*/
    virtual void Connect(Ptr<IAlarmSink> sink) NSFX_OVERRIDE
    {
        alarm_.Connect(std::move(sink));
    }

    virtual void Disconnect(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        alarm_.Disconnect();
    }

    virtual void ScheduleAt(const TimePoint& t) NSFX_OVERRIDE
    {
        alarm_.ScheduleAt(t);
    }

    virtual void ScheduleIn(const Duration& dt) NSFX_OVERRIDE
    {
        alarm_.ScheduleIn(dt);
    }

    virtual bool IsPending(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return alarm_.IsPending();
    }

    virtual void Cancel(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        alarm_.Cancel();
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(Simulator)
        NSFX_INTERFACE_ENTRY(ISimulator)
        NSFX_INTERFACE_ENTRY(IClock)
        NSFX_INTERFACE_ENTRY(IAlarm)
    NSFX_INTERFACE_MAP_END()

private:
    TimePoint t_;
    Alarm     alarm_;
    bool      started_;
    bool      finished_;
    SimulatorSinks sinks_;
}; // class Simulator


NSFX_DEFINE_CLASS_UUID4(Simulator, 0xC079AC9A, 0x0F83, 0x48F4, 0x82F354924DBBA46CLL);
NSFX_REGISTER_CLASS(Simulator);


NSFX_CLOSE_NAMESPACE


#define NSFX_CID_Simulator  NSFX_UUID_OF(::nsfx::Simulator)


#endif // SIMULATOR_H__DE24AB2E_CEA6_41E0_A0C7_B1949A5BB548

