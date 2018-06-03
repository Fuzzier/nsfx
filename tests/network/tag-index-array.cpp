/**
 * @file
 *
 * @brief Test Tag.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-02
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/packet/tag-index.h>
#include <iostream>


int k = 0;
struct Test
{
    Test(int i, double j) : i_(i), j_(j) { ++k; }
    ~Test(void) { --k; }
    int i_;
    double j_;
};


NSFX_TEST_SUITE(TagIndexArray)
{
    NSFX_TEST_CASE(Allocate)
    {
        nsfx::TagIndexArray* tia = nsfx::TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);
        nsfx::TagIndexArray::Release(tia);
    }

    NSFX_TEST_CASE(RefCount)
    {
        nsfx::TagIndexArray* tia = nsfx::TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);

        // Addref.
        nsfx::TagIndexArray::AddRef(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 2);

        // Release.
        nsfx::TagIndexArray::Release(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);

        // Add elements.
        nsfx::TagIndex* idx = tia->indices_;
        for (size_t i = 0; i < tia->capacity_; ++i)
        {
            nsfx::TagStorage* tag = nsfx::TagStorage::Allocate<Test>(1, 2.3);
            size_t tagId = 4;
            size_t tagStart = 5;
            size_t tagEnd = 6;
            nsfx::TagIndex::Ctor(idx, tagId, tagStart, tagEnd, tag);
            ++tia->dirty_;
            ++idx;
        }
        NSFX_TEST_EXPECT_EQ(k, tia->dirty_);

        // Release.
        // All elements shall also be released.
        nsfx::TagIndexArray::Release(tia);
        NSFX_TEST_EXPECT_EQ(k, 0);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

