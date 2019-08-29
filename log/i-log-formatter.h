/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
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

#ifndef I_STREAM_FORMATTER_H__8A28811F_C7A9_4360_8B10_BA64463D39B8
#define I_STREAM_FORMATTER_H__8A28811F_C7A9_4360_8B10_BA64463D39B8


#include <nsfx/log/config.h>
#include <nsfx/log/log-record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/i-user.h>
#include <ostream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The log formatter interface.
 */
class ILogFormatter :
    virtual public IObject
{
public:
    virtual ~ILogFormatter(void) BOOST_NOEXCEPT {}

    /**
     * @brief Format and output a log record to an output stream.
     */
    virtual void Format(std::ostream& os, const LogRecord& record) = 0;
};

NSFX_DEFINE_CLASS_UID(ILogFormatter, "edu.uestc.nsfx.ILogFormatter");


////////////////////////////////////////
/**
 * @ingroup Log
 * @brief User of log formatter.
 * @class ILogFormatter
 *
 * Prototype:
 *
 *     void Use(Ptr<ILogFormatter> formatter);
 *
 * @see \c ILogFormatter
 */
NSFX_DEFINE_USER_INTERFACE(
    ILogFormatterUser, "edu.uestc.nsfx.ILogFormatterUser",
    ILogFormatter);


NSFX_CLOSE_NAMESPACE


#endif // I_STREAM_FORMATTER_H__8A28811F_C7A9_4360_8B10_BA64463D39B8

