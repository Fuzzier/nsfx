/**
 * @file
 *
 * @brief Test ClassFactory.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-27
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/component/class-factory.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(ClassFactory)
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

    NSFX_TEST_CASE(Object)
    {
        typedef nsfx::ClassFactory<Test>  TestFactory;
        typedef nsfx::Object<TestFactory> TestFactoryClass;
        nsfx::Ptr<nsfx::IClassFactory> factory(new TestFactoryClass);
        try
        {
            // Create a non-aggregable object via class factory.
            nsfx::IObject* o = static_cast<nsfx::IObject*>(
                    factory->CreateObject(nsfx::uid_of<nsfx::IObject>(),
                                          nullptr));
            nsfx::Ptr<nsfx::IObject> p(o);

            // Create a non-aggregable object via class factory.
            ITest* t = static_cast<ITest*>(
                    factory->CreateObject(nsfx::uid_of<ITest>(),
                                          nullptr));
            nsfx::Ptr<ITest> q(t);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

    struct Foo : public nsfx::IObject
    {
        Foo(void)
        {
            typedef nsfx::ClassFactory<Test>  TestFactory;
            typedef nsfx::Object<TestFactory> TestFactoryClass;
            nsfx::Ptr<nsfx::IClassFactory> factory(new TestFactoryClass);
            try
            {
                // Create an aggregable object via class factory.
                t = static_cast<nsfx::IObject*>(
                    factory->CreateObject(nsfx::uid_of<nsfx::IObject>(),
                                          this));
            }
            catch (boost::exception& e)
            {
                std::cerr << diagnostic_information(e) << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }

            try
            {
                // Create an aggregable object via class factory:
                // throw nsfx::BadAggregation.
                t = static_cast<ITest*>(
                    factory->CreateObject(nsfx::uid_of<ITest>(),
                                          this));
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::BadAggregation& )
            {
                // Should come here.
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what() << std::endl;
            }
        }

        virtual ~Foo(void) {}

        NSFX_INTERFACE_MAP_BEGIN(Foo)
            NSFX_INTERFACE_AGGREGATED_ENTRY(ITest, &*t)
        NSFX_INTERFACE_MAP_END()

        nsfx::Ptr<IObject> t;
    };

    template<class C>
    struct Visitor
    {
        bool Visit(int);
        bool Visit(double);

        template<class V>
        void Visit(V&& v) {}
    };

    NSFX_TEST_CASE(AggObject)
    {
        struct Test
        {
            typedef Visitor<Test>  V;
        };

        typedef nsfx::ClassFactory<Foo>  FooFactory;
        typedef nsfx::Object<FooFactory> FooFactoryClass;
        nsfx::Ptr<nsfx::IClassFactory> factory(new FooFactoryClass);
        try
        {
            nsfx::Ptr<nsfx::IObject> o;
            o = static_cast<nsfx::IObject*>(
                    factory->CreateObject(nsfx::uid_of<nsfx::IObject>(),
                                          nullptr));

            nsfx::Ptr<ITest> t;
            t = static_cast<ITest*>(
                    factory->CreateObject(nsfx::uid_of<ITest>(),
                                          nullptr));
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

