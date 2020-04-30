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

#ifndef I_STATISTICS_ENUMERATOR_H__201FFB51_78C2_427D_AEDB_CFD14E910F2D
#define I_STATISTICS_ENUMERATOR_H__201FFB51_78C2_427D_AEDB_CFD14E910F2D


#include <nsfx/statistics/config.h>
#include <nsfx/component/i-object.h>
#include <string>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief An interface that enumerates the names of statistics.
 */
class IStatisticsEnumerator :
    virtual public IObject
{
public:
    virtual ~IStatisticsEnumerator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Reset the enumeration sequence.
     */
    virtual void Reset(void) = 0;

    /**
     * @brief Has more statistics to enumerate?
     */
    virtual bool HasNext(void) = 0;

    /**
     * @brief Get the name of the next supported statistic.
     *
     * @pre `HasNext() == true`.
     *
     * @throw OutOfBounds
     */
    virtual const std::string& Next(void) = 0;

};


NSFX_DEFINE_CLASS_UID(IStatisticsEnumerator, "edu.uestc.nsfx.IStatisticsEnumerator");


NSFX_CLOSE_NAMESPACE


#endif // I_STATISTICS_ENUMERATOR_H__201FFB51_78C2_427D_AEDB_CFD14E910F2D

