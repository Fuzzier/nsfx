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

#ifndef SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7
#define SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7


#include <nsfx/log/config.h>
#include <nsfx/log/filter/filter.h>
#include <nsfx/log/filter/severity-level-filter.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
Ptr<ISeverityLevelFilter> CreateSeverityLevelFilter(uint32_t acceptedLevels);


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief A severity level filter.
 *
 * ## Interfaces
 * ### Provides
 * * IFilter
 * * ISeverityLevelFilter
 */
class SeverityLevelFilter :
    public ISeverityLevelFilter
{
public:
    SeverityLevelFilter(void) :
        acceptedLevels_(LOG_ALL)
    {}

    SeverityLevelFilter(acceptedLevels) :
        acceptedLevels_(acceptedLevels)
    {}

    virtual ~SeverityLevel(void) {}

    virtual uint32_t Decide(const std::shared_ptr<Record>& record) NSFX_OVERRIDE;
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


////////////////////////////////////////////////////////////////////////////////
inline uint32_t SeverityLevelFilter::Decide(const std::shared_ptr<Record>& record)
{
    uint32_t decision = ACCEPT;
    record->VisitIfExists(
        "SeverityLevel",
        [&] (const AttributeValue& value) {
            if (value->Get<uint32_t>() & acceptedLevels_ == 0)
            {
                decision = DECLINE;
            }
    });
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


////////////////////////////////////////////////////////////////////////////////
inline Ptr<ISeverityLevelFilter>
CreateSeverityLevelFilter(uint32_t acceptedLevels)
{
    return Ptr<ISeverityLevelFilter>(
        new Object<SeverityLevelFilter>(acceptedLevels));
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // SEVERITY_LEVEL_FILTER_H__5B825859_0E58_4502_B2C0_95965773F9E7

