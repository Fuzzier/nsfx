/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-08-09
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
#include <iostream>
#include <string>


NSFX_TEST_SUITE(Statistics)
{
    void Test(void)
    {
        Ptr<ISimulator> simulator = CreateObject<ISimulator>(
                "edu.uestc.nsfx.Simulator");
        Ptr<IScheduler> scheduler = CreateObject<IScheduler>(
                "edu.uestc.nsfx.SetScheduler");

        Ptr<IClock> clock(simulator);
        Ptr<IClockUser>(scheduler)->Use(clock);
        Ptr<ISchedulerUser>(simulator)->Use(scheduler);

        Ptr<IProbeContainer> probes = CreateObject<IProbeContainer>(
                "edu.uestc.nsfx.tutorial.StatisticsProvider");

        Ptr<ISchedulerUser>(probes)->Use(scheduler);
        Ptr<IInitializable>(probes)->Initialize();

        // Obtain a probe.
        Ptr<IProbeEvent> sizeProbe = probes->GetProbe("sample size");
        // If the probe does not exit, ProbeNotRegistered exception will be thrown.

        double totalSize = 0;
        Ptr<IProbeEventSink> summation =
            CreateEventSink<IProbeEventSink>(nullptr, [&] (double v) {
                totalSize += v;
            });

        double totalCount = 0;
        Ptr<IProbeEventSink> counter =
            CreateEventSink<IProbeEventSink>(nullptr, [&] (double ) {
                ++totalCount;
            });

        sizeProbe->Connect(summation);
        sizeProbe->Connect(counter);

        Ptr<ISummary> summary = CreateObject<ISummary>(
                "edu.uestc.nsfx.statistics.Summary");
        sizeProbe->Connect(summary);

        simulator->RunFor(Seconds(100));

        // Summary provides several commonly used statistics.
        double minValue = summary->Min();
        double maxValue = summary->Max();
        double count    = summary->Count();
        double sum      = summary->Sum();
        double mean     = summary->Mean();
        double stddev   = summary->Stddev();

        std::cout << "100 seconds:" << std::endl;
        std::cout << "min   : " << minValue << std::endl;
        std::cout << "max   : " << maxValue << std::endl;
        std::cout << "count : " << count    << std::endl;
        std::cout << "sum   : " << sum      << std::endl;
        std::cout << "mean  : " << mean     << std::endl;
        std::cout << "stddev: " << stddev   << std::endl;

        // Reset the statistics.
        summary->Reset();

        simulator->RunFor(Seconds(20));

        minValue = summary->Min();
        maxValue = summary->Max();
        count    = summary->Count();
        sum      = summary->Sum();
        mean     = summary->Mean();
        stddev   = summary->Stddev();

        std::cout << std::endl;
        std::cout << "20 seconds:" << std::endl;
        std::cout << "min   : " << minValue << std::endl;
        std::cout << "max   : " << maxValue << std::endl;
        std::cout << "count : " << count    << std::endl;
        std::cout << "sum   : " << sum      << std::endl;
        std::cout << "mean  : " << mean     << std::endl;
        std::cout << "stddev: " << stddev   << std::endl;

    }

    NSFX_TEST_CASE(Test)
    {
        try
        {
            Test();
        }
        catch (boost::exception& e)
        {
            std::cout << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

