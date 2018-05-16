/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2015-02-26
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__A5BE74BF_215D_493B_8BB1_8F90D9316F15
#define CONFIG_H__A5BE74BF_215D_493B_8BB1_8F90D9316F15


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Log
 *
 * @brief Log support.
 *
 * # Motivation
 *   The log library is provided to ease the task of tracing a program.
 *   The program outputs useful runtime information to a log that can be
 *   examined to analyze the program's behaviors.
 *
 *   The log library aims to provide a set of log tools that are header-only,
 *   easy-to-use, flexible, configurable, extensible and fast.
 *
 *   The loggers in several existing network simulators are not flexible.
 *   e.g., in NS3, the granularity of logging is a model/class (precisely,
 *   a compilation unit).
 *   i.e., it logs all events from all objects of the class, and it's hard to
 *   monitor a single object of the class.
 *   As a result, the log is huge and hard to read, and must be filtered for
 *   humans to find their focus.
 *   It is not only efficient for analyzers, but also slows down the program.
 *
 * ## Extensibility
 *    Extensibility and performance are usually contradictory.
 *    Extensibility means the library has to use interfaces (virtual functions)
 *    to hide the operation details implemented by users.
 *    A logging operation can involve tens of virtual function calls, which can
 *    be more expensive than the operation that generates the log.
 *
 * ### Log record
 *     The information to be logged is carried by log records.
 *     The log record must be extensible to be able to carry arbitrary types of
 *     values.
 *     i.e., the values have to be type-erased.
 *     The type-erasure can be implemented as union-based or interface-based.
 *     However, union-based type-erasure only supports a limited set of value
 *     types, thus cannot provide sufficient extensibility.
 *     Interface-based type-erasure has more overhead, since virtual function
 *     are used, but it is more extensible.
 *
 *     There must be a way to identify and obtain a value in the log record.
 *     The possible approaches include compile-time tag-based dispatch and
 *     runtime index-based dispatch.
 *     The compile-time tag-based dispatch means that the contents of log
 *     records are determined at compile-time.
 *     This approach is not easy-to-use.
 *     The runtime index-based dispatch uses a container to store the values.
 *     The container can be associative to speed up lookup.
 *     The index is usually a human-readible string.
 *
 * ### Configurability
 *     BOOST provides a log library.
 *     In the library, compile-time functionals are adopted to make log filters.
 *     It can be fast, but it does not provide runtime configurability directly.
 *
 *     The log library in NSFX provides log filters in a modular way.
 *     Each log filter is provided as a component that provides interface for
 *     configurability.
 *     Users can combine log filters to suit their specific needs.
 *
 * ### Logger usage
 *     Users can use the following macros to use a logger.
 *     @code
 *     NSFX_LOG(logger, severity) << "a message";
 *     NSFX_LOG_FATAL(logger)     << "a message";
 *     NSFX_LOG_ERROR(logger)     << "a message";
 *     NSFX_LOG_WARNING(logger)   << "a message";
 *     NSFX_LOG_INFO(logger)      << "a message";
 *     NSFX_LOG_DEBUG(logger)     << "a message";
 *     NSFX_LOG_FUNCTION(logger)  << "a message";
 *     NSFX_LOG_TRACE(logger)     << "a message";
 *     @endcode
 *
 * # Concept
 *   A <i>log source</i> generates runtime information, and feed them into
 *   <i>log sinks</i>.
 *
 *   Typical information includes human-readable message, severity level,
 *   timestamp, object name, function name, file name and line number, etc.
 *   The information is represented as a collection of <i>named values</i>.
 *
 *   A <i>log record</i> is a collection of named values.
 *   A log source generates log records, and passes them to log sinks.
 *   And the log sinks are responsible to process the log records,
 *   e.g., modify, forward, format, display or store the log records.
 *
 *   A log record may be passed along a series of log sinks.
 *   Each of the log sink performs certain operations upon the log record.
 *   In such perspective, each log sink in the chain is a log source that
 *   feeds log records to the log sinks after it.
 *   Therefore, there is essentially no difference between the log sources and
 *   sinks, and they are both called <i>loggers</i>.
 *
 *   The ultimate log source is the user program that generates the inital
 *   log record and feeds it into the chain.
 *   The piece of program where an initial log record is generated is called
 *   the <i>logging site</i>.
 *
 * # Core tools
 * ## Attribute value
 *    A log record holds named values.
 *    The values are held in a type-neutral way.
 *    Each value is an instance of the \c AttributeValue class.
 *    The class holds a pointer pointing to the \c IAttributeValue interface.
 *
 *    The \c IAttributeValue interfaces provides a single method
 *    \c IAttributeValue::GetTypeId() that gives the type of the stored value.
 *
 *    The \c ITypedAttributeValue interfaces extends the \c IAttributeValue
 *    interface, and provides a method \c ITypedAttributeValue::Get() that
 *    allows log sinks to query the stored value.
 *
 *    A concrete attribute class <b>must</b> implement \c ITypedAttributeValue
 *    interface.
 *
 *    Users that implement attribute value classes <b>must</b> make sure that
 *    the underlying value is kept available when log sinks access it.
 *    However, the log sources may not always know when the underlying value will
 *    be accessed by the log sinks.
 *    e.g., it is not a problem in a single-threaded environment, the logger will
 *    deliver log records to log sinks immediately and wait for them to complete.
 *    However, in a multithreaded environment, the log sinks may reside in a
 *    different thread and consumes the log records in a later time.
 *    Therefore, attribute value <b>shall</b> store or refer to persistent values
 *    whenever possible.
 *
 * ## Logger
 *    In order to process log records, a log sink often have to understand
 *    the meanings of the named values generated by the log source.
 *    Therefore, log sources and sinks are usually coupled, and agree upon
 *    the collection of named values in log records.
 *
 *    The names of values are decided at design-time for log sources and sinks,
 *    while values are usually generated by log sources at runtime.
 *
 *    The library provides \c ILog interface.
 *    It is defined as an event sink that provides a single method
 *    \c ILog::Fire() that accepts a log record.
 *
 *    An intermediate logger <b>shall</b> provide \c ILogEvent interface to
 *    chain/signal other loggers to further process log records.
 *    A terminal logger do not have to provide \c ILogEvent interface.
 *
 * ## Record
 *    Log records are passed to loggers as an \c std::shared_ptr.
 *    Thus, a logger can pass a single instance of log record to several
 *    loggers for further processing.
 *
 *    Since log records are shared among loggers, any modification to the log
 *    record is visible to all other logger.
 *    Thus, logger writters must be careful to make the order and content of
 *    the modifications consistent among the loggers, especially in a
 *    multithreaded logging system.
 *
 * # Utilities
 * ## Attribute
 *    It can be painful for users to provide common information at every
 *    logging site, such as timestamps, function names and line numbers, etc.
 *    To ease the coding burden of users, certain information should be
 *    generated automatically.
 *
 *    An \c Attribute can be used to generate attribute values automatically.
 *    It has a single method \c Attribute::GetValue() that returns an instance
 *    of \c AttributeValue.
 *
 *    A logger can provide \c IAttributeCollection interface to allow users
 *    add, remove or clear \c IAttribute.
 *
 * ## Filter
 *    A log filter examines the named values in a log record, and makes
 *    a decision whether the log record should be discarded.
 *
 *    A filter implements \c IFilter interface.
 *
 * ## Formatter
 */


////////////////////////////////////////////////////////////////////////////////
#define NSFX_LOG_OPEN_NAMESPACE     \
    NSFX_OPEN_NAMESPACE             \
    namespace log {

#define NSFX_LOG_CLOSE_NAMESPACE    \
    } /* namespace log */           \
    NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__A5BE74BF_215D_493B_8BB1_8F90D9316F15

