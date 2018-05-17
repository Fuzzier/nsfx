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

#ifndef STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC
#define STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
class StreamSink :
    public IStreamSink
{
public:
    StreamSink(void);

    virtual ~StreamSink(void) {}

public:
    virtual void Fire(const std:shared_ptr<Record>& record) NSFX_OVERRIDE;
    virtual void Use(std::ostream* os) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStreamSink)
        NSFX_INTERFACE_ENTRY(ILogger)
    NSFX_INTERFACE_MAP_END()

private:
    std::ostream* os_;
};


////////////////////////////////////////////////////////////////////////////////
inline StreamSink::StreamSink(void) :
    os_(nullptr)
{
}

inline void StreamSink::Fire(const std:shared_ptr<Record>& record)
{
    if (!os_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
    record->;
}

inline void StreamSink::Use(std::ostream* os)
{
    if (!os)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    os_ = os;
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC

