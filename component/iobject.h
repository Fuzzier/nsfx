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

#ifndef IOBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8
#define IOBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8


#include <nsfx/component/config.h>
#include <nsfx/component/uuid.h>
#include <nsfx/component/exception.h>
#include <boost/type_traits/is_base_of.hpp>


NSFX_OPEN_NAMESPACE


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
    virtual ~IObject(void) {}

public:
    virtual refcount_t AddRef(void) BOOST_NOEXCEPT = 0;
    virtual refcount_t Release(void) BOOST_NOEXCEPT = 0;
    /**
     * @brief Query an interface of the object.
     *
     * @param iid The uuid of the interface.
     *
     * @return A pointer to the interface.<br/>
     *         Use \c static_cast to convert it to a pointer of the actual
     *         interface type.
     *         Users must make sure the interface type is correct.<br/>
     *         As a design pattern, the \c iid of an interface type is equal to
     *         the return value of <code>uuid_of(type*)</code>.
     *         <p>
     *         If \c nullptr is returned, the object does not have the queried
     *         interface.
     */
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT = 0;

}; // class IObject /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IObject, 0, 0, 0, 0LL);


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
                  "The type is not an IObject");

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
 * @see \c NSFX_DEFINE_CLASS_UUID4.
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


#endif // IOBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8

