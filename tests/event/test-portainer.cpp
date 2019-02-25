/**
 * @file
 *
 * @brief Test Portainer.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-01-23
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/event/portainer.h>
#include <iostream>
#include <set>


NSFX_TEST_SUITE(Portainer)
{
    // Default constructible, copy assignable.
    class Item
    {
    public:
        Item(void) :
            i_(0)
        {}

        Item(int i) :
            i_(i)
        {}

        bool operator!(void) const
        {
            return !i_;
        }

        int Get(void) const
        {
            return i_;
        }

    private:
        int i_;
    };

    NSFX_TEST_CASE(Add)
    {
        try
        {
            nsfx::Portainer<Item, 3> ct;
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 0);

            ////////////////////////////////////////
            // Add.
            nsfx::cookie_t cookies[3];
            Item b(1);
            cookies[0] = ct.Add(b); // copy
            cookies[1] = ct.Add(b);
            cookies[2] = ct.Add(Item(2)); // move
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 3);
            NSFX_TEST_EXPECT_EQ(cookies[0], 1);
            NSFX_TEST_EXPECT_EQ(cookies[1], 2);
            NSFX_TEST_EXPECT_EQ(cookies[2], 3);

            ////////////////////////////////////////
            // Connection limit reached.
            NSFX_TEST_EXPECT_EQ(ct.Add(Item(3)), 0);
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 3);

            ////////////////////////////////////////
            // Remove.
            ct.Remove(cookies[1]);
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 2);
            NSFX_TEST_EXPECT(!ct.Get(cookies[1]));

            ////////////////////////////////////////
            // Add again.
            cookies[1] = ct.Add(Item(4));
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 3);
            NSFX_TEST_EXPECT_EQ(cookies[1], 2);

            ////////////////////////////////////////
            // Visit connections.
            ct.Remove(cookies[1]);
            std::set<int> s;
            s.insert(1);
            s.insert(2);
            ct.Visit([&] (const Item& c) {
                NSFX_TEST_EXPECT(s.count(c.Get()) == 1);
                s.erase(c.Get());
            });
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

    NSFX_TEST_CASE(Add1)
    {
        try
        {
            nsfx::Portainer<Item, 1> ct;
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 0);

            ////////////////////////////////////////
            // Add.
            nsfx::cookie_t cookie;
            Item b(1);
            cookie = ct.Add(b); // copy
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 1);
            NSFX_TEST_EXPECT_EQ(cookie, 1);

            ////////////////////////////////////////
            // Connection limit reached.
            NSFX_TEST_EXPECT_EQ(ct.Add(Item(2)), 0);
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 1);

            ////////////////////////////////////////
            // Remove.
            ct.Remove(cookie);
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 0);
            NSFX_TEST_EXPECT(!ct[cookie]);

            ////////////////////////////////////////
            // Add again.
            cookie = ct.Add(Item(3));
            NSFX_TEST_EXPECT_EQ(ct.GetSize(), 1);
            NSFX_TEST_EXPECT_EQ(cookie, 1);

            ////////////////////////////////////////
            // Visit connections.
            std::set<int> s;
            s.insert(3);
            ct.Visit([&] (const Item& c) {
                NSFX_TEST_EXPECT(s.count(c.Get()) == 1);
                s.erase(c.Get());
            });
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

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

