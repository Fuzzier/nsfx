/**
 * @file
 *
 * @brief Test LogFilter.
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
#include <nsfx/log/create-log-filter.h>
#include <nsfx/log/make-log-value.h>
#include <iostream>


NSFX_TEST_SUITE(LogFilter)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::Ptr<nsfx::ILogFilter> filter = nsfx::CreateLogFilter(
                    [] (const nsfx::LogRecord& r) {
                return (r.Exists("Level") && r.Get<int>("Level") > 0) ?
                       nsfx::LOG_ACCEPT : nsfx::LOG_DISCARD;
            });

            nsfx::LogRecord record;
            record.Add("Name", nsfx::MakeConstantLogValue<std::string>("Test"));
            record.Add("Level", nsfx::MakeConstantLogValue<int>(0));
            NSFX_TEST_EXPECT_EQ(filter->Decide(record), nsfx::LOG_DISCARD);

            record.Update("Level", nsfx::MakeConstantLogValue<int>(1));
            NSFX_TEST_EXPECT_EQ(filter->Decide(record), nsfx::LOG_ACCEPT);

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

