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

#ifndef LOGGER_H__4E8BB7A8_40CB_4373_BE91_83AF5EF0DA7C
#define LOGGER_H__4E8BB7A8_40CB_4373_BE91_83AF5EF0DA7C


#include <nsfx/log/config.h>
#include <nsfx/log/record.h>
#include <nsfx/log/attribute.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log source interface.
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
class Logger
{
    virtual ~Logger(void) BOOST_NOEXCEPT {}

    virtual Record CreateRecord(void) = 0;
    virtual void CommitRecord(Record&& record) = 0;
    virtual bool AddAttribute(const std::string& name,
                              const Attribute& attribute) = 0;
}; // class Logger


NSFX_LOG_CLOSE_NAMESPACE


#endif // LOGGER_H__4E8BB7A8_40CB_4373_BE91_83AF5EF0DA7C

