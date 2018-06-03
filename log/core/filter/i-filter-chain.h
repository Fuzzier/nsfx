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

#ifndef I_FILTER_CHAIN_H__66E81027_F059_41AB_B079_2EE479D6665F
#define I_FILTER_CHAIN_H__66E81027_F059_41AB_B079_2EE479D6665F


#include <nsfx/log/config.h>
#include <nsfx/log/core/filter/i-filter.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log filter chain interface.
 */
class IFilterChain :
    virtual public IObject
{
public:
    virtual ~IFilterChain(void) BOOST_NOEXCEPT {}

    virtual size_t GetNumFilters(void) = 0;
    virtual void PushBack(Ptr<IFilter> filter) = 0;
    virtual void PopBack(void) = 0;
};


NSFX_DEFINE_CLASS_UID(IFilterChain, "edu.uestc.nsfx.log.IFilterChain");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_FILTER_CHAIN_H__66E81027_F059_41AB_B079_2EE479D6665F

