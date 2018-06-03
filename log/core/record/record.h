/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-11
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396
#define RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396


#include <nsfx/log/config.h>
#include <nsfx/log/core/record/attribute-value-info.h>
#include <nsfx/log/core/attribute-value/attribute-value.h>
#include <nsfx/log/core/exception.h>
#include <boost/type_traits/decay.hpp>
#include <string>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief Log record.
 *
 * A log record carries a set of named values.
 *
 * The default logger provides the following named values by default.
 * * severity level
 * * message
 * * function name
 * * file name
 * * line number
 *
 * To support <i>timestamp</i>, users shall add a \c TimestampAttribute to the
 * logger via the \c IAttributeSet interface.
 */
class Record
{
public:
    Record(void) BOOST_NOEXCEPT {}

    // Copyable.
public:
    Record(const Record& rhs);
    Record& operator=(const Record& rhs);

    // Methods.
public:
    /**
     * @brief Add a named value.
     * @return If there's already an attribute value with the same name,
     *         this function returns \c false.
     */
    bool Add(const std::string& name, const AttributeValue& value);

    /**
     * @brief Add or replace a named value.
     *
     * If named value exists, the value is replaced.
     */
    void Update(const std::string& name, const AttributeValue& value);

    bool Exists(const std::string& name) const;

    /**
     * @brief Get the named value.
     *
     * @tparam T    The type of the attribute value.
     * @param name  The name of the attribute value.
     *
     * @throw \c AttributeValueNotFound
     */
    template<class T>
    const T& Get(const std::string& name) const;

    /**
     * @brief Get the named value via an info class.
     *
     * @tparam AttributeValueInfo An info class that is defined by
     *                            \c NSFX_DEFINE_ATTRIBUTE_VALUE_INFO().
     */
    template<class AttributeValueInfo>
    const typename AttributeValueInfo::Type& Get(void) const;

public:
    template<class Visitor>
    class AttributeValueVisitorConcept
    {
    public:
        BOOST_CONCEPT_USAGE(AttributeValueVisitorConcept)
        {
            typename boost::decay<Visitor>::type* visitor = nullptr;
            const AttributeValue* value = nullptr;
            (*visitor)(*value);
        }
    };

    /**
     * @brief Visit an attribute value.
     *
     * @tparam Visitor It must conforms to \c AttributeValueVisitorConcept.
     *                 i.e., it is a functor class that has the prototype of
     *                 <code>void(const AttributeValue& value)</code>.
     */
    template<class Visitor>
    void VisitIfExists(const std::string& name, Visitor&& visitor) const;

    // Properties.
private:
    unordered_map<std::string, AttributeValue>  values_;
};


////////////////////////////////////////////////////////////////////////////////
inline Record::Record(const Record& rhs) :
    values_(rhs.values_)
{
}

inline Record& Record::operator=(const Record& rhs)
{
    if (this != &rhs)
    {
        values_ = rhs.values_;
    }
    return *this;
}

inline bool Record::Add(const std::string& name, const AttributeValue& value)
{
    auto result = values_.emplace(name, value);
    return result.second;
}

inline void Record::Update(const std::string& name, const AttributeValue& value)
{
    auto result = values_.emplace(name, value);
    if (!result.second)
    {
        auto& it = result.first;
        it->second = value;
    }
}

inline bool Record::Exists(const std::string& name) const
{
    return values_.count(name);
}

template<class T>
inline const T& Record::Get(const std::string& name) const
{
    auto it = values_.find(name);
    if (it != values_.cend())
    {
        const AttributeValue& value = it->second;
        return value.Get<T>();
    }
    else
    {
        BOOST_THROW_EXCEPTION(
            AttributeValueNotFound() <<
            ErrorMessage("Cannot find the log attribute value."));
    }
}

template<class AttributeValueInfo>
inline const typename AttributeValueInfo::Type& Record::Get(void) const
{
    static_assert(NsfxIsAttributeValueInfo<AttributeValueInfo>::value,
                  "Invalid AttributeValueInfo class.");
    return Get<typename AttributeValueInfo::Type>(
                AttributeValueInfo::GetName());
}

template<class Visitor>
inline void Record::VisitIfExists(const std::string& name, Visitor&& visitor) const
{
    BOOST_CONCEPT_ASSERT((AttributeValueVisitorConcept<Visitor>));
    auto it = values_.find(name);
    if (it != values_.cend())
    {
        const AttributeValue& value = it->second;
        visitor(value);
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396

