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
#include <nsfx/log/core/record/record.h>
#include <nsfx/log/core/attribute-value/const-attribute-value.h>
#include <nsfx/log/default/severity-level.h>
#include <iostream>


NSFX_TEST_SUITE(Record)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            auto record = std::make_shared<nsfx::log::Record>();
            record->Add("Name", nsfx::log::MakeConstantAttributeValue<std::string>("Test"));
            record->Add("Level", nsfx::log::MakeConstantAttributeValue<int>(0));
            record->Add(nsfx::log::SeverityLevelInfo::GetName(),
                        nsfx::log::MakeConstantAttributeValue<
                        typename nsfx::log::SeverityLevelInfo::Type>(nsfx::log::LOG_INFO));
            NSFX_TEST_EXPECT(record->Exists("Name"));
            NSFX_TEST_EXPECT(record->Exists("Level"));
            NSFX_TEST_EXPECT(record->Exists(nsfx::log::SeverityLevelInfo::GetName()));
            NSFX_TEST_EXPECT_EQ(record->Get<std::string>("Name"), "Test");
            NSFX_TEST_EXPECT_EQ(record->Get<int>("Level"), 0);
            NSFX_TEST_EXPECT_EQ(record->Get<nsfx::log::SeverityLevelInfo>(), nsfx::log::LOG_INFO);

            record->Update("Level", nsfx::log::MakeConstantAttributeValue<int>(1));
            NSFX_TEST_EXPECT(record->Exists("Name"));
            NSFX_TEST_EXPECT(record->Exists("Level"));
            NSFX_TEST_EXPECT_EQ(record->Get<std::string>("Name"), "Test");
            NSFX_TEST_EXPECT_EQ(record->Get<int>("Level"), 1);

            int level = 0;
            record->VisitIfExists(
                "Level",
                [&] (const nsfx::log::AttributeValue& value) {
                    level = value.Get<int>();
            });
            NSFX_TEST_EXPECT_EQ(level, 1);

            try
            {
                record->Get<int>("Other");
            }
            catch (nsfx::log::AttributeValueNotFound& )
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
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

