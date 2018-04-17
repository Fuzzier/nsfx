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
 * @tparam ObjectImpl A class that conforms to \c ObjectImplConcept.
 *
 * Objects created by a factory are enveloped in \c Object or \c AggObject
 * according to whether a controller is specified.<br/>
 *
 * The specialized class template conforms to \c ObjectImplConcept.<br/>
 * Thus, it shall be used in conjunction with \c Object or \c AggObject.<br/>
 */
template<class ObjectImpl>
class ClassFactory :/*{{{*/
    public IClassFactory
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<ObjectImpl>));

public:
    virtual ~ClassFactory(void) BOOST_NOEXCEPT {}

public:
    // IClassFactory /*{{{*/
    virtual void* CreateObject(const Uid& iid, IObject* controller) NSFX_FINAL NSFX_OVERRIDE
    {
        if (controller && iid != uid_of<IObject>())
        {
            BOOST_THROW_EXCEPTION(BadAggregation());
        }
        return controller ? CreateAggregable(controller)
                          : CreateNonAggregable(iid);
    }

    void* CreateNonAggregable(const Uid& iid)
    {
        typedef Object<ObjectImpl>  ObjectClass;
        ObjectClass* o = new ObjectClass;
        try
        {
            void* result = o->QueryInterface(iid);
            return result;
        }
        catch (NoInterface& )
        {
            delete o;
            throw;
        }
    }

    void* CreateAggregable(IObject* controller)
    {
        typedef AggObject<ObjectImpl>  ObjectClass;
        ObjectClass* o = new ObjectClass(controller);
        return o->QueryInterface(uid_of<IObject>());
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(ClassFactory)
        NSFX_INTERFACE_ENTRY(IClassFactory)
    NSFX_INTERFACE_MAP_END()

}; // class ClassFactory /*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CLASS_FACTORY_H__9BDAEBA3_E910_4BA6_8216_41E659E75C96

