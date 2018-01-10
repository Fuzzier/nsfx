/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-26
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CLASS_FACTORY_H__9BDAEBA3_E910_4BA6_8216_41E659E75C96
#define CLASS_FACTORY_H__9BDAEBA3_E910_4BA6_8216_41E659E75C96


#include <nsfx/component/config.h>
#include <nsfx/component/i-class-factory.h>
#include <nsfx/component/object.h>
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ClassFactory
/**
 * @ingroup Component
 * @brief A class factory.
 *
 * @tparam Envelopable A class that conforms to \c EnvelopableConcept.
 *
 * Objects created by a factory are enveloped in \c Object or \c AggObject
 * according to whether a controller is specified.<br/>
 *
 * The specialized class template conforms to \c EnvelopableConcept.<br/>
 * Thus, it shall be used in conjunction with \c Object or \c AggObject.<br/>
 */
template<class Envelopable>
class ClassFactory :/*{{{*/
    public IClassFactory
{
private:
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<Envelopable>));

public:
    virtual ~ClassFactory(void) BOOST_NOEXCEPT {}

public:
    // IClassFactory /*{{{*/
    virtual void* CreateObject(const uuid& iid, IObject* controller) NSFX_FINAL NSFX_OVERRIDE
    {
        if (controller && iid != NSFX_IID_IObject)
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
        return controller ? CreateAggregable(controller)
                          : CreateNonAggregable(iid);
    }

    void* CreateNonAggregable(const uuid& iid)
    {
        typedef Object<Envelopable>  ObjectType;
        ObjectType* o = new ObjectType;
        void* result = o->QueryInterface(iid);
        if (!result)
        {
            delete o;
            BOOST_THROW_EXCEPTION(NoInterface());
        }
        return result;
    }

    void* CreateAggregable(IObject* controller)
    {
        typedef AggObject<Envelopable>  ObjectType;
        ObjectType* o = new ObjectType(controller);
        return o->QueryInterface(NSFX_IID_IObject);
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(ClassFactory)
        NSFX_INTERFACE_ENTRY(IClassFactory)
    NSFX_INTERFACE_MAP_END()

}; // class ClassFactory /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CLASS_FACTORY_H__9BDAEBA3_E910_4BA6_8216_41E659E75C96

