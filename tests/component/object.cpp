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

    NSFX_DEFINE_CLASS_UUID4(ITest, 0, 0, 0, 1LL);

    struct IFoobar :/*{{{*/
        virtual nsfx::IObject
    {
        virtual ~IFoobar(void) {}

        virtual refcount_t GetRefCount(void) = 0;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(IFoobar, 0, 0, 0, 2LL);

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

    NSFX_DEFINE_CLASS_UUID4(Test, 0, 0, 1, 1LL);

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

    NSFX_DEFINE_CLASS_UUID4(TestNoDefaultCtor, 0, 0, 1, 2LL);

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

    ////////////////////////////////////////////////////////////////////////////
    // Non-aggregable.
    NSFX_TEST_SUITE(NonAggregable)/*{{{*/
    {
        NSFX_TEST_CASE(ObjectOnHeap)
        {
            try
            {
                // Has default ctor.
                {
                    deallocated = false;
                    typedef nsfx::Object<Test>  TestType;
                    TestType* t = new TestType;
                    nsfx::Ptr<ITest> q(t);
                    NSFX_TEST_EXPECT(!deallocated);
                    NSFX_TEST_EXPECT(q);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    nsfx::Ptr<nsfx::IObject> p(q);
                    NSFX_TEST_EXPECT(p);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                    NSFX_TEST_EXPECT(p == q);
                    p.Reset();
                    NSFX_TEST_EXPECT(!deallocated);
                    q.Reset();
                    NSFX_TEST_EXPECT(deallocated);
                    NSFX_TEST_ASSERT(t->GetEnveloped());
                    NSFX_TEST_EXPECT_EQ(t->GetEnveloped()->Internal(), 0);
                }

                // Has no default ctor.
                {
                    deallocated = false;
                    typedef nsfx::Object<TestNoDefaultCtor>  TestType;
                    TestType* t = new TestType(1);
                    nsfx::Ptr<ITest> q(t);
                    NSFX_TEST_EXPECT(!deallocated);
                    NSFX_TEST_EXPECT(q);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    nsfx::Ptr<nsfx::IObject> p(q);
                    NSFX_TEST_EXPECT(p);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                    NSFX_TEST_EXPECT(p == q);
                    p.Reset();
                    NSFX_TEST_EXPECT(!deallocated);
                    q.Reset();
                    NSFX_TEST_EXPECT(deallocated);
                    NSFX_TEST_ASSERT(t->GetEnveloped());
                    NSFX_TEST_EXPECT_EQ(t->GetEnveloped()->Internal(), 1);
                }

            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e);
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what();
            }
        }

        NSFX_TEST_CASE(ObjectOnStack)
        {
            try
            {
                // Has default ctor.
                {
                    deallocated = false;
                    typedef nsfx::Object<Test, false>  TestType;
                    TestType o;
                    NSFX_TEST_EXPECT(!deallocated);
                    nsfx::Ptr<ITest> q(&o);
                    NSFX_TEST_EXPECT(q);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    nsfx::Ptr<nsfx::IObject>  p(q);
                    NSFX_TEST_EXPECT(p);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    NSFX_TEST_EXPECT(p == q);
                    p.Reset();
                    q.Reset();
                    NSFX_TEST_EXPECT_EQ(o.GetRefCount(), 1);
                    NSFX_TEST_EXPECT(!deallocated);
                    NSFX_TEST_ASSERT(o.GetEnveloped());
                    NSFX_TEST_EXPECT_EQ(o.GetEnveloped()->Internal(), 0);
                }

                // Has no default ctor.
                {
                    deallocated = false;
                    typedef nsfx::Object<TestNoDefaultCtor, false>  TestType;
                    TestType o(1);
                    NSFX_TEST_EXPECT(!deallocated);
                    nsfx::Ptr<ITest> q(&o);
                    NSFX_TEST_EXPECT(q);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    nsfx::Ptr<nsfx::IObject>  p(q);
                    NSFX_TEST_EXPECT(p);
                    NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                    NSFX_TEST_EXPECT(p == q);
                    p.Reset();
                    q.Reset();
                    NSFX_TEST_EXPECT_EQ(o.GetRefCount(), 1);
                    NSFX_TEST_EXPECT(!deallocated);
                    NSFX_TEST_ASSERT(o.GetEnveloped());
                    NSFX_TEST_EXPECT_EQ(o.GetEnveloped()->Internal(), 1);
                }

            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e);
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what();
            }
            NSFX_TEST_EXPECT(deallocated);
        }
    }/*}}}*/

    ////////////////////////////////////////////////////////////////////////////
    // Aggregable.
    NSFX_TEST_SUITE(Aggregable)/*{{{*/
    {
        NSFX_TEST_CASE(RequireController)
        {
            deallocated = false;
            typedef nsfx::AggObject<Test>  TestType;
            try
            {
                // Must pass a non-null controller.
                nsfx::Ptr<nsfx::IObject> t(new TestType(nullptr));
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::BadAggregation& )
            {
                // Should come here.
            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e);
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what();
            }
        }

        // Heap
        struct Wedge : public IFoobar
        {
            typedef nsfx::AggObject<Test>  TestType;

            Wedge(void)
            {
                t.Reset(new TestType(this));
                if (!t)
                {
                    BOOST_THROW_EXCEPTION(nsfx::NoInterface());
                }
            }

            virtual ~Wedge(void) {}

            virtual refcount_t GetRefCount(void)
            {
                AddRef();
                return Release();
            }

            NSFX_INTERFACE_MAP_BEGIN(Wedge)
                NSFX_INTERFACE_ENTRY(IFoobar)
                NSFX_INTERFACE_AGGREGATED_ENTRY(ITest, t)
            NSFX_INTERFACE_MAP_END()

            nsfx::Ptr<nsfx::IObject> t;
        };

        NSFX_TEST_CASE(AggObjectOnHeap)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Wedge> WedgeType;
                nsfx::Ptr<ITest> q(new WedgeType);
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
                NSFX_TEST_EXPECT(deallocated);
            }
            catch (std::exception& )
            {
                NSFX_TEST_EXPECT(false);
            }
        }

        // Stack
        struct Vee : public IFoobar
        {
            typedef nsfx::AggObject<Test, false>  TestType;

#pragma warning(push)
#pragma warning(disable: 4355)
            Vee(void) :
                t(this)
            {
            }
#pragma warning(pop)

            virtual ~Vee(void) {}

            virtual refcount_t GetRefCount(void)
            {
                AddRef();
                return Release();
            }

            NSFX_INTERFACE_MAP_BEGIN(Wedge)
                NSFX_INTERFACE_ENTRY(IFoobar)
                NSFX_INTERFACE_AGGREGATED_ENTRY(ITest, &t)
            NSFX_INTERFACE_MAP_END()

            TestType t;
        };

        NSFX_TEST_CASE(AggObjectOnStack)
        {
            try
            {
                deallocated = false;
                typedef nsfx::Object<Vee, false> VeeType;
                VeeType v;
                nsfx::Ptr<ITest> q(&v);
                NSFX_TEST_EXPECT(!deallocated);
                NSFX_TEST_EXPECT(q);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                nsfx::Ptr<nsfx::IObject> p(q);
                NSFX_TEST_EXPECT(p);
                NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 1);
                NSFX_TEST_EXPECT(p == q);
                p.Reset();
                NSFX_TEST_EXPECT(!deallocated);
                q.Reset();
            }
            catch (boost::exception& e)
            {
                NSFX_TEST_EXPECT(false) << diagnostic_information(e);
            }
            catch (std::exception& e)
            {
                NSFX_TEST_EXPECT(false) << e.what();
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

