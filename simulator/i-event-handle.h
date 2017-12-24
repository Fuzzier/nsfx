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

#ifndef I_EVENT_HANDLE_H__2925FDD7_6BF4_4674_9FC8_B703DE318D56
#define I_EVENT_HANDLE_H__2925FDD7_6BF4_4674_9FC8_B703DE318D56


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


typedef uint64_t  event_id_t;


////////////////////////////////////////////////////////////////////////////////
// IEventHandle.
class IEventHandle :
    virtual public IObject
{
public:
    virtual ~IEventHandle(void) BOOST_NOEXCEPT {}

    virtual bool IsValid(void) = 0;
    virtual bool IsPending(void) = 0;
    virtual void Cancel(void) = 0;
    virtual event_id_t GetId(void) = 0;
    virtual TimePoint GetTimePoint(void) = 0;
};


NSFX_DEFINE_CALSS_UUID4(IEventHandle, 0x2925FDD7, 0x6BF4, 0x4674, 0x9FC8B703DE318D56LL)


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_HANDLE_H__2925FDD7_6BF4_4674_9FC8_B703DE318D56
