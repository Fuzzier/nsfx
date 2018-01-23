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

    NSFX_DEFINE_CLASS_UUID(ITest, 0, 0, 0, 1LL);

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

    NSFX_DEFINE_CLASS_UUID(Test, 0, 0, 1, 1LL);

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
    typedef nsfx::Object<TestFactory> TestFactoryType;

    NSFX_TEST_CASE(Direct)/*{{{*/
    {
        nsfx::IClassRegistry* registry =
                nsfx::ClassRegistry::GetIClassRegistry();
        NSFX_TEST_ASSERT(registry);

        // Register with empty factory (throw).
        try
        {
            registry->Register(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::InvalidPointer& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Get unregistered factory (throw).
        try
        {
            registry->GetClassFactory(nsfx::uuid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Register.
        nsfx::Ptr<nsfx::IClassFactory> factory;
        try
        {
            factory = new TestFactoryType;
            registry->Register(nsfx::uuid_of<Test>(), factory);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Register already registered (throw).
        try
        {
            registry->Register(nsfx::uuid_of<Test>(), factory);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassIsRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Get registered factory.
        nsfx::Ptr<nsfx::IClassFactory> f;
        try
        {
            f = registry->GetClassFactory(nsfx::uuid_of<Test>());
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        NSFX_TEST_ASSERT(f);
        NSFX_TEST_EXPECT(factory == f);

        // Use factory to create objects.
        try
        {
            // Create a non-aggregable object via class factory.
            nsfx::IObject* o = static_cast<nsfx::IObject*>(
                    f->CreateObject(nsfx::uuid_of<nsfx::IObject>(),
                                          nullptr));
            nsfx::Ptr<nsfx::IObject> p(o);

            // Create a non-aggregable object via class factory.
            ITest* t = static_cast<ITest*>(
                    f->CreateObject(nsfx::uuid_of<ITest>(),
                                          nullptr));
            nsfx::Ptr<ITest> q(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Unregister.
        registry->Unregister(nsfx::uuid_of<Test>());
        try
        {
            registry->GetClassFactory(nsfx::uuid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Unregister all.
        try
        {
            registry->Register(nsfx::uuid_of<Test>(), factory);
            registry->UnregisterAll();
            registry->GetClassFactory(nsfx::uuid_of<Test>());
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(ViaFunctions)/*{{{*/
    {
        // Register class with default factory.
        try
        {
            nsfx::RegisterClass<Test>();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Create object.
        nsfx::Ptr<ITest> t;
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Unregister class (template-based).
        nsfx::UnregisterClass<Test>();
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Register a class with a custom factory.
        try
        {
            nsfx::Ptr<nsfx::IClassFactory> factory(new TestFactoryType);
            nsfx::RegisterClass(nsfx::uuid_of<Test>(), factory);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Create object.
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(t);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Unregister class.
        nsfx::UnregisterClass(nsfx::uuid_of<Test>());
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

        // Unregister all classes.
        try
        {
            nsfx::RegisterClass<Test>();
            nsfx::UnregisterAllClasses();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        try
        {
            t = nsfx::CreateObject<ITest>(nsfx::uuid_of<Test>(), nullptr);
            NSFX_TEST_EXPECT(false);
        }
        catch (nsfx::ClassNotRegistered& )
        {
            // Should come here.
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

