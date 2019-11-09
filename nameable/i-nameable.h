/**
 * @file
 *
 * @brief INameable.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-15
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_NAMEABLE_H__FEFA4A40_36E8_4A3A_804B_0B31990690BA
#define I_NAMEABLE_H__FEFA4A40_36E8_4A3A_804B_0B31990690BA


#include <nsfx/all.h>
#include <string>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
class INameable :
    virtual public IObject
{
public:
    virtual ~INameable(void) BOOST_NOEXCEPT {}

    virtual void SetName(const std::string& name) = 0;
    virtual std::string GetName(void) = 0;
};

NSFX_DEFINE_CLASS_UID(INameable, "edu.uestc.nsfx.tutorial.INameable");


#endif // I_NAMEABLE_H__FEFA4A40_36E8_4A3A_804B_0B31990690BA

