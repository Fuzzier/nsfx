#include <nsfx/test.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(Ptr)
{
    struct Object : virtual nsfx::IObject/*{{{*/
    {
        Object(void) BOOST_NOEXCEPT :
            refCount_(1)
        {
        }

        virtual ~Object(void) BOOST_NOEXCEPT
        {
            NSFX_ASSERT(refCount_ == 0);
        }

        virtual uint32_t AddRef(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return ++refCount_;
        }

        virtual uint32_t Release(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            uint32_t result = --refCount_;
            if (refCount_ == 0)
            {
                delete this;
            }
            return result;
        }

        virtual void* QueryInterface(const nsfx::uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            void* result = nullptr;
            if (iid == nsfx::uuid_of<nsfx::IObject>())
            {
                ++refCount_;
                result = this;
            }
            return result;
        }

        friend uint32_t ObjectRefCount(const nsfx::Ptr<nsfx::IObject>& p);

    private:
        uint32_t refCount_;
    };/*}}}*/

    uint32_t ObjectRefCount(const nsfx::Ptr<nsfx::IObject>& p)/*{{{*/
    {
        nsfx::IObject* a = p.Get();
        Object* b = dynamic_cast<Object*>(a);
        uint32_t result = 0;
        if (b)
        {
            result = b->refCount_;
        }
        return result;
    }/*}}}*/

    struct ITest : virtual nsfx::IObject/*{{{*/
    {
        virtual ~ITest(void) {}
    };/*}}}*/

    struct Test : virtual ITest/*{{{*/
    {
        NSFX_DEFINE_CLASS_UUID4(0, 0, 0, 0x1LL);

        Test(void) BOOST_NOEXCEPT :
            refCount_(1)
        {
        }

        virtual ~Test(void) BOOST_NOEXCEPT
        {
            NSFX_ASSERT(refCount_ == 0);
        }

        virtual uint32_t AddRef(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return ++refCount_;
        }

        virtual uint32_t Release(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            uint32_t result = --refCount_;
            if (refCount_ == 0)
            {
                delete this;
            }
            return result;
        }

        virtual void* QueryInterface(const nsfx::uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            void* result = nullptr;
            if (iid == nsfx::uuid_of<nsfx::IObject>())
            {
                ++refCount_;
                result = static_cast<nsfx::IObject*>(this);
            }
            else if (iid == nsfx::uuid_of<ITest>())
            {
                ++refCount_;
                result = this;
            }
            return result;
        }

        friend uint32_t TestRefCount(const nsfx::Ptr<nsfx::IObject>& p);

    private:
        uint32_t refCount_;
    };/*}}}*/

    uint32_t TestRefCount(const nsfx::Ptr<nsfx::IObject>& p)/*{{{*/
    {
        nsfx::IObject* a = p.Get();
        Test*  b = dynamic_cast<Test*>(a);
        return b->refCount_;
    }/*}}}*/

    NSFX_TEST_CASE(ctor0)
    {
        nsfx::Ptr<nsfx::IObject> p;
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() == nullptr);
    }

    NSFX_TEST_CASE(ctor1_false)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 1);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() != nullptr);
    }

    NSFX_TEST_CASE(ctor1_true)
    {
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o, true);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 2);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() != nullptr);
        o->Release();
    }

    NSFX_TEST_CASE(copy_ctor1)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        nsfx::Ptr<nsfx::IObject> q(p);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 2);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 2);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
    }

    NSFX_TEST_CASE(copy_ctor2)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        nsfx::Ptr<nsfx::IObject> q = p;
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 2);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 2);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
    }

    NSFX_TEST_CASE(copy_assign)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        nsfx::Ptr<nsfx::IObject> q(new Object, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 1);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 1);
        q = p;
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 2);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 2);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
    }

    NSFX_TEST_CASE(move_ctor1)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 1);
        NSFX_TEST_EXPECT(p);
        nsfx::Ptr<nsfx::IObject> q(std::move(p));
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 0);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 1);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
    }

    NSFX_TEST_CASE(move_ctor2)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 1);
        NSFX_TEST_EXPECT(p);
        nsfx::Ptr<nsfx::IObject> q = std::move(p);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 0);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 1);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
    }

    NSFX_TEST_CASE(move_assign)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        nsfx::Ptr<nsfx::IObject> q(new Object, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 1);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 1);
        q = std::move(p);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(p), 0);
        NSFX_TEST_EXPECT_EQ(ObjectRefCount(q), 1);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
    }

    NSFX_TEST_CASE(deref_op)
    {
        nsfx::Ptr<Object> p(new Object, false);
        NSFX_TEST_EXPECT_EQ((*p).AddRef(), 2);
        NSFX_TEST_EXPECT_EQ((*p).Release(), 1);
    }

    NSFX_TEST_CASE(pointer_member_access_op)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, false);
        NSFX_TEST_EXPECT_EQ(p->AddRef(), 2);
        NSFX_TEST_EXPECT_EQ(p->Release(), 1);
    }

    NSFX_TEST_CASE(eq_op_template)
    {
        nsfx::IObject* o = new Test;
        ITest* t = static_cast<ITest*>(o->QueryInterface(uuid_of(t)));
        nsfx::Ptr<nsfx::IObject> p(o, false);
        nsfx::Ptr<ITest> q(t, false);
        NSFX_TEST_EXPECT(p == q);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

