/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef FILTER_CHAIN_H__05B3092E_04D4_4227_AE44_D6D4DF48CFBE
#define FILTER_CHAIN_H__05B3092E_04D4_4227_AE44_D6D4DF48CFBE


#include <nsfx/log/config.h>
#include <nsfx/log/core/filter/i-filter-chain.h>
#include <nsfx/component/object.h>
#include <nsfx/component/class-registry.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log filter chain.
 */
class FilterChain :
    public IFilterChain,
    public IFilter
{
    typedef FilterChain  ThisClass;

public:
    virtual ~FilterChain(void) {}

    virtual size_t GetNumFilters(void) NSFX_OVERRIDE;
    virtual void PushBack(Ptr<IFilter> filter) NSFX_OVERRIDE;
    virtual void PopBack(void) NSFX_OVERRIDE;

    virtual FilterDecision Decide(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IFilterChain)
        NSFX_INTERFACE_ENTRY(IFilter)
    NSFX_INTERFACE_MAP_END()

private:
    vector<Ptr<IFilter>>  filters_;
};


////////////////////////////////////////////////////////////////////////////////
inline size_t FilterChain::GetNumFilters(void)
{
    return filters_.size();
}

inline void FilterChain::PushBack(Ptr<IFilter> filter)
{
    filters_.emplace_back(std::move(filter));
}

inline void FilterChain::PopBack(void)
{
    filters_.pop_back();
}

inline FilterDecision FilterChain::Decide(const std::shared_ptr<Record>& record)
{
    FilterDecision result = ACCEPT;
    for (auto it = filters_.begin();
         result == ACCEPT && it != filters_.end();
         ++it)
    {
        result = (*it)->Decide(record);
    }
    return result;
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // FILTER_CHAIN_H__05B3092E_04D4_4227_AE44_D6D4DF48CFBE

