/**
 * @file
 *
 * @brief Log support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TIMESTAMP_ATTRIBUTE_H__1CDF8EFB_718E_4FE2_A0C5_DF7B5A5A8103
#define TIMESTAMP_ATTRIBUTE_H__1CDF8EFB_718E_4FE2_A0C5_DF7B5A5A8103


#include <nsfx/log/config.h>
#include <nsfx/log/core/attribute/attribute.h>
#include <nsfx/log/core/attribute-value/const-attribute-value.h>
#include <nsfx/log/core/record/attribute-value-info.h>
#include <nsfx/simulation/i-clock.h>
#include <memory>  // shared_ptr
#include <utility> // move, forward


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Make a timestamp attribute.
 */
Attribute MakeTimestampAttribute(Ptr<IClock> clock);


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief An attribute generates attribute values.
 */
class TimestampAttribute :
    public IAttribute
{
    typedef TimestampAttribute  ThisClass;

public:
    TimestampAttribute(Ptr<IClock> clock);

    virtual ~TimestampAttribute(void) {}

    virtual AttributeValue GetValue(void) NSFX_OVERRIDE;

private:
    Ptr<IClock> clock_;
};


////////////////////////////////////////////////////////////////////////////////
inline Attribute MakeTimestampAttribute(Ptr<IClock> clock)
{
    if (!clock)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    return Attribute(std::make_shared<TimestampAttribute>(clock));
}


////////////////////////////////////////////////////////////////////////////////
inline TimestampAttribute::TimestampAttribute(Ptr<IClock> clock) :
    clock_(clock)
{
}

inline AttributeValue TimestampAttribute::GetValue(void)
{
    return MakeConstantAttributeValue<TimePoint>(clock_->Now());
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The timestamp information class.
 */
NSFX_DEFINE_ATTRIBUTE_VALUE_INFO(TimestampInfo, "Timestamp", TimePoint);


NSFX_LOG_CLOSE_NAMESPACE


#endif // TIMESTAMP_ATTRIBUTE_H__1CDF8EFB_718E_4FE2_A0C5_DF7B5A5A8103

