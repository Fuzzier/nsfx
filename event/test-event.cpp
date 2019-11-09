/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-22
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
#include "i-countdown.h"
#include "i-alarm-event.h"
#include <iostream>
#include <string>


NSFX_TEST_SUITE(CountdownAlarm)
{
    void Test(void)
    {
        Ptr<ICountdown> alarm = CreateObject<ICountdown>(
                    "edu.uestc.nsfx.tutorial.CountdownAlarm");

        // Connect two event sinks.
        Ptr<IAlarmEventSink> sink1 =
            CreateEventSink<IAlarmEventSink>(nullptr, [] {
                std::cout << "Alarm fired!" << std::endl;
            });
        cookie_t cookie1 = Ptr<IAlarmEvent>(alarm)->Connect(sink1);

        Ptr<IAlarmEventSink> sink2 = CreateObject<IObject>(
                    "edu.uestc.nsfx.tutorial.AlarmListener");
        cookie_t cookie2 = Ptr<IAlarmEvent>(alarm)->Connect(sink2);

        // Fire the event.
        alarm->SetCounter(1);
        alarm->Decrement();

        // Disconnect the first event sink.
        Ptr<IAlarmEvent>(alarm)->Disconnect(cookie1);
        cookie1 = 0;

        // Fire the event again.
        alarm->SetCounter(1);
        alarm->Decrement();
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

