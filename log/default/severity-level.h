/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef SEVERITY_LEVEL_H__DCFE699B_E7C7_4B4B_8D5D_0345944D2ED1
#define SEVERITY_LEVEL_H__DCFE699B_E7C7_4B4B_8D5D_0345944D2ED1


#include <nsfx/log/config.h>
#include <nsfx/log/core/record/attribute-value-info.h>
#include <iostream>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief Log severity level.
 */
enum SeverityLevel
{
    /**
     * @brief Any error that forces the program to shutdown to prevent from corruption.
     *
     * The program is unusable or malfunctioning, and have to be terminated
     * in order to prevent (further) data loss or logic corruption.
     *
     * For example.
     * * Try to access an array out of bound.
     * * Try to use an invalid pointer.
     */
    LOG_FATAL    = 0x00000001,

    /**
     * @brief Any error that cause an operation to fail.
     *
     * There is no data loss or logic corruption yet, but the program cannot
     * resolve the error without user intervention.
     *
     * For example.
     * * Failed to retrieve the required data.
     *   - Failed to connect to a database.
     *   - Failed to open a file.
     * * Provide invalid data.
     */
    LOG_ERROR    = 0x00000002,

    /**
     * @brief Any condition that can potentially cause oddities.
     *
     * The program is able to recover from the condition, but the operation is
     * not guaranteed to be performed as expected.
     *
     * For example.
     */
    LOG_WARNING  = 0x00000004,

    /**
     * @brief General information about the state of the program.
     *
     */
    LOG_INFO     = 0x00000008,

    /**
     * @brief Information that diagnostically helpful.
     */
    LOG_DEBUG    = 0x00000010,

    /**
     * @brief Detailed information about the invoked function.
     */
    LOG_FUNCTION = 0x00000020,

    /**
     * @brief Detailed information about the performed operations.
     */
    LOG_TRACE    = 0x00000040,

    LOG_NONE     = 0x00000000,
    LOG_ALL      = 0xFFFFFFFF,
};


////////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<<(std::ostream& os, SeverityLevel level)
{
    const char* label;
    switch (level)
    {
    case LOG_FATAL:
        label = "FATAL";
        break;

    case LOG_ERROR:
        label = "ERROR";
        break;

    case LOG_WARNING:
        label = "WARNING";
        break;

    case LOG_DEBUG:
        label = "DEBUG";
        break;

    case LOG_INFO:
        label = "INFO";
        break;

    case LOG_FUNCTION:
        label = "FUNCTION";
        break;

    case LOG_TRACE:
        label = "TRACE";
        break;

    default:
        BOOST_THROW_EXCEPTION(
            InvalidArgument() <<
            ErrorMessage("Unknown log severity level."));
        break;
    }
    return (os << label);
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The severity level information class.
 */
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(SeverityLevelInfo, "SeverityLevel", SeverityLevel);


NSFX_LOG_CLOSE_NAMESPACE


#endif // SEVERITY_LEVEL_H__DCFE699B_E7C7_4B4B_8D5D_0345944D2ED1

