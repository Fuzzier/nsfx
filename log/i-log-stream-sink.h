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

#ifndef I_LOG_STREAM_SINK_H__3F1CC554_1F37_457E_9EF2_F9EDF6FEEC70
#define I_LOG_STREAM_SINK_H__3F1CC554_1F37_457E_9EF2_F9EDF6FEEC70


#include <nsfx/log/config.h>
#include <nsfx/log/i-log.h>
#include <ostream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The stream-based log sink.
 */
class ILogStreamSink :
    public ILogEventSink
{
public:
    virtual ~ILogStreamSink(void) BOOST_NOEXCEPT {}

    // ILogEventSink
    virtual void Fire(LogRecord record) NSFX_OVERRIDE = 0;

    // Pending log value.
    /**
     * @brief Add a pending log value.
     *
     * The pending log value will be added to each received log record.
     *
     * @param[in] name  The name of the log value.
     * @param[in] value The log value.
     *
     * @return If there's already a value with the same name,
     *         this function returns `false`.
     */
    virtual bool AddValue(const std::string& name, LogValue value) = 0;

    /**
     * @brief Add or udpate  a pending log value.
     *
     * @param[in] name  The name of the log value.
     * @param[in] value The log value.
     */
    virtual void UpdateValue(const std::string& name, LogValue value) = 0;

    /**
     * @brief Remove a pending log value.
     *
     * @param[in] name The name of the log value.
     */
    virtual void RemoveValue(const std::string& name) = 0;

    // Log filter.
    /**
     * @brief Set a log filter.
     *
     * @param[in] filter Set a log filter.
     *                   If `nullptr` is specified, the filter is removed.
     */
    virtual void SetFilter(Ptr<ILogFilter> filter) = 0;

    // Output stream.
    /**
     * @brief Add an output stream.
     */
    virtual void AddStream(std::ostream& os) = 0;

    /**
     * @brief Add an output file.
     */
    virtual void AddFile(
        const std::string& filename,
        std::ios_base::openmode mode = std::ios_base::out) = 0;

};

NSFX_DEFINE_CLASS_UID(ILogStreamSink, "edu.uestc.nsfx.ILogStreamSink");


NSFX_CLOSE_NAMESPACE


#endif // I_LOG_STREAM_SINK_H__3F1CC554_1F37_457E_9EF2_F9EDF6FEEC70

