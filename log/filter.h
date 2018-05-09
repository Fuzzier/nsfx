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
#include <nsfx/log/record.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log filter interface.
 */
class IFilter
{
public:
    virtual ~IFilter(void) BOOST_NOEXCEPT {}

    virtual bool Filter(const std::shared_ptr<Record>& record) = 0;
};


NSFX_LOG_CLOSE_NAMESPACE


#endif // FILTER_H__B2B7049C_D12B_4CDE_B979_F821264B17BF

