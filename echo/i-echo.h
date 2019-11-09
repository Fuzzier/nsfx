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

#ifndef I_ECHO_H__7DAE9C10_CD3F_4912_8F32_F423E091D8F3
#define I_ECHO_H__7DAE9C10_CD3F_4912_8F32_F423E091D8F3


#include <nsfx/all.h>
#include <string>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
class IEcho :
    virtual public IObject
{
public:
    virtual ~IEcho(void) BOOST_NOEXCEPT {}

    virtual std::string Shout(const std::string& msg) = 0;
};

NSFX_DEFINE_CLASS_UID(IEcho, "edu.uestc.nsfx.tutorial.IEcho");


#endif // I_ECHO_H__7DAE9C10_CD3F_4912_8F32_F423E091D8F3

