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

#include "config.h"
#include "aggregation.h"


Aggregation::Aggregation(void) :
    // This component is the controller of the aggregated component.
    nameableObj_(CreateObject<IObject>("edu.uestc.nsfx.tutorial.Nameable",
                                       /* controller = */this))
{
    // The implementation of IObject on the aggregate component depends upon
    // the implemenation of IObject on its controller.
    // However, IObject on this component has not been implemented yet.
    //
    // Thus, neither query interface, nor modify the reference counter of
    // the aggregated component in the contructor.
}

Aggregation::~Aggregation(void)
{
}

