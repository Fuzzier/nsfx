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
#include <nsfx/log/record.h>
#include <nsfx/log/attribute.h>
#include <nsfx/log/attribute-collection.h>
#include <nsfx/log/i-filter.h>
#include <nsfx/component/class-registry.h>


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
 * * ILog
 * * IAttributeCollection
 * ### Events
 * * ILogEvent
 */
class Logger :
    public ILog,
    public IFilterUser
{
    typedef Logger  ThisClass;

public:
    Logger(void);

    virtual ~Logger(void) {}

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILog)
        NSFX_INTERFACE_ENTRY(IFilterUser)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ILogEvent, &logEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAttributeCollection, &attributeCollection_)
    NSFX_INTERFACE_MAP_END()

    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;
    virtual void Use(const std::shared_ptr<IFilter>& filter) NSFX_OVERRIDE;

private:
    void ApplyAttributes(const std::shared_ptr<Record>& record);
    void FilterAndFire(const std::shared_ptr<Record>& record);

private:
    MemberAggObject<Event<ILogEvent> >    logEvent_;
    MemberAggObject<AttributeCollection>  attributeCollection_;
    std::shared_ptr<IFilter> filter_;
};


NSFX_REGISTER_CLASS(Logger, "edu.uestc.nsfx.log.Logger");


////////////////////////////////////////////////////////////////////////////////
inline Logger::Logger(void) :
    logEvent_(/* controller = */this)
{
}

inline void Use(const std::shared_ptr<IFilter>& filter)
{
    filter_ = filter;
}

inline void Logger::Fire(const std::shared_ptr<Record>& record)
{
    ApplyAttributes(record);
    FilterAndFire(record);
}

inline void Logger::ApplyAttributes(const std::shared_ptr<Record>& record)
{
    attributeCollection_.GetImpl()->Visit(
        [&] (const std::string& name, const Attribute& attribute) {
            // The existing named values are not replaced.
            record->Add(name, attribute->GetValue());
        });
}

inline void Logger::FilterAndFire(const std::shared_ptr<Record>& record)
{
    if (!!filter_)
    {
        if (filter_->Decide(record) == ACCEPT)
        {
            logEvent_.GetImpl()->Visit([&] (ILog* log) {
                log->Fire(record);
            });
        }
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // LOGGER_H__3DB24DDB_2072_41E8_9F81_3189137A238B

