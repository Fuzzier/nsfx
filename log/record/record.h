/**
 * @file
 *
 * @brief Log support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
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
#include <nsfx/log/attribute/attribute-value.h>


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 *
 * @brief Log record.
 *
 * A log record carries a set of named values, such as
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
    bool Add(const char* name, const AttributeValue& value);

    /**
     * @brief Add or replace a named value.
     *
     * If named value exists, the value is replaced.
     */
    void Update(const char* name, const AttributeValue& value);

    bool Exists(const char* name) const;

    /**
     * @brief Get the named value.
     *
     * @tparam T    The type of the attribute value.
     * @param name  The name of the attribute value.
     *
     * @throw \c AttributeValueNotFound
     */
    template<class T>
    const T& Get(const char* name) const;

    template<class Visitor>
    void VisitIfExists(const char* name, Visitor&& visitor) const;

    // Properties.
private:
    struct StringLiteralHash
    {
    };
    unordered_map<char*, AttributeValue>  values_;
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

template<class Visitor>
inline void VisitIfExists(const std::string& name, Visitor&& visitor) const
{
    auto it = values_.find(name);
    if (it != values_.cend())
    {
        const AttributeValue& value = it->second;
        visitor(value);
    }
}


NSFX_LOG_CLOSE_NAMESPACE


#endif // RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396

