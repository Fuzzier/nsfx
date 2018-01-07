#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <iostream>

#if defined(NDEBUG)
# error  Test cannot compile, since NDEBUG is defined.
#endif // defined(NDEBUG)


#define TEST_THROW(x, expr) \
    try { expr; NSFX_TEST_EXPECT(false); } \
    catch(x& ) { /* Should come here */ } \
    catch(boost::exception& e) { NSFX_TEST_EXPECT(false) << diagnostic_information(e); } \
    catch(std::exception& e) { NSFX_TEST_EXPECT(false) << e.what(); }

#define TEST_NOTHROW(expr) \
    try { expr; } \
    catch(boost::exception& e) { NSFX_TEST_EXPECT(false) << diagnostic_information(e); } \
    catch(std::exception& e) { NSFX_TEST_EXPECT(false) << e.what(); }


NSFX_TEST_SUITE(Buffer)
{
    NSFX_TEST_CASE(Ctor0)/*{{{*/
    {
        nsfx::Buffer b0;
        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
        NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
        NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 0);
        NSFX_TEST_EXPECT(!b0.GetStorage());
    }/*}}}*/

    NSFX_TEST_CASE(Ctor1)/*{{{*/
    {
        try
        {
            {
                nsfx::Buffer b0(1024);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1024);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);
            }

            {
                nsfx::Buffer b0(1024, 512);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);
            }
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(AddAtStart)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1024, 512);
            // data not moved.
            b0.AddAtStart(512);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // data moved.
            b0.AddAtStart(512);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1024);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // buffer reallocated.
            b0.AddAtStart(1);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1025);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1025);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1025);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(AddAtEnd)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1024, 512);
            // data not moved.
            b0.AddAtEnd(512);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1024);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // data moved.
            b0.AddAtEnd(512);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1024);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // buffer reallocated.
            b0.AddAtEnd(1);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1025);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 1025);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1025);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(RemoveAtStart)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1024, 512);
            b0.AddAtStart(8);
            // data area not empty.
            b0.RemoveAtStart(4);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 4);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 508);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // data area becomes empty.
            b0.RemoveAtStart(8);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(RemoveAtEnd)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1024, 512);
            b0.AddAtEnd(8);
            // data area not empty.
            b0.RemoveAtEnd(4);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 4);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 516);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

            // data area becomes empty.
            b0.RemoveAtEnd(8);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
            NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(CopyMoveSwap)/*{{{*/
    {
        try
        {
            // copy ctor
            {
                nsfx::Buffer b0(1024, 512);
                nsfx::Buffer b1(b0);

                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage() == b1.GetStorage());
            }

            // copy assignment
            {
                nsfx::Buffer b0(1024, 512);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                nsfx::Buffer b1(456, 123);
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 123);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 123);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 456);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

                b1 = b0;
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);
                NSFX_TEST_EXPECT(b0.GetStorage() == b1.GetStorage());

            }

            // move ctor
            {
                nsfx::Buffer b0(1024, 512);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                nsfx::Buffer b1(std::move(b0));
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 0);
                NSFX_TEST_ASSERT(!b0.GetStorage());

                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

            }

            // move assignment
            {
                nsfx::Buffer b0(1024, 512);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                nsfx::Buffer b1(456, 123);
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 123);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 123);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 456);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

                b1 = std::move(b0);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 0);
                NSFX_TEST_ASSERT(!b0.GetStorage());

                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

            }

            // swap
            {
                nsfx::Buffer b0(1024, 512);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                nsfx::Buffer b1(456, 123);
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 123);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 123);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 456);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

                swap(b0, b1);
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetDataStart(), 123);
                NSFX_TEST_EXPECT_EQ(b0.GetDataEnd(), 123);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->size_, 456);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dataLockCount_, 0);

                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 512);
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 512);
                NSFX_TEST_ASSERT(b1.GetStorage());
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->size_, 1024);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dataLockCount_, 0);

            }

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

}


