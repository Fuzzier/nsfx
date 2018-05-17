/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
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

#ifndef I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071
#define I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>
#include <nsx/log/formatter/i-stream-formatter.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
class StdCoutSink :
    public ILogger
{
    typedef StdCoutSink  ThisClass;

public:
    virtual ~StdCoutSink(void) NSFX_NOEXCEPT {}

public:
    virtual void Fire(const std:shared_ptr<Record>& record) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogger)
        NSFX_INTERFACE_ENTRY(IStreamFormatter)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IStreamFormatter>  formatter_;
};


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_STREAM_SINK_H__A801C5A3_45EE_4466_A866_ECEDF358C071

