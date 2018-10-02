/**
 * @file
 *
 * @brief Test Attribute.
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
#include <nsfx/log/default/timestamp-attribute.h>
#include <nsfx/component/object.h>
#include <iostream>


NSFX_TEST_SUITE(TimestampAttribute)
{
    nsfx::TimePoint t;
    struct Clock : public nsfx::IClock
    {
        virtual ~Clock(void) {}
        virtual nsfx::TimePoint Now(void) { return t; }

        NSFX_INTERFACE_MAP_BEGIN(Clock)
            NSFX_INTERFACE_ENTRY(nsfx::IClock)
        NSFX_INTERFACE_MAP_END()
    };
    typedef nsfx::Object<Clock>  ClockClass;
    nsfx::Ptr<nsfx::IClock>  clock(new ClockClass);

    NSFX_TEST_CASE(Test)
    {
        try
        {
            {
                nsfx::log::Attribute a =
                    nsfx::log::MakeTimestampAttribute(clock);
                nsfx::log::AttributeValue v = a.GetValue();
                NSFX_TEST_EXPECT(v.GetTypeId() ==
                                 boost::typeindex::type_id<nsfx::TimePoint>());
                NSFX_TEST_EXPECT_EQ(v.Get<nsfx::TimePoint>(), t);

                t += nsfx::Seconds(10);
                v = a.GetValue();
                NSFX_TEST_EXPECT_EQ(v.Get<nsfx::TimePoint>(), t);
            }

        }
        catch (boost::exception& e)
        {
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
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

