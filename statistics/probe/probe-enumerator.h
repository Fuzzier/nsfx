/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PROBE_ENUMERATOR_H__6796FA4F_4580_4CAE_B604_BB640350FDB9
#define PROBE_ENUMERATOR_H__6796FA4F_4580_4CAE_B604_BB640350FDB9


#include <nsfx/statistics/config.h>
#include <nsfx/statistics/exception.h>
#include <nsfx/statistics/probe/i-probe-enumerator.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/object.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief An enumerator of probes.
 *
 * # Interfaces
 * * Provides
 *   + \c IProbeEnumerator
 */
class ProbeEnumerator :
    public IProbeEnumerator
{
    typedef ProbeEnumerator ThisClass;

public:
    /**
     * @brief Construct a probe enumerator.
     *
     * @tparam InputIterator An input iterator whose value type is string.
     */
    template<class InputIterator>
    ProbeEnumerator(InputIterator first, InputIterator last);

    virtual ~ProbeEnumerator(void);

    // IProbeEnumerator
    virtual void Reset(void) NSFX_OVERRIDE;
    virtual bool HasNext(void) NSFX_OVERRIDE;
    virtual std::string Next(void) NSFX_OVERRIDE;

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IProbeEnumerator)
    NSFX_INTERFACE_MAP_END()

private:
    vector<std::string>  items_;
    size_t index_;
};


////////////////////////////////////////////////////////////////////////////////
template<class InputIterator>
inline ProbeEnumerator::ProbeEnumerator(InputIterator first,
                                        InputIterator last) :
    items_(first, last),
    index_(0)
{
}

inline ProbeEnumerator::~ProbeEnumerator(void)
{
}

inline void ProbeEnumerator::Reset(void)
{
    index_ = 0;
}

inline bool ProbeEnumerator::HasNext(void)
{
    return index_ < items_.size();
}

inline std::string ProbeEnumerator::Next(void)
{
    if (index_ >= items_.size())
    {
        BOOST_THROW_EXCEPTION(
            OutOfBounds() <<
            ErrorMessage("Cannot get the next probe, "
                         "since there are no more probes."));
    }
    return items_[index_++];
}


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // PROBE_ENUMERATOR_H__6796FA4F_4580_4CAE_B604_BB640350FDB9

