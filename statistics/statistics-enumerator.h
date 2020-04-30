/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-04-30
 *
 * @copyright Copyright (c) 2020.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STATISTICS_ENUMERATOR_H__164D50E7_939D_4FC8_8841_15D8A7A3C3CA
#define STATISTICS_ENUMERATOR_H__164D50E7_939D_4FC8_8841_15D8A7A3C3CA


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/i-statistics-enumerator.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief An enumerator of statistics.
 *
 * # Interfaces
 * * Provides
 *   + `IStatisticsEnumerator`
 */
class StatisticsEnumerator :
    public IStatisticsEnumerator
{
    typedef StatisticsEnumerator ThisClass;

public:
    /**
     * @brief Construct a statistics enumerator.
     *
     * @tparam InputIterator An input iterator whose value type is `std::string`.
     */
    template<class InputIterator>
    StatisticsEnumerator(InputIterator first, InputIterator last);

    virtual ~StatisticsEnumerator(void);

    // IStatisticsEnumerator
    virtual void Reset(void) NSFX_OVERRIDE;
    virtual bool HasNext(void) NSFX_OVERRIDE;
    virtual const std::string& Next(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStatisticsEnumerator)
    NSFX_INTERFACE_MAP_END()

private:
    vector<std::string>  items_;
    size_t index_;
};


////////////////////////////////////////////////////////////////////////////////
template<class InputIterator>
inline StatisticsEnumerator::StatisticsEnumerator(InputIterator first,
                                                  InputIterator last) :
    items_(first, last),
    index_(0)
{
}

inline StatisticsEnumerator::~StatisticsEnumerator(void)
{
}

inline void StatisticsEnumerator::Reset(void)
{
    index_ = 0;
}

inline bool StatisticsEnumerator::HasNext(void)
{
    return index_ < items_.size();
}

inline const std::string& StatisticsEnumerator::Next(void)
{
    if (index_ >= items_.size())
    {
        BOOST_THROW_EXCEPTION(
            OutOfBounds() <<
            ErrorMessage("Cannot get the next statistics, "
                         "since there are no more statistics."));
    }
    return items_[index_++];
}


NSFX_CLOSE_NAMESPACE


#endif // STATISTICS_ENUMERATOR_H__164D50E7_939D_4FC8_8841_15D8A7A3C3CA

