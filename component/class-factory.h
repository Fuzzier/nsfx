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
#include <memory> // unique_ptr


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ClassFactory
/**
 * @ingroup Component
 * @brief A class factory.
 *
 * @tparam T A class that conforms to `ObjectImplConcept`.
 *
 * The factory uses `Object` or `AggObject` to make `T` a concrete class
 * according to whether a controller is specified.
 *
 * The specialized class template conforms to `ObjectImplConcept`.
 * Thus, it shall be used in conjunction with `Object` or `AggObject`.
 */
template<class T>
class ClassFactory :
    public IClassFactory
{
private:
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<T>));

public:
    virtual ~ClassFactory(void) BOOST_NOEXCEPT {}

public:
    // IClassFactory
    virtual Ptr<IObject> CreateObject(IObject* controller) NSFX_FINAL NSFX_OVERRIDE;

private:
    Ptr<IObject> CreateNonAggregable(void);
    Ptr<IObject> CreateAggregable(IObject* controller);

private:
    NSFX_INTERFACE_MAP_BEGIN(ClassFactory)
        NSFX_INTERFACE_ENTRY(IClassFactory)
    NSFX_INTERFACE_MAP_END()

};


////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<IObject>
ClassFactory<T>::CreateObject(IObject* controller)
{
    return controller ? CreateAggregable(controller)
                      : CreateNonAggregable();
}

template<class T>
inline Ptr<IObject>
ClassFactory<T>::CreateNonAggregable(void)
{
    typedef Object<T>  ObjectClass;
    IObject* o = new ObjectClass;
    return o;
}

template<class T>
inline Ptr<IObject>
ClassFactory<T>::CreateAggregable(IObject* controller)
{
    typedef AggObject<T>  ObjectClass;
    IObject* o = new ObjectClass(controller);
    return o;
}


NSFX_CLOSE_NAMESPACE


#endif // CLASS_FACTORY_H__9BDAEBA3_E910_4BA6_8216_41E659E75C96

