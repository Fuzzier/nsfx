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

#ifndef LOG_SOURCE_POOL_H__F62A7BFD_476B_4008_BBEF_12AD48C6EC64
#define LOG_SOURCE_POOL_H__F62A7BFD_476B_4008_BBEF_12AD48C6EC64


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <utility> // pair, make_pair


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The pool of registered log sources.
 *
 * This class is to support the <i>passive connection</i> strategy.
 *
 * @internal
 */
class LogSourcePool
{
    typedef std::pair<Ptr<ILogEvent>, cookie_t>  ElemType;
    typedef vector<ElemType>  ContainerType;

public:
    cookie_t Register(Ptr<ILogEvent> source);
    void Unregister(cookie_t cookie);
    void UnregisterAll(void);

    /**
     * @brief Connect a log sink to all registered log sources.
     */
    void Connect(Ptr<ILogEventSink> sink);

    /**
     * @brief Disconnect from all registered log sources.
     */
    void Disconnect(void);

private:
    // The registered log sources.
    ContainerType  items_;

};


////////////////////////////////////////////////////////////////////////////////
inline cookie_t LogSourcePool::Register(Ptr<ILogEvent> source)
{
    cookie_t cookie = 0;
    if (!!source)
    {
        ++cookie;
        auto it = items_.begin();
        while (it != items_.end())
        {
            // If the log source is nullptr.
            if (!it->first)
            {
                // The cookie must be 0.
                BOOST_ASSERT(!it->second);
                break;
            }
            ++cookie;
            ++it;
        }
        if (it == items_.end())
        {
            items_.emplace_back(std::move(source), 0);
        }
        else
        {
            it->first = std::move(source);
        }
    }
    return cookie;
}

inline void LogSourcePool::Unregister(cookie_t cookie)
{
    BOOST_ASSERT(cookie <= items_.size());
    if (cookie > 0)
    {
        --cookie;
        ElemType& e = items_[cookie];
        // If the cookie is non-zero.
        if (e.second)
        {
            // Disconnect from the source.
            e.first->Disconnect(e.second);
        }
        e.first  = nullptr;
        e.second = 0;
    }
}

inline void LogSourcePool::UnregisterAll(void)
{
    Disconnect();
    items_.clear();
}

inline void LogSourcePool::Connect(Ptr<ILogEventSink> sink)
{
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        // If the source is not nullptr.
        if (!!it->first)
        {
            // Connect the sink.
            it->second = it->first->Connect(sink);
        }
    }
}

inline void LogSourcePool::Disconnect(void)
{
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        // If the source is not nullptr.
        if (!!it->first)
        {
            // Disconnect the sink.
            it->first->Disconnect(it->second);
            // Clear the cookie.
            it->second = 0;
        }
    }
}


NSFX_CLOSE_NAMESPACE


#endif // LOG_SOURCE_POOL_H__F62A7BFD_476B_4008_BBEF_12AD48C6EC64

