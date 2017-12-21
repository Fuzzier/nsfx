/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2
#define CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2


#include <nsfx/simulator/config.h>
#include <nsfx/component/iobject.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IClock.
/**
 * @ingroup Simulator
 * @brief The clock interface.
 */
class IClock :
    virtual public IObject
{
public:
    virtual ~IClock(void) {}

    virtual TimePoint Now(void) = 0;
    virtual void AdvanceTo(const TimePoint& t) = 0;
    virtual void Advance(const Duration& dt) = 0;
};


NSFX_DEFINE_CLASS_UUID4(IClock, 0x49523D2A, 0xBE6A, 0x45FA, 0x84EA88307B44F4D2LL);


NSFX_CLOSE_NAMESPACE


#endif // CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2
