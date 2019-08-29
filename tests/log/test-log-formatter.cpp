/**
 * @file
 *
 * @brief Test LogFormatter.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-08-29
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/log/i-log-formatter.h>
#include <nsfx/log/create-log-formatter.h>
#include <nsfx/log/make-log-value.h>
#include <iostream>
#include <sstream>


NSFX_TEST_SUITE(LogFormatter)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::Ptr<nsfx::ILogFormatter> fmtr = nsfx::CreateLogFormatter(
                    [] (std::ostream& os, const nsfx::LogRecord& r) {
                os << r.Get<std::string>("Name") << ", " << r.Get<int>("Level");
            });

            nsfx::LogRecord record;
            record.Add("Name", nsfx::MakeConstantLogValue<std::string>("Test"));
            record.Add("Level", nsfx::MakeConstantLogValue<int>(10));

            std::ostringstream oss;
            fmtr->Format(oss, record);

            std::string s = oss.str();
            NSFX_TEST_EXPECT_EQ(s, "Test, 10");

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

