/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-07
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_COLLECTION_H__BEAC510C_4B86_4607_AB3C_333C035B895E
#define ATTRIBUTE_COLLECTION_H__BEAC510C_4B86_4607_AB3C_333C035B895E


#include <nsfx/log/config.h>
#include <nsfx/log/i-attribute-collection.h>
#include <nsfx/event/event.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief A collection of attributes.
 */
class AttributeCollection :
    virtual public IObject
{
public:
    virtual ~AttributeCollection(void) {}

    virtual bool Add(const std::string& name, const Attribute& attribute) NSFX_OVERRIDE;
    virtual void Remove(const std::string& name) NSFX_OVERRIDE;
    virtual void Clear(void) NSFX_OVERRIDE;

private:
    unordered_map<std::string, Attribute>  map_;
};


////////////////////////////////////////////////////////////////////////////////
inline bool AttributeCollection::Add(const std::string& name,
                                     const Attribute& attribute)
{
    auto result = map_.emplace(name, attribute);
    return result.second;
}

inline void AttributeCollection::Remove(const std::string& name)
{
    map_.erase(name);
}

inline void AttributeCollection::Clear(void)
{
    map_.clear()
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_COLLECTION_H__BEAC510C_4B86_4607_AB3C_333C035B895E

