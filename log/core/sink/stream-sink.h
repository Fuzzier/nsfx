/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
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

#ifndef STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC
#define STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC


#include <nsfx/log/config.h>
#include <nsfx/log/core/sink/i-stream-sink.h>
#include <nsfx/log/core/formatter/i-stream-formatter.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The default stream sink.
 *
 * ## Interfaces
 * ### Uses
 * * \c IStreamFormatter
 * ### Provides
 * * \c ILogger
 * * \c IStreamSink
 */
class StreamSink :
    public IStreamSink,
    public IStreamFormatterUser
{
    typedef StreamSink  ThisClass;

public:
    StreamSink(void);

    virtual ~StreamSink(void) {}

public:
    virtual void Use(Ptr<IStreamFormatter> formatter) NSFX_OVERRIDE;

public:
    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;
    virtual void SetStream(std::ostream* os) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogger)
        NSFX_INTERFACE_ENTRY(IStreamSink)
        NSFX_INTERFACE_ENTRY(IStreamFormatterUser)
    NSFX_INTERFACE_MAP_END()

private:
    std::ostream* os_;
    Ptr<IStreamFormatter> formatter_;
};


NSFX_REGISTER_CLASS(StreamSink, "edu.uestc.nsfx.log.StreamSink");


////////////////////////////////////////////////////////////////////////////////
inline StreamSink::StreamSink(void) :
    os_(nullptr)
{
}

inline void StreamSink::Use(Ptr<IStreamFormatter> formatter)
{
    if (!formatter)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    formatter_ = formatter;
}

inline void StreamSink::SetStream(std::ostream* os)
{
    if (!os)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    os_ = os;
}

inline void StreamSink::Fire(const std::shared_ptr<Record>& record)
{
    if (!os_ || !formatter_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
    formatter_->Format(*os_, record);
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // STREAM_SINK_H__C4C6833E_6AE2_47BE_8563_3948E05061EC

