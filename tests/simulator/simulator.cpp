#include <nsfx/test.h>
#include <nsfx/simulator/simulator.h>
#include <iostream>


NSFX_TEST_SUITE(Simulator)
{
    using nsfx::Ptr;

    struct ISinkInit : virtual nsfx::IObject
    {
        virtual ~ISinkInit() {}
        virtual void Wire(Ptr<> simulator) = 0;
    };

    NSFX_DEFINE_CLASS_UUID4(ISinkInit, 0x017EE551, 0x255B, 0x4798, 0x83FCFD903B0FD0C5LL);

    static int counter = 0;
    struct Sink :/*{{{*/
        ISinkInit,
        nsfx::IAlarmSink,
        nsfx::ISimulatorSink
    {
        virtual ~Sink(void) {}

        virtual void Wire(Ptr<> simulator)
        {
            simulator_ = simulator;
            NSFX_TEST_ASSERT(simulator_);
            clock_ = simulator;
            NSFX_TEST_ASSERT(clock_);
            alarm_ = simulator;
            NSFX_TEST_ASSERT(alarm_);
        }

        virtual void OnAlarm(void) NSFX_OVERRIDE
        {
            std::cout << clock_->Now() << ": " << ++counter << std::endl;
            if (counter < 10)
            {
                alarm_->ScheduleAt(clock_->Now() + nsfx::chrono::Seconds(1));
            }
            else if (counter < 20)
            {
                alarm_->ScheduleIn(nsfx::chrono::Seconds(1));
            }
        }

        virtual void OnSimulatorEvent(nsfx::SimulatorEventType type) NSFX_OVERRIDE
        {
            switch (type)
            {
            case nsfx::NSFX_SIMULATOR_EVENT_BEGIN:
                std::cout << "begin" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_RUN:
                std::cout << "run" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_PAUSE:
                std::cout << "pause" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_END:
                std::cout << "end" << std::endl;
                simulator_ = nullptr;
                clock_ = nullptr;
                alarm_ = nullptr;
                break;
            default:
                break;
            }
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(ISinkInit)
            NSFX_INTERFACE_ENTRY(IAlarmSink)
            NSFX_INTERFACE_ENTRY(ISimulatorSink)
        NSFX_INTERFACE_MAP_END()

    private:
        Ptr<nsfx::ISimulator> simulator_;
        Ptr<nsfx::IClock> clock_;
        Ptr<nsfx::IAlarm> alarm_;
    };/*}}}*/

    NSFX_DEFINE_CLASS_UUID4(Sink, 0xEDFE3391, 0x734E, 0x476C, 0x9FB27EDAE1370AEFLL);
    NSFX_REGISTER_CLASS(Sink);

    NSFX_TEST_CASE(Simulator)
    {
        typedef nsfx::Object<Sink>  SinkType;
        try
        {
            counter = 0;
            Ptr<>  o = nsfx::CreateObject<nsfx::IObject>(NSFX_CID_Simulator);
            NSFX_TEST_ASSERT(o);
            Ptr<nsfx::IAlarm>  alarm(o);
            NSFX_TEST_ASSERT(alarm);

            Ptr<>  sink(new SinkType);
            NSFX_TEST_ASSERT(sink);
            Ptr<ISinkInit>  sinkInit(sink);
            NSFX_TEST_ASSERT(sinkInit);
            sinkInit->Wire(o);

            Ptr<nsfx::IAlarmSink>  alarmSink(sink);
            NSFX_TEST_ASSERT(alarmSink);
            alarm->Connect(alarmSink);
            NSFX_TEST_EXPECT(!alarm->IsPending());

            Ptr<nsfx::ISimulatorSink>  simSink(sink);
            NSFX_TEST_ASSERT(simSink);

            Ptr<nsfx::ISimulator>  simulator(o);
            NSFX_TEST_ASSERT(simulator);

            nsfx::cookie_t cookie = simulator->Connect(
                simSink, nsfx::NSFX_SIMULATOR_EVENT_ALL);
            simulator->Disconnect(cookie);
            cookie = simulator->Connect(
                simSink, nsfx::NSFX_SIMULATOR_EVENT_ALL);

            Ptr<nsfx::IClock>  clock(o);
            NSFX_TEST_ASSERT(clock);

            // start at 1s.
            alarm->ScheduleAt(clock->Now() + nsfx::chrono::Seconds(1));
            NSFX_TEST_EXPECT(alarm->IsPending());

            // run to 1s.
            simulator->RunUntil(clock->Now() + nsfx::chrono::Seconds(1));
            NSFX_TEST_EXPECT_EQ(counter, 1);

            // run to 10s.
            simulator->RunFor(nsfx::chrono::Seconds(9));
            NSFX_TEST_EXPECT_EQ(counter, 10);

            // run to the end (20s).
            simulator->Run();
            NSFX_TEST_EXPECT_EQ(counter, 20);

            alarm->Disconnect();
            NSFX_TEST_EXPECT(!alarm->IsPending());
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what();
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

