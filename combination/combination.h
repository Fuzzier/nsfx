/**
 * @file
 *
 * @brief Tutorials.
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

#ifndef COMBINATION_H__B572C5D1_9945_452A_BD3F_81BC4791A91B
#define COMBINATION_H__B572C5D1_9945_452A_BD3F_81BC4791A91B


#include "nameable/i-nameable.h"


class Combination :
    public INameable
{
public:
    Combination(void);
    virtual ~Combination(void);

    // INameable
    virtual void SetName(const std::string& name) NSFX_OVERRIDE;
    virtual std::string GetName(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(Combination)
        NSFX_INTERFACE_ENTRY(INameable)
    NSFX_INTERFACE_MAP_END()

private:
    // The INameable interface of the inner component is stored.
    Ptr<INameable> nameable_;
};

NSFX_REGISTER_CLASS(Combination, "edu.uestc.nsfx.tutorial.Combination");


#endif // COMBINATION_H__B572C5D1_9945_452A_BD3F_81BC4791A91B

