/**
 * @file
 *
 * @brief Test CircularSequenceNumber.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-05
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/least-int.h>
#include <iostream>


NSFX_TEST_SUITE(LeastInt)/*{{{*/
{
    NSFX_TEST_CASE(0)
    {
        typedef nsfx::LeastInt<0>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint8_t>::value));

        typedef nsfx::LeastInt<0>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int32_t>::value));
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::LeastInt<8>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint8_t>::value));

        typedef nsfx::LeastInt<8>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int32_t>::value));
    }

    NSFX_TEST_CASE(17)
    {
        typedef nsfx::LeastInt<17>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint32_t>::value));

        typedef nsfx::LeastInt<17>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int32_t>::value));
    }

    NSFX_TEST_CASE(32)
    {
        typedef nsfx::LeastInt<32>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint32_t>::value));

        typedef nsfx::LeastInt<32>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int32_t>::value));
    }

    NSFX_TEST_CASE(33)
    {
        typedef nsfx::LeastInt<33>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint64_t>::value));

        typedef nsfx::LeastInt<33>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int64_t>::value));
    }

    NSFX_TEST_CASE(64)
    {
        typedef nsfx::LeastInt<64>::UintType Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint64_t>::value));

        typedef nsfx::LeastInt<64>::IntOpType OpType;
        NSFX_TEST_EXPECT((std::is_same<OpType, nsfx::int64_t>::value));
    }

}/*}}}*/


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

