/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842
#define I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842


#include <nsfx/log/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/log/record.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log sink interface.
 */
class ISink :
    virtual public IObject
{
public:
    virtual ~ISink(void) BOOST_NOEXCEPT {}

    virtual void Commit(Record&& record) = 0;
};


NSFX_DEFINE_CLASS_UID(ISink, "edu.uestc.nsfx.log.ISink");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_SINK_H__66B958B1_3770_42CC_B75C_14D7498C4842

