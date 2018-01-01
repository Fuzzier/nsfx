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


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Simulator
 * @brief Simulator event type.
 */
enum SimulatorEventType
{
    NSFX_SIMULATOR_EVENT_BEGIN = 1,
    NSFX_SIMULATOR_EVENT_RUN   = 2,
    NSFX_SIMULATOR_EVENT_PAUSE = 4,
    NSFX_SIMULATOR_EVENT_END   = 8,
    NSFX_SIMULATOR_EVENT_ALL   = 0x0F
};


////////////////////////////////////////////////////////////////////////////////
// ISimulatorSink.
/**
 * @ingroup Simulator
 * @brief The simulator sink interface.
 */
class ISimulatorSink :
    virtual public IObject
{
public:
    virtual ~ISimulatorSink(void) BOOST_NOEXCEPT {}

    virtual void OnSimulatorEvent(SimulatorEventType type) = 0;

}; // class ISimulatorSink


NSFX_DEFINE_CLASS_UUID4(ISimulatorSink, 0x0E1B1E22, 0x33BF, 0x42D8, 0x8F5BF4FFE3536CE9LL);


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
     * @brief Connect to a sink.
     *
     * @param[in] sink The simulation sink.
     * @param[in] mask The event types listened by the sink.
     *
     * @return  Return a cookie that can be used to disconnect the sink.
     *
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw InvalidArgument The mask is \c 0.
     *
     * @see \c SimulatorEventType.
     */
    virtual cookie_t Connect(Ptr<ISimulatorSink> sink,
                             SimulatorEventType mask) = 0;

    /**
     * @brief Disconnect a sink.
     *
     * @param[in] cookie  The cookie obtain at connection.
     *
     * All sinks are automatically disconnected when the simulation ends.
     *
     * @see \c ISimulatorSink.
     */
    virtual void Disconnect(cookie_t cookie) BOOST_NOEXCEPT = 0;

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


NSFX_DEFINE_CLASS_UUID4(ISimulator, 0xC079AC9A, 0x0F83, 0x48F4, 0x82F354924DBBA46CLL);


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


NSFX_DEFINE_CLASS_UUID4(ISimulatorUser, 0xF4055358, 0x7910, 0x4982, 0x89BA0839D539482FLL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_ISimulatorSink  NSFX_UUID_OF(::nsfx::ISimulatorSink)
#define NSFX_IID_ISimulator      NSFX_UUID_OF(::nsfx::ISimulator)
#define NSFX_IID_ISimulatorUser  NSFX_UUID_OF(::nsfx::ISimulatorUser)


#endif // I_SIMULATOR_H__A3B18FB8_B2C5_45A6_A822_9C4A6AC94D52

