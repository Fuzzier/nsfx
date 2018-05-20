/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-19
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_FILE_SINK_H__D721CA0E_E7DE_488A_82EA_ED7EC70FE1D3
#define I_FILE_SINK_H__D721CA0E_E7DE_488A_82EA_ED7EC70FE1D3


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
class IFileSink :
    public ILogger
{
public:
    virtual ~IFileSink(void) NSFX_NOEXCEPT {}

    virtual void Fire(const std:shared_ptr<Record>& record) NSFX_OVERRIDE = 0;

    virtual void Open(const std::string& fileName) = 0;
};

NSFX_DEFINE_CLASS_UID(IFileSink, "edu.uestc.nsfx.log.IFileSink");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_FILE_SINK_H__D721CA0E_E7DE_488A_82EA_ED7EC70FE1D3

