/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-09
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CREATE_LOG_FILTER_H__B6B4E910_6D36_4002_BE58_2C27D340350C
#define CREATE_LOG_FILTER_H__B6B4E910_6D36_4002_BE58_2C27D340350C


#include <nsfx/log/config.h>
#include <nsfx/log/i-log-filter.h>
#include <nsfx/component/ptr.h>
#include <type_traits> // decay


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The concept of functors that can be used to create a log filter.
 * @internal
 */
template<class Functor>
class LogFilterFunctorConcept
{
public:
    BOOST_CONCEPT_USAGE(LogFilterFunctorConcept)
    {
        typename std::decay<Functor>::type* functor = nullptr;
        const LogRecord* record = nullptr;
        LogFilterDecision decision = (*functor)(*record);
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Functor based log filter.
 */
template<class Functor>
class FunctorBasedLogFilter :
    public ILogFilter
{
    BOOST_CONCEPT_ASSERT((LogFilterFunctorConcept<Functor>));

    typedef FunctorBasedLogFilter  ThisClass;

public:
    template<class F>
    FunctorBasedLogFilter(F&& functor) :
        functor_(std::forward<F>(functor))
    {}

    virtual ~FunctorBasedLogFilter(void) {}

    virtual LogFilterDecision Decide(const LogRecord& record) NSFX_OVERRIDE
    {
        return functor_(record);
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ILogFilter)
    NSFX_INTERFACE_MAP_END()

private:
    Functor functor_;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Create a funtor based log filter.
 *
 * @tparam Functor Must conform to \c LogFilterFunctorConcept.
 *                 i.e., the prototype must be
 *                 \c FilterDecision(const Record& record).
 */
template<class Functor>
inline Ptr<ILogFilter> CreateLogFilter(Functor&& functor)
{
    typedef Object<FunctorBasedLogFilter<Functor>>  Impl;
    return Ptr<ILogFilter>(new Impl(std::forward<Functor>(functor)));
}


NSFX_CLOSE_NAMESPACE


#endif // CREATE_LOG_FILTER_H__B6B4E910_6D36_4002_BE58_2C27D340350C

