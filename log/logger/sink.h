/**
 * @file
 *
 * @brief Logging support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49
#define SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49


#include <nsfx/log/config.h>
#include <nsfx/log/record.h>


NSFX_LOG_OPEN_NAMESPACE /*{{{*/


////////////////////////////////////////////////////////////////////////////////
class Sink /*{{{*/
{
public:
    virtual ~Sink(void) NSFX_NOEXCEPT {}

    // Sink operations./*{{{*/
public:
    virtual void CommitRecord(const Record& record) = 0;
    /*}}}*/
}; // class Sink /*}}}*/


NSFX_LOG_CLOSE_NAMESPACE /*}}}*/


#endif // SINK_H__72BA2E38_8BE7_472F_B664_A0BD5D2A6A49

