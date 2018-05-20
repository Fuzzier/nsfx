/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-09
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef FILTER_H__B2B7049C_D12B_4CDE_B979_F821264B17BF
#define FILTER_H__B2B7049C_D12B_4CDE_B979_F821264B17BF


#include <nsfx/log/config.h>
#include <nsfx/log/core/filter/i-filter.h>
#include <nsfx/component/object.h>
#include <boost/type_traits/decay.hpp>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Create a funtor based log filter.
 *
 * @tparam Functor Must conform to \c FilterFunctorConcept.
 *                 i.e., the prototype must be
 *                 <code>FilterDecision (const std::shared_ptr<Record>& record)</code>.
 */
template<class Functor>
Ptr<IFilter> CreateFilter(Functor&& functor);


////////////////////////////////////////////////////////////////////////////////
template<class Functor>
class FilterFunctorConcept
{
public:
    BOOST_CONCEPT_USAGE(FilterFunctorConcept)
    {
        typename ::boost::decay<Functor>::type* functor = nullptr;
        std::shared_ptr<Record>* record = nullptr;
        FilterDecision decision = (*functor)(*record);
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Functor based log filter.
 */
template<class Functor>
class FunctorBasedFilter :
    public IFilter
{
    BOOST_CONCEPT_ASSERT((FilterFunctorConcept<Functor>));

    typedef FunctorBasedFilter  ThisClass;

public:
    FunctorBasedFilter(Functor&& f) :
        f_(std::move(f))
    {}

    virtual ~FunctorBasedFilter(void) {}

    virtual FilterDecision Decide(const std::shared_ptr<Record>& record) NSFX_OVERRIDE
    {
        return f_(record);
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IFilter)
    NSFX_INTERFACE_MAP_END()

private:
    Functor f_;
    Ptr<IFilter> next_;
};


////////////////////////////////////////////////////////////////////////////////
template<class Functor>
inline Ptr<IFilter> CreateFilter(Functor&& functor)
{
    typedef Object<FunctorBasedFilter<Functor> >  FilterClass;
    return Ptr<IFilter>(new FilterClass(std::move(functor)));
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // FILTER_H__B2B7049C_D12B_4CDE_B979_F821264B17BF

