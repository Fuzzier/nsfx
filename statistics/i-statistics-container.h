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

#ifndef I_STATISTICS_CONTAINER_H__00B604DE_4AE5_433F_BC11_AD963709CCBF
#define I_STATISTICS_CONTAINER_H__00B604DE_4AE5_433F_BC11_AD963709CCBF


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/i-statistic.h>
#include <nsfx/statistics/i-statistics-enumerator.h>
#include <nsfx/component/ptr.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A container of statistics.
 */
class IStatisticsContainer :
    virtual public IObject
{
public:
    virtual ~IStatisticsContainer(void) BOOST_NOEXCEPT {}

    /**
     * @brief Get an enumerator.
     */
    virtual Ptr<IStatisticsEnumerator> GetEnumerator(void) = 0;

    /**
     * @brief Get a statistic via its name.
     *
     * @param[in] name The name of the statistic.
     *
     * @throw StatisticsNotRegistered
     */
    virtual Ptr<IStatistic> Get(const std::string& name) = 0;

    /**
     * @brief Connect a collector to a statistic.
     *
     * @param[in] name The name of the statistic.
     * @param[in] sink The collector of the statistic.
     *
     * @throw StatisticsNotRegistered
     * @throw NoInterface
     *
     * @return A cookie that can be used to disconnect.
     */
    virtual cookie_t Connect(const std::string& name, Ptr<IObject> sink) = 0;

    /**
     * @brief Disconnect a collector from a statistic.
     *
     * @param[in] name The name of the statistic.
     * @param[in] sink The cookie.
     *
     * @throw StatisticsNotRegistered
     */
    virtual void Disconnect(const std::string& name, cookie_t cookie) = 0;

};


NSFX_DEFINE_CLASS_UID(IStatisticsContainer, "edu.uestc.nsfx.IStatisticsContainer");


NSFX_CLOSE_NAMESPACE


#endif // I_STATISTICS_CONTAINER_H__00B604DE_4AE5_433F_BC11_AD963709CCBF

