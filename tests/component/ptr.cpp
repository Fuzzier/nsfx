/**
 * @file
 *
 * @brief Test Ptr.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-14
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/component/ptr.h>
#include <iostream>


NSFX_TEST_SUITE(Ptr)
{
    using nsfx::refcount_t;

    struct Object : virtual nsfx::IObject/*{{{*/
    {
        Object(void) BOOST_NOEXCEPT :
            refCount_(0)
        {
        }

        virtual ~Object(void) BOOST_NOEXCEPT
        {
            BOOST_ASSERT(refCount_ == 0);
        }

        virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return ++refCount_;
        }

        virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            refcount_t result = --refCount_;
            if (!refCount_)
            {
                delete this;
            }
            return result;
        }

        virtual void* QueryInterface(const nsfx::Uid& iid) NSFX_OVERRIDE
        {
            void* result = nullptr;
            if (iid == nsfx::uid_of<nsfx::IObject>())
            {
                AddRef();
                result = static_cast<nsfx::IObject*>(this);
            }
            else
            {
                BOOST_THROW_EXCEPTION(nsfx::NoInterface());
            }
            return result;
        }

    private:
        refcount_t refCount_;
    };/*}}}*/

    struct ITest : virtual nsfx::IObject/*{{{*/
    {
        virtual ~ITest(void) {}
    };/*}}}*/

    NSFX_DEFINE_CLASS_UID(ITest, "edu.uestc.nsfx.test.ITest");

    struct Test : virtual ITest/*{{{*/
    {
        Test(void) BOOST_NOEXCEPT :
            refCount_(0)
        {
        }

        virtual ~Test(void) BOOST_NOEXCEPT
        {
            BOOST_ASSERT(refCount_ == 0);
        }

        virtual refcount_t AddRef(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return ++refCount_;
        }

        virtual refcount_t Release(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            refcount_t result = --refCount_;
            if (!refCount_)
            {
                delete this;
            }
            return result;
        }

        virtual void* QueryInterface(const nsfx::Uid& iid) NSFX_OVERRIDE
        {
            void* result = nullptr;
            if (iid == nsfx::uid_of<nsfx::IObject>())
            {
                AddRef();
                result = static_cast<nsfx::IObject*>(this);
            }
            else if (iid == nsfx::uid_of<ITest>())
            {
                AddRef();
                result = static_cast<ITest*>(this);
            }
            else
            {
                BOOST_THROW_EXCEPTION(nsfx::NoInterface());
            }
            return result;
        }

    private:
        refcount_t refCount_;
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

    NSFX_TEST_CASE(HasNoUid)
    {
        // Object has no UID, but can be managed by nsfx::Ptr<>.
        try
        {
            nsfx::Ptr<Object> o(new Object);
            nsfx::Ptr<nsfx::IObject> p(o);
            nsfx::Ptr<Test>  t(new Test);
            nsfx::Ptr<ITest> q(t);
            p = t;
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
        // from nullptr
        nsfx::Ptr<nsfx::IObject> p(nullptr);
        NSFX_TEST_EXPECT(!p);
        NSFX_TEST_EXPECT(p == p);
        NSFX_TEST_EXPECT(p.Get() == nullptr);
    }

    NSFX_TEST_CASE(ctor2)
    {
        // from raw pointer, don't take refcount
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(p == p);
            NSFX_TEST_EXPECT(p.Get() != nullptr);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(ctor3)
    {
        // from raw pointer, don't take refcount
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object, false);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(p == p);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(ctor4)
    {
        // from raw pointer, take refcount
        try
        {
            Object* o = new Object;
            o->AddRef();
            nsfx::Ptr<nsfx::IObject> p(o, true);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(p == p);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(query_ctor1)
    {
        // from raw pointer (diff type), don't take refcount
        try
        {
            nsfx::IObject* t = new Test;
            nsfx::Ptr<ITest> q(t);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(q == t);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
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

    NSFX_TEST_CASE(query_ctor2)
    {
        // from raw pointer (diff type), don't take refcount
        try
        {
            nsfx::IObject* t = new Test;
            nsfx::Ptr<ITest> q(t, false);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(q == t);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
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

    NSFX_TEST_CASE(query_ctor3)
    {
        // from raw pointer (diff type), take refcount
        try
        {
            nsfx::IObject* t = new Test;
            t->AddRef();
            nsfx::Ptr<ITest> q(t, true);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
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

    NSFX_TEST_CASE(query_ctor4)
    {
        // from raw pointer (diff type), don't take refcount, no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Object);
            try
            {
                nsfx::Ptr<ITest> q(o.Get());
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoInterface& )
            {
                // Should come here.
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
    }

    NSFX_TEST_CASE(query_ctor5)
    {
        // from raw pointer (diff type), don't take refcount, no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Object);
            try
            {
                nsfx::Ptr<ITest> q(o.Get(), false);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoInterface& )
            {
                // Should come here.
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
    }

    NSFX_TEST_CASE(query_ctor6)
    {
        // from raw pointer (diff type), take refcount, no interface
        try
        {
            nsfx::IObject* o = new Object;
            o->AddRef();
            try
            {
                nsfx::Ptr<ITest> q(o, true);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoInterface& )
            {
                // Should come here.
                o->Release();
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
    }

    NSFX_TEST_CASE(copy_ctor)
    {
        // from smart pointer
        try
        {
            nsfx::Ptr<ITest> q(new Test);
            nsfx::Ptr<ITest> p(q);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p == q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
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

    NSFX_TEST_CASE(query_copy_ctor1)
    {
        // from smart pointer (diff type)
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Test);
            nsfx::Ptr<ITest> q(p);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p == q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
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

    NSFX_TEST_CASE(query_copy_ctor2)
    {
        // from smart pointer (diff type), no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            try
            {
                nsfx::Ptr<ITest>  q(p);
                NSFX_TEST_EXPECT(false);
            }
            catch (boost::exception& e)
            {
                // Should come here.
            }
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(copy_assign)
    {
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            nsfx::Ptr<nsfx::IObject> q(new Object);
            q = p;
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p == q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
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

    NSFX_TEST_CASE(query_copy_assign1)
    {
        // diff type
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Test);
            nsfx::Ptr<ITest> q(new Test);
            q = p;
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p == q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
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

    NSFX_TEST_CASE(query_copy_assign2)
    {
        // diff type, no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            nsfx::Ptr<ITest> q(new Test);
            try
            {
                q = p;
                NSFX_TEST_EXPECT(false);
            }
            catch (boost::exception& e)
            {
                // Should come here
            }
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(!q);
            NSFX_TEST_EXPECT(p != q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(move_ctor)
    {
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Object);
            nsfx::Ptr<nsfx::IObject> p(o);
            nsfx::Ptr<nsfx::IObject> q(std::move(p));
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p != q);
            NSFX_TEST_EXPECT(q == o);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 2);
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

    NSFX_TEST_CASE(move_assign)
    {
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Object);
            nsfx::Ptr<nsfx::IObject> p(o);
            nsfx::Ptr<nsfx::IObject> q(new Object);
            q = std::move(p);
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p != q);
            NSFX_TEST_EXPECT(q == o);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 2);
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

    NSFX_TEST_CASE(query_move_assign1)
    {
        // diff type
        try
        {
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<nsfx::IObject> p(t);
            nsfx::Ptr<ITest> q(new Test);
            q = std::move(p);
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p != q);
            NSFX_TEST_EXPECT(q == t);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
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

    NSFX_TEST_CASE(query_move_assign2)
    {
        // diff type, no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            nsfx::Ptr<ITest> q(new Test);
            try
            {
                q = std::move(p);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoInterface& )
            {
                // Should come here.
            }
            NSFX_TEST_EXPECT(p);  // the source pointer is intact
            NSFX_TEST_EXPECT(!q);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(address_of_op)
    {
        // operator&
        try
        {
            nsfx::Ptr<Object> p(new Object);
            NSFX_TEST_EXPECT_EQ(*&p, p.Get());
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

    NSFX_TEST_CASE(deref_op)
    {
        // operator*
        try
        {
            nsfx::Ptr<Object> p(new Object);
            NSFX_TEST_EXPECT_EQ((*p).AddRef(), 2);
            NSFX_TEST_EXPECT_EQ((*p).Release(), 1);
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

    NSFX_TEST_CASE(pointer_member_access_op)
    {
        // operator->
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            NSFX_TEST_EXPECT_EQ(p->AddRef(), 2);
            NSFX_TEST_EXPECT_EQ(p->Release(), 1);
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

    NSFX_TEST_CASE(equality)
    {
        //   raw pointer == smart pointer
        // smart pointer ==   raw pointer
        // smart pointer == smart pointer
        try
        {
            nsfx::Ptr<nsfx::IObject> p(new Object);
            nsfx::IObject* o = p.Get();
            nsfx::Ptr<ITest> q(new Test);
            ITest* t = q.Get();
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

    NSFX_TEST_CASE(swap1)
    {
        // raw pointer <-> smart pointer
        try
        {
            ITest* t = nullptr;
            nsfx::Ptr<ITest> q(new Test);
            swap(t, q);
            NSFX_TEST_EXPECT(!q);
            NSFX_TEST_EXPECT(t);
            NSFX_TEST_EXPECT_EQ(RefCount(t), 1);
            t->Release();
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

    NSFX_TEST_CASE(swap2)
    {
        // smart pointer <-> raw pointer
        try
        {
            ITest* t = new Test;
            t->AddRef();
            nsfx::Ptr<ITest> q;
            swap(q, t);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(!t);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 1);
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

    NSFX_TEST_CASE(swap3)
    {
        // smart pointer <-> smart pointer
        try
        {
            nsfx::Ptr<ITest> o(new Test);
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<ITest> p(o);
            nsfx::Ptr<ITest> q(t);
            swap(p, q);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(q);
            NSFX_TEST_EXPECT(p == t);
            NSFX_TEST_EXPECT(o == q);
            NSFX_TEST_EXPECT_EQ(RefCount(q.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
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

    NSFX_TEST_CASE(Reset0)
    {
        // reset to nullptr
        try
        {
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<ITest> p(t);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
            p.Reset();
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 1);
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

    NSFX_TEST_CASE(Reset1)
    {
        // don't take refcount
        try
        {
            nsfx::Ptr<ITest> o(new Test);
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<ITest> p(o);
            p.Reset(t.Get());
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
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

    NSFX_TEST_CASE(Reset2)
    {
        // don't take refcount
        try
        {
            nsfx::Ptr<ITest> o(new Test);
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<ITest> p(o);
            p.Reset(t.Get(), false);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
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

    NSFX_TEST_CASE(Reset3)
    {
        // take refcount
        try
        {
            nsfx::Ptr<ITest> o(new Test);
            nsfx::Ptr<ITest> t(new Test);
            nsfx::Ptr<ITest> p(o);
            p.Reset(t.Detach(), true);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(!t);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(query_Reset1)
    {
        // diff type, don't take refcount
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Test);
            nsfx::Ptr<nsfx::IObject> t(new Test);
            nsfx::Ptr<ITest> p(o);
            p.Reset(t.Get());
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
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

    NSFX_TEST_CASE(query_Reset2)
    {
        // diff type, don't take refcount
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Test);
            nsfx::Ptr<nsfx::IObject> t(new Test);
            nsfx::Ptr<ITest> p(o);
            p.Reset(t.Get(), false);
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 2);
            NSFX_TEST_EXPECT_EQ(RefCount(t.Get()), 2);
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

    NSFX_TEST_CASE(query_Reset3)
    {
        // diff type, take refcount
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Test);
            nsfx::Ptr<nsfx::IObject> t(new Test);
            nsfx::Ptr<ITest> p(o);
            nsfx::IObject* q = t.Detach();
            try
            {
                p.Reset(q, true);
            }
            catch (nsfx::NoInterface& )
            {
                q->Release();
                throw;
            }
            NSFX_TEST_EXPECT(p);
            NSFX_TEST_EXPECT(!t);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
            NSFX_TEST_EXPECT_EQ(RefCount(p.Get()), 1);
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

    NSFX_TEST_CASE(query_Reset4)
    {
        // diff type, take refcount, no interface
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Test);
            nsfx::Ptr<nsfx::IObject> t(new Object);
            nsfx::Ptr<ITest> p(o);
            nsfx::IObject* q = t.Detach();
            try
            {
                p.Reset(q, true);
            }
            catch (nsfx::NoInterface& )
            {
                // Should come here
                q->Release();
            }
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT(!t);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 1);
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

    NSFX_TEST_CASE(Detach)
    {
        try
        {
            nsfx::Ptr<nsfx::IObject> o(new Test);
            nsfx::Ptr<ITest> p(o);
            nsfx::Ptr<ITest> q(o);
            ITest* t = p.Detach();
            NSFX_TEST_EXPECT(!p);
            NSFX_TEST_EXPECT(t == o);
            NSFX_TEST_EXPECT_EQ(RefCount(o.Get()), 3);
            t->Release();
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

    NSFX_TEST_CASE(Hashable)
    {
        try
        {
            nsfx::Ptr<ITest> q(new Test);
            auto h = boost::hash<nsfx::Ptr<ITest> >();
            size_t v = h(q);
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

    NSFX_TEST_CASE(IsPtr)
    {
        NSFX_TEST_EXPECT(nsfx::IsPtr<nsfx::Ptr<nsfx::IObject> >::value);
        NSFX_TEST_EXPECT(nsfx::IsPtr<nsfx::Ptr<ITest> >::value);
        NSFX_TEST_EXPECT(nsfx::IsPtr<nsfx::Ptr<Test> >::value);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

