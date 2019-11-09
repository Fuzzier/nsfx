/**
 * @file
 *
 * @brief Simulation.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-08-08
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
#include <iostream>
#include <string>


NSFX_TEST_SUITE(Nameable)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            Ptr<ISimulator> simulator = CreateObject<ISimulator>(
                    "edu.uestc.nsfx.Simulator");
            Ptr<IScheduler> scheduler = CreateObject<IScheduler>(
                    "edu.uestc.nsfx.SetScheduler");

            Ptr<IClock> clock(simulator);
            Ptr<IClockUser>(scheduler)->Use(clock);
            Ptr<ISchedulerUser>(simulator)->Use(scheduler);

            Ptr<IEventSink<>> s = CreateEventSink<IEventSink<>>(nullptr, [&] {
                    std::cout << clock->Now() << std::endl;
                    if (clock->Now() <= TimePoint(Seconds(3)))
                    {
                        scheduler->ScheduleIn(Seconds(1), s);
                    }
                });

            scheduler->ScheduleNow(s);
            simulator->RunUntil(TimePoint(Seconds(1)));
            simulator->RunFor(Seconds(1));
            simulator->Run();

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

