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

#ifndef I_TEXT_FORMATTER_H__A0D24B36_B0C3_4C25_8D88_7CCB952614F3
#define I_TEXT_FORMATTER_H__A0D24B36_B0C3_4C25_8D88_7CCB952614F3


#include <nsfx/log/config.h>
#include <nsfx/log/record/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <memory> // shared_ptr
#include <string>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The text log formatter interface.
 */
class ITextFormatter :
    virtual public IObject
{
public:
    virtual ~ITextFormatter(void) BOOST_NOEXCEPT {}

    /**
     * @brief Format a log record.
     */
    virtual std::string Format(const std::shared_ptr<Record>& record) = 0;
};


NSFX_DEFINE_CLASS_UID(ITextFormatter, "edu.uestc.nsfx.log.ITextFormatter");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_TEXT_FORMATTER_H__A0D24B36_B0C3_4C25_8D88_7CCB952614F3

