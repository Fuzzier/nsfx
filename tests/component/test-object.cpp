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

            // Ptr.
            {
                typedef nsfx::Object<Test>  TestClass;
                nsfx::Ptr<Test> t(new TestClass);
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 1);

                nsfx::Ptr<Test> p1(t.Get());
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 2);

                nsfx::Ptr<Test> p2(t.Get(), true);
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 3);

                nsfx::Ptr<Test> p3(t.Get(), false);
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 3);
                p3->AddRef();

                nsfx::Ptr<Test> q(new TestClass);
                p1.Reset(q.Get());
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 3);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);

                p2.Reset(q.Get(), true);
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 2);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);

                p3.Reset(q.Get(), false);
                NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 1);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
                p3->AddRef();
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

            // Ptr.
            {
                typedef nsfx::StaticObject<Test>  TestClass;
                static TestClass t;

                nsfx::Ptr<Test> p1(&t);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);

                nsfx::Ptr<Test> p2(&t, true);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);

                nsfx::Ptr<Test> p3(&t, false);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);
                p3->AddRef();

                static TestClass q;
                p1.Reset(&q);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);
                NSFX_TEST_EXPECT_EQ(q.GetRefCount(), 1);

                p2.Reset(&q, true);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);
                NSFX_TEST_EXPECT_EQ(q.GetRefCount(), 1);

                p3.Reset(&q, false);
                NSFX_TEST_EXPECT_EQ(t.GetRefCount(), 1);
                NSFX_TEST_EXPECT_EQ(q.GetRefCount(), 1);
                p3->AddRef();
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

        NSFX_TEST_CASE(ShareLifetime)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<IFoobar> w(new WedgeClass);
                nsfx::Ptr<ITest> q(w); // Expose interfaces of aggregated object.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
                NSFX_TEST_EXPECT(w == q); // Same object.
                p.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                w.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1); // Share reference count with the controller.
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();

                // Ptr.
                {
                    typedef nsfx::AggObject<Test>  TestClass;

                    nsfx::Ptr<nsfx::IObject> c(new WedgeClass);
                    nsfx::Ptr<nsfx::IObject> d(new WedgeClass);

                    nsfx::Ptr<Test> t(new TestClass(c.Get()));
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 2);

                    nsfx::Ptr<Test> p1(t.Get());
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 3);

                    nsfx::Ptr<Test> p2(t.Get(), true);
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 4);

                    nsfx::Ptr<Test> p3(t.Get(), false);
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 4);
                    p3->AddRef();

                    nsfx::Ptr<Test> q(new TestClass(d.Get()));
                    p1.Reset(q.Get());
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 4);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);

                    p2.Reset(q.Get(), true);
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 3);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 4);

                    p3.Reset(q.Get(), false);
                    NSFX_TEST_EXPECT_EQ(t->GetRefCount(), 2);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 4);
                    p3->AddRef();
                }

            }
            catch (std::exception& )
            {
                NSFX_TEST_EXPECT(false);
            }
            NSFX_TEST_EXPECT(deallocated);
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

        NSFX_TEST_CASE(ShareLifetime)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<IFoobar> w(new WedgeClass);
                nsfx::Ptr<ITest> q(w); // Expose interfaces of aggregated object.
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3); // Share reference count with the controller.
                NSFX_TEST_EXPECT(w == q); // Same object.
                p.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                w.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1); // Share reference count with the controller.
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();

                // Ptr.
                {
                    typedef nsfx::MemberAggObject<Test>  TestClass;

                    nsfx::Ptr<WedgeClass> c(new WedgeClass);
                    nsfx::Ptr<WedgeClass> d(new WedgeClass);

                    TestClass t(c.Get());
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 1);

                    nsfx::Ptr<Test> p1(&t);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 2);

                    nsfx::Ptr<Test> p2(&t, true);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);

                    nsfx::Ptr<Test> p3(&t, false);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);
                    p3->AddRef();

                    TestClass q(d.Get());
                    p1.Reset(&q);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 2);

                    p2.Reset(&q, true);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 2);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 3);

                    p3.Reset(&q, false);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 1);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 3);
                    p3->AddRef();

                    p1.Reset();
                    p2.Reset();
                    p3.Reset();
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
            NSFX_TEST_EXPECT(deallocated);
        }

    }/*}}}*/

    NSFX_TEST_SUITE(MutualObject)/*{{{*/
    {
        NSFX_TEST_CASE(RequireController)
        {
            deallocated = false;
            typedef nsfx::MutualObject<Test>  TestClass;
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
            typedef nsfx::MutualObject<Test>  TestClass;

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

            nsfx::Ptr<ITest> GetTest(void)
            {
                return nsfx::Ptr<ITest>(&t_);
            }

            NSFX_INTERFACE_MAP_BEGIN(Wedge)
                NSFX_INTERFACE_ENTRY(IFoobar)
            NSFX_INTERFACE_MAP_END()

            TestClass t_; // A member variable.
        };

        NSFX_TEST_CASE(SeparateObject)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<WedgeClass> w(new WedgeClass);
                nsfx::Ptr<IFoobar> f(w); // Controller exposes its own interfaces.
                nsfx::Ptr<nsfx::IObject> q(w->GetTest());
                nsfx::Ptr<ITest> t(q); // Mutual exposes its own interfaces.
                NSFX_TEST_EXPECT(f != q); // Separate object.
                // Does not expose interfaces of controller object.
                nsfx::Ptr<IFoobar> p(q);
                NSFX_TEST_EXPECT(false);

                // Ptr.
                {
                    typedef nsfx::MutualObject<Test>  TestClass;

                    nsfx::Ptr<WedgeClass> c(new WedgeClass);
                    nsfx::Ptr<WedgeClass> d(new WedgeClass);

                    TestClass t(c.Get());
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 1);

                    nsfx::Ptr<Test> p1(&t);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 2);

                    nsfx::Ptr<Test> p2(&t, true);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);

                    nsfx::Ptr<Test> p3(&t, false);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);
                    p3->AddRef();

                    TestClass q(d.Get());
                    p1.Reset(&q);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 3);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 2);

                    p2.Reset(&q, true);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 2);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 3);

                    p3.Reset(&q, false);
                    NSFX_TEST_EXPECT_EQ(c->GetRefCount(), 1);
                    NSFX_TEST_EXPECT_EQ(d->GetRefCount(), 3);
                    p3->AddRef();

                    p1.Reset();
                    p2.Reset();
                    p3.Reset();
                }

            }
            catch (nsfx::NoInterface& )
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
            NSFX_TEST_EXPECT(deallocated);
        }

        NSFX_TEST_CASE(ShareLifetime)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeClass;
                nsfx::Ptr<WedgeClass>  w(new WedgeClass);
                nsfx::Ptr<ITest> q(w->GetTest());
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3); // Share reference count with the controller.
                p.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2); // Share reference count with the controller.
                w.Reset();
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1); // Share reference count with the controller.
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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

