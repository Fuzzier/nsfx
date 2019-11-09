/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-20
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef ECHO_H__0E27FCFE_942D_440D_B2CC_4D466FB41010
#define ECHO_H__0E27FCFE_942D_440D_B2CC_4D466FB41010


#include "i-echo.h"


class Echo :
    public IEcho
{
public:
    Echo(void);
    virtual ~Echo(void);

    // IEcho
    virtual std::string Shout(const std::string& msg) NSFX_OVERRIDE;

private:
    // The interface map.
    NSFX_INTERFACE_MAP_BEGIN(Echo)
        NSFX_INTERFACE_ENTRY(IEcho)
    NSFX_INTERFACE_MAP_END()
};

// Register component.
NSFX_REGISTER_CLASS(Echo, "edu.uestc.nsfx.tutorial.Echo");


#endif // ECHO_H__0E27FCFE_942D_440D_B2CC_4D466FB41010

