/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-08-30
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOG_PENDING_VALUE_POOL_H__C5EF1263_66C7_466B_B766_B526AB91103C
#define LOG_PENDING_VALUE_POOL_H__C5EF1263_66C7_466B_B766_B526AB91103C


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <nsfx/log/log-value.h>
#include <nsfx/log/make-log-value.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The pool of pending log values.
 * @internal
 */
class LogPendingValuePool
{
    typedef unordered_map<std::string, LogValue>  ContainerType;

public:
    bool Add(const std::string& name, LogValue value);
    void Update(const std::string& name, LogValue value);
    void Remove(const std::string& name);

    void Apply(LogRecord& record);

private:
    /**
     * @brief Normalize a log value.
     *
     * A first-order log value will be made a second-order log value.
     * A second-order or higher-order log value is unchanged.
     */
    LogValue NormalizeLogValue(LogValue value);

private:
    // The pending log values.
    ContainerType  values_;

};


////////////////////////////////////////////////////////////////////////////////
inline bool LogPendingValuePool::Add(const std::string& name, LogValue value)
{
    value = NormalizeLogValue(value);
    auto result = values_.emplace(name, value);
    return result.second;
}

inline void LogPendingValuePool::Update(const std::string& name, LogValue value)
{
    value = NormalizeLogValue(value);
    auto result = values_.emplace(name, value);
    if (!result.second)
    {
        auto& it = result.first;
        it->second = value;
    }
}

inline void LogPendingValuePool::Remove(const std::string& name)
{
    values_.erase(name);
}

inline void LogPendingValuePool::Apply(LogRecord& record)
{
    for (auto it = values_.cbegin(); it != values_.cend(); ++it)
    {
        record.Add(it->first, it->second.Get<LogValue>());
    }
}

inline LogValue LogPendingValuePool::NormalizeLogValue(LogValue value)
{
    if (value.GetTypeId() != boost::typeindex::type_id<LogValue>())
    {
        value = MakeConstantLogValue<LogValue>(value);
    }
    return value;
}


NSFX_CLOSE_NAMESPACE


#endif // LOG_PENDING_VALUE_POOL_H__C5EF1263_66C7_466B_B766_B526AB91103C

