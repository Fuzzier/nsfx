/**
 * @file
 *
 * @brief Xxx component.
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
#ifndef COMPONENT_H
#define COMPONENT_H


// Interfaces.
#include "i-interface.h"


////////////////////////////////////////////////////////////////////////////////
// Component class definition.
/**
 * @brief Xxx component.
 *
 * Interfaces
 * * Provided
 *   + IXxx
 */
class Xxx :
    virtual public IXxx
{
    typedef Xxx ThisClass;

public:
    // Default constructor.
    Xxx(void);

    // Virtual destructor.
    virtual ~Xxx(void);

    // IXxx.
    virtual void Foo(void) NSFX_OVERRIDE;

private:
    // Interface map.
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        // Expose derived interface.
        NSFX_INTERFACE_ENTRY(IXxx)
    NSFX_INTERFACE_MAP_END()
};

// Register CID (component ID).
NSFX_REGISTER_CLASS(Xxx, "edu.uestc.nsfx.Xxx");


#endif // COMPONENT_H

