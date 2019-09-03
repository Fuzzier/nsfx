/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOG_VALUE_H__8A7FF073_7A8B_4991_A621_74628E6FE9C9
#define LOG_VALUE_H__8A7FF073_7A8B_4991_A621_74628E6FE9C9


#include <nsfx/log/config.h>
#include <nsfx/log/exception.h>
#include <boost/type_index.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <utility> // move


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The type-neutral log value interface.
 */
class ILogValue :
    public boost::intrusive_ref_counter<ILogValue>
{
public:
    virtual ~ILogValue(void) BOOST_NOEXCEPT {}

    /**
     * @brief Get the type info of the log value.
     */
    virtual const boost::typeindex::type_info& GetTypeId(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The type-specific log value interfaces.
 *
 * Concrete log value classes <b>must</b> implement this interface.
 */
template<class T>
class ITypedLogValue :
    public ILogValue
{
public:
    virtual ~ITypedLogValue(void) BOOST_NOEXCEPT {}

    virtual const boost::typeindex::type_info&
    GetTypeId(void) NSFX_OVERRIDE NSFX_FINAL
    {
        return boost::typeindex::type_id<T>().type_info();
    }

    /**
     * @brief Get the stored value.
     *
     * @throw LogValueTypeMismatch
     */
    virtual T Get(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Log values are carried by log records.
 *
 * An \c LogValue stores concrete value.
 */
class LogValue
{
    // Xtructors.
public:
    template<class T>
    explicit LogValue(ITypedLogValue<T>* p);

    // Methods.
public:
    const boost::typeindex::type_info& GetTypeId(void) const;

    template<class T>
    T Get(void) const;

    // Properties.
private:
    boost::intrusive_ptr<ILogValue> p_;
};


////////////////////////////////////////////////////////////////////////////////
// LogValue
template<class T>
inline LogValue::LogValue(ITypedLogValue<T>* p) :
    p_(p)
{
    if (!p_)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
}

inline const boost::typeindex::type_info&
LogValue::GetTypeId(void) const
{
    return p_->GetTypeId();
}

template<class T>
inline T LogValue::Get(void) const
{
    if (p_->GetTypeId() != boost::typeindex::type_id<T>())
    {
        BOOST_THROW_EXCEPTION(
            LogValueTypeMismatch() <<
            LogValueTypeErrorInfo(p_->GetTypeId()) <<
            QueriedLogValueTypeErrorInfo(boost::typeindex::type_id<T>()) <<
            ErrorMessage("Cannot access the log value, since "
                         "the requested type mismatches the value type."));
    }
    typedef ITypedLogValue<T>  Typed;
    Typed* p = static_cast<Typed*>(p_.get());
    return p->Get();
}


NSFX_CLOSE_NAMESPACE


#endif // LOG_VALUE_H__8A7FF073_7A8B_4991_A621_74628E6FE9C9

