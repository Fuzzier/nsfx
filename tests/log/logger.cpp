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
#include <nsfx/log/attribute-value/const-attribute-value.h>
#include <nsfx/log/misc/attribute-value-info.h>
#include <iostream>


NSFX_TEST_SUITE(Logger)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            // for (bool go = true; go; go = false)
            // for (auto record = std::make_shared<nsfx::log::Record>(); go; go = false)
            // for ([&record] {
            //     record->Add(nsfx::log::FileNameInfo::GetName(),
            //                 nsfx::log::MakeConstantAttributeValue<
            //                     typename nsfx::log::FileNameInfo::Type>(__FILE__));
            //     record->Add(nsfx::log::LineNumberInfo::GetName(),
            //                 nsfx::log::MakeConstantAttributeValue<
            //                     typename nsfx::log::LineNumberInfo::Type>(__LINE__));
            // }(); go; go = false);

            for (auto record = std::make_shared<nsfx::log::Record>();
                 [&record] () -> bool {
                     record->Add(nsfx::log::FileNameInfo::GetName(),
                                 nsfx::log::MakeConstantAttributeValue<
                                 typename nsfx::log::FileNameInfo::Type>(__FILE__));
                     record->Add(nsfx::log::LineNumberInfo::GetName(),
                                 nsfx::log::MakeConstantAttributeValue<
                                 typename nsfx::log::LineNumberInfo::Type>(__LINE__));
                     return false;
                 }(); );
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

