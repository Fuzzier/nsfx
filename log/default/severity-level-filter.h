/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-09
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7
#define SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7


#include <nsfx/log/config.h>
#include <nsfx/log/core/filter/i-filter.h>
#include <nsfx/log/default/i-severity-level-filter.h>
#include <nsfx/component/class-registry.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The default severity level filter.
 *
 * ## Interfaces
 * ### Provides
 * * IFilter
 * * ISeverityLevelFilter
 */
class SeverityLevelFilter :
    public ISeverityLevelFilter
{
    typedef SeverityLevelFilter  ThisClass;

public:
    SeverityLevelFilter(void) :
        acceptedLevels_(LOG_ALL)
    {}

    SeverityLevelFilter(uint32_t acceptedLevels) :
        acceptedLevels_(acceptedLevels)
    {}

    virtual ~SeverityLevelFilter(void) {}

    virtual FilterDecision Decide(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;
    virtual void ToggleAccept(uint32_t severityLevels) NSFX_OVERRIDE;
    virtual void ToggleDecline(uint32_t severityLevels) NSFX_OVERRIDE;
    virtual void SetAcceptedLevels(uint32_t severityLevels) NSFX_OVERRIDE;
    virtual uint32_t GetAcceptedLevels(void) NSFX_OVERRIDE;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IFilter)
        NSFX_INTERFACE_ENTRY(ISeverityLevelFilter)
    NSFX_INTERFACE_MAP_END()

private:
    uint32_t acceptedLevels_;
};


NSFX_REGISTER_CLASS(SeverityLevelFilter, "edu.uestc.nsfx.log.SeverityLevelFilter");


////////////////////////////////////////////////////////////////////////////////
inline FilterDecision SeverityLevelFilter::Decide(const std::shared_ptr<Record>& record)
{
    FilterDecision decision = ACCEPT;
    record->VisitIfExists(
        SeverityLevelInfo::GetName(),
        [&] (const AttributeValue& value) {
            if ((value.Get<typename SeverityLevelInfo::Type>() & acceptedLevels_) == 0)
            {
                decision = DECLINE;
            }
    });
    return decision;
}

inline void SeverityLevelFilter::ToggleAccept(uint32_t severityLevels)
{
    acceptedLevels_ |= severityLevels;
}

inline void SeverityLevelFilter::ToggleDecline(uint32_t severityLevels)
{
    acceptedLevels_ &= ~severityLevels;
}

inline void SeverityLevelFilter::SetAcceptedLevels(uint32_t severityLevels)
{
    acceptedLevels_ = severityLevels;
}

inline uint32_t SeverityLevelFilter::GetAcceptedLevels(void)
{
    return acceptedLevels_;
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7

