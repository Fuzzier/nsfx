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
#include <nsfx/network/packet/tag.h>
#include <iostream>


int k = 0;
struct Test
{
    Test(int i, double j) : i_(i), j_(j) { ++k; }
    ~Test(void) { --k; }
    int i_;
    double j_;
};


NSFX_TEST_SUITE(Tag)
{
    NSFX_TEST_CASE(MakeTag)
    {
        {
            size_t tagId = 1;
            nsfx::Tag tag = nsfx::MakeTag<Test>(tagId, 2, 3.4);
            // Test::Test() is called.
            NSFX_TEST_EXPECT_EQ(k, 1);
            NSFX_TEST_EXPECT_EQ(tag.GetId(), tagId);
            NSFX_TEST_EXPECT(tag.GetTypeId() ==
                             boost::typeindex::type_id<Test>());
            const Test& t = tag.GetValue<Test>();
            NSFX_TEST_EXPECT_EQ(t.i_, 2);
            NSFX_TEST_EXPECT_EQ(t.j_, 3.4);
        }
        // Test::~Test() is called.
        NSFX_TEST_EXPECT_EQ(k, 0);
    }

    // NSFX_TEST_CASE(MakeTagIndex)
    // {
    //     {
    //         size_t tagId = 1;
    //         nsfx::Tag tag = nsfx::MakeTag<Test>(tagId, 2, 3.4);
    //         size_t tagStart = 5;
    //         size_t tagEnd   = 6;
    //         nsfx::TagIndex idx = tag.MakeTagIndex(tagStart, tagEnd);
    //         NSFX_TEST_EXPECT_EQ(idx.tagId_, tagId);
    //         NSFX_TEST_EXPECT_EQ(idx.tagStart_, tagStart);
    //         NSFX_TEST_EXPECT_EQ(idx.tagEnd_, tagEnd);
    //         NSFX_TEST_EXPECT(nsfx::TagStorage::GetTypeId(idx.tag_) ==
    //                          boost::typeindex::type_id<Test>());
    //         NSFX_TEST_EXPECT_EQ(idx.tag_->refCount_, 2);
    //         const Test& t = nsfx::TagStorage::GetValue<Test>(idx.tag_);
    //         NSFX_TEST_EXPECT_EQ(t.i_, 2);
    //         NSFX_TEST_EXPECT_EQ(t.j_, 3.4);
    //         nsfx::TagIndex::Release(&idx);
    //     }
    //     NSFX_TEST_EXPECT_EQ(k, 0);
    // }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

