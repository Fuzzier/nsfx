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
            nsfx::Ptr<nsfx::IObject> o = factory->CreateObject(nullptr);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            nsfx::Ptr<ITest> q(o);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 2);
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
                t = factory->CreateObject(this);
                NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 1);
            }
            catch (boost::exception& e)
            {
                std::cerr << diagnostic_information(e) << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
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
            nsfx::Ptr<nsfx::IObject> o = factory->CreateObject(nullptr);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            nsfx::Ptr<ITest> t(o);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 2);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

