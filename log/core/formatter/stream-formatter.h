/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-17
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef STREAM_FORMATTER_H__46E9AE1B_0879_4B96_B0DF_D812994679D2
#define STREAM_FORMATTER_H__46E9AE1B_0879_4B96_B0DF_D812994679D2


#include <nsfx/log/config.h>
#include <nsfx/log/core/formatter/i-stream-formatter.h>
#include <nsfx/component/object.h>
#include <boost/type_traits/decay.hpp>
#include <ostream>
#include <memory> // shared_ptr


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Create a stream formatter via a functor.
 *
 * @tparam Functor Must conform to \c StreamFormatterFunctorConcept. i.e.,
 *         <code>void(std::ostream& os, const std::shared_ptr<Record>& record)</code>.
 */
template<class Functor>
Ptr<IStreamFormatter> CreateStreamFormatter(Functor&& functor);


////////////////////////////////////////////////////////////////////////////////
template<class Functor>
class StreamFormatterFunctorConcept
{
public:
    BOOST_CONCEPT_USAGE(StreamFormatterFunctorConcept)
    {
        typename boost::decay<Functor>::type* functor = nullptr;
        std::ostream* os = nullptr;
        std::shared_ptr<Record>* record = nullptr;
        (*functor)(*os, *record);
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The functor-based stream formatter.
 *
 * @tparam Functor Must conform to \c StreamFormatterFunctorConcept. i.e.,
 *         <code>void(std::ostream& os, const std::shared_ptr<Record>& record)</code>.
 */
template<class Functor>
class StreamFormatter :
    public IStreamFormatter
{
    BOOST_CONCEPT_ASSERT((StreamFormatterFunctorConcept<Functor>));

    typedef StreamFormatter  ThisClass;

public:
    StreamFormatter(Functor&& functor) :
        functor_(std::move(functor))
    {}

    virtual ~StreamFormatter(void) {}

    virtual void Format(std::ostream& os,
                        const std::shared_ptr<Record>& record) NSFX_OVERRIDE
    {
        if (!os)
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("The log stream formatter cannot "
                             "use an invalid stream."));
        }
        functor_(os, record);
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStreamFormatter)
    NSFX_INTERFACE_MAP_END()

private:
    Functor functor_;
};


////////////////////////////////////////////////////////////////////////////////
template<class Functor>
inline Ptr<IStreamFormatter> CreateStreamFormatter(Functor&& functor)
{
    typedef Object<StreamFormatter<Functor> >  StreamFormatterClass;
    return Ptr<IStreamFormatter>(new StreamFormatterClass(std::move(functor)));
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // STREAM_FORMATTER_H__46E9AE1B_0879_4B96_B0DF_D812994679D2

