/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-17
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef MEMBER_AGGREGATION_H__7632DD7F_FEFA_450A_B32D_AD58538ABABF
#define MEMBER_AGGREGATION_H__7632DD7F_FEFA_450A_B32D_AD58538ABABF


#include "nameable/nameable.h"


class MemberAggregation :
    virtual public IObject
{
public:
    MemberAggregation(void);
    virtual ~MemberAggregation(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(MemberAggregation)
        // The second argument MUST be the IObject* interface
        // exposed by the aggregated component.
        NSFX_INTERFACE_AGGREGATED_ENTRY(INameable, &nameable_)
    NSFX_INTERFACE_MAP_END()

private:
    // MemberAggObject implements the IObject interface of the Nameable class,
    // which supports aggregation.
    MemberAggObject<Nameable>  nameable_;
};

NSFX_REGISTER_CLASS(MemberAggregation,
                    "edu.uestc.nsfx.tutorial.MemberAggregation");


#endif // MEMBER_AGGREGATION_H__7632DD7F_FEFA_450A_B32D_AD58538ABABF

