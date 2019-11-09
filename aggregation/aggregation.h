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

#ifndef AGGREGATION_H__A9F65BB6_67B7_48ED_A166_A593D1A89101
#define AGGREGATION_H__A9F65BB6_67B7_48ED_A166_A593D1A89101


#include "nameable/i-nameable.h"


class Aggregation :
    virtual public IObject // Do not implement INameable directly.
{
public:
    Aggregation(void);
    virtual ~Aggregation(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(Aggregation)
        // Provide an interface via aggregation.
        // The second argument MUST be the IObject* interface
        // exposed by the aggregated component.
        NSFX_INTERFACE_AGGREGATED_ENTRY(INameable, &*nameableObj_)
    NSFX_INTERFACE_MAP_END()

private:
    // The IObject interface of the aggregated component is stored.
    Ptr<IObject> nameableObj_;
};

NSFX_REGISTER_CLASS(Aggregation, "edu.uestc.nsfx.tutorial.Aggregation");


#endif // AGGREGATION_H__A9F65BB6_67B7_48ED_A166_A593D1A89101

