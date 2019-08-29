/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-17
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOG_STREAM_SINK_H__CE0B27F7_A742_4F2B_8120_9BB3835E148A
#define LOG_STREAM_SINK_H__CE0B27F7_A742_4F2B_8120_9BB3835E148A


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <nsfx/log/i-log-stream-sink.h>
#include <nsfx/log/i-log-formatter.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The default log sink.
 *
 * # Interfaces
 * * Uses
 *   + \c ILogFormatter
 * * Provides
 *   + \c ILogStreamSink
 * * Events
 *   + \c ILogEventSink
 */
class LogStreamSink :
    public ILogFormatterUser,
    public ILogStreamSink
{
    typedef LogStreamSink  ThisClass;

public:
    LogStreamSink(void);
    virtual ~LogStreamSink(void) {}

public:
    // ILogFormatterUser
    virtual void Use(Ptr<ILogFormatter> formatter) NSFX_OVERRIDE;

    // ILogStreamSink
    virtual void Fire(LogRecord record) NSFX_OVERRIDE;

    virtual bool AddValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void UpdateValue(const std::string& name, LogValue value) NSFX_OVERRIDE;
    virtual void RemoveValue(const std::string& name) NSFX_OVERRIDE;

    virtual void SetFilter(Ptr<ILogFilter> filter) NSFX_OVERRIDE;

    virtual void AddStream(std::ostream& os) NSFX_OVERRIDE;
    virtual void AddFile(const std::string& filename,
                         std::ios_base::openmode mode) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogFormatterUser)
        NSFX_INTERFACE_ENTRY(ILogEventSink)
        NSFX_INTERFACE_ENTRY(ILogStreamSink)
    NSFX_INTERFACE_MAP_END()

private:
    // The log formatter.
    Ptr<ILogFormatter>  formatter_;

    // The pending log values.
    unordered_map<std::string, LogValue>  values_;

    // The log filter.
    Ptr<ILogFilter> filter_;

    // The output streams.
    vector<std::ostream*>  ostreams_;
    vector<std::ofstream>  files_;
};

NSFX_REGISTER_CLASS(LogStreamSink, "edu.uestc.nsfx.LogStreamSink");


////////////////////////////////////////////////////////////////////////////////
inline LogStreamSink::LogStreamSink(void)
{
}

inline void LogStreamSink::Use(Ptr<ILogFormatter> formatter)
{
    if (!formatter)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    formatter_ = formatter;
}

inline void LogStreamSink::Fire(LogRecord record)
{
    if (!formatter_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
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
        for (auto it = ostreams_.cbegin(); it != ostreams_.cend(); ++it)
        {
            std::ostream* os = *it;
            formatter_->Format(*os, record);
        }
    }
    while (false);
}

inline bool LogStreamSink::AddValue(const std::string& name, LogValue value)
{
    auto result = values_.emplace(name, std::move(value));
    return result.second;
}

inline void LogStreamSink::UpdateValue(const std::string& name, LogValue value)
{
    auto result = values_.emplace(name, value);
    if (!result.second)
    {
        auto& it = result.first;
        it->second = std::move(value);
    }
}

inline void LogStreamSink::RemoveValue(const std::string& name)
{
    values_.erase(name);
}

inline void LogStreamSink::SetFilter(Ptr<ILogFilter> filter)
{
    filter_ = std::move(filter);
}

inline void LogStreamSink::AddStream(std::ostream& os)
{
    if (!os)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("Invalid output stream."));
    }
    ostreams_.push_back(&os);
}

inline void LogStreamSink::AddFile(const std::string& filename,
                                   std::ios_base::openmode mode)
{
    std::ofstream ofs(filename, mode);
    if (!ofs)
    {
        BOOST_THROW_EXCEPTION(
            Unexpected() <<
            ErrorMessage("Cannot create log file."));
    }
    files_.push_back(std::move(ofs));
    ostreams_.push_back(&files_.back());
}


NSFX_CLOSE_NAMESPACE


#endif // LOG_STREAM_SINK_H__CE0B27F7_A742_4F2B_8120_9BB3835E148A

