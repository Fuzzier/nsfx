/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-06
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef I_ATTRIBUTE_SET_H__2D9B41B8_2437_4FA4_BABF_AEC7CEDA9A87
#define I_ATTRIBUTE_SET_H__2D9B41B8_2437_4FA4_BABF_AEC7CEDA9A87


#include <nsfx/log/config.h>
#include <nsfx/log/core/attribute/attribute.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>
#include <string>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The attribute set interface.
 */
class IAttributeSet :
    virtual public IObject
{
public:
    virtual ~IAttributeSet(void) BOOST_NOEXCEPT {}

    virtual bool Add(const std::string& name, const Attribute& attribute) = 0;
    virtual void Remove(const std::string& name) = 0;
    virtual void Clear(void) = 0;
};


NSFX_DEFINE_CLASS_UID(IAttributeSet, "edu.uestc.nsfx.log.IAttributeSet");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_ATTRIBUTE_SET_H__2D9B41B8_2437_4FA4_BABF_AEC7CEDA9A87

