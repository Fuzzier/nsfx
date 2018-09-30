/**
 * @file
 *
 * @brief Test ClassRegistry.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-28
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/component/class-registry.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(ClassRegistry)
{
    using nsfx::refcount_t;

    struct ITest : virtual nsfx::IObject/*{{{*/
    {
        virtual ~ITest(void) {}

        virtual refcount_t GetRefCount(void) = 0;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(ITest, "edu.uestc.nsfx.test.ITest");

    struct Test : public ITest/*{{{*/
    {
        virtual ~Test(void) {}

        virtual refcount_t GetRefCount(void)
        {
            AddRef();
            return Release();
        }

        NSFX_INTERFACE_MAP_BEGIN(Test)
            NSFX_INTERFACE_ENTRY(ITest)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(Test, "edu.uestc.nsfx.test.Test");

    refcount_t RefCount(nsfx::IObject* p)/*{{{*/
    {
        refcount_t result = 0;
        if (p)
        {
            p->AddRef();
            result = p->Release();
        }
        return result;
    }/*}}}*/

    typedef nsfx::ClassFactory<Test>  TestFactory;
    typedef nsfx::Object<TestFactory> TestFactoryClass;

    NSFX_TEST_CASE(Direct)/*{{{*/
    {
        nsfx::IClassRegistry* registry =
                nsfx::ClassRegistry::GetIClassRegistry();
        NSFX_TEST_ASSERT(registry);

        // Register with empty factory (throw).
        try
        {
            registry->Register(nsfx::uid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::InvalidPointer& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Get unregistered factory (throw).
        try
        {
            registry->GetClassFactory(nsfx::uid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Register.
        nsfx::Ptr<nsfx::IClassFactory> factory;
        try
        {
            factory = new TestFactoryClass;
            registry->Register(nsfx::uid_of<Test>(), factory);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Register already registered (throw).
        try
        {
            registry->Register(nsfx::uid_of<Test>(), factory);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassIsRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Get registered factory.
        nsfx::Ptr<nsfx::IClassFactory> f;
        try
        {
            f = registry->GetClassFactory(nsfx::uid_of<Test>());
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        NSFX_TEST_ASSERT(f);
        NSFX_TEST_EXPECT(factory == f);

        // Use factory to create objects.
        try
        {
            // Create a non-aggregable object via class factory.
            nsfx::IObject* o = static_cast<nsfx::IObject*>(
                    f->CreateObject(nsfx::uid_of<nsfx::IObject>(),
                                          nullptr));
            nsfx::Ptr<nsfx::IObject> p(o);

            // Create a non-aggregable object via class factory.
            ITest* t = static_cast<ITest*>(
                    f->CreateObject(nsfx::uid_of<ITest>(),
                                          nullptr));
            nsfx::Ptr<ITest> q(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Unregister.
        registry->Unregister(nsfx::uid_of<Test>());
        try
        {
            registry->GetClassFactory(nsfx::uid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Unregister all.
        try
        {
            registry->Register(nsfx::uid_of<Test>(), factory);
            registry->UnregisterAll();
            registry->GetClassFactory(nsfx::uid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

    }/*}}}*/

    NSFX_TEST_CASE(ViaFunctions)/*{{{*/
    {
        // Register class with default factory.
        try
        {
            nsfx::RegisterClassFactory<Test>(nsfx::uid_of<Test>());
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Create object.
        nsfx::Ptr<ITest> t;
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Unregister class (template-based).
        nsfx::UnregisterClassFactory(nsfx::uid_of<Test>());
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Register a class with a custom factory.
        try
        {
            nsfx::Ptr<nsfx::IClassFactory> factory(new TestFactoryClass);
            nsfx::RegisterClassFactory(nsfx::uid_of<Test>(), factory);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Create object.
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // Unregister class.
        nsfx::UnregisterClassFactory(nsfx::uid_of<Test>());
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }

        // // Unregister all classes.
        // try
        // {
        //     nsfx::RegisterClassFactory<Test>(nsfx::uid_of<Test>());
        //     nsfx::UnregisterAllClassFactories();
        // }
        // catch (boost::exception& e)
        // {
        //     NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        // }
        // try
        // {
        //     t = nsfx::CreateObject<ITest>(nsfx::uid_of<Test>(), nullptr);
        //     NSFX_TEST_EXPECT(false);
        // }
        // catch (nsfx::ClassNotRegistered& )
        // {
        //     // Should come here.
        // }
        // catch (boost::exception& e)
        // {
        //     NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        // }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

