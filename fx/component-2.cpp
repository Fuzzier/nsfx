/**
 * @file
 *
 * @brief Yyy component.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-07-24
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

// Precompiled header.
#include "config.h"
// Component header.
#include "component-2.h"


////////////////////////////////////////////////////////////////////////////////
// Component function definition.
Yyy::Yyy(void) :
    uuuEvent_(/* controller = */this),

    vvvEventSink_(/* controller = */this, /* o = */this,
                  /* ptmf = */&ThisClass::OnVvv),

    sssEvent_(/* controller = */this),
    sssEventCookie_(0),

    tttEventSink_(/* controller = */this, /* o = */this,
                  /* ptmf = */&ThisClass::OnTtt),
    tttEventCookie_(0)
{
}

Yyy::~Yyy(void)
{
}

void Yyy::Use(Ptr<IXxx> xxx)
{
    if (!xxx)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    xxx_ = xxx;
}

void Yyy::Bar(void)
{
}

void Yyy::OnVvv(void)
{
}