NSFX_TEST_SUITE(BufferIterator)
{
    NSFX_TEST_CASE(Comparison)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1000, 500);
            b0.AddAtStart(50);

            nsfx::BufferIterator it0 = b0.begin();
            nsfx::BufferIterator it1 = b0.end();

            NSFX_TEST_EXPECT(it0 == it0);
            NSFX_TEST_EXPECT(it0 <= it0);
            NSFX_TEST_EXPECT(it0 >= it0);

            NSFX_TEST_EXPECT(it1 == it1);
            NSFX_TEST_EXPECT(it1 <= it1);
            NSFX_TEST_EXPECT(it1 >= it1);

            NSFX_TEST_EXPECT(it0 != it1);
            NSFX_TEST_EXPECT(it1 != it0);

            NSFX_TEST_EXPECT(it0 < it1);
            NSFX_TEST_EXPECT(it0 <= it1);

            NSFX_TEST_EXPECT(it1 > it0);
            NSFX_TEST_EXPECT(it1 >= it0);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(Move)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1000, 500);
            b0.AddAtStart(50);

            nsfx::BufferIterator it = b0.begin();
            NSFX_TEST_EXPECT_EQ(it.GetStart(), b0.GetDataStart());
            NSFX_TEST_EXPECT_EQ(it.GetEnd(), b0.GetDataEnd());
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), b0.GetDataStart());

            // cannot move beyond the start
            TEST_THROW(nsfx::OutOfBounds, it.MoveBackward(1));

            // cannot move beyond the end
            it.MoveForward(50);
            TEST_THROW(nsfx::OutOfBounds, it.MoveForward(1));
            auto it2 = b0.end();
            NSFX_TEST_EXPECT(it == it2);
            it.MoveBackward(50);

            ++it;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 451);
            --it;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 450);

            auto it1 = it++;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 451);
            NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 450);

            it1 = it--;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 450);
            NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 451);

            it += 50;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 500);
            it -= 50;
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 450);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(IO)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1000, 500);
            b0.AddAtStart(50);
            nsfx::BufferIterator it = b0.begin();
            nsfx::BufferStorage* storage = b0.GetStorage();
            uint8_t*  p8  = reinterpret_cast<uint8_t* >(storage->bytes_ + 450);
            uint16_t* p16 = reinterpret_cast<uint16_t*>(storage->bytes_ + 450);
            uint32_t* p32 = reinterpret_cast<uint32_t*>(storage->bytes_ + 450);
            uint64_t* p64 = reinterpret_cast<uint64_t*>(storage->bytes_ + 450);
            // n: native, r: reverse
            uint8_t  n8  = 0xfe;
            uint8_t  r8  = 0xfe;
            uint16_t n16 = 0xfedc;
            uint16_t r16 = 0xdcfe;
            uint32_t n32 = 0xfedcba98UL;
            uint32_t r32 = 0x98badcfeUL;
            uint64_t n64 = 0xfedcba9876543210ULL;
            uint64_t r64 = 0x1032547698badcfeULL;
            // l: little, b: big
#if defined(BOOST_LITTLE_ENDIAN)
            uint8_t  l8  = n8;
            uint8_t  b8  = r8;
            uint16_t l16 = n16;
            uint16_t b16 = r16;
            uint32_t l32 = n32;
            uint32_t b32 = r32;
            uint64_t l64 = n64;
            uint64_t b64 = r64;
#else // if defined(BOOST_BIG_ENDIAN)
            uint8_t  l8  = r8;
            uint8_t  b8  = n8;
            uint16_t l16 = r16;
            uint16_t b16 = n16;
            uint32_t l32 = r32;
            uint32_t b32 = n32;
            uint64_t l64 = r64;
            uint64_t b64 = n64;
