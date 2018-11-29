/**
 * @file
 *
 * @brief Statistics support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-11-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_PROBE_ENUMERATOR_H__A8D454B4_7743_4FFC_A2F7_424019FDCA5B
#define I_PROBE_ENUMERATOR_H__A8D454B4_7743_4FFC_A2F7_424019FDCA5B


#include <nsfx/statistics/config.h>
#include <nsfx/component/i-object.h>
#include <string>


NSFX_STATISTICS_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Statistics
 * @brief An enumerator of probes.
 */
class IProbeEnumerator :
    virtual public IObject
{
public:
    virtual ~IProbeEnumerator(void) BOOST_NOEXCEPT {}

    /**
     * @brief Reset the enumeration sequence.
     */
    virtual void Reset(void) = 0;

    /**
     * @brief Has more probes to enumerate?
     */
    virtual bool HasNext(void) = 0;

    /**
     * @brief Get the name of the next supported probe.
     *
     * @pre <code>HasNext() == true</code>.
     *
     * @throw OutOfBounds
     */
    virtual std::string Next(void) = 0;

};


NSFX_DEFINE_CLASS_UID(IProbeEnumerator,
                      "edu.uestc.nsfx.statistics.IProbeEnumerator");


NSFX_STATISTICS_CLOSE_NAMESPACE


#endif // I_PROBE_ENUMERATOR_H__A8D454B4_7743_4FFC_A2F7_424019FDCA5B

