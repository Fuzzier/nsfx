/**
 * @file
 *
 * @brief Log support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE
#define ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE


#include <nsfx/log/config.h>
#include <nsfx/log/attribute-value/attribute-value.h>
#include <memory>  // shared_ptr
#include <utility> // move, forward


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief An attribute generates attribute values.
 */
class IAttribute
{
public:
    virtual ~IAttribute(void) BOOST_NOEXCEPT {}

    virtual AttributeValue GetValue(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Attributes are used by log sources to generate attribute values.
 *
 * \c Attribute object generates \c AttributeValue.
 */
class Attribute
{
public:
    Attribute(const std::shared_ptr<IAttribute>& attr) :
        attr_(attr)
    {
        if (!attr_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    Attribute(std::shared_ptr<IAttribute>&& attr) :
        attr_(std::move(attr))
    {
        if (!attr_)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
    }

    // Methods.
public:
    AttributeValue GetValue(void) const
    {
        return attr_->GetValue();
    }

    // Properties.
private:
    std::shared_ptr<IAttribute> attr_;
};


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_H__015B46E7_7444_4BDD_A1DE_4F70E1D8BBCE

