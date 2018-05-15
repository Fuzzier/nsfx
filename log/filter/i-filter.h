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

#ifndef I_FILTER_H__B74A4CA2_2091_481B_88E9_0A88CF85805F
#define I_FILTER_H__B74A4CA2_2091_481B_88E9_0A88CF85805F


#include <nsfx/log/config.h>
#include <nsfx/log/record/record.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <memory> // shared_ptr


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
enum FilterDecision
{
    /**
     * @brief Discard the log record.
     */
    DECLINE = 0x00000000,

    /**
     * @brief Keep processing the log record.
     */
    ACCEPT = 0x00000001
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log filter interface.
 */
class IFilter :
    virtual public IObject
{
public:
    virtual ~IFilter(void) BOOST_NOEXCEPT {}

    /**
     * @brief Filter a log record.
     *
     * @return \c FilterDecision.
     */
    virtual uint32_t Decide(const std::shared_ptr<Record>& record) = 0;
};


NSFX_DEFINE_CLASS_UID(IFilter, "edu.uestc.nsfx.log.IFilter");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_FILTER_H__B74A4CA2_2091_481B_88E9_0A88CF85805F

