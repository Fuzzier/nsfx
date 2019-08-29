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
 *   + \c ILogPassiveConnection
 * * Events
 *   + \c ILoggerEvent
 *   + \c ILoggerEventSink
 *   + \c ILoggerEventSinkEx
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

    virtual void RegisterSource(Ptr<ILogEvent> source) NSFX_OVERRIDE;
    virtual void UnregisterAllSources(void) NSFX_OVERRIDE;
    virtual bool IsEnabled(void) NSFX_OVERRIDE;

    virtual bool AddValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void UpdateValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void RemoveValue(const std::string& name) NSFX_OVERRIDE;

    virtual void SetFilter(Ptr<ILogFilter> filter) NSFX_OVERRIDE;

private:
    /**
     * @brief Connect this logger to all registered log sources.
     */
    void ConnectToAllSources(void);

    /**
     * @brief Disconnect this logger from all registered log sources.
     */
    void DisconnectFromAllSources(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogEvent)
        NSFX_INTERFACE_ENTRY(ILogEventSink)
        NSFX_INTERFACE_ENTRY(ILogEventSinkEx)
    NSFX_INTERFACE_MAP_END()

private:
    // The registered log sources.
    typedef std::pair<Ptr<ILogEvent>, cookie_t>  ElemType;
    vector<ElemType>  sources_;

    // The pending log values.
    unordered_map<std::string, LogValue>  values_;

    // The log filter.
    Ptr<ILogFilter> filter_;

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
        ConnectToAllSources();
    }
    return logEvent_.GetImpl()->EventType::Connect(std::move(sink));
}

inline void Logger::Disconnect(cookie_t cookie)
{
    logEvent_.GetImpl()->EventType::Disconnect(cookie);
    if (!logEvent_.GetImpl()->EventType::GetNumSinks())
    {
        DisconnectFromAllSources();
    }
}

inline void Logger::Fire(LogRecord record)
{
    do
    {
        for (auto it = values_.cbegin(); it != values_.cend(); ++it)
        {
            record.Add(it->first, it->second);
        }
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

inline void Logger::RegisterSource(Ptr<ILogEvent> source)
{
    if (!!source)
    {
        sources_.push_back(std::make_pair(std::move(source), 0));
    }
}

inline void Logger::UnregisterAllSources(void)
{
    DisconnectFromAllSources();
    sources_.clear();
}

inline bool Logger::IsEnabled(void)
{
    return !!logEvent_.GetImpl()->EventType::GetNumSinks();
}

inline bool Logger::AddValue(const std::string& name, LogValue value)
{
    auto result = values_.emplace(name, std::move(value));
    return result.second;
}

inline void Logger::UpdateValue(const std::string& name, LogValue value)
{
    auto result = values_.emplace(name, value);
    if (!result.second)
    {
        auto& it = result.first;
        it->second = std::move(value);
    }
}

inline void Logger::RemoveValue(const std::string& name)
{
    values_.erase(name);
}

inline void Logger::SetFilter(Ptr<ILogFilter> filter)
{
    filter_ = std::move(filter);
}

inline void Logger::ConnectToAllSources(void)
{
    for (auto it = sources_.begin(); it != sources_.end(); ++it)
    {
        it->second = it->first->Connect(static_cast<ILogEventSink*>(this));
    }
}

inline void Logger::DisconnectFromAllSources(void)
{
    for (auto it = sources_.begin(); it != sources_.end(); ++it)
    {
        if (it->second)
        {
            it->first->Disconnect(it->second);
            it->second = 0;
        }
    }
}


NSFX_CLOSE_NAMESPACE


#endif // LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B

