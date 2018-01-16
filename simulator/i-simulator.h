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

#ifndef I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52
#define I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/event/i-event.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ISimulator;
class ISimulatorUser;

class ISimulationBeginEvent;
class ISimulationRunEvent;
class ISimulationPauseEvent;
class ISimulationEndEvent;

class ISimulationBeginEventSink;
class ISimulationRunEventSink;
class ISimulationPauseEventSink;
class ISimulationEndEventSink;

#define NSFX_IID_ISimulator                 NSFX_UUID_OF(::nsfx::ISimulator)
#define NSFX_IID_ISimulatorUser             NSFX_UUID_OF(::nsfx::ISimulatorUser)

#define NSFX_IID_ISimulationBeginEvent      NSFX_UUID_OF(::nsfx::ISimulationBeginEvent)
#define NSFX_IID_ISimulationRunEvent        NSFX_UUID_OF(::nsfx::ISimulationRunEvent)
#define NSFX_IID_ISimulationPauseEvent      NSFX_UUID_OF(::nsfx::ISimulationPauseEvent)
#define NSFX_IID_ISimulationEndEvent        NSFX_UUID_OF(::nsfx::ISimulationEndEvent)

#define NSFX_IID_ISimulationBeginEventSink  NSFX_UUID_OF(::nsfx::ISimulationBeginEventSink)
#define NSFX_IID_ISimulationRunEventSink    NSFX_UUID_OF(::nsfx::ISimulationRunEventSink)
#define NSFX_IID_ISimulationPauseEventSink  NSFX_UUID_OF(::nsfx::ISimulationPauseEventSink)
#define NSFX_IID_ISimulationEndEventSink    NSFX_UUID_OF(::nsfx::ISimulationEndEventSink)


////////////////////////////////////////////////////////////////////////////////
// ISimulator.
/**
 * @ingroup Simulator
 * @brief The simulator interface.
 */
class ISimulator :/*{{{*/
    virtual public IObject
{
public:
    virtual ~ISimulator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Run until there is no scheduled alarm.
     */
    virtual void Run(void) = 0;

    /**
     * @brief Run until a specific time point.
     */
    virtual void RunUntil(const TimePoint& t) = 0;

    /**
     * @brief Run for a specific duration.
     */
    virtual void RunFor(const Duration& dt) = 0;

}; // class ISimulator /*}}}*/


NSFX_DEFINE_CLASS_UUID(ISimulator, 0xC079AC9A, 0x0F83, 0x48F4, 0x82F354924DBBA46CLL);


////////////////////////////////////////////////////////////////////////////////
// Simulator events.
NSFX_DEFINE_EVENT_SINK_INTERFACE(ISimulationBeginEventSink, ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(ISimulationRunEventSink,   ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(ISimulationPauseEventSink, ( void(void) ));
NSFX_DEFINE_EVENT_SINK_INTERFACE(ISimulationEndEventSink,   ( void(void) ));

NSFX_DEFINE_CLASS_UUID(ISimulationBeginEventSink, 0x1E93735B, 0xDE24, 0x49A3, 0xA63CB63B3F4D93E5LL);
NSFX_DEFINE_CLASS_UUID(ISimulationRunEventSink,   0x7B38A8CA, 0x8876, 0x4610, 0x8B8DA51254028C58LL);
NSFX_DEFINE_CLASS_UUID(ISimulationPauseEventSink, 0x1811B62D, 0x42D4, 0x4F1B, 0x98CE397D524F912FLL);
NSFX_DEFINE_CLASS_UUID(ISimulationEndEventSink,   0xF6A303AC, 0x6088, 0x4EAE, 0xA532C6CFB34F2E8BLL);

NSFX_DEFINE_EVENT_INTERFACE(ISimulationBeginEvent, ISimulationBeginEventSink);
NSFX_DEFINE_EVENT_INTERFACE(ISimulationRunEvent,   ISimulationRunEventSink);
NSFX_DEFINE_EVENT_INTERFACE(ISimulationPauseEvent, ISimulationPauseEventSink);
NSFX_DEFINE_EVENT_INTERFACE(ISimulationEndEvent,   ISimulationEndEventSink);

NSFX_DEFINE_CLASS_UUID(ISimulationBeginEvent, 0x6B0AC801, 0x9746, 0x4E4E, 0xAE347E957F7899F2LL);
NSFX_DEFINE_CLASS_UUID(ISimulationRunEvent,   0x3551885D, 0xB839, 0x4278, 0xB49CAF9CDFBEFA95LL);
NSFX_DEFINE_CLASS_UUID(ISimulationPauseEvent, 0x6B886CF2, 0x0FF3, 0x47E2, 0xBAFEC75501EA01BBLL);
NSFX_DEFINE_CLASS_UUID(ISimulationEndEvent,   0x3CA80256, 0xBF64, 0x4D1A, 0x8106B3F3C0C2B912LL);


////////////////////////////////////////////////////////////////////////////////
// ISimulatorUser.
/**
 * @ingroup Simulator
 * @brief The simulator user interface.
 */
class ISimulatorUser :/*{{{*/
    virtual public IObject
{
public:
    virtual ~ISimulatorUser(void) BOOST_NOEXCEPT {}

    virtual void UseSimulator(Ptr<ISimulator> simulator) = 0;

}; // class ISimulatorUser /*}}}*/


NSFX_DEFINE_CLASS_UUID(ISimulatorUser, 0xF4055358, 0x7910, 0x4982, 0x89BA0839D539482FLL);


NSFX_CLOSE_NAMESPACE


#endif // I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52

