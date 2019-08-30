/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_LOG_H__AF6043F0_0AAF_4A8B_8E05_4E94FB858779
#define I_LOG_H__AF6043F0_0AAF_4A8B_8E05_4E94FB858779


#include <nsfx/log/config.h>
#include <nsfx/log/log-record.h>
#include <nsfx/log/i-log-filter.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/event/i-event.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ILogEvent;
class ILogEventSink;


////////////////////////////////////////////////////////////////////////////////
// Events.
/**
 * @ingroup Log
 * @brief The log event sink.
 * @class ILogEventSink
 *
 * Prototype:
 *
 *     void Fire(LogRecord record);
 *
 * @see ILogEvent
 */
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ILogEventSink, "edu.uestc.nsfx.ILogEventSink",
    ( void(LogRecord record) ));


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The log event.
 * @class ILogEvent
 *
 * Prototype:
 *
 *     void Fire(LogRecord record);
 *
 * @see ILogEventSink
 */
NSFX_DEFINE_EVENT_INTERFACE(
    ILogEvent, "edu.uestc.nsfx.ILogEvent",
    ILogEventSink);


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The extented log event sink.
 *
 * It extends \c ILogEventSink, and provides an extra method \c IsEnabled().
 * The log macros use this interface, so they can work more efficiently.
 *
 * @see \c ILogEventSink
 */
class ILogEventSinkEx :
    public ILogEventSink
{
public:
    virtual ~ILogEventSinkEx(void) BOOST_NOEXCEPT {}

    // ILogEventSink
    virtual void Fire(LogRecord record) NSFX_OVERRIDE = 0;

    // Passive connection.
    /**
     * @brief Register an upstream log source.
     *
     * @param[in] source The log source.
     *
     * @return A cookie value that can be used to unregister the log source.
     */
    virtual cookie_t RegisterSource(Ptr<ILogEvent> source) = 0;

    /**
     * @brief Unregister an upstream log source.
     *
     * @param[in] source The cookie value obtained when the log source is
     *                   registered.
     */
    virtual void UnregisterSource(cookie_t cookie) = 0;

    /**
     * @brief Unregister all upstream log sources.
     */
    virtual void UnregisterAllSources(void) = 0;

    /**
     * @brief Get the state of the log sink.
     *
     * It shall be determined by the number of downstream log sinks.
     * If there are no downstream log sinks, the logger is disabled.
     */
    virtual bool IsEnabled(void) = 0;

    // Pending log value.
    /**
     * @brief Add a pending log value.
     *
     * The pending log value will be added to each received log record.
     *
     * @param[in] name  The name of the log value.
     * @param[in] value The log value.
     *
     * @return If there's already a value with the same name,
     *         this function returns \c false.
     */
    virtual bool AddValue(const std::string& name, LogValue value) = 0;

    /**
     * @brief Add or udpate  a pending log value.
     *
     * @param[in] name  The name of the log value.
     * @param[in] value The log value.
     */
    virtual void UpdateValue(const std::string& name, LogValue value) = 0;

    /**
     * @brief Remove a pending log value.
     *
     * @param[in] name The name of the log value.
     */
    virtual void RemoveValue(const std::string& name) = 0;

    // Log filter.
    /**
     * @brief Set a log filter.
     *
     * @param[in] filter Set a log filter.
     *                   If \c nullptr is specified, the filter is removed.
     */
    virtual void SetFilter(Ptr<ILogFilter> filter) = 0;

};

NSFX_DEFINE_CLASS_UID(ILogEventSinkEx, "edu.uestc.nsfx.ILogEventSinkEx");


////////////////////////////////////////////////////////////////////////////////
namespace aux {

///////////////////////////////////////////////////////////
// Tools to distinguish ILogEventSinkEx from ILogEventSink.
///////////////////////////////////////////////////////////
struct LogSinkTag   {};
struct LogSinkExTag {};

////////////////////////////////////////
template<class Sink>
struct MakeLogSinkTag
{
    typedef LogSinkTag type;
};

template<>
struct MakeLogSinkTag<Ptr<ILogEventSinkEx>>
{
    typedef LogSinkExTag type;
};

template<>
struct MakeLogSinkTag<ILogEventSinkEx*>
{
    typedef LogSinkExTag type;
};

////////////////////////////////////////
template<class Sink>
inline bool IsLogSinkEnabled(Sink& sink, LogSinkTag)
{
    return true;
}

template<class Sink>
inline bool IsLogSinkEnabled(Sink& sink, LogSinkExTag)
{
    return sink->IsEnabled();
}

} /* namespace aux */


////////////////////////////////////////
template<class Sink>
inline bool IsLogSinkEnabled(Sink& sink)
{
    typedef aux::MakeLogSinkTag<Sink>::type  Tag;
    return aux::IsLogSinkEnabled(sink, Tag());
}


NSFX_CLOSE_NAMESPACE


#endif // I_LOG_H__AF6043F0_0AAF_4A8B_8E05_4E94FB858779

