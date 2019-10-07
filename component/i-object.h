/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-07
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_OBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8
#define I_OBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8


#include <nsfx/component/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/exception.h>
#include <boost/concept_check.hpp>
#include <type_traits> // is_base_of


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Error info.
typedef boost::error_info<struct tag_QueriedClass,         std::string>  QueriedClassErrorInfo;
typedef boost::error_info<struct tag_QueriedInterface,     std::string>  QueriedInterfaceErrorInfo;
typedef boost::error_info<struct tag_QueriedInterfaceUid,  Uid>          QueriedInterfaceUidErrorInfo;
typedef boost::error_info<struct tag_QueriedFromInterface, std::string>  QueriedFromInterfaceErrorInfo;

typedef boost::error_info<struct tag_Class,                std::string>  ClassErrorInfo;
typedef boost::error_info<struct tag_ClassUid,             Uid>          ClassUidErrorInfo;
typedef boost::error_info<struct tag_Interface,            std::string>  InterfaceErrorInfo;
typedef boost::error_info<struct tag_InterfaceUid,         Uid>          InterfaceUidErrorInfo;


////////////////////////////////////////////////////////////////////////////////
// IObject
/**
 * @ingroup Component
 * @brief The base interface of all components.
 *
 * @remarks Every interface **shall** *virtual* inherit from it.
 */
class IObject /*{{{*/
{
public:
    virtual ~IObject(void) BOOST_NOEXCEPT {}

public:
    virtual refcount_t AddRef(void) = 0;
    virtual refcount_t Release(void) = 0;

    /**
     * @brief Query an interface of the object.
     *
     * @param iid The UID of the interface.
     *
     * @return A pointer to the interface that holds one reference count.
     *         Users is responsible to release the reference count.
     *         Users must use `static_cast` to convert it to a pointer of the
     *         actual interface type.
     *         Users must make sure the interface type is correct.
     *         As a design pattern, the `iid` of an interface `IXxx` is
     *         equal to the return value of `uid_of<IXxx>()`.
     *
     * @throw NoInterface The queried interface is not supported.
     */
    virtual void* QueryInterface(const Uid& iid) = 0;

}; // class IObject /*}}}*/


NSFX_DEFINE_CLASS_UID(IObject, "edu.uestc.nsfx.IObject");


////////////////////////////////////////////////////////////////////////////////
// IObject concept.
/**
 * @ingroup Component
 * @brief IObject concept.
 *
 * @tparam T A type that is, extends or implements `IObject`.
 */
template<class T>
class IObjectConcept/*{{{*/
{
public:
    static_assert(std::is_base_of<IObject, T>::value,
                  "The type does not conform to IObjectConcept since it is not "
                  "derived from IObject");

public:
    BOOST_CONCEPT_USAGE(IObjectConcept)
    {
        IsIObject();
    }

    void IsIObject(void)
    {
        T* o = nullptr;
        refcount_t r1 = o->AddRef();
        refcount_t r2 = o->Release();
        T* p = static_cast<T*>(o->QueryInterface(uid_of<IObject>()));
    }

}; // class IObjectConcept /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// HasUid concept.
/**
 * @ingroup Component
 * @brief Has UID concept.
 *
 * @tparam T A class that is associated with a UID.
 *
 * @see `NSFX_DEFINE_CLASS_UID`.
 */
template<class T>
class HasUidConcept/*{{{*/
{
public:
    BOOST_CONCEPT_USAGE(HasUidConcept)
    {
        HasUid();
    }

    void HasUid(void)
    {
        const Uid& uid = uid_of<T>();
    }

}; // class HasUidConcept /*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class T>
struct IsObject : std::is_base_of<IObject, T> {};


NSFX_CLOSE_NAMESPACE


#endif // I_OBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8

