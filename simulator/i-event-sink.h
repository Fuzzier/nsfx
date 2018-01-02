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

#ifndef I_EVENT_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46
#define I_EVENT_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class IEventSink;


#define NSFX_IID_IEventSink  NSFX_UUID_OF(::nsfx::IEventSink)


////////////////////////////////////////////////////////////////////////////////
// IEventSink.
/**
 * @ingroup Simulator
 * @brief The event sink interface.
 */
class IEventSink :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IEventSink(void) BOOST_NOEXCEPT {}

    virtual void OnEvent(void) = 0;

}; // class IEventSink /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IEventSink, 0x88630CC3, 0xFCE6, 0x452B, 0x825ABCA384FC8C89LL);


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46

