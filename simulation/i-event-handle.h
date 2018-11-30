/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
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

#ifndef I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2
#define I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2


#include <nsfx/simulation/config.h>
#include <nsfx/event/i-event-sink.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IEventHandle.
/**
 * @ingroup Simulator
 * @brief The event handle interface.
 */
class IEventHandle :
    virtual public IObject
{
public:
    virtual ~IEventHandle(void) BOOST_NOEXCEPT {}

    /**
     * @brief The id of the event.
     */
    virtual event_id_t GetId(void) = 0;

    /**
     * @brief The event is waiting to be fired.
     */
    virtual bool IsPending(void) = 0;

    /**
     * @brief The event is being fired.
     */
    virtual bool IsRunning(void) = 0;

    /**
     * @brief The event is waiting or being fired.
     */
    virtual bool IsValid(void) = 0;

    /**
     * @brief Cancel the event.
     */
    virtual void Cancel(void) = 0;

    /**
     * @brief Get the time point when the event is about to be fired.
     */
    virtual TimePoint GetTimePoint(void) = 0;

};


NSFX_DEFINE_CLASS_UID(IEventHandle, "edu.uestc.nsfx.IEventHandle");


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2

