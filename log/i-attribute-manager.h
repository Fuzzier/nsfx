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

#ifndef I_ATTRIBUTE_MANAGER_H__E4F1DEC9_82ED_4A86_89B1_FBB01B36659E
#define I_ATTRIBUTE_MANAGER_H__E4F1DEC9_82ED_4A86_89B1_FBB01B36659E


#include <nsfx/log/config.h>
#include <nsfx/log/attribute.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The log source interface.
 */
class IAttributeManager :
    virtual public IObject
{
public:
    virtual ~IAttributeManager(void) BOOST_NOEXCEPT {}

    virtual bool Add(const char* name, const Attribute& attribute) = 0;
    virtual void Remove(const char* name) = 0;
    virtual void Clear(void) = 0;
};


NSFX_DEFINE_CLASS_UID(IAttributeManager, "edu.uestc.nsfx.log.IAttributeManager");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_ATTRIBUTE_MANAGER_H__E4F1DEC9_82ED_4A86_89B1_FBB01B36659E

