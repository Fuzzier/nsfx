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

// Header file guard.
#ifndef COMPONENT_2_H
#define COMPONENT_2_H


// Interfaces.
#include "i-interface.h"
#include "i-event.h"


////////////////////////////////////////////////////////////////////////////////
// Component class definition.
/**
 * @brief Yyy component.
 *
 * Interfaces
 * * Used
 *   + IXxx
 * * Provided
 *   + IYyy
 * * Events
 *   + IUuuEvent
 *   + IVvvEventSink
 */
class Yyy :
    virtual public IXxxUser, // Use IXxx interface
    virtual public IYyy      // Provide IYyy interface
{
    typedef Yyy ThisClass;

public:
    // Default constructor.
    Yyy(void);

    // Virtual destructor.
    virtual ~Yyy(void);

    // IXxxUser
    virtual void Use(Ptr<IXxx> xxx) NSFX_OVERRIDE;

    // IYyy
    virtual void Bar(void) NSFX_OVERRIDE;

public:
    void OnVvv(void); // Process IVvvEvent.
    void OnTtt(void); // Process ITttEvent.

    
    // Interface map.
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        // Expose derived interfaces.
        NSFX_INTERFACE_ENTRY(IXxxUser)
        NSFX_INTERFACE_ENTRY(IYyy)
        // Expose aggregated interfaces.
        NSFX_INTERFACE_AGGREGATED_ENTRY(IUuuEvent,     &uuuEvent_)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IVvvEventSink, &vvvEventSink_)
        // Do not expose interfaces from mutual components.
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IXxx> xxx_; // Store used interface.

    // IUuuEvent.
    MemberAggObject<Event<IUuuEvent>>
             uuuEvent_;

    // -> OnVvv().
    MemberAggObject<MemberFunctionBasedEventSink<
        IVvvEventSink, ThisClass>>
             vvvEventSink_;

    // ISssEvent.
    MutualObject<Event<ISssEvent>>
             sssEvent_;
    cookie_t sssEventCookie_;

    // -> OnTtt().
    MutualObject<MemberFunctionBasedEventSink<
        ITttEventSink, ThisClass>>
             tttEventSink_;
    cookie_t tttEventCookie_;

};

// Register CID (component ID).
NSFX_REGISTER_CLASS(Yyy, "edu.uestc.nsfx.Yyy");


#endif // COMPONENT_2_H

