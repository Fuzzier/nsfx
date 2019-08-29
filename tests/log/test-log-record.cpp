/**
 * @file
 *
 * @brief Test LogRecord.
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
#include <nsfx/log/log-record.h>
#include <nsfx/log/make-log-value.h>
#include <nsfx/log/log-value-traits.h>
#include <iostream>


NSFX_TEST_SUITE(LogRecord)
{
    NSFX_DEFINE_LOG_VALUE_TRAITS(IntLogValueTraits, "Int", int);

    NSFX_TEST_CASE(Values)
    {
        try
        {
            nsfx::LogRecord record;
            record.Add("Name", nsfx::MakeConstantLogValue<std::string>("Test"));
            record.Add("Level", nsfx::MakeLogValue<int>([] { return 0; }));
            record.Add(IntLogValueTraits::GetName(),
                       nsfx::MakeConstantLogValue<IntLogValueTraits::Type>(10));

            NSFX_TEST_ASSERT(record.Exists("Name"));
            NSFX_TEST_ASSERT(record.Exists("Level"));
            NSFX_TEST_ASSERT(record.Exists(IntLogValueTraits::GetName()));

            NSFX_TEST_EXPECT_EQ(record.Get<std::string>("Name"), "Test");
            NSFX_TEST_EXPECT_EQ(record.Get<int>("Level"), 0);
            NSFX_TEST_EXPECT_EQ(record.Get<IntLogValueTraits>(), 10);

            record.Update("Level", nsfx::MakeLogValue<int>(
                                    [] { return 1; }));
            NSFX_TEST_ASSERT(record.Exists("Level"));
            NSFX_TEST_EXPECT_EQ(record.Get<int>("Level"), 1);

            int level = 0;
            record.VisitIfExists(
                "Level", [&] (const nsfx::LogValue& value) {
                                    level = value.Get<int>(); });
            NSFX_TEST_EXPECT_EQ(level, 1);

            try
            {
                record.Get<int>("Other");
            }
            catch (nsfx::LogValueNotFound& )
            {
                // Should come here.
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

    NSFX_TEST_CASE(Copy)
    {
        try
        {
            nsfx::LogRecord r1;
            // Shallow copy.
            nsfx::LogRecord r2 = r1;

            // Modify r1.
            r1.Add("Level", nsfx::MakeLogValue<int>([] { return 0; }));

            // Examine r2.
            NSFX_TEST_EXPECT(r2.Exists("Level"));
            NSFX_TEST_EXPECT_EQ(r1.Get<int>("Level"), 0);

            // Deep copy.
            nsfx::LogRecord r3 = r1.Copy();

            // Modify r1.
            r1.Add("Value", nsfx::MakeConstantLogValue<int>(1));

            // Examine r2.
            NSFX_TEST_EXPECT(r2.Exists("Value"));
            NSFX_TEST_EXPECT_EQ(r2.Get<int>("Value"), 1);

            // Examine r3.
            NSFX_TEST_EXPECT(r3.Exists("Level"));
            NSFX_TEST_EXPECT(!r3.Exists("Value"));

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

