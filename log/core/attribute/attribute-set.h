/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-07
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_SET_H__344E3C60_8A9F_4F1C_8853_B646AB59958C
#define ATTRIBUTE_SET_H__344E3C60_8A9F_4F1C_8853_B646AB59958C


#include <nsfx/log/config.h>
#include <nsfx/log/core/attribute/i-attribute-set.h>
#include <nsfx/component/object.h>
#include <boost/type_traits/decay.hpp>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief A set of attributes.
 */
class AttributeSet :
    public IAttributeSet
{
    typedef AttributeSet  ThisClass;

public:
    virtual ~AttributeSet(void) {}

    virtual bool Add(const std::string& name, const Attribute& attribute) NSFX_OVERRIDE;
    virtual void Remove(const std::string& name) NSFX_OVERRIDE;
    virtual void Clear(void) NSFX_OVERRIDE;

public:
    template<class Visitor>
    class NamedAttributeVisitorConcept
    {
    public:
        BOOST_CONCEPT_USAGE(NamedAttributeVisitorConcept)
        {
            typename boost::decay<Visitor>::type* visitor = nullptr;
            const std::string* name = nullptr;
            const Attribute* attribute = nullptr;
            (*visitor)(*name, *attribute);
        }
    };

    /**
     * @brief Visit the attributes.
     *
     * @tparam Visitor It must conforms to \c NamedAttributeVisitorConcept.
     *                 i.e., it is a functor class that has the prototype of
     *                 <code>void(const std::string& name,
     *                            const Attribute& attribute)</code>.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const;

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IAttributeSet)
    NSFX_INTERFACE_MAP_END()

private:
    unordered_map<std::string, Attribute>  map_;
};


////////////////////////////////////////////////////////////////////////////////
inline bool AttributeSet::Add(const std::string& name, const Attribute& attribute)
{
    auto result = map_.emplace(name, attribute);
    return result.second;
}

inline void AttributeSet::Remove(const std::string& name)
{
    map_.erase(name);
}

inline void AttributeSet::Clear(void)
{
    map_.clear();
}

template<class Visitor>
inline void AttributeSet::Visit(Visitor&& visitor) const
{
    BOOST_CONCEPT_ASSERT((NamedAttributeVisitorConcept<Visitor>));
    for (auto it = map_.cbegin(); it != map_.cend(); ++it)
    {
        const std::string& name = it->first;
        const Attribute& attribute = it->second;
        visitor(name, attribute);
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_SET_H__344E3C60_8A9F_4F1C_8853_B646AB59958C

