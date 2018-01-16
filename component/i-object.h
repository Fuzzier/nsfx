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
#include <nsfx/component/uuid.h>
#include <nsfx/component/exception.h>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/concept_check.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Error info.
typedef boost::error_info<struct tag_QuerySourceClass, std::string> QuerySourceClassErrorInfo;
typedef boost::error_info<struct tag_QuerySourceInterface, std::string> QuerySourceInterfaceErrorInfo;
typedef boost::error_info<struct tag_QueryTargetInterface, std::string> QueryTargetInterfaceErrorInfo;
typedef boost::error_info<struct tag_QueryTargetIid, uuid> QueryTargetIidErrorInfo;
typedef boost::error_info<struct tag_Class, std::string> ClassErrorInfo;
typedef boost::error_info<struct tag_ClassUuid, uuid> ClassUuidErrorInfo;
typedef boost::error_info<struct tag_Controller, class IObject*> ControllerErrorInfo;


////////////////////////////////////////////////////////////////////////////////
// IObject
/**
 * @ingroup Component
 * @brief The base interface of all components.
 *
 * @remarks Every interface should <b>virtual</b> inherit from it.
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
     * @param iid The uuid of the interface.
     *
     * @return A pointer to the interface that holds one reference count.<br/>
     *         Users is responsible to release the reference count.<br/>
     *         Users must use \c static_cast to convert it to a pointer of the
     *         actual interface type.<br/>
     *         Users must make sure the interface type is correct.<br/>
     *         As a design pattern, the \c iid of an interface type is equal to
     *         the return value of <code>uuid_of(type*)</code>.
     *
     * @throw NoInterface The queried interface is not supported.
     */
    virtual void* QueryInterface(const uuid& iid) = 0;

}; // class IObject /*}}}*/


NSFX_DEFINE_CLASS_UUID(IObject, 0, 0, 0, 0LL);


////////////////////////////////////////////////////////////////////////////////
// IObject concept.
/**
 * @ingroup Component
 * @brief IObject concept.
 *
 * @tparam T A class that is, extends or implements \c IObject.
 * @tparam hasUuid If \c true, the class must have an associated uuid.
 */
template<class T>
class IObjectConcept/*{{{*/
{
public:
    static_assert(boost::is_base_of<IObject, T>::value,
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
        T* p = static_cast<T*>(o->QueryInterface(uuid_of<IObject>()));
    }

}; // class IObjectConcept /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// HasUuid concept.
/**
 * @ingroup Component
 * @brief Has uuid concept.
 *
 * @tparam T A class that is associated with a uuid.
 *
 * @see \c NSFX_DEFINE_CLASS_UUID.
 */
template<class T>
class HasUuidConcept/*{{{*/
{
public:
    BOOST_CONCEPT_USAGE(HasUuidConcept)
    {
        HasUuid();
    }

    void HasUuid(void)
    {
        T* o = nullptr;
        uuid iid = uuid_of<T>(o);
    }

}; // class HasUuidConcept /*}}}*/


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IObject  NSFX_UUID_OF(::nsfx::IObject)


#endif // I_OBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8

