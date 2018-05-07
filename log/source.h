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

#ifndef SOURCE_H__2344A004_4493_444E_9B9A_D81C4C91AF96
#define SOURCE_H__2344A004_4493_444E_9B9A_D81C4C91AF96


#include <nsfx/log/config.h>
#include <nsfx/log/record.h>
#include <nsfx/log/attribute.h>
#include <nsfx/log/attribute-collection.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The default log source.
 *
 * A logger object possesses a set of attributes, when a record is created, the
 * attributes are used to generate attribute values. If multiple threads share
 * a logger object, the attributes must be locked before accessed.
 *
 * The logger generates log attribute values which are carried by a log record.
 * The log record is passed to log core.
 * The log core passes the log record to log sinks.
 * The log sink formats and outputs the log attribute values.
 */
class Source :
    public ILog
{
    typedef Source  ThisClass;

public:
    Source(void);

    virtual ~Source(void) {}

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILog)
        NSFX_INTERFACE_AGGREGATED_ENTRY(ILogEvent, &event_)
        NSFX_INTERFACE_ENTRY(IAttributeCollection, &attributeCollection_)
    NSFX_INTERFACE_MAP_END()

    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;

private:
    MemberAggObject<Event<ILogEvent> >    event_;
    MemberAggObject<AttributeCollection>  attributeCollection_;
};


////////////////////////////////////////////////////////////////////////////////
inline Source::Source(void) :
    event_(/* controller = */this)
{
}

inline void Source::Fire(const std::shared_ptr<Record>& record)
{
    record->Add()

    event_.GetImpl()->Visit([&] (ILog* log) {
        log->Fire(record);
    });
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // SOURCE_H__2344A004_4493_444E_9B9A_D81C4C91AF96

