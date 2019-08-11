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
    using nsfx::statistics::IProbeEvent;
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

            Ptr<ProbeContainer> pc(new Object<ProbeContainer>);

            Ptr<Probe> p1 = pc->Add(n1);
            Ptr<Probe> p2 = pc->Add(n2);

            NSFX_TEST_EXPECT(pc->Has(n1));
            NSFX_TEST_EXPECT(pc->Has(n2));

            Ptr<IProbeEvent> p1a = pc->GetProbe(n1);
            Ptr<IProbeEvent> p2a = pc->GetProbe(n2);
            NSFX_TEST_EXPECT(p1 == p1a);
            NSFX_TEST_EXPECT(p2 == p2a);

            Ptr<IProbeContainer> c = pc;
            Ptr<IProbeEvent> p1b = c->GetProbe(n1);
            Ptr<IProbeEvent> p2b = c->GetProbe(n2);
            NSFX_TEST_EXPECT(p1 == p1b);
            NSFX_TEST_EXPECT(p2 == p2b);
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

            Ptr<ProbeContainer> pc(new Object<ProbeContainer>);
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

    NSFX_TEST_CASE(Merge)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            Ptr<ProbeContainer> pc1(new Object<ProbeContainer>);
            Ptr<IProbeEvent> p1 = pc1->Add(n1);
            Ptr<IProbeEvent> p2 = pc1->Add(n2);
            Ptr<IProbeContainer> pc1a(pc1);

            Ptr<ProbeContainer> pc2(new Object<ProbeContainer>);
            Ptr<IProbeEvent> p3 = pc2->Add(n1);
            Ptr<IProbeEvent> p4 = pc2->Add(n2);
            Ptr<IProbeContainer> pc2a(pc2);

            pc1->MergeFrom("pc2.", pc2a);

            nsfx::unordered_set<std::string> items;
            items.emplace(n1);
            items.emplace(n2);
            items.emplace("pc2." + n1);
            items.emplace("pc2." + n2);

            NSFX_TEST_EXPECT(pc1a->GetProbe("pc2." + n1) == pc2a->GetProbe(n1));
            NSFX_TEST_EXPECT(pc1a->GetProbe("pc2." + n2) == pc2a->GetProbe(n2));

            Ptr<IProbeEnumerator> pe = pc1a->GetEnumerator();
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

