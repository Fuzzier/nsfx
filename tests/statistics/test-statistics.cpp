/**
 * @file
 *
 * @brief Test Statistics.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-04-30
 *
 * @copyright Copyright (c) 2020.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/event/event-sink.h>
#include <nsfx/statistics/statistics-container.h>
#include <nsfx/statistics/statistics-enumerator.h>
#include <iostream>


NSFX_TEST_SUITE(Statistics)
{
    using nsfx::IObject;
    using nsfx::Object;
    using nsfx::Ptr;
    using nsfx::cookie_t;
    using nsfx::Event;
    using nsfx::CreateEventSink;
    using nsfx::IStatistic;
    using nsfx::IStatisticsContainer;
    using nsfx::IStatisticsEnumerator;
    using nsfx::StatisticsContainer;

    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        IDataWithPriorityStatSink, "edu.uestc.nsfx.IDataWithPriorityStatSink",
        ( void(double v, uint32_t priority) ));

    NSFX_DEFINE_EVENT_INTERFACE(
        IDataWithPriorityStat, "edu.uestc.nsfx.IDataWithPriorityStat",
        IDataWithPriorityStatSink);

    NSFX_TEST_CASE(StatisticsContainer)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            Ptr<StatisticsContainer> sc(new Object<StatisticsContainer>);

            Ptr<Event<IDataWithPriorityStat>> s1 = sc->Add<IDataWithPriorityStat>(n1);
            Ptr<Event<IDataWithPriorityStat>> s2 = sc->Add<IDataWithPriorityStat>(n2);

            NSFX_TEST_EXPECT(sc->Has(n1));
            NSFX_TEST_EXPECT(sc->Has(n2));

            Ptr<IStatistic> s1a = sc->Get(n1);
            Ptr<IStatistic> s2a = sc->Get(n2);
            NSFX_TEST_EXPECT(s1 == s1a);
            NSFX_TEST_EXPECT(s2 == s2a);

            Ptr<IStatisticsContainer> c = sc;
            Ptr<IStatistic> s1b = c->Get(n1);
            Ptr<IStatistic> s2b = c->Get(n2);
            NSFX_TEST_EXPECT(s1 == s1b);
            NSFX_TEST_EXPECT(s2 == s2b);

            Ptr<IDataWithPriorityStatSink> sink =
                CreateEventSink<IDataWithPriorityStatSink>(
                        nullptr, [] (double v, uint32_t priority) {
                    NSFX_TEST_EXPECT_EQ(v, 1.25);
                    NSFX_TEST_EXPECT_EQ(priority, 2);
                });
            cookie_t c1 = sc->Connect(n1, sink);
            cookie_t c2 = s2->Connect(sink);
            s1->Fire(1.25, 2);
            s2->Fire(1.25, 2);
            s1->Disconnect(c1);
            sc->Disconnect(n2, c2);
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

    NSFX_TEST_CASE(StatisticsEnumerator)
    {
        try
        {
            std::string n1 = "number of bits sent";
            std::string n2 = "number of bits received";

            nsfx::unordered_set<std::string> items;
            items.emplace(n1);
            items.emplace(n2);

            Ptr<StatisticsContainer> sc(new Object<StatisticsContainer>);
            sc->Add<IDataWithPriorityStat>(n1);
            sc->Add<IDataWithPriorityStat>(n2);

            Ptr<IStatisticsEnumerator> se = sc->GetEnumerator();
            while (se->HasNext())
            {
                std::string n = se->Next();
                NSFX_TEST_EXPECT(items.count(n) > 0);
                items.erase(n);
            }
            NSFX_TEST_EXPECT_EQ(items.size(), 0);

            se->Reset();
            items.emplace(n1);
            items.emplace(n2);
            while (se->HasNext())
            {
                std::string n = se->Next();
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

            Ptr<StatisticsContainer> sc1(new Object<StatisticsContainer>);
            Ptr<Event<IDataWithPriorityStat>> s1 = sc1->Add<IDataWithPriorityStat>(n1);
            Ptr<Event<IDataWithPriorityStat>> s2 = sc1->Add<IDataWithPriorityStat>(n2);
            Ptr<IStatisticsContainer> sc1a(sc1);

            Ptr<StatisticsContainer> sc2(new Object<StatisticsContainer>);
            Ptr<Event<IDataWithPriorityStat>> s3 = sc2->Add<IDataWithPriorityStat>(n1);
            Ptr<Event<IDataWithPriorityStat>> s4 = sc2->Add<IDataWithPriorityStat>(n2);
            Ptr<IStatisticsContainer> sc2a(sc2);

            sc1->MergeFrom("sc2.", sc2a);

            nsfx::unordered_set<std::string> items;
            items.emplace(n1);
            items.emplace(n2);
            items.emplace("sc2." + n1);
            items.emplace("sc2." + n2);

            NSFX_TEST_EXPECT(sc1a->Get("sc2." + n1) == sc2a->Get(n1));
            NSFX_TEST_EXPECT(sc1a->Get("sc2." + n2) == sc2a->Get(n2));

            Ptr<IStatisticsEnumerator> se = sc1a->GetEnumerator();
            while (se->HasNext())
            {
                std::string n = se->Next();
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

