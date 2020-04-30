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

#ifndef STATISTICS_CONTAINER_H__FE71F857_4DCD_4308_950A_476C4C538C91
#define STATISTICS_CONTAINER_H__FE71F857_4DCD_4308_950A_476C4C538C91


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/i-statistics-container.h>
#include <nsfx/statistics/statistics-enumerator.h>
#include <nsfx/statistics/statistic.h>
#include <nsfx/component/object.h>
#include <nsfx/event/event.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A container of statistics.
 *
 * # Interfaces
 * * Provides
 *   + `IStatisticsContainer`
 */
class StatisticsContainer :
    public IStatisticsContainer
{
	typedef StatisticsContainer ThisClass;

public:
    StatisticsContainer(void);
    virtual ~StatisticsContainer(void);

    // IStatisticsContainer
    virtual Ptr<IStatisticsEnumerator> GetEnumerator(void) NSFX_OVERRIDE;
    virtual Ptr<IStatistic> Get(const std::string& name) NSFX_OVERRIDE;
    virtual cookie_t Connect(const std::string& name, Ptr<IObject> sink) NSFX_OVERRIDE;
    virtual void Disconnect(const std::string& name, cookie_t cookie) NSFX_OVERRIDE;

public:
    /**
     * @brief Add a named statistic.
     *
     * @tparam IStat   The event interface of the statistic.
     * @param[in] name The name of the statistic.
     *
     * @return A pointer to `Event<IStat>` that provides a `Fire()` method to
     *         push data to the collectors.
     *
     * @throws `StatisticAlreadyRegistered`
     */
    template<class IStat>
    Ptr<Event<IStat>> Add(const std::string& name);

    bool Has(const std::string& name) const;

    /**
     * @brief Merge statistics from a statistics enumerator.
     *
     * @param[in] prefix The prefix to the names of the merged statistics.
     * @param[in] merged The statistics container to be merged.
     *
     * The current set of statistics in the `merged` is merged.
     * If the `merged` is changed, there is no guarantee that the this
     * container would change accordingly.
     *
     * If the name already exists, the existing statistic is **not** replaced.
     */
    void MergeFrom(const std::string& prefix, Ptr<IStatisticsContainer> merged);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStatisticsContainer)
    NSFX_INTERFACE_MAP_END()

private:
    typedef unordered_map<std::string, Ptr<IStatistic>>  ContainerType;
    ContainerType  items_;

private:
    // KeyIterator./*{{{*/
public:
    class KeyIterator
    {
        friend class StatisticsContainer;

    public:
        typedef StatisticsContainer::ContainerType        ContainerType;
        typedef ContainerType::const_iterator             BaseIteratorType;
        typedef BaseIteratorType::difference_type         difference_type;
        typedef BaseIteratorType::value_type::first_type  value_type;
        typedef BaseIteratorType::value_type::first_type* pointer;
        typedef BaseIteratorType::value_type::first_type& reference;
        typedef BaseIteratorType::iterator_category       iterator_category;

    private:
        explicit KeyIterator(BaseIteratorType it) :
            it_(it)
        {
        }

    public:
        KeyIterator& operator++(void)
        {
            ++it_;
            return *this;
        }

        KeyIterator operator++(int)
        {
            return KeyIterator(it_++);
        }

        reference operator*(void) const
        {
            return it_->first;
        }

        pointer operator->(void) const
        {
            return &(it_->first);
        }

        bool operator==(const KeyIterator& rhs) const
        {
            return it_ == rhs.it_;
        }

        bool operator!=(const KeyIterator& rhs) const
        {
            return it_ != rhs.it_;
        }

    private:
        BaseIteratorType it_;

    }; // class KeyIterator

public:
    KeyIterator kbegin(void) const
    {
        return KeyIterator(items_.cbegin());
    }

    KeyIterator kend(void) const
    {
        return KeyIterator(items_.cend());
    }

    KeyIterator erase(KeyIterator it)
    {
        return KeyIterator(items_.erase(it.it_));
    }

    /*}}}*/

};


////////////////////////////////////////////////////////////////////////////////
inline StatisticsContainer::StatisticsContainer(void)
{
}

inline StatisticsContainer::~StatisticsContainer(void)
{
}

inline Ptr<IStatisticsEnumerator> StatisticsContainer::GetEnumerator(void)
{
    return Ptr<IStatisticsEnumerator>(
        new Object<StatisticsEnumerator>(kbegin(), kend()));
}

inline Ptr<IStatistic> StatisticsContainer::Get(const std::string& name)
{
    auto it = items_.find(name);
    if (it == items_.end())
    {
        BOOST_THROW_EXCEPTION(
            StatisticNotRegistered() <<
            StatisticNameErrorInfo(name));
    }
    return it->second;
}

inline cookie_t
StatisticsContainer::Connect(const std::string& name, Ptr<IObject> sink)
{
    Ptr<IStatistic> stat = Get(name);
    return stat->Connect(sink);
}

inline void
StatisticsContainer::Disconnect(const std::string& name, cookie_t cookie)
{
    Ptr<IStatistic> stat = Get(name);
    stat->Disconnect(cookie);
}

template<class IStat>
inline Ptr<Event<IStat>> StatisticsContainer::Add(const std::string& name)
{
    auto result = items_.emplace(name, nullptr);
    if (!result.second)
    {
        BOOST_THROW_EXCEPTION(
            StatisticAlreadyRegistered() <<
            StatisticNameErrorInfo(name));
    }
    auto it = result.first;

    Ptr<Statistic<IStat>> stat(new Object<Statistic<IStat>>);
    it->second = stat;
    return stat->GetEvent();
}

inline bool StatisticsContainer::Has(const std::string& name) const
{
    return !!items_.count(name);
}

inline void StatisticsContainer::MergeFrom(const std::string& prefix,
                                           Ptr<IStatisticsContainer> merged)
{
    Ptr<IStatisticsEnumerator> e = merged->GetEnumerator();
    while (e->HasNext())
    {
        const std::string& name  = e->Next();
        Ptr<IStatistic> stat = merged->Get(name);

        std::string pname = prefix + name;
        auto result = items_.emplace(pname, nullptr);
        if (result.second)
        {
            auto it = result.first;
            it->second = std::move(stat);
        }
    }
}


NSFX_CLOSE_NAMESPACE


#endif // STATISTICS_CONTAINER_H__FE71F857_4DCD_4308_950A_476C4C538C91

