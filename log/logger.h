/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B
#define LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <nsfx/log/detail/log-source-pool.h>
#include <nsfx/log/detail/log-pending-value-pool.h>
#include <nsfx/event/event.h>
#include <nsfx/component/class-registry.h>
#include <utility> // pair, make_pair


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The default logger.
 *
 * It is a single-threaded logger.
 * It processes the log records, and forwards the log records to other log
 * event sinks.
 *
 * # Interfaces
 * * Uses
 * * Provides
 *   + `ILogPassiveConnection`
 * * Events
 *   + `ILoggerEvent`
 *   + `ILoggerEventSink`
 *   + `ILoggerEventSinkEx`
 *
 * @remarks At most **one** log sink can be connected to this logger.
 *
 */
class Logger :
    public ILogEvent,
    public ILogEventSinkEx
{
    typedef Logger  ThisClass;

public:
    Logger(void);
    virtual ~Logger(void) {}

    // ILogEvent
    virtual cookie_t Connect(Ptr<ILogEventSink> sink) NSFX_OVERRIDE;
    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE;

    // ILogEventSinkEx
    virtual void Fire(LogRecord record) NSFX_OVERRIDE;

    virtual cookie_t RegisterSource(Ptr<ILogEvent> source) NSFX_OVERRIDE;
    virtual void UnregisterSource(cookie_t cookie) NSFX_OVERRIDE;

    virtual bool IsEnabled(void) NSFX_OVERRIDE;

    virtual bool AddValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void UpdateValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void RemoveValue(const std::string& name) NSFX_OVERRIDE;

    virtual void SetFilter(Ptr<ILogFilter> filter) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogEvent)
        NSFX_INTERFACE_ENTRY(ILogEventSink)
        NSFX_INTERFACE_ENTRY(ILogEventSinkEx)
    NSFX_INTERFACE_MAP_END()

private:
    // The registered log sources.
    LogSourcePool  sourcePool_;

    // The pending log values.
    LogPendingValuePool  pendingValuePool_;

    // The log filter.
    Ptr<ILogFilter>  filter_;

    // ILogEvent.
    typedef Event<ILogEvent>   EventType;
    MemberAggObject<EventType> logEvent_;
};

NSFX_REGISTER_CLASS(Logger, "edu.uestc.nsfx.Logger");


////////////////////////////////////////////////////////////////////////////////
inline Logger::Logger(void) :
    logEvent_(/* controller = */this)
{
}

inline cookie_t Logger::Connect(Ptr<ILogEventSink> sink)
{
    if (!logEvent_.GetImpl()->EventType::GetNumSinks())
    {
        sourcePool_.Connect(static_cast<ILogEventSink*>(this));
    }
    return logEvent_.GetImpl()->EventType::Connect(std::move(sink));
}

inline void Logger::Disconnect(cookie_t cookie)
{
    logEvent_.GetImpl()->EventType::Disconnect(cookie);
    if (!logEvent_.GetImpl()->EventType::GetNumSinks())
    {
        sourcePool_.Disconnect();
    }
}

inline void Logger::Fire(LogRecord record)
{
    do
    {
        pendingValuePool_.Apply(record);
        if (!!filter_)
        {
            if (filter_->Decide(record) != LOG_ACCEPT)
            {
                break;
            }
        }
        logEvent_.GetImpl()->EventType::Fire(std::move(record));
    }
    while (false);
}

inline cookie_t Logger::RegisterSource(Ptr<ILogEvent> source)
{
    return sourcePool_.Register(std::move(source));
}

inline void Logger::UnregisterSource(cookie_t cookie)
{
    sourcePool_.Unregister(cookie);
}

inline bool Logger::IsEnabled(void)
{
    return !!logEvent_.GetImpl()->EventType::GetNumSinks();
}

inline bool Logger::AddValue(const std::string& name, LogValue value)
{
    return pendingValuePool_.Add(name, value);
}

inline void Logger::UpdateValue(const std::string& name, LogValue value)
{
    pendingValuePool_.Update(name, value);
}

inline void Logger::RemoveValue(const std::string& name)
{
    pendingValuePool_.Remove(name);
}

inline void Logger::SetFilter(Ptr<ILogFilter> filter)
{
    filter_ = std::move(filter);
}


NSFX_CLOSE_NAMESPACE


#endif // LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B

