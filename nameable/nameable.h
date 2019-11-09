/**
 * @file
 *
 * @brief Nameable.
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

#ifndef NAMEABLE_H__312BE40B_4592_4320_B9C7_78E8FA9C9936
#define NAMEABLE_H__312BE40B_4592_4320_B9C7_78E8FA9C9936


#include "i-nameable.h"


class Nameable :
    public INameable
{
public:
    Nameable(void);
    virtual ~Nameable(void);

    // INameable
    virtual void SetName(const std::string& name) NSFX_OVERRIDE;
    virtual std::string GetName(void) NSFX_OVERRIDE;

public:
    // A public function.
    // However, this function is not exposed by an interface.
    void SetDefault(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(Nameable)
        NSFX_INTERFACE_ENTRY(INameable)
    NSFX_INTERFACE_MAP_END()

private:
    std::string name_;
};

NSFX_REGISTER_CLASS(Nameable, "edu.uestc.nsfx.tutorial.Nameable");


#endif // NAMEABLE_H__312BE40B_4592_4320_B9C7_78E8FA9C9936

