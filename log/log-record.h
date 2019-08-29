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
#include <nsfx/log/log-value.h>
#include <nsfx/log/log-value-traits.h>
#include <nsfx/log/exception.h>
#include <boost/type_index.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <string>
#include <memory> // move
#include <type_traits> // decay


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The implementation of a log record.
 */
class LogRecordImpl :
    public boost::intrusive_ref_counter<LogRecordImpl>
{
    friend class LogRecord;

    typedef unordered_map<std::string, LogValue>  ContainerType;

private:
    LogRecordImpl(void) BOOST_NOEXCEPT {}

private:
    /**
     * @brief Add a named value.
     * @return If there's already a value with the same name,
     *         this function returns \c false.
     */
    bool Add(const std::string& name, LogValue value);

    /**
     * @brief Add or replace a named value.
     *
     * If named value exists, the value is replaced.
     */
    void Update(const std::string& name, LogValue value);

    /**
     * @brief Check the existance of a named value.
     *
     * @param[in] name The name of the value.
     */
    bool Exists(const std::string& name) const;

    /**
     * @brief Get the named value.
     *
     * @param[in] name The name of the value.
     *
     * @throw LogValueNotFound
     */
    const LogValue& Get(const std::string& name) const;

private:
    ContainerType  items_;

};


////////////////////////////////////////
inline bool LogRecordImpl::Add(const std::string& name, LogValue value)
{
    auto result = items_.emplace(name, std::move(value));
    return result.second;
}

inline void LogRecordImpl::Update(const std::string& name, LogValue value)
{
    auto result = items_.emplace(name, value);
    if (!result.second)
    {
        auto& it = result.first;
        it->second = std::move(value);
    }
}

inline bool LogRecordImpl::Exists(const std::string& name) const
{
    return !!items_.count(name);
}

