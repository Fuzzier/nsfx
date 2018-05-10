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

#ifndef I_ATTRIBUTE_COLLECTION_H__1909CBE7_F24A_46D6_AAEB_3319895C4C54
#define I_ATTRIBUTE_COLLECTION_H__1909CBE7_F24A_46D6_AAEB_3319895C4C54


#include <nsfx/log/config.h>
#include <nsfx/log/attribute/attribute.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief The attribute collection interface.
 */
class IAttributeCollection :
    virtual public IObject
{
public:
    virtual ~IAttributeCollection(void) BOOST_NOEXCEPT {}

    virtual bool Add(const std::string& name, const Attribute& attribute) = 0;
    virtual void Remove(const std::string& name) = 0;
    virtual void Clear(void) = 0;
};


NSFX_DEFINE_CLASS_UID(IAttributeCollection, "edu.uestc.nsfx.log.IAttributeCollection");


NSFX_LOG_CLOSE_NAMESPACE


#endif // I_ATTRIBUTE_COLLECTION_H__1909CBE7_F24A_46D6_AAEB_3319895C4C54

