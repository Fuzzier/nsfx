/**
 * @file
 *
 * @brief Test Filter.
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
#include <nsfx/log/filter/filter.h>
#include <nsfx/log/attribute-value/const-attribute-value.h>
#include <nsfx/component/object.h>
#include <iostream>


NSFX_TEST_SUITE(Filter)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::Ptr<nsfx::log::IFilter> filter =
                nsfx::log::CreateFilter(
                    [] (const std::shared_ptr<nsfx::log::Record>& record) -> uint32_t {
                        uint32_t decision = nsfx::log::DECLINE;
                        if (record->Exists("Level"))
                        {
                            if (record->Get<int>("Level") > 0)
                            {
                                decision = nsfx::log::ACCEPT;
                            }
                        }
                        return decision;
                    });

            auto record = std::make_shared<nsfx::log::Record>();
            record->Add("Name", nsfx::log::MakeConstantAttributeValue<std::string>("Test"));
            record->Add("Level", nsfx::log::MakeConstantAttributeValue<int>(0));
            NSFX_TEST_EXPECT(record->Exists("Name"));
            NSFX_TEST_EXPECT(record->Exists("Level"));
            NSFX_TEST_EXPECT_EQ(record->Get<std::string>("Name"), "Test");
            NSFX_TEST_EXPECT_EQ(record->Get<int>("Level"), 0);
            NSFX_TEST_EXPECT_EQ(filter->Decide(record), nsfx::log::DECLINE);

            record->Update("Level", nsfx::log::MakeConstantAttributeValue<int>(1));
            NSFX_TEST_EXPECT(record->Exists("Name"));
            NSFX_TEST_EXPECT(record->Exists("Level"));
            NSFX_TEST_EXPECT_EQ(record->Get<std::string>("Name"), "Test");
            NSFX_TEST_EXPECT_EQ(record->Get<int>("Level"), 1);
            NSFX_TEST_EXPECT_EQ(filter->Decide(record), nsfx::log::ACCEPT);

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
            catch (nsfx::log::AttributeValueNotFound& e)
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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

