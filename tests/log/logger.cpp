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
#include <nsfx/log/misc/tool.h>
#include <iostream>
#include <sstream>
#include <iomanip>


NSFX_TEST_SUITE(Logger)
{
    class Sink : public nsfx::log::ILogger
    {
    public:
        virtual ~Sink(void) {}
        virtual void Fire(const std::shared_ptr<nsfx::log::Record>& record) NSFX_OVERRIDE
        {
            std::cout << record->Get<nsfx::log::FileNameInfo>() << std::endl;
            std::cout << record->Get<nsfx::log::SeverityLevelInfo>() << std::endl;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::log::ILogger)
        NSFX_INTERFACE_MAP_END()
    };

    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::Ptr<nsfx::log::ILogger> logger =
                ::nsfx::CreateObject<nsfx::log::ILogger>(
                    "edu.uestc.nsfx.log.Logger");

            nsfx::Ptr<nsfx::Object<Sink> > sink(new nsfx::Object<Sink>);
            nsfx::Ptr<nsfx::log::ILoggerEvent>(logger)->Connect(sink);

            NSFX_LOG(logger, nsfx::log::LOG_FATAL) << "fatal";

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

