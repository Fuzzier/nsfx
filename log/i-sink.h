/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842
#define I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842


#include <nsfx/log/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/log/record.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log interface.
 */
class ILog :
    virtual public IObject
{
public:
    virtual ~ILog(void) BOOST_NOEXCEPT {}

    virtual void Commit(const std::shared_ptr<Record>& record) = 0;
};


NSFX_DEFINE_CLASS_UID(ILog, "edu.uestc.nsfx.log.ILog");


////////////////////////////////////////////////////////////////////////////////
// Events.
NSFX_DEFINE_EVENT_SINK_INTERFACE(
    ILogEventSink, "edu.uestc..nsfx.log.ILogEventSink",
    ( void(const std::shared_ptr<Record>&) ));


NSFX_DEFINE_EVENT_INTERFACE(
    ILogEvent, "edu.uestc.nsfx.log.ILogEvent",
    ILogEventSink);


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842

