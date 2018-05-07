/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396
#define RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396


#include <nsfx/log/config.h>
#include <nsfx/log/attribute-value.h>
#include <nsfx/log/attribute.h>
#include <boost/container_hash/hash.hpp>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief Log record.
 *
 * A log record carries a set of attributes, such as
 * 1) a severity level
 * 2) a message
 * 3) a timestamp
 * 4) the file name
 * 5) the line number
 * 6) the function name
 * 7) a scope name
 * etc.
 */
class Record
{
public:
    Record(void) BOOST_NOEXCEPT {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Record(const Record&));
    BOOST_DELETED_FUNCTION(Record& operator=(const Record&));

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

    /**
     * @brief Get the named value.
     *
     * @tparam T    The type of the attribute value.
     * @param name  The name of the attribute value.
     *
     * @throw \c AttributeNotFound
     */
    template<class T>
    const T& Get(const std::string& name) const;

    // Properties.
private:
    unordered_map<std::string, AttributeValue>  values_;
};


////////////////////////////////////////////////////////////////////////////////
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
            AttributeNotFound() <<
            ErrorMessage("Cannot find the log attribute value."));
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396

