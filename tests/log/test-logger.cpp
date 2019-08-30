/**
 * @file
 *
 * @brief Test Record.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/log/logger.h>
#include <nsfx/log/log-tool.h>
#include <nsfx/log/create-log-filter.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/event/event-sink.h>
#include <iostream>
#include <sstream>
#include <iomanip>


NSFX_TEST_SUITE(Logger)
{
    nsfx::TimePoint t;
    class Clock : public nsfx::IClock
    {
    public:
        virtual ~Clock(void) {}
        virtual nsfx::TimePoint Now(void) NSFX_OVERRIDE
        {
            return t;
        }
        NSFX_INTERFACE_MAP_BEGIN(Clock)
            NSFX_INTERFACE_ENTRY(nsfx::IClock)
        NSFX_INTERFACE_MAP_END()
    };

    nsfx::Ptr<nsfx::IClock> clock(new nsfx::Object<Clock>());

    NSFX_TEST_CASE(Output)
    {
        try
        {

            nsfx::Ptr<nsfx::ILogEventSinkEx> source =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");
            nsfx::Ptr<nsfx::ILogEventSinkEx> middle =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");

            nsfx::cookie_t c1 = middle->RegisterSource(source);
            nsfx::cookie_t c2 = middle->RegisterSource(source);
            nsfx::cookie_t c3 = middle->RegisterSource(source);
            NSFX_TEST_ASSERT_NE(c1, c2);
            NSFX_TEST_ASSERT_NE(c1, c3);
            NSFX_TEST_ASSERT_NE(c2, c3);
            middle->UnregisterSource(c2);
            middle->UnregisterSource(c3);
            c2 = middle->RegisterSource(source);
            c3 = middle->RegisterSource(source);

            std::string output;
            size_t count = 0;
            nsfx::Ptr<nsfx::ILogEventSink> sink =
                nsfx::CreateEventSink<nsfx::ILogEventSink>(
                        nullptr, [&] (nsfx::LogRecord r) {
                std::ostringstream oss;
                if (r.Exists<nsfx::LogSeverityTraits>())
                {
                    oss << "[" << r.Get<nsfx::LogSeverityTraits>() << "] ";
                }
                oss << r.Get<nsfx::LogMessageTraits>() << std::endl;
                output = oss.str();
                ++count;
            });

            // Log (without terminal sink)
            NSFX_LOG(source)       << "plain";
            NSFX_LOG_FATAL(source) << "fatal";
            NSFX_LOG_ERROR(source) << "error";
            NSFX_LOG_WARN(source)  << "warn";
            NSFX_LOG_INFO(source)  << "info";
            NSFX_LOG_DEBUG(source) << "debug";
            NSFX_LOG_TRACE(source) << "trace";
            NSFX_TEST_EXPECT_EQ(count, 0);
            NSFX_TEST_EXPECT(output.empty());

            // Log (with terminal sink)
            nsfx::Ptr<nsfx::ILogEvent>(middle)->Connect(sink);
            NSFX_LOG(source)       << "plain";
            NSFX_LOG_FATAL(source) << "fatal";
            NSFX_LOG_ERROR(source) << "error";
            NSFX_LOG_WARN(source)  << "warn";
            NSFX_LOG_INFO(source)  << "info";
            NSFX_LOG_DEBUG(source) << "debug";
            NSFX_LOG_TRACE(source) << "trace";
            NSFX_TEST_EXPECT_EQ(count, 21);
            NSFX_TEST_EXPECT(!output.empty());
            output.clear();

            // Log (to terminal sink)
            NSFX_LOG(sink) << "plain";
            NSFX_TEST_EXPECT_EQ(count, 22);
            NSFX_TEST_EXPECT(!output.empty());
            output.clear();

            middle->UnregisterSource(c2);
            middle->UnregisterSource(c3);
            middle->UnregisterAllSources();

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false);
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false);
            std::cerr << e.what() << std::endl;
        }
    }

    NSFX_TEST_CASE(Filter)
    {
        try
        {
            nsfx::Ptr<nsfx::ILogEventSinkEx> logger =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");

            // Add a pending value.
            logger->AddValue("Value", nsfx::MakeConstantLogValue<int>(10));

            // Create a terminal log sink.
            bool output = false;
            nsfx::LogRecord record;
            nsfx::Ptr<nsfx::ILogEventSink> sink =
                nsfx::CreateEventSink<nsfx::ILogEventSink>(
                        nullptr, [&] (nsfx::LogRecord r) {
                output = true;
                record = r;
            });

            nsfx::Ptr<nsfx::ILogEvent>(logger)->Connect(sink);

            ////////////////////
            // Log without filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is added.
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_ASSERT(record.Exists("Value"));
            NSFX_TEST_EXPECT_EQ(record.Get<int>("Value"), 10);
            output = false;
            record = nsfx::LogRecord();

            ////////////////////
            // Set a filter.
            nsfx::Ptr<nsfx::ILogFilter> filter =
                    nsfx::CreateLogFilter([] (const nsfx::LogRecord& r) {
                return (r.Exists("Value") && r.Get<int>("Value") > 0) ?
                       nsfx::LOG_DISCARD : nsfx::LOG_ACCEPT;
            });
            logger->SetFilter(filter);

            ////////////////////
            // Log with filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is added before filtering.
            NSFX_TEST_EXPECT(!output);

            ////////////////////
            // Update the pending value.
            logger->UpdateValue("Value", nsfx::MakeConstantLogValue<int>(0));

            // Log with filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is updated.
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_ASSERT(record.Exists("Value"));
            NSFX_TEST_EXPECT_EQ(record.Get<int>("Value"), 0);
            output = false;
            record = nsfx::LogRecord();

            ////////////////////
            // Remove the pending value.
            logger->RemoveValue("Value");

            // Log with filter.
            NSFX_LOG(logger) << "plain";
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_EXPECT(!record.Exists("Value"));

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false);
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false);
            std::cerr << e.what() << std::endl;
        }
    }

    NSFX_TEST_CASE(HighOrderValue)
    {
        try
        {
            // A second-order log value.
            nsfx::LogValue ts = nsfx::MakeLogValue<nsfx::LogValue>(
            [&] { return nsfx::MakeConstantLogValue<nsfx::TimePoint>(
                             clock->Now());
            });

            nsfx::Ptr<nsfx::ILogEventSinkEx> logger =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");

            // Add a pending value.
            logger->AddValue("Timestamp", ts);

            // Create a terminal log sink.
            bool output = false;
            nsfx::LogRecord record;
            nsfx::Ptr<nsfx::ILogEventSink> sink =
                nsfx::CreateEventSink<nsfx::ILogEventSink>(
                        nullptr, [&] (nsfx::LogRecord r) {
                output = true;
                record = r;
            });

            nsfx::Ptr<nsfx::ILogEvent>(logger)->Connect(sink);

            ////////////////////
            nsfx::TimePoint t0(nsfx::Seconds(1));
            nsfx::TimePoint t1(nsfx::Seconds(2));

            ////////////////////
            // Log at t0.
            t = t0;
            NSFX_TEST_EXPECT_EQ(clock->Now(), t0);
            NSFX_LOG(logger) << "plain";

            // Examine the record at t1.
            t = t1;
            NSFX_TEST_EXPECT_EQ(clock->Now(), t1);

            NSFX_TEST_EXPECT(output);
            NSFX_TEST_ASSERT(record.Exists("Timestamp"));
            NSFX_TEST_EXPECT_EQ(record.Get<nsfx::TimePoint>("Timestamp"), t0);
            output = false;
            record = nsfx::LogRecord();

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false);
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false);
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

