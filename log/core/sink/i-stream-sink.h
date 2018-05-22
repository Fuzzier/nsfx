/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-17
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071
#define I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071


#include <nsfx/log/config.h>
#include <nsfx/log/core/logger/i-logger.h>
#include <ostream>
#include <memory> // shared_ptr


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The stream sink interface.
 */
class IStreamSink :
    public ILogger
{
public:
    virtual ~IStreamSink(void) BOOST_NOEXCEPT {}

public:
    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE = 0;

    virtual void SetStream(std::ostream* os) = 0;
};


NSFX_DEFINE_CLASS_UID(IStreamSink, "edu.uestc.nsfx.log.IStreamSink");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071

