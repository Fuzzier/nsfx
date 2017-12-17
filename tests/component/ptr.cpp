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
                result = static_cast<nsfx::IObject*>(this);
            }
            return result;
        }

    private:
        uint32_t refCount_;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(Object, 0, 0, 1, 0LL);

    struct ITest : virtual nsfx::IObject/*{{{*/
    {
        virtual ~ITest(void) {}
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(ITest, 0, 0, 0, 1LL);

    struct Test : virtual ITest/*{{{*/
    {
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
                result = static_cast<ITest*>(this);
            }
            return result;
        }

    private:
        uint32_t refCount_;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(Test, 0, 0, 1, 1LL);

    uint32_t RefCount(nsfx::IObject* p)/*{{{*/
    {
        uint32_t result = 0;
        if (p)
        {
            p->AddRef();
            result = p->Release();
        }
        return result;
    }/*}}}*/

    NSFX_TEST_CASE(ctor0)
    {
        // default to nullptr
        nsfx::Ptr<nsfx::IObject> p;
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() == nullptr);
    }

    NSFX_TEST_CASE(ctor1)
    {
        // from raw pointer, don't take refcount
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() != nullptr);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        o->Release();
    }

    NSFX_TEST_CASE(ctor2)
    {
        // from raw pointer, don't take refcount
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        o->Release();
    }

    NSFX_TEST_CASE(ctor3)
    {
        // from raw pointer, take refcount
        nsfx::Ptr<nsfx::IObject> p(new Object, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
    }

    NSFX_TEST_CASE(query_ctor1)
    {
        // from raw pointer (diff type), don't take refcount
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> q(t);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(q == t);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        t->Release();
    }

    NSFX_TEST_CASE(query_ctor2)
    {
        // from raw pointer (diff type), don't take refcount
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> q(t, false);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(q == t);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        t->Release();
    }

    NSFX_TEST_CASE(query_ctor3)
    {
        // from raw pointer (diff type), take refcount
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> q(t, true);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
    }

    NSFX_TEST_CASE(query_ctor4)
    {
        // from raw pointer (diff type), don't take refcount, no interface
        nsfx::IObject* o = new Object;
        nsfx::Ptr<ITest> q(o);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(q != o);
        o->Release();
    }

    NSFX_TEST_CASE(query_ctor5)
    {
        // from raw pointer (diff type), don't take refcount, no interface
        nsfx::IObject* o = new Object;
        nsfx::Ptr<ITest> q(o, false);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(q != o);
        o->Release();
    }

    NSFX_TEST_CASE(query_ctor6)
    {
        // from raw pointer (diff type), take refcount, no interface
        nsfx::IObject* o = new Object;
        o->AddRef();
        nsfx::Ptr<ITest> q(o, true);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(q != o);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        o->Release();
    }

    NSFX_TEST_CASE(copy_ctor)
    {
        // from smart pointer
        nsfx::Ptr<ITest> q(new Test, true);
        nsfx::Ptr<ITest> p(q);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
    }

    NSFX_TEST_CASE(query_copy_ctor1)
    {
        // from smart pointer (diff type)
        nsfx::Ptr<nsfx::IObject> p(new Test, true);
        nsfx::Ptr<ITest> q(p);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
    }

    NSFX_TEST_CASE(query_copy_ctor2)
    {
        // from smart pointer (diff type), no interface
        nsfx::Ptr<nsfx::IObject> p(new Object, true);
        nsfx::Ptr<ITest> q(p);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
    }

    NSFX_TEST_CASE(copy_assign)
    {
        nsfx::Ptr<nsfx::IObject> p(new Object, true);
        nsfx::Ptr<nsfx::IObject> q(new Object, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = p;
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
    }

    NSFX_TEST_CASE(query_copy_assign1)
    {
        // diff type
        nsfx::Ptr<nsfx::IObject> p(new Test, true);
        nsfx::Ptr<ITest> q(new Test, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = p;
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
    }

    NSFX_TEST_CASE(query_copy_assign2)
    {
        // diff type, no interface
        nsfx::Ptr<nsfx::IObject> p(new Object, true);
        nsfx::Ptr<ITest> q(new Test, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = p;
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
    }

    NSFX_TEST_CASE(move_ctor)
    {
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        nsfx::Ptr<nsfx::IObject> q(std::move(p));
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT(q == o);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 2);
        o->Release();
    }

    NSFX_TEST_CASE(move_assign)
    {
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o);
        nsfx::Ptr<nsfx::IObject> q(new Object, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = std::move(p);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT(q == o);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 2);
        o->Release();
    }

    NSFX_TEST_CASE(query_move_assign1)
    {
        // diff type
        ITest* t = new Test;
        nsfx::Ptr<nsfx::IObject> p(t);
        nsfx::Ptr<ITest> q(new Test, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = std::move(p);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT(q == t);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        t->Release();
    }

    NSFX_TEST_CASE(query_move_assign2)
    {
        // diff type, no interface
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o);
        nsfx::Ptr<ITest> q(new Test, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p != q);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
        q = std::move(p);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        o->Release();
    }

    NSFX_TEST_CASE(deref_op)
    {
        // operator*
        nsfx::Ptr<Object> p(new Object, true);
        NSFX_TEST_EXPECT_EQ((*p).AddRef(), 2);
        NSFX_TEST_EXPECT_EQ((*p).Release(), 1);
    }

    NSFX_TEST_CASE(pointer_member_access_op)
    {
        // operator->
        nsfx::Ptr<nsfx::IObject> p(new Object, true);
        NSFX_TEST_EXPECT_EQ(p->AddRef(), 2);
        NSFX_TEST_EXPECT_EQ(p->Release(), 1);
    }

    NSFX_TEST_CASE(equality)
    {
        //   raw pointer == smart pointer
        // smart pointer ==   raw pointer
        // smart pointer == smart pointer
        nsfx::IObject* o = new Object;
        nsfx::Ptr<nsfx::IObject> p(o, false);
        ITest* t = new Test;
        nsfx::Ptr<ITest> q(t, false);
        NSFX_TEST_EXPECT(o == p);
        NSFX_TEST_EXPECT(p == o);
        NSFX_TEST_EXPECT(t == q);
        NSFX_TEST_EXPECT(q == t);
        NSFX_TEST_EXPECT(!(o != p));
        NSFX_TEST_EXPECT(!(p != o));
        NSFX_TEST_EXPECT(!(t != q));
        NSFX_TEST_EXPECT(!(q != t));
        NSFX_TEST_EXPECT(t != p);
        NSFX_TEST_EXPECT(p != t);
        NSFX_TEST_EXPECT(t != p);
        NSFX_TEST_EXPECT(p != t);
        NSFX_TEST_EXPECT(!(t == p));
        NSFX_TEST_EXPECT(!(p == t));
        NSFX_TEST_EXPECT(!(t == p));
        NSFX_TEST_EXPECT(!(p == t));
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(swap1)
    {
        // raw pointer <-> smart pointer
        ITest* t = nullptr;
        nsfx::Ptr<ITest> q(new Test, true);
        swap(t, q);
        NSFX_TEST_EXPECT(!q);
        NSFX_TEST_EXPECT(t);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 1);
        t->Release();
    }

    NSFX_TEST_CASE(swap2)
    {
        // smart pointer <-> raw pointer
        ITest* t = new Test;
        nsfx::Ptr<ITest> q;
        swap(q, t);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(!t);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
    }

    NSFX_TEST_CASE(swap3)
    {
        // smart pointer <-> smart pointer
        ITest* o = new Test;
        ITest* t = new Test;
        nsfx::Ptr<ITest> p(o, false);
        nsfx::Ptr<ITest> q(t, false);
        swap(p, q);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT(q);
        NSFX_TEST_EXPECT(p == t);
        NSFX_TEST_EXPECT(o == q);
        NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
        NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(Reset0)
    {
        // reset to nullptr
        ITest* t = new Test;
        nsfx::Ptr<ITest> p(t, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        p.Reset();
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 1);
        t->Release();
    }

    NSFX_TEST_CASE(Reset1)
    {
        // don't take refcount
        ITest* o = new Test;
        ITest* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(Reset2)
    {
        // don't take refcount
        ITest* o = new Test;
        ITest* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(Reset3)
    {
        // take refcount
        ITest* o = new Test;
        ITest* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 1);
        o->Release();
    }

    NSFX_TEST_CASE(query_Reset1)
    {
        // diff type, don't take refcount
        nsfx::IObject* o = new Test;
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(query_Reset2)
    {
        // diff type, don't take refcount
        nsfx::IObject* o = new Test;
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t, false);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 2);
        o->Release();
        t->Release();
    }

    NSFX_TEST_CASE(query_Reset3)
    {
        // diff type, take refcount
        nsfx::IObject* o = new Test;
        nsfx::IObject* t = new Test;
        nsfx::Ptr<ITest> p(o);
        p.Reset(t, true);
        NSFX_TEST_EXPECT(p);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 1);
        NSFX_TEST_EXPECT_EQ(RefCount(t), 1);
        o->Release();
    }

    NSFX_TEST_CASE(Detach)
    {
        nsfx::IObject* o = new Test;
        nsfx::Ptr<ITest> p(o);
        nsfx::Ptr<ITest> q(o);
        ITest* t = p.Detach();
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(t == q);
        NSFX_TEST_EXPECT_EQ(RefCount(o), 3);
        o->Release();
        t->Release();
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

