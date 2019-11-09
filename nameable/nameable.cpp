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

#include "config.h"
#include "nameable.h"


Nameable::Nameable(void)
{
}

Nameable::~Nameable(void)
{
}

void Nameable::SetName(const std::string& name)
{
    name_ = name;
}

std::string Nameable::GetName(void)
{
    return name_;
}

void Nameable::SetDefault(void)
{
    name_ = "A default name";
}

