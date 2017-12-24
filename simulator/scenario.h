/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef SCENARIO_H__863373C3_BF04_4732_A672_7A3138BFF839
#define SCENARIO_H__863373C3_BF04_4732_A672_7A3138BFF839


#include <nsfx/component/config.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/exception.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/simulator/i-alarm.h>
#include <nsfx/simulator/i-event-scheduler.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Scenario.
/**
 * @ingroup Simulator
 * @brief The scenario.
 *
 * A scenario provides various resources shared by nodes.<br/>
 *
 * A scenario exposes the following interfaces:<br/>
 * \c IClock, \c IAlarm and \c IEventScheduler.
 */
class Scenario :
    virtual public IObject
{
public:
    virtual ~Scenario(void) BOOST_NOEXCEPT {}

private:
    Ptr<IClock> clock_;
    Ptr<IAlarm> alarm_;
    Ptr<IEventScheduler> scheduler_;
};


NSFX_DEFINE_CALSS_UUID4(Scenario, 0x863373C3, 0xBF04, 0x4732, 0xA6727A3138BFF839LL)


NSFX_CLOSE_NAMESPACE


#endif // SCENARIO_H__863373C3_BF04_4732_A672_7A3138BFF839

