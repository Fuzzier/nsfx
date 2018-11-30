/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EVENT_HANDLE_H__8555C8AA_DABA_4ED6_A57C_7F163F4DD31A
#define EVENT_HANDLE_H__8555C8AA_DABA_4ED6_A57C_7F163F4DD31A


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-event-handle.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// EventHandle.
/**
 * @ingroup Simulator
 * @brief An event handle.
 *
 * * Interfaces
 *   + Provides
 *     - \c IEventHandle
 */
class EventHandle :
    public IEventHandle
{
public:
    EventHandle(event_id_t id,
                const TimePoint& t,
                const Ptr<IEventSink<>>& sink) :
        id_(id),
        t_(t),
        sink_(sink),
        running_(false)
    {}

    EventHandle(event_id_t id,
                const TimePoint& t,
                Ptr<IEventSink<>>&& sink) :
        id_(id),
        t_(t),
        sink_(std::move(sink)),
        running_(false)
    {}

    virtual ~EventHandle(void) {}

    // IEventHandle /*{{{*/
public:
    virtual event_id_t GetId(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return id_;
    }

    virtual bool IsPending(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return (sink_ && !running_);
    }

    virtual bool IsRunning(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return (sink_ && running_);
    }

    virtual bool IsValid(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return !!sink_;
    }

    virtual void Cancel(void) NSFX_OVERRIDE
    {
        sink_ = nullptr;
    }

    virtual TimePoint GetTimePoint(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return t_;
    }

    /*}}}*/

public:
    IEventHandle* GetIntf(void) BOOST_NOEXCEPT
    {
        return this;
    }

    void Fire(void)
    {
        if (sink_)
        {
            running_ = true;
            sink_->Fire();
            running_ = false;
            sink_ = nullptr;
        }
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(EventHandle)
        NSFX_INTERFACE_ENTRY(IEventHandle)
    NSFX_INTERFACE_MAP_END()

private:
    event_id_t id_;
    TimePoint  t_;
    Ptr<IEventSink<>>  sink_;
    bool running_;

};


////////////////////////////////////////////////////////////////////////////////
inline bool operator==(const Ptr<EventHandle>& lhs,
                       const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT(lhs && rhs);
    return lhs->GetId() == rhs->GetId();
}

inline bool operator!=(const Ptr<EventHandle>& lhs,
                       const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    return !(lhs == rhs);
}

inline bool operator<(const Ptr<EventHandle>& lhs,
                      const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT(lhs && rhs);
    TimePoint t0 = lhs->GetTimePoint();
    TimePoint t1 = rhs->GetTimePoint();
    bool result = (t0 < t1);
    if (t0 == t1)
    {
        result = (lhs->GetId() < rhs->GetId());
    }
    return result;
}

inline bool operator<=(const Ptr<EventHandle>& lhs,
                       const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    return (lhs == rhs) || (lhs < rhs);
}

inline bool operator>=(const Ptr<EventHandle>& lhs,
                       const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    BOOST_ASSERT(lhs && rhs);
    return !(lhs < rhs);
}

inline bool operator>(const Ptr<EventHandle>& lhs,
                      const Ptr<EventHandle>& rhs) BOOST_NOEXCEPT
{
    return !(lhs <= rhs);
}


NSFX_CLOSE_NAMESPACE


#endif // EVENT_HANDLE_H__8555C8AA_DABA_4ED6_A57C_7F163F4DD31A

