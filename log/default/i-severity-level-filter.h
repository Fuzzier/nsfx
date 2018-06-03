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

#ifndef I_SEVERITY_LEVEL_FILTER_H__300E7BE4_40F8_4ABE_9861_61FCD7CC0ED6
#define I_SEVERITY_LEVEL_FILTER_H__300E7BE4_40F8_4ABE_9861_61FCD7CC0ED6


#include <nsfx/log/config.h>
#include <nsfx/log/default/severity-level.h>
#include <nsfx/log/core/filter/i-filter.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log filter interface.
 */
class ISeverityLevelFilter :
    public IFilter
{
public:
    virtual ~ISeverityLevelFilter(void) BOOST_NOEXCEPT {}

    virtual FilterDecision Decide(const std::shared_ptr<Record>& record) NSFX_OVERRIDE = 0;

    /**
     * @brief Accept the specified severity levels.
     *
     * @see \c SeverityLevel
     */
    virtual void ToggleAccept(uint32_t severityLevels) = 0;

    /**
     * @brief Deline the specified severity levels.
     *
     * @see \c SeverityLevel
     */
    virtual void ToggleDecline(uint32_t severityLevels) = 0;

    /**
     * @brief Set accepted severity levels while declining other levels.
     *
     * @see \c SeverityLevel
     */
    virtual void SetAcceptedLevels(uint32_t severityLevels) = 0;

    /**
     * @brief Get the accepted severity levels.
     *
     * @see \c SeverityLevel
     */
    virtual uint32_t GetAcceptedLevels(void) = 0;
};


NSFX_DEFINE_CLASS_UID(ISeverityLevelFilter, "edu.uestc.nsfx.log.ISeverityLevelFilter");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_SEVERITY_LEVEL_FILTER_H__300E7BE4_40F8_4ABE_9861_61FCD7CC0ED6

