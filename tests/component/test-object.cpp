/**
 * @file
 *
 * @brief Test Object and AggObject.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-19
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(Object)
{
    ////////////////////////////////////////////////////////////////////////////
    // Types.
    using nsfx::refcount_t;

    struct ITest :/*{{{*/
        virtual nsfx::IObject
    {
        virtual ~ITest(void) {}

        virtual refcount_t GetRefCount(void) = 0;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(ITest, "edu.uestc.nsfx.test.ITest");

    struct IFoobar :/*{{{*/
        virtual nsfx::IObject
    {
        virtual ~IFoobar(void) {}

        virtual refcount_t GetRefCount(void) = 0;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(IFoobar, "edu.uestc.nsfx.test.IFoobar");

    static bool deallocated = false;

    struct Test :/*{{{*/
        ITest
    {
        virtual ~Test(void)
        {
            deallocated = true;
        }

        virtual refcount_t GetRefCount(void)
        {
            AddRef();
            return Release();
        }

        int Internal(void) { return 0; }

        NSFX_INTERFACE_MAP_BEGIN(Test)
            NSFX_INTERFACE_ENTRY(ITest)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(Test, "edu.uestc.nsfx.test.Test");

    struct TestNoDefaultCtor :/*{{{*/
        ITest
    {
        TestNoDefaultCtor(int n) : n_(n) {}

        virtual ~TestNoDefaultCtor(void)
        {
            deallocated = true;
        }

        virtual refcount_t GetRefCount(void)
        {
            AddRef();
            return Release();
        }

        int Internal(void) { return 1; }

        NSFX_INTERFACE_MAP_BEGIN(TestNoDefaultCtor)
            NSFX_INTERFACE_ENTRY(ITest)
        NSFX_INTERFACE_MAP_END()

    private:
        int n_;

    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(TestNoDefaultCtor, "edu.uestc.nsfx.test.TestNoDefaultCtor");

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

    NSFX_TEST_CASE(IsObject)
    {
        NSFX_TEST_EXPECT(nsfx::IsObject<nsfx::IObject>::value);
        NSFX_TEST_EXPECT(nsfx::IsObject<ITest>::value);
        NSFX_TEST_EXPECT(nsfx::IsObject<Test>::value);
    }

    NSFX_TEST_CASE(Object)/*{{{*/
    {
        try
        {
            // Has default ctor.
            {
                deallocated = false;
                typedef nsfx::Object<Test>  TestClass;
                nsfx::Ptr<TestClass> t(new TestClass);
                nsfx::Ptr<ITest> q(t); // Allocate on heap.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_ASSERT(t->GetImpl());
                NSFX_TEST_EXPECT_EQ(t->GetImpl()->Internal(), 0);
            }

            // Has no default ctor.
            {
                deallocated = false;
                typedef nsfx::Object<TestNoDefaultCtor>  TestClass;
                nsfx::Ptr<TestClass> t(new TestClass(1));
                nsfx::Ptr<ITest> q(t); // Allocate on heap.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_ASSERT(t->GetImpl());
                NSFX_TEST_EXPECT_EQ(t->GetImpl()->Internal(), 1);
            }

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }

    }/*}}}*/

    NSFX_TEST_CASE(StaticObject)/*{{{*/
    {
        try
        {
            // Has default ctor.
            {
                deallocated = false;
                typedef nsfx::StaticObject<Test>  TestClass;
                static TestClass t; // Define as a static variable.
                nsfx::Ptr<ITest> q(&t);
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1); // Reference count is always 1.
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_ASSERT(t.GetImpl());
                NSFX_TEST_EXPECT_EQ(t.GetImpl()->Internal(), 0);
            }

            // Has no default ctor.
            {
                deallocated = false;
                typedef nsfx::StaticObject<TestNoDefaultCtor>  TestClass;
                static TestClass t(2); // Define as a static variable.
                nsfx::Ptr<ITest> q(&t);
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1); // Reference count is always 1.
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_ASSERT(t.GetImpl());
                NSFX_TEST_EXPECT_EQ(t.GetImpl()->Internal(), 1);
            }

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }

    }/*}}}*/

    NSFX_TEST_SUITE(AggObject)/*{{{*/
    {
        NSFX_TEST_CASE(RequireController)
        {
            deallocated = false;
            typedef nsfx::AggObject<Test>  TestClass;
            try
            {
                // Must pass a non-null controller.
                nsfx::Ptr<nsfx::IObject> t(new TestClass(nullptr));
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::BadAggregation& )
            {
                // Should come here.
            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what() << std::endl;
            }
        }

        struct Wedge : public IFoobar
        {
            typedef nsfx::AggObject<Test>  TestClass;

            Wedge(void) :
                t_(new TestClass(this)) // Construct with a controller.
            {}

            virtual ~Wedge(void) {}

            virtual refcount_t GetRefCount(void)
            {
                AddRef();
                return Release();
            }

            NSFX_INTERFACE_MAP_BEGIN(Wedge)
                NSFX_INTERFACE_ENTRY(IFoobar)
                NSFX_INTERFACE_AGGREGATED_ENTRY(ITest, &*t_)
            NSFX_INTERFACE_MAP_END()

            nsfx::Ptr<nsfx::IObject> t_; // A smart pointer.
        };

        NSFX_TEST_CASE(Test)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<WedgeClass> w(new WedgeClass);
                nsfx::Ptr<ITest> q(w); // Expose interfaces of aggregated object.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(!deallocated);
            }
            catch (std::exception& )
            {
                NSFX_TEST_EXPECT(false);
            }
        }
    }/*}}}*/

    NSFX_TEST_SUITE(MemberAggObject)/*{{{*/
    {
        NSFX_TEST_CASE(RequireController)
        {
            deallocated = false;
            typedef nsfx::MemberAggObject<Test>  TestClass;
            try
            {
                // Must pass a non-null controller.
                TestClass t(nullptr);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::BadAggregation& )
            {
                // Should come here.
            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what() << std::endl;
            }
        }

        struct Wedge : public IFoobar
        {
            typedef nsfx::MemberAggObject<Test>  TestClass;

            Wedge(void) :
                t_(this) // Construct with a controller.
            {
            }

            virtual ~Wedge(void) {}

            virtual refcount_t GetRefCount(void)
            {
                AddRef();
                return Release();
            }

            NSFX_INTERFACE_MAP_BEGIN(Wedge)
                NSFX_INTERFACE_ENTRY(IFoobar)
                NSFX_INTERFACE_AGGREGATED_ENTRY(ITest, &t_)
            NSFX_INTERFACE_MAP_END()

            TestClass t_; // A member variable.
        };

        NSFX_TEST_CASE(Test)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<WedgeClass>  w(new WedgeClass);
                nsfx::Ptr<ITest> q(w); // Expose interfaces of aggregated object.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3); // Share reference count with the controller.
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what() << std::endl;
            }
            NSFX_TEST_EXPECT(deallocated);
        }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

