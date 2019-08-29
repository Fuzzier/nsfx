/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-09
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_LOG_FILTER_H__D9ED2E1F_9E78_4704_B7A2_3969296F919E
#define I_LOG_FILTER_H__D9ED2E1F_9E78_4704_B7A2_3969296F919E


#include <nsfx/log/config.h>
#include <nsfx/log/log-record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The decision of a log filter.
 */
enum LogFilterDecision
{
    /**
     * @brief Discard the log record.
     */
    LOG_DECLINE = 0x00000000,

    /**
     * @brief Keep processing the log record.
     */
    LOG_ACCEPT  = 0x00000001
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The log filter interface.
 */
class ILogFilter :
    virtual public IObject
{
public:
    virtual ~ILogFilter(void) BOOST_NOEXCEPT {}

    /**
     * @brief Filter a log record.
     */
    virtual LogFilterDecision Decide(const LogRecord& record) = 0;
};


NSFX_DEFINE_CLASS_UID(ILogFilter, "edu.uestc.nsfx.ILogFilter");


NSFX_CLOSE_NAMESPACE


#endif // I_LOG_FILTER_H__D9ED2E1F_9E78_4704_B7A2_3969296F919E

