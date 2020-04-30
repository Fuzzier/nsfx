/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-04-30
 *
 * @copyright Copyright (c) 2020.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STATISTIC_H__58554387_BB19_4F40_9B6A_F1F60D7D336A
#define STATISTIC_H__58554387_BB19_4F40_9B6A_F1F60D7D336A


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/i-statistic.h>
#include <nsfx/event/event.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief A statistic.
 *
 * @tparam IStat The event interface of the statistic.
 *
 * # Interfaces
 * * Provides
 *   + `IStatistic`
 */
template<class IStat>
class Statistic :
    public IStatistic
{
    typedef Statistic ThisClass;

public:
    Statistic(void);
    virtual ~Statistic(void);

    // IStatistic
    virtual cookie_t Connect(Ptr<IObject> sink) NSFX_OVERRIDE;
    virtual void Disconnect(cookie_t cookie) NSFX_OVERRIDE;

    Ptr<Event<IStat>> GetEvent(void);

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStatistic)
    NSFX_INTERFACE_MAP_END()

private:
    MemberAggObject<Event<IStat>>  stat_;
};


////////////////////////////////////////////////////////////////////////////////
template<class IStat>
inline Statistic<IStat>::Statistic(void) :
    stat_(/* controller = */this)
{
}

template<class IStat>
inline Statistic<IStat>::~Statistic(void)
{
}

template<class IStat>
inline cookie_t Statistic<IStat>::Connect(Ptr<IObject> sink)
{
    return stat_.GetImpl()->Connect(sink);
}

template<class IStat>
inline void Statistic<IStat>::Disconnect(cookie_t cookie)
{
    stat_.GetImpl()->Disconnect(cookie);
}

template<class IStat>
inline Ptr<Event<IStat>> Statistic<IStat>::GetEvent(void)
{
    return stat_.GetImpl();
}


NSFX_CLOSE_NAMESPACE


#endif // STATISTIC_H__58554387_BB19_4F40_9B6A_F1F60D7D336A

