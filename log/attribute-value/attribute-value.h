/**
 * @file
 *
 * @brief Logging support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
 * @date    2018-05-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B
#define ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B


#include <nsfx/log/config.h>
#include <boost/type_index.hpp>
#include <memory>  // shared_ptr
#include <utility> // move, forward


NSFX_LOG_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The type-neutral attribute value interface.
 */
class IAttributeValue
{
public:
    virtual ~IAttributeValue(void) BOOST_NOEXCEPT {}

    virtual const boost::typeindex::type_index& GetTypeId(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief The type-specific attribute value interfaces.
 *
 * Concrete attribute value classes must implement this interface.
 */
template<class T>
class ITypedAttributeValue :
    public IAttributeValue
{
public:
    virtual ~ITypedAttributeValue(void) BOOST_NOEXCEPT {}

    virtual const boost::typeindex::type_index& GetTypeId(void) NSFX_OVERRIDE NSFX_FINAL
    {
        return boost::typeindex::type_id<T>();
    }

    virtual const T& Get(void) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Log
 * @brief Attribute values are carried by log records.
 *
 * An \c AttributeValue stores concrete value.
 */
class AttributeValue
{
    // Xtructors.
public:
    template<class T>
    AttributeValue(const std::shared_ptr<ITypedAttributeValue<T> >& value);

    template<class T>
    AttributeValue(std::shared_ptr<ITypedAttributeValue<T> >&& value);

    // Methods.
public:
    const boost::typeindex::type_index& GetTypeId(void) const;

    template<class T>
    const T& Get(void) const;

    // Properties.
private:
    std::shared_ptr<IAttributeValue> value_;
};


////////////////////////////////////////////////////////////////////////////////
// AttributeValue
template<class T>
inline AttributeValue::AttributeValue(
    const std::shared_ptr<ITypedAttributeValue<T> >& value) :
        value_(value)
{
    if (!value_)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
}

template<class T>
inline AttributeValue::AttributeValue(
    std::shared_ptr<ITypedAttributeValue<T> >&& value) :
        value_(std::move(value))
{
    if (!value_)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
}

inline const boost::typeindex::type_index&
AttributeValue::GetTypeId(void) const
{
    BOOST_ASSERT(!!value_);
    return value_->GetTypeId();
}

template<class T>
inline const T& AttributeValue::Get(void) const
{
    if (value_->GetTypeId() != boost::typeindex::type_id<T>())
    {
        BOOST_THROW_EXCEPTION(
            IllegalMethodCall() <<
            ErrorMessage("Cannot access the log attribute value, since "
                         "the requested type mismatches the value type."));
    }
    return static_cast<ITypedAttributeValue<T>*>(value_.get())->Get();
}



NSFX_LOG_CLOSE_NAMESPACE


#endif // ATTRIBUTE_VALUE_H__5D31C8F6_9FF7_4FA8_9DC4_7DDF3716CE1B

