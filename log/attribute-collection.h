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
#include <boost/type_traits/decay.hpp>


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

public:
    template<class Visitor>
    class AttributeVisitorConcept
    {
    public:
        BOOST_CONCEPT_USAGE(AttributeVisitorConcept)
        {
            typename boost::type_traits::decay<Visitor>::type* visitor = nullptr;
            const std::string* name = nullptr;
            const Attribute* attribute = nullptr;
            (*visitor)(*name, *attribute);
        }
    };

    /**
     * @brief Visit the attributes.
     *
     * @tparam Visitor A functor class that has the prototype of
     *                 \c void(const std::string& name, const Attribute& attribute).
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const;

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

template<class Visitor>
inline void Visit(Visitor&& visitor) const
{
    BOOST_CONCEPT_ASSERT((AttributeVisitorConcept<Visitor>));
    for (auto it = map_.cbegin(); it != map_.cend(); ++it)
    {
        const std::string& name = it->first;
        const Attribute& attribute = it->second;
        visitor(name, attribute);
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_COLLECTION_H__BEAC510C_4B86_4607_AB3C_333C035B895E

