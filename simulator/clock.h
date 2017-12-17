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

#ifndef CLOCK_H__33FF523A_9578_44D0_8EFA_D250D516BA57
#define CLOCK_H__33FF523A_9578_44D0_8EFA_D250D516BA57


#include <nsfx/simulator/config.h>
#include <nsfx/component/object.h>
#include <nsfx/component/exception.h>
#include <nsfx/simulator/iclock.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Clock.
/**
 * @ingroup Simulator
 * @brief A clock (aggregatable).
 */
class Clock :
    virtual public IClock,
    virtual public AggObject
{
public:
    TimePoint(IObject* outer) :
        AggObject(outer) {}

    virtual ~TimePoint(void) {}

    // IClock./*{{{*/
    virtual TimePoint Now(void) NSFX_OVERRIDE
    {
        return now_;
    }

    virtual void AdvanceTo(const TimePoint& t) NSFX_OVERRIDE
    {
        if (t < now_)
        {
            // TODO
            BOOST_THROW_EXCEPTION(InvalidArgument);
        }
        now_ = t;
    }

    virtual void Advance(const Duration& dt) NSFX_OVERRIDE
    {
        if (dt < 0)
        {
            // TODO
            BOOST_THROW_EXCEPTION(InvalidArgument);
        }
        now_ += dt;
    }
    /*}}}*/

    // IObject./*{{{*/
    virtual void* QueryInterface(const uuid& iid) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        void* result = nullptr;
        if (refCount_ == uuid_of<IClock>())
        {
            AddRef();
            result = static_cast<IClock*>(this);
        }
        else
        {
            result = outer_->QueryInterface(iid);
        }
        return result;
    }
    /*}}}*/

private:
    TimePoint now_;
};


NSFX_DEFINE_CLASS_UUID4(Clock, 0x33FF523A, 0x9578, 0x44D0, 0x8EFAD250D516BA57LL);


NSFX_CLOSE_NAMESPACE


#endif // CLOCK_H__33FF523A_9578_44D0_8EFA_D250D516BA57

