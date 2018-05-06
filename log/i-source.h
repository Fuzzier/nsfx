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

#ifndef I_SOURCE_H__8FC50935_7D83_4C87_B5E7_50C5337AA3BA
#define I_SOURCE_H__8FC50935_7D83_4C87_B5E7_50C5337AA3BA


#include <nsfx/log/config.h>
#include <nsfx/log/record.h>
#include <nsfx/log/attribute.h>
#include <nsfx/log/i-sink.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log source interface.
 */
class ISource :
    virtual public IObject
{
public:
    virtual ~ISource(void) BOOST_NOEXCEPT {}

    virtual bool AddAttribute(const std::string& name,
                              const Attribute& attribute) = 0;
    virtual void RemoveAttribute(const std::string& name) = 0;
    virtual void Commit(Ptr<ISink> sink) = 0;
};


NSFX_DEFINE_CLASS_UID(ISource, "edu.uestc.nsfx.log.ISource");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_SOURCE_H__8FC50935_7D83_4C87_B5E7_50C5337AA3BA

