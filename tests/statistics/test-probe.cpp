/**
 * @file
 *
 * @brief Test Probe.
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
#include <iostream>


NSFX_TEST_SUITE(Probe)
{
    using nsfx::Ptr;
    using nsfx::Object;
    using nsfx::statistics::IProbe;
    using nsfx::statistics::IProbeContainer;
    using nsfx::statistics::IProbeEnumerator;
    using nsfx::statistics::Probe;
    using nsfx::statistics::ProbeContainer;

    NSFX_TEST_CASE(ProbeContainer)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            Ptr<ProbeContainer> pc(new Object<ProbeContainer>());

            Ptr<Probe> p1 = pc->Add(n1);
            Ptr<Probe> p2 = pc->Add(n2);
            NSFX_TEST_EXPECT_EQ(p1->GetName(),n1);
            NSFX_TEST_EXPECT_EQ(p2->GetName(),n2);

            NSFX_TEST_EXPECT(pc->Has(n1));
            NSFX_TEST_EXPECT(pc->Has(n2));

            p1 = pc->Get(n1);
            p2 = pc->Get(n2);
            NSFX_TEST_EXPECT_EQ(p1->GetName(), n1);
            NSFX_TEST_EXPECT_EQ(p2->GetName(), n2);

            Ptr<IProbeContainer> c = pc;
            Ptr<IProbe> p1a = c->GetProbe(n1);
            Ptr<IProbe> p2a = c->GetProbe(n2);
            NSFX_TEST_EXPECT(p1 == p1a);
            NSFX_TEST_EXPECT(p2 == p2a);
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

    NSFX_TEST_CASE(ProbeEnumerator)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            nsfx::unordered_set<std::string> items;
            items.emplace(n1);
            items.emplace(n2);

            Ptr<ProbeContainer> pc(new Object<ProbeContainer>());
            pc->Add(n1);
            pc->Add(n2);

            Ptr<IProbeEnumerator> pe = pc->GetEnumerator();
            while (pe->HasNext())
            {
                std::string n = pe->Next();
                NSFX_TEST_EXPECT(items.count(n) > 0);
                items.erase(n);
            }
            NSFX_TEST_EXPECT_EQ(items.size(), 0);

            pe->Reset();
            items.emplace(n1);
            items.emplace(n2);
            while (pe->HasNext())
            {
                std::string n = pe->Next();
                NSFX_TEST_EXPECT(items.count(n) > 0);
                items.erase(n);
            }
            NSFX_TEST_EXPECT_EQ(items.size(), 0);
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

