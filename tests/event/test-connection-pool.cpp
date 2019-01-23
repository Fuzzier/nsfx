/**
 * @file
 *
 * @brief Test Event.
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
#include <nsfx/event/connection-pool.h>
#include <iostream>
#include <set>


NSFX_TEST_SUITE(ConnectionPool)
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

    NSFX_TEST_CASE(Connect)
    {
        try
        {
            nsfx::ConnectionPool<Item, 3> cp;
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 0);

            ////////////////////////////////////////
            // Cannot insert empty item.
            Item a;
            try
            {
                cp.Connect(a);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::InvalidArgument& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Connect.
            nsfx::cookie_t cookies[3];
            Item b(1);
            cookies[0] = cp.Connect(b); // copy
            cookies[1] = cp.Connect(b);
            cookies[2] = cp.Connect(Item(2)); // move
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 3);
            NSFX_TEST_EXPECT_EQ(cookies[0], 1);
            NSFX_TEST_EXPECT_EQ(cookies[1], 2);
            NSFX_TEST_EXPECT_EQ(cookies[2], 3);

            ////////////////////////////////////////
            // Connection limit reached.
            try
            {
                cp.Connect(Item(3));
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::ConnectionLimit& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Disconnect.
            cp.Disconnect(cookies[1]);
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 2);
            try
            {
                cp.GetConnection(cookies[1]);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoConnection& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Connect again.
            cookies[1] = cp.Connect(Item(4));
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 3);
            NSFX_TEST_EXPECT_EQ(cookies[1], 2);

            ////////////////////////////////////////
            // Visit connections.
            cp.Disconnect(cookies[1]);
            std::set<int> s;
            s.insert(1);
            s.insert(2);
            cp.Visit([&] (const Item& c) {
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

    NSFX_TEST_CASE(Connect1)
    {
        try
        {
            nsfx::ConnectionPool<Item, 1> cp;
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 0);

            ////////////////////////////////////////
            // Cannot insert empty item.
            Item a;
            try
            {
                cp.Connect(a);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::InvalidArgument& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Connect.
            nsfx::cookie_t cookie;
            Item b(1);
            cookie = cp.Connect(b); // copy
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 1);
            NSFX_TEST_EXPECT_EQ(cookie, 1);

            ////////////////////////////////////////
            // Connection limit reached.
            try
            {
                cp.Connect(Item(2));
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::ConnectionLimit& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Disconnect.
            cp.Disconnect(cookie);
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 0);
            try
            {
                cp.GetConnection(cookie);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::NoConnection& )
            {
                // Should come here.
            }

            ////////////////////////////////////////
            // Connect again.
            cookie = cp.Connect(Item(3));
            NSFX_TEST_EXPECT_EQ(cp.GetNumConnections(), 1);
            NSFX_TEST_EXPECT_EQ(cookie, 1);

            ////////////////////////////////////////
            // Visit connections.
            std::set<int> s;
            s.insert(3);
            cp.Visit([&] (const Item& c) {
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

