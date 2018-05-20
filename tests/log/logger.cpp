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
#include <nsfx/log/logger/logger.h>
#include <nsfx/log/record/record.h>
#include <nsfx/log/formatter/stream-formatter.h>
#include <nsfx/log/misc/tool.h>
#include <nsfx/log/misc/severity-level-filter.h>
#include <nsfx/log/misc/timestamp-attribute.h>
#include <nsfx/log/sink/stream-sink.h>
#include <nsfx/simulation/i-clock.h>
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

    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::Ptr<nsfx::log::ILogger> logger =
                nsfx::CreateObject<nsfx::log::ILogger>(
                    "edu.uestc.nsfx.log.Logger");

            // Add attributes
            {
                nsfx::Ptr<nsfx::log::IAttributeSet> attrs = logger;
                attrs->Add(
                    nsfx::log::TimestampInfo::GetName(),
                    nsfx::log::MakeTimestampAttribute(clock));
            }

            // Add filters
            {
                nsfx::Ptr<nsfx::log::ISeverityLevelFilter> filter =
                    nsfx::CreateObject<nsfx::log::ISeverityLevelFilter>(
                        "edu.uestc.nsfx.log.SeverityLevelFilter");
                filter->SetAcceptedLevels(nsfx::log::LOG_INFO |
                                          nsfx::log::LOG_DEBUG);

                nsfx::Ptr<nsfx::log::IFilterChain> filters = logger;
                filters->PushBack(filter);
            }

            nsfx::Ptr<nsfx::log::IStreamSink> sink =
                nsfx::CreateObject<nsfx::log::IStreamSink>(
                    "edu.uestc.nsfx.log.StreamSink");
            sink->SetStream(&std::cout);

            nsfx::Ptr<nsfx::log::IStreamFormatter> formatter =
                nsfx::log::CreateStreamFormatter(
                    [] (std::ostream& os, const std::shared_ptr<nsfx::log::Record>& record) {
                        os << "[" << record->Get<nsfx::log::SeverityLevelInfo>() << "] "
                           << "@" << record->Get<nsfx::log::TimestampInfo>() << " "
                           << "in " << record->Get<nsfx::log::FunctionNameInfo>() << "() "
                           << ": " << record->Get<nsfx::log::FileNameInfo>()
                           << ":" << record->Get<nsfx::log::LineNumberInfo>() << " "
                           << record->Get<nsfx::log::MessageInfo>()
                           << std::endl;
                    });
            nsfx::Ptr<nsfx::log::IStreamFormatterUser>(sink)->Use(formatter);

            nsfx::Ptr<nsfx::log::ILoggerEvent>(logger)->Connect(sink);

            NSFX_LOG(logger, nsfx::log::LOG_FATAL)    << "fatal";
            NSFX_LOG(logger, nsfx::log::LOG_ERROR)    << "error";
            NSFX_LOG(logger, nsfx::log::LOG_WARNING)  << "warning";
            NSFX_LOG(logger, nsfx::log::LOG_INFO)     << "info";
            NSFX_LOG(logger, nsfx::log::LOG_DEBUG)    << "debug";
            NSFX_LOG(logger, nsfx::log::LOG_FUNCTION) << "function";
            NSFX_LOG(logger, nsfx::log::LOG_TRACE)    << "trace";

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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

