/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-19
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef FILE_SINK_H__0D5E7C54_48CC_4C56_BB63_D420AB9FFCF0
#define FILE_SINK_H__0D5E7C54_48CC_4C56_BB63_D420AB9FFCF0


#include <nsfx/log/config.h>
#include <nsfx/log/core/sink/i-file-sink.h>
#include <nsfx/log/core/formatter/i-stream-formatter.h>
#include <fstream> // ofstream


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The default file sink.
 *
 * ## Interfaces
 * ### Uses
 * * \c IStreamFormatter
 * ### Provides
 * * \c ILogger
 * * \c IFileSink
 */
class FileSink :
    public IFileSink,
    public IStreamFormatterUser
{
    typedef FileSink  ThisClass;

public:
    FileSink(void);

    virtual ~FileSink(void) {}

public:
    virtual void Use(Ptr<IStreamFormatter> formatter) NSFX_OVERRIDE;

public:
    virtual void Fire(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;

    virtual void Open(const std::string& fileName) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogger)
        NSFX_INTERFACE_ENTRY(IFileSink)
        NSFX_INTERFACE_ENTRY(IStreamFormatterUser)
    NSFX_INTERFACE_MAP_END()

private:
    std::ofstream ofs_;
    std::string fileName_;
    Ptr<IStreamFormatter> formatter_;
};


NSFX_REGISTER_CLASS(FileSink, "edu.uestc.nsfx.log.FileSink");


////////////////////////////////////////////////////////////////////////////////
inline FileSink::FileSink(void)
{
}

inline void FileSink::Open(const std::string& fileName)
{
    if (ofs_.is_open())
    {
        BOOST_THROW_EXCEPTION(
            IllegalMethodCall() <<
            ErrorMessage("Cannot set another file name "
                         "while the log file sink has already opened a file."));
    }
    ofs_.open(fileName, std::ofstream::out | std::ofstream::trunc);
}

inline void FileSink::Use(Ptr<IStreamFormatter> formatter)
{
    if (!formatter)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    formatter_ = formatter;
}

inline void FileSink::Fire(const std::shared_ptr<Record>& record)
{
    if (!ofs_.is_open() || !formatter_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
    formatter_->Format(ofs_, record);
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // FILE_SINK_H__0D5E7C54_48CC_4C56_BB63_D420AB9FFCF0

