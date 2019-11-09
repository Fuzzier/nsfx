/**
 * @file
 *
 * @brief IXxx interface.
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

// Header file guard.
#ifndef I_INTERFACE_H
#define I_INTERFACE_H


// NSFX.
#include <nsfx/all.h>
using namespace nsfx;


////////////////////////////////////////////////////////////////////////////////
// Interface definition.
class IXxx :
    virtual public IObject
{
public:
    // Virtual destructor (required by C++).
    virtual ~IXxx(void) BOOST_NOEXCEPT {}

    // Methods.
    virtual void Foo(void) = 0;
};

// Define IID (interface ID).
NSFX_DEFINE_CLASS_UID(IXxx, "edu.uestc.nsfx.IXxx");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IXxxUser, "edu.uestc.nsfx.IXxxUser",
    IXxx);


////////////////////////////////////////////////////////////////////////////////
class IYyy :
    virtual public IObject
{
public:
    // Virtual destructor (required by C++).
    virtual ~IYyy(void) BOOST_NOEXCEPT {}

    // Methods.
    virtual void Bar(void) = 0;
};

// Define IID (interface ID).
NSFX_DEFINE_CLASS_UID(IYyy, "edu.uestc.nsfx.IYyy");


////////////////////////////////////////
NSFX_DEFINE_USER_INTERFACE(
    IYyy, "edu.uestc.nsfx.IYyyUser",
    IYyy);


#endif // I_INTERFACE_H

