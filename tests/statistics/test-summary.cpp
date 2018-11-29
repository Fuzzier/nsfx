/**
 * @file
 *
 * @brief Test Summary.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/statistics/probe/probe.h>
#include <nsfx/statistics/probe/probe-container.h>
#include <nsfx/statistics/probe/probe-enumerator.h>
#include <nsfx/statistics/summary/summary.h>
#include <boost\math\special_functions\fpclassify.hpp>
#include <iostream>


NSFX_TEST_SUITE(Probe)
{
    using nsfx::Ptr;
    using nsfx::Object;
    using nsfx::statistics::IProbe;
    using nsfx::statistics::Probe;
    using nsfx::statistics::ProbeContainer;
    using nsfx::statistics::ISummary;

    NSFX_TEST_CASE(Summary)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            Ptr<ProbeContainer> pc(new Object<ProbeContainer>());
            Ptr<Probe> p1 = pc->Insert(n1);
            Ptr<Probe> p2 = pc->Insert(n2);

            Ptr<ISummary> s = nsfx::CreateObject<ISummary>(
                                "edu.uestc.nsfx.statistics.Summary");
            p1->Connect(s);
            p2->Connect(s);

            NSFX_TEST_EXPECT((boost::math::isnan)(s->Min()));
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Max()));
            NSFX_TEST_EXPECT_EQ(s->Count(), 0);
            NSFX_TEST_EXPECT_EQ(s->Sum(), 0);
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Mean()));
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Stddev()));

            p1->Fire(1);
            NSFX_TEST_EXPECT_EQ(s->Min(), 1);
            NSFX_TEST_EXPECT_EQ(s->Max(), 1);
            NSFX_TEST_EXPECT_EQ(s->Count(), 1);
            NSFX_TEST_EXPECT_EQ(s->Sum(), 1);
            NSFX_TEST_EXPECT_EQ(s->Mean(), 1);
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Stddev()));

            p2->Fire(3);
            NSFX_TEST_EXPECT_EQ(s->Min(), 1);
            NSFX_TEST_EXPECT_EQ(s->Max(), 3);
            NSFX_TEST_EXPECT_EQ(s->Count(), 2);
            NSFX_TEST_EXPECT_EQ(s->Sum(), 4);
            NSFX_TEST_EXPECT_EQ(s->Mean(), 2);
            NSFX_TEST_EXPECT_EQ(s->Stddev(), std::sqrt(2.0));

            s->Reset();
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Min()));
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Max()));
            NSFX_TEST_EXPECT_EQ(s->Count(), 0);
            NSFX_TEST_EXPECT_EQ(s->Sum(), 0);
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Mean()));
            NSFX_TEST_EXPECT((boost::math::isnan)(s->Stddev()));

        }
        catch (boost::exception& e)
        {
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

