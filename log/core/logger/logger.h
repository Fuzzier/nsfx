/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
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
#include <nsfx/log/core/logger/i-logger.h>
#include <nsfx/log/core/attribute/attribute-set.h>
#include <nsfx/log/core/filter/filter-chain.h>
#include <nsfx/component/class-registry.h>
#include <nsfx/event/event.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The default logger.
 *
 * It is a single-threaded logger.
 *
 * ## Interfaces
 * ### Uses
 * * IFilter
 * ### Provides
 * * ILogger
 * * IAttributeSet
 * * IFilterChain
 * ### Events
 * * ILoggerEvent
 */
class Logger :
    public ILogger
{
    typedef Logger  ThisClass;

public:
    Logger(void);

    virtual ~Logger(void) {}

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogger)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAttributeSet, &attributeSet_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IFilterChain,  &filterChain_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ILoggerEvent,  &loggerEvent_)
    NSFX_INTERFACE_MAP_END()

    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;

private:
    void ApplyAttributes(const std::shared_ptr<Record>& record);
    void FilterAndFire(const std::shared_ptr<Record>& record);

private:
    MemberAggObject<AttributeSet>  attributeSet_;
    MemberAggObject<FilterChain>   filterChain_;
    MemberAggObject<Event<ILoggerEvent> >  loggerEvent_;
};


NSFX_REGISTER_CLASS(Logger, "edu.uestc.nsfx.log.Logger");


////////////////////////////////////////////////////////////////////////////////
inline Logger::Logger(void) :
    attributeSet_(/* constroller = */this),
    filterChain_(/* constroller = */this),
    loggerEvent_(/* controller = */this)
{
}

inline void Logger::Fire(const std::shared_ptr<Record>& record)
{
    ApplyAttributes(record);
    FilterAndFire(record);
}

inline void Logger::ApplyAttributes(const std::shared_ptr<Record>& record)
{
    attributeSet_.GetImpl()->Visit(
        [&] (const std::string& name, const Attribute& attribute) {
            // The existing named values are not replaced.
            record->Add(name, attribute.GetValue());
        });
}

inline void Logger::FilterAndFire(const std::shared_ptr<Record>& record)
{
    if (filterChain_.GetImpl()->Decide(record) == ACCEPT)
    {
        loggerEvent_.GetImpl()->Visit([&] (ILogger* logger) {
            logger->Fire(record);
        });
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B

