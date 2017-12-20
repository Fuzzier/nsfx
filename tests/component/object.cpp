#include <nsfx/test.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(Object)
{
    using nsfx::refcount_t;

    struct ITest :/*{{{*/
        virtual nsfx::IObject
    {
        virtual ~ITest(void) {}

        virtual refcount_t GetRefCount(void) = 0;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(ITest, 0, 0, 0, 1LL);

    struct Test :/*{{{*/
        virtual nsfx::Object<Test>,
        virtual ITest
    {
        virtual ~Test(void) {}

        virtual refcount_t GetRefCount(void)
        {
            AddRef();
            return Release();
        }

        NSFX_INTERFACE_MAP_BEGIN()
            NSFX_INTERFACE_ENTRY(ITest)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(Test, 0, 0, 1, 1LL);

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

    NSFX_TEST_CASE(Test)
    {
        nsfx::Ptr<nsfx::IObject> p(new Test);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
        nsfx::Ptr<ITest> q(p);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
        NSFX_TEST_EXPECT(q == p);
        nsfx::Ptr<nsfx::IObject> r(q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 3);
        NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
        NSFX_TEST_EXPECT(q == p);
        NSFX_TEST_EXPECT(r == q);
    }

}


// NSFX_TEST_SUITE(AggObject)
// {
//     using nsfx::refcount_t;
//
//     struct ITest :#<{(|{{{|)}>#
//         virtual nsfx::IObject
//     {
//         virtual ~ITest(void) {}
//
//         virtual refcount_t GetRefCount(void) = 0;
//     };#<{(|}}}|)}>#
//
//     NSFX_DEFINE_CLASS_UUID4(ITest, 0, 0, 0, 1LL);
//
//     struct AggTest :#<{(|{{{|)}>#
//         virtual public nsfx::AggObject<AggTest>,
//         virtual public ITest
//     {
//         AggTest(IObject* outer) :
//             nsfx::AggObject<AggTest>(outer)
//         {
//         }
//
//         virtual ~AggTest(void) {};
//
//         virtual refcount_t GetRefCount(void)
//         {
//             AddRef();
//             return Release();
//         }
//
//         NSFX_BEGIN_INTERFACE_OBJECT_MAP()
//             NSFX_INTERFACE_OBJECT_ENTRY(ITest)
//         NSFX_END_INTERFACE_OBJECT_MAP()
//
//     };#<{(|}}}|)}>#
//
//     struct Test :#<{(|{{{|)}>#
//         virtual nsfx::Object<Test>
//     {
//         Test(void)
//         {
//             AggTest* o = new AggTest(this);
//             if (o)
//             {
//                 aggTest_.Reset(o->GetInner());
//             }
//         }
//
//         virtual ~Test(void) {}
//
//         NSFX_BEGIN_INTERFACE_OBJECT_MAP()
//             NSFX_INTERFACE_OBJECT_ENTRY_AGGREGATE(ITest, aggTest_)
//         NSFX_END_INTERFACE_OBJECT_MAP()
//
//         nsfx::Ptr<nsfx::IObject> aggTest_;
//     };#<{(|}}}|)}>#
//
//     NSFX_DEFINE_CLASS_UUID4(Test, 0, 0, 1, 1LL);
//
//     refcount_t RefCount(nsfx::IObject* p)#<{(|{{{|)}>#
//     {
//         refcount_t result = 0;
//         if (p)
//         {
//             p->AddRef();
//             result = p->Release();
//         }
//         return result;
//     }#<{(|}}}|)}>#
//
//     NSFX_TEST_CASE(Test)
//     {
//         nsfx::Ptr<nsfx::IObject> p(new Test);
//         NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
//         nsfx::Ptr<ITest> q(p);
//         NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
//         NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 2);
//         NSFX_TEST_EXPECT(q == p);
//         nsfx::Ptr<nsfx::IObject> r(q);
//         NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 3);
//         NSFX_TEST_EXPECT_EQ(q->GetRefCount(), 3);
//         NSFX_TEST_EXPECT(q == p);
//         NSFX_TEST_EXPECT(r == q);
//     }
//
// }


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

