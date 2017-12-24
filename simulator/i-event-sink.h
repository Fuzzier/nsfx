/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-24
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_SINK_H__E1CAA0E1_032A_4BE3_92A8_E94CDD18FD5E
#define I_EVENT_SINK_H__E1CAA0E1_032A_4BE3_92A8_E94CDD18FD5E


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IEventSink.
/**
 * @ingroup Simulator
 * @brief The event sink interface.
 */
class IEventSink :
    virtual public IObject
{
public:
    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    virtual void Fire(void) = 0;

};


NSFX_DEFINE_CALSS_UUID4(IEventSink, 0xE1CAA0E1, 0x032A, 0x4BE3, 0x92A8E94CDD18FD5ELL)


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SINK_H__E1CAA0E1_032A_4BE3_92A8_E94CDD18FD5E

