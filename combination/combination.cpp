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
#include "combination.h"


Combination::Combination(void) :
    // Black-box reuse.
    // The underlying type of "edu.uestc.nsfx.Nameable" is unknown, and
    // the component is accessed via its interfaces.
    nameable_(CreateObject<INameable>("edu.uestc.nsfx.tutorial.Nameable"))
{
}

Combination::~Combination(void)
{
}

void Combination::SetName(const std::string& name)
{
    // Delegate to the inner component.
    nameable_->SetName(name);
}

std::string Combination::GetName(void)
{
    // Delegate to the inner component.
    return nameable_->GetName();
}

