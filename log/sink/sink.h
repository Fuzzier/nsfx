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

#ifndef SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49
#define SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49


#include <nsfx/log/config.h>
#include <nsfx/log/logger/i-logger.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
class IStreamLogger :
    public ILogger
{
public:
    virtual ~IStreamLogger(void) NSFX_NOEXCEPT {}

public:
    virtual void Fire(const std:shared_ptr<Record>& record) NSFX_OVERRIDE = 0;

    virtual void Use(std::ostream* os) = 0;
};


NSFX_LOG_CLOSE_NAMESPACE


#endif // SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49

