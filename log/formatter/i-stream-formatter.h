/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
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
#include <nsfx/log/record/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <ostream>
#include <memory> // shared_ptr


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The text log formatter interface.
 */
class IStreamFormatter :
    virtual public IObject
{
public:
    virtual ~IStreamFormatter(void) BOOST_NOEXCEPT {}

    /**
     * @brief Format a log record.
     */
    virtual void Format(std::ostream& os,
                        const std::shared_ptr<Record>& record) = 0;
};


NSFX_DEFINE_CLASS_UID(IStreamFormatter, "edu.uestc.nsfx.log.IStreamFormatter");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_STREAM_FORMATTER_H__8A28811F_C7A9_4360_8B10_BA64463D39B8

