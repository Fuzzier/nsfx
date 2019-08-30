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
#include <nsfx/log/log-stream-sink.h>
#include <nsfx/log/log-tool.h>
#include <nsfx/log/create-log-filter.h>
#include <nsfx/log/create-log-formatter.h>
#include <nsfx/simulation/i-clock.h>
#include <iostream>
#include <sstream>
#include <iomanip>


NSFX_TEST_SUITE(LogStreamSink)
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
            nsfx::Ptr<nsfx::ILogEventSinkEx> logger =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");

            // Create a terminal log sink.
            nsfx::Ptr<nsfx::ILogStreamSink> sink =
                nsfx::CreateObject<nsfx::ILogStreamSink>(
                    "edu.uestc.nsfx.LogStreamSink");

            nsfx::Ptr<nsfx::ILogEvent>(logger)->Connect(sink);

            // Add a pending value.
            sink->AddValue("Value", nsfx::MakeConstantLogValue<int>(10));

            // Set a formatter.
            bool output = false;
            nsfx::Ptr<nsfx::ILogFormatter> fmtr = nsfx::CreateLogFormatter(
                    [&] (std::ostream& os, const nsfx::LogRecord& r) {
                output = true;
                if (r.Exists("Value"))
                {
                    os << r.Get<int>("Value") << ", ";
                }
                os << r.Get<const char*>("LogMessage");
            });
            nsfx::Ptr<nsfx::ILogFormatterUser>(sink)->Use(fmtr);

            // Add output streams.
            std::ostringstream oss;
            sink->AddStream(oss);
            sink->AddFile("test.log");

            ////////////////////
            // Log without filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is added.
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_EXPECT_EQ(oss.str(), "10, plain");
            output = false;
            oss.str("");

            ////////////////////
            // Set a filter.
            nsfx::Ptr<nsfx::ILogFilter> filter =
                    nsfx::CreateLogFilter([] (const nsfx::LogRecord& r) {
                return (r.Exists("Value") && r.Get<int>("Value") > 0) ?
                       nsfx::LOG_DISCARD : nsfx::LOG_ACCEPT;
            });
            sink->SetFilter(filter);

            ////////////////////
            // Log with filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is added before filtering.
            NSFX_TEST_EXPECT(!output);

            ////////////////////
            // Update the pending value.
            sink->UpdateValue("Value", nsfx::MakeConstantLogValue<int>(0));

            // Log with filter.
            NSFX_LOG(logger) << "plain";

            // The pending value is updated.
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_EXPECT_EQ(oss.str(), "0, plain");
            output = false;
            oss.str("");

            ////////////////////
            // Remove the pending value.
            sink->RemoveValue("Value");

            // Log with filter.
            NSFX_LOG(logger) << "plain";
            NSFX_TEST_EXPECT(output);
            NSFX_TEST_EXPECT_EQ(oss.str(), "plain");

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
            nsfx::Ptr<nsfx::ILogEventSinkEx> logger =
                nsfx::CreateObject<nsfx::ILogEventSinkEx>(
                    "edu.uestc.nsfx.Logger");

            // Create a terminal log sink.
            nsfx::Ptr<nsfx::ILogStreamSink> sink =
                nsfx::CreateObject<nsfx::ILogStreamSink>(
                    "edu.uestc.nsfx.LogStreamSink");

            nsfx::Ptr<nsfx::ILogEvent>(logger)->Connect(sink);

            // Add a second-order pending value.
            nsfx::LogValue ts = nsfx::MakeLogValue<nsfx::LogValue>(
            [&] { return nsfx::MakeConstantLogValue<nsfx::TimePoint>(
                             clock->Now());
            });

            logger->AddValue("Timestamp", ts);

            // Set a formatter.
            bool output = false;
            nsfx::Ptr<nsfx::ILogFormatter> fmtr = nsfx::CreateLogFormatter(
                    [&] (std::ostream& os, const nsfx::LogRecord& r) {
                output = true;
                if (r.Exists("Timestamp"))
                {
                    os << r.Get<nsfx::TimePoint>("Timestamp");
                }
            });
            nsfx::Ptr<nsfx::ILogFormatterUser>(sink)->Use(fmtr);

            // Add an output stream.
            std::ostringstream oss;
            sink->AddStream(oss);

            ////////////////////
            nsfx::TimePoint t0(nsfx::Seconds(1));
            nsfx::TimePoint t1(nsfx::Seconds(2));

            ////////////////////
            // Log at t0.
            t = t0;
            NSFX_TEST_EXPECT_EQ(clock->Now(), t0);
            NSFX_LOG(logger);

            // Examine the output at t1.
            t = t1;
            NSFX_TEST_EXPECT_EQ(clock->Now(), t1);

            NSFX_TEST_EXPECT(output);
            NSFX_TEST_EXPECT_EQ(oss.str(), t0.ToString());
            output = false;
            oss.str("");

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