inline const LogValue& LogRecordImpl::Get(const std::string& name) const
{
    auto it = items_.find(name);
    if (it != items_.cend())
    {
        const LogValue& value = it->second;
        return value;
    }
    else
    {
        BOOST_THROW_EXCEPTION(
            LogValueNotFound() <<
            ErrorMessage("Cannot find the log value."));
    }
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief A log record.
 *
 * A log record carries a set of named values.
 */
class LogRecord
{
public:
    LogRecord(void);

private:
    struct CopyTag {};
    LogRecord(LogRecordImpl* rhs, CopyTag);

    // Copyable.
public:
    LogRecord(const LogRecord& rhs);
    LogRecord& operator=(const LogRecord& rhs);

    // Movable.
public:
    LogRecord(const LogRecord&& rhs);
    LogRecord& operator=(const LogRecord&& rhs);

    // Name-value pair.
public:
    /**
     * @brief Add a named value.
     * @return If there's already a value with the same name,
     *         this function returns \c false.
     */
    bool Add(const std::string& name, LogValue value);

    /**
     * @brief Add or replace a named value.
     *
     * If named value exists, the value is replaced.
     */
    void Update(const std::string& name, LogValue value);

    /**
     * @brief Check the existance of a named value.
     *
     * @param[in] name The name of the value.
     */
    bool Exists(const std::string& name) const;

    /**
     * @brief Get the named value.
     *
     * @tparam T  The type of the value.
     * @param[in] name The name of the value.
     *
     * @throw LogValueNotFound
     * @throw LogValueTypeMismatch
     */
    template<class T>
    T Get(const std::string& name) const;

    /**
     * @brief Get the type info of the named value.
     *
     * @param[in] name The name of the value.
     *
     * @throw LogValueNotFound
     */
    const boost::typeindex::type_info&
    GetTypeId(const std::string& name) const;

    // Value traits.
public:
    /**
     * @brief Get the named value via an info class.
     *
     * @tparam LogValueTraits A traits class that is defined by
     *                        \c NSFX_DEFINE_LOG_VALUE_TRAITS().
     */
    template<class LogValueTraits>
    bool Exists(void) const;

    /**
     * @brief Get the named value via an info class.
     *
     * @tparam LogValueTraits A traits class that is defined by
     *                        \c NSFX_DEFINE_LOG_VALUE_TRAITS().
     */
    template<class LogValueTraits>
    typename LogValueTraits::Type Get(void) const;

    // Deep copy.
public:
    LogRecord Copy(void) const
    {
        return LogRecord(impl_.get(), CopyTag());
    }

public:
    template<class Visitor>
    class LogValueVisitorConcept
    {
    public:
        BOOST_CONCEPT_USAGE(LogValueVisitorConcept)
        {
            typename std::decay<Visitor>::type* visitor = nullptr;
            const LogValue* value = nullptr;
            (*visitor)(*value);
        }
    };

    /**
     * @brief Visit a log value.
     *
     * @tparam Visitor It must conforms to \c LogValueVisitorConcept.
     *                 i.e., it is a functor class that has the prototype of
     *                 <code>void(const LogValue& value)</code>.
     */
    template<class Visitor>
    void VisitIfExists(const std::string& name, Visitor&& visitor) const;

    // Properties.
private:
    boost::intrusive_ptr<LogRecordImpl>  impl_;

};


////////////////////////////////////////////////////////////////////////////////
inline LogRecord::LogRecord(void) :
    impl_(new LogRecordImpl)
{
}

inline LogRecord::LogRecord(LogRecordImpl* rhs, CopyTag) :
    impl_(new LogRecordImpl(*rhs))
{
}

inline LogRecord::LogRecord(const LogRecord& rhs) :
    impl_(rhs.impl_)
{
}

inline LogRecord& LogRecord::operator=(const LogRecord& rhs)
{
    if (this != &rhs)
    {
        impl_ = rhs.impl_;
    }
    return *this;
}

inline LogRecord::LogRecord(const LogRecord&& rhs) :
    impl_(std::move(rhs.impl_))
{
}

inline LogRecord& LogRecord::operator=(const LogRecord&& rhs)
{
    if (this != &rhs)
    {
        impl_ = std::move(rhs.impl_);
    }
    return *this;
}

inline bool LogRecord::Add(const std::string& name, LogValue value)
{
    return impl_->Add(name, std::move(value));
}

inline void LogRecord::Update(const std::string& name, LogValue value)
{
    return impl_->Update(name, std::move(value));
}

inline bool LogRecord::Exists(const std::string& name) const
{
    return impl_->Exists(name);
}

template<class T>
inline T LogRecord::Get(const std::string& name) const
{
    const LogValue& value = impl_->Get(name);
    return value.Get<T>();
}

inline const boost::typeindex::type_info&
LogRecord::GetTypeId(const std::string& name) const
{
    const LogValue& value = impl_->Get(name);
    return value.GetTypeId();
}

template<class LogValueTraits>
inline bool LogRecord::Exists(void) const
{
    static_assert(NsfxIsLogValueTraits<LogValueTraits>::value,
                  "Invalid LogValueTraits class.");
    return impl_->Exists(LogValueTraits::GetName());
}

template<class LogValueTraits>
inline typename LogValueTraits::Type LogRecord::Get(void) const
{
    static_assert(NsfxIsLogValueTraits<LogValueTraits>::value,
                  "Invalid LogValueTraits class.");
    return Get<LogValueTraits::Type>(LogValueTraits::GetName());
}

template<class Visitor>
inline void LogRecord::VisitIfExists(const std::string& name, Visitor&& visitor) const
{
    BOOST_CONCEPT_ASSERT((LogValueVisitorConcept<Visitor>));
    auto it = impl_->items_.find(name);
    if (it != impl_->items_.cend())
    {
        const LogValue& value = it->second;
        visitor(value);
    }
}


NSFX_CLOSE_NAMESPACE


#endif // RECORD_H__27491AE4_C2CF_4F26_BC06_B9C70D297396

