/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
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

#ifndef MAKE_LOG_FORMATTER_H__18391425_98CC_4EBB_8DB2_B69E7BB492C0
#define MAKE_LOG_FORMATTER_H__18391425_98CC_4EBB_8DB2_B69E7BB492C0


#include <nsfx/log/config.h>
#include <nsfx/log/i-log-formatter.h>
#include <nsfx/component/object.h>
#include <type_traits> // decay
#include <ostream>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The concept of functors that can be used to create log formatters.
 * @internal
 */
template<class Functor>
class LogFormatterFunctorConcept
{
public:
    BOOST_CONCEPT_USAGE(LogFormatterFunctorConcept)
    {
        typename std::decay<Functor>::type* functor = nullptr;
        std::ostream* os = nullptr;
        const LogRecord* record = nullptr;
        (*functor)(*os, *record);
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The functor-based stream formatter.
 *
 * @tparam Functor Must conform to `LogFormatterFunctorConcept`. i.e.,
 *                 `void(std::ostream& os, const Record& record)`.
 */
template<class Functor>
class FunctorBasedLogFormatter :
    public ILogFormatter
{
    BOOST_CONCEPT_ASSERT((LogFormatterFunctorConcept<Functor>));

    typedef FunctorBasedLogFormatter  ThisClass;

public:
    template<class F>
    FunctorBasedLogFormatter(F&& functor) :
        functor_(std::forward<F>(functor))
    {}

    virtual ~FunctorBasedLogFormatter(void) {}

    virtual void Format(std::ostream& os, const LogRecord& record) NSFX_OVERRIDE
    {
        if (!os)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("The log formatter cannot use an invalid stream."));
        }
        functor_(os, record);
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogFormatter)
    NSFX_INTERFACE_MAP_END()

private:
    Functor functor_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Create a functor-based log formatter.
 *
 * @tparam Functor Must conform to `LogFormatterFunctorConcept`. i.e.,
 *                 `void(std::ostream& os, const Record& record)`.
 */
template<class Functor>
inline Ptr<ILogFormatter> CreateLogFormatter(Functor&& functor)
{
    typedef Object<FunctorBasedLogFormatter<Functor>>  Impl;
    return Ptr<ILogFormatter>(new Impl(std::forward<Functor>(functor)));
}


NSFX_CLOSE_NAMESPACE


#endif // MAKE_LOG_FORMATTER_H__18391425_98CC_4EBB_8DB2_B69E7BB492C0

