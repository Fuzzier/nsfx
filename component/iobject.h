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


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IObject
/**
 * @ingroup Component
 * @brief The base interface of all components.
 */
class IObject /*{{{*/
{
public:
    NSFX_DEFINE_CLASS_UUID4(0, 0, 0, 0LL);

public:
    virtual ~IObject(void) {}

public:
    virtual uint32_t AddRef(void) BOOST_NOEXCEPT = 0;
    virtual uint32_t Release(void) BOOST_NOEXCEPT = 0;
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
     *
     * @remarks This method may throw exceptions to indicate errors.
     */
    virtual void* QueryInterface(const uuid& iid) = 0;

}; // class IObject /*}}}*/


////////////////////////////////////////////////////////////////////////////////
// IObject concept
/**
 * @ingroup Component
 * @brief IObject concept.
 *
 * @tparam T A class that extends or implements \c IObject.
 */
template<class T>
class IObjectConcept
{
public:
    BOOST_CONCEPT_USAGE(IObjectConcept)
    {
        T* o = nullptr;
        uuid iid = uuid_of(o);
        uint32_t r1 = o->AddRef();
        uint32_t r2 = o->Release();
        T* p = static_cast<T*>(o->QueryInterface(iid));
    }
};


NSFX_CLOSE_NAMESPACE


#endif // IOBJECT_H__6ADF3ED4_FCD6_4E8F_B847_B9596FBA75E8