#endif // defined(BOOST_LITTLE_ENDIAN)

            // native
            it.Write<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(n8));

            it.Write<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, n8);

            it.Write<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(n16));

            it.Write<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, n16);

            it.Write<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(n32));

            it.Write<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, n32);

            it.Write<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(n64));

            it.Write<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, n64);

            // little
            it.WriteL<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(l8));

            it.WriteL<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, l8);

            it.WriteL<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(l16));

            it.WriteL<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, l16);

            it.WriteL<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(l32));

            it.WriteL<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, l32);

            it.WriteL<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(l64));

            it.WriteL<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            // big
            it.WriteB<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(b8));

            it.WriteB<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, b8);

            it.WriteB<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(b16));

            it.WriteB<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, b16);

            it.WriteB<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(b32));

            it.WriteB<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, b32);

            it.WriteB<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(b64));

            it.WriteB<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(Copy)/*{{{*/
    {
        try
        {
            // destination buffer has enough space for data and post-data area,
            // leave same post-data space
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(1000, 500);
                b1.AddAtStart(50);
                b1.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateData());
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 920); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 950);
            }

            // destination buffer has enough space for data area,
            // but not enough space for the extra post-data area,
            // leave no pre-data space and smaller post-data space
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(70, 40);
                b1.AddAtStart(50);
                b1.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateData());
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 0); // post-data = 40
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 30);
            }

            // destination buffer has not enough space for data area,
            // clone the source buffer
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(20, 10);
                b1.AddAtStart(10);
                b1.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateData());
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 20); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 50);
                nsfx::Buffer b2;
                b2.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateData());
                NSFX_TEST_EXPECT_EQ(b2.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b2.GetDataStart(), 20); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b2.GetDataEnd(), 50);
            }

            // destination buffer has equal or larger space than source,
            // leave same post-data space
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(1000, 500);
                b1.AddAtStart(50);
                b1.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateSize());
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 920); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 950);
            }

            // destination buffer has smaller space than source,
            // clone the source buffer
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(20, 10);
                b1.AddAtStart(10);
                b1.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateSize());
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 20); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 50);
                nsfx::Buffer b2;
                b2.CopyFrom(b0, nsfx::BufferCopyPolicyAccommodateData());
                NSFX_TEST_EXPECT_EQ(b2.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b2.GetDataStart(), 20); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b2.GetDataEnd(), 50);
            }

            // clone
            {
                nsfx::Buffer b0(100, 50);
                b0.AddAtStart(30); // data = 30, post-data = 50
                nsfx::Buffer b1(20, 10);
                b1.AddAtStart(10);
                b1 = b0.Copy();
                NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
                NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 20); // post-data = 50
                NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 50);
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

    }/*}}}*/

    NSFX_TEST_CASE(DataLock)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(1000, 500);
            b0.AddAtStart(50);
            // check reference count and data lock count
            {
                nsfx::BufferStorage* storage = b0.GetStorage();
                NSFX_TEST_ASSERT(storage);
                NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(storage->dataLockCount_, 0);
            }
            nsfx::BufferIterator it = b0.begin();
            // the iterator holds a reference count and a data lock count
            {
                nsfx::BufferStorage* storage = b0.GetStorage();
                NSFX_TEST_ASSERT(storage);
                NSFX_TEST_EXPECT_EQ(storage->refCount_, 2);
                NSFX_TEST_EXPECT_EQ(storage->dataLockCount_, 1);
            }

            // add
            TEST_THROW(nsfx::Unexpected, b0.AddAtStart(1));
            TEST_THROW(nsfx::Unexpected, b0.AddAtEnd(1));
            // remove
            TEST_THROW(nsfx::Unexpected, b0.RemoveAtStart(1));
            TEST_THROW(nsfx::Unexpected, b0.RemoveAtEnd(1));
            // copy from
            TEST_THROW(nsfx::Unexpected, {
                nsfx::Buffer b1;
                b0.CopyFrom(b1);
            });
            TEST_THROW(nsfx::Unexpected, {
                    nsfx::Buffer b1;
                    b0.CopyFrom(b1, nsfx::BufferCopyPolicyAccommodateData());
            });
            TEST_THROW(nsfx::Unexpected, {
                    nsfx::Buffer b1;
                    b0.CopyFrom(b1, nsfx::BufferCopyPolicyAccommodateSize());
            });
            // copy to
            TEST_THROW(nsfx::Unexpected, {
                nsfx::Buffer b1;
                b1.CopyTo(b0);
            });
            TEST_THROW(nsfx::Unexpected, {
                    nsfx::Buffer b1;
                    b0.CopyFrom(b1, nsfx::BufferCopyPolicyAccommodateData());
            });
            TEST_THROW(nsfx::Unexpected, {
                    nsfx::Buffer b1;
                    b0.CopyFrom(b1, nsfx::BufferCopyPolicyAccommodateSize());
            });

            nsfx::Buffer b1(123, 45);
            b0 = b1;
            // add
            TEST_NOTHROW(b0.AddAtStart(1));
            TEST_NOTHROW(b0.AddAtEnd(1));
            // remove
            TEST_NOTHROW(b0.RemoveAtStart(1));
            TEST_NOTHROW(b0.RemoveAtEnd(1));
            // copy from
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b0.CopyFrom(b2);
            });
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b0.CopyFrom(b2, nsfx::BufferCopyPolicyAccommodateData());
            });
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b0.CopyFrom(b2, nsfx::BufferCopyPolicyAccommodateSize());
            });
            // copy to
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b2.CopyTo(b0);
            });
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b2.CopyTo(b0, nsfx::BufferCopyPolicyAccommodateData());
            });
            TEST_NOTHROW({
                nsfx::Buffer b2(678, 90);
                b2.CopyTo(b0, nsfx::BufferCopyPolicyAccommodateSize());
            });

            // data lock count
            {
                nsfx::BufferStorage* storage = it.GetStorage();
                NSFX_TEST_ASSERT(storage);
                NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(storage->dataLockCount_, 1);
                // release, check there's no leak
                nsfx::BufferStorage::AddRef(storage);
                nsfx::BufferIterator it2;
                it = it2;
                NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(storage->dataLockCount_, 0);
                nsfx::BufferStorage::Release(storage);
            }

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }

    }/*}}}*/

    NSFX_TEST_CASE(Chunk)/*{{{*/
    {
        try
        {
            nsfx::Buffer b0(100, 50);
            b0.AddAtStart(30); // data = 30, post-data = 50
            nsfx::Buffer b1;
            b1 = b0.GetChunk(0, 30);
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 30); // data = 30
            NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 20); // post-data = 50
            NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 50);
            b1 = b0.GetChunk(1, 28);
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 28); // data = 28
            NSFX_TEST_EXPECT_EQ(b1.GetDataStart(), 21); // post-data = 51
            NSFX_TEST_EXPECT_EQ(b1.GetDataEnd(), 49);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what();
        }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

