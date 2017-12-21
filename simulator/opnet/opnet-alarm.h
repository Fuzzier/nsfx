/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-18
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef OPNET_ALARM_H__95F58633_03C9_4123_9117_5EFE6E7D68D4
#define OPNET_ALARM_H__95F58633_03C9_4123_9117_5EFE6E7D68D4


#include <nsfx/simulator/config.h>
#include <nsfx/component/iobject.h>
#include <nsfx/component/ialarm-sink.h>
#include <nsfx/component/ialarm.h>
#include <nsfx/component/ptr.h>
#include <opnet.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// OpnetAlarm.
/**
 * @ingroup Simulator
 * @brief The alarm provided by OPNET.
 */
class OpnetAlarm :
    virtual public IAlarm,
    virtual public AggObject
{
public:
    typedef OpnetAlarm  ThisType;

public:
    OpnetAlarm(void) {}

    virtual ~OpnetAlarm(void) BOOST_NOEXCEPT {}

    // IObject./*{{{*/
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (iid == )
        {
            // code
        }
        return result;
    }
    /*}}}*/

    // IAlarm./*{{{*/
    virtual void Connect(IUnknown* sink) NSFX_FINAL NSFX_OVERRIDE
    {
        if (sink_)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        else if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        sink_.Reset(sink);
        if (!sink_)
        {
            BOOST_THROW_EXCEPTION(CannotConnect());
        }
    }

    virtual void Disconnect(void) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!sink_)
        {
            BOOST_THROW_EXCEPTION(NotConnected());
        }
        ThisType::Cancel();
        sink_.Reset();
    }

    virtual void ScheduleAt(const TimePoint& t) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!sink_)
        {
            BOOST_THROW_EXCEPTION(NotConnected());
        }
        double t0 = to_opnet_time_point(t);
        TimePoint now = op_sim_time();
        if (t0 < now)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        evh_ = ::op_intrpt_schedule_call(t0
                                         0,
                                         &ThisType::OnTimeout,
                                         sink_.Get());
    }

    virtual void ScheduleFor(const Duration& dt) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!sink_)
        {
            BOOST_THROW_EXCEPTION(NotConnected());
        }
        double dt0 = to_opnet_duration(dt);
        if (dt0 < 0)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        evh_ = ::op_intrpt_schedule_call(op_sim_time() + dt0,
                                         0,
                                         &ThisType::OnTimeout,
                                         sink_.Get());
    }

    virtual void IsPending(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        return !!::op_ev_pending(evh_);
    }

    virtual void Cancel(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        ::op_ev_cancel_if_pending(evh_);
    }
    /*}}}*/

private:
    static void OnTimeout(IAlarmSink* sink, int /* code */ )
    {
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        sink->Fire();
    }

private:
    Ptr<IClock> clock_;
    Ptr<IAlarmSink> sink_;
    ::Evhandle evh_;
};


NSFX_DEFINE_CLASS_UUID4(OpnetAlarm, 0x0FC1DAC2, 0x82D2, 0x46A6, 0x9C61E23484B8608CLL);


NSFX_CLOSE_NAMESPACE


#endif // OPNET_ALARM_H__95F58633_03C9_4123_9117_5EFE6E7D68D4
