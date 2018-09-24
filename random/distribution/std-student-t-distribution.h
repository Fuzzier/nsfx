/**
 * @file
 *
 * @brief Random support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-24
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef STD_STUDENT_T_DISTRIBUTION_H__A08D5B4E_1B50_43C0_A6A1_F68B757F3B5C
#define STD_STUDENT_T_DISTRIBUTION_H__A08D5B4E_1B50_43C0_A6A1_F68B757F3B5C


#include <nsfx/random/config.h>
#include <nsfx/random/distribution/i-student-t-distribution.h>
#include <nsfx/component/object.h>
#include <boost/random/student_t_distribution.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Random
 * @brief A Student t-distribution.
 *
 * @tparam StdRng A <code>StdRandomNumberEngine<></code> or
 *                a <code>StdRandomDevice</code>.
 *                <p>
 *                It <b>must</b> have a public member function that returns
 *                a pointer to the underlying random number engine or random
 *                device.
 *
 * Provides \c IStudentTDistribution.
 */
template<class StdRng>
class StdStudentTDistribution :
    public IStudentTDistribution
{
    typedef StdStudentTDistribution ThisClass;
    typedef boost::random::student_t_distribution<double> DistributionType;
    typedef StdRng RngType;

public:
    StdStudentTDistribution(Ptr<RngType> rng, double degreesOfFreedom) :
        rng_(rng),
        dist_(degreesOfFreedom)
    {
        BOOST_ASSERT(rng);
    }

    virtual ~StdStudentTDistribution(void) {}

    virtual double Generate(void) NSFX_OVERRIDE
    {
        return dist_(*rng_->GetEngine());
    }

    virtual void Reset(void) NSFX_OVERRIDE
    {
        return dist_.reset();
    }

    virtual double GetMinValue(void) NSFX_OVERRIDE
    {
        return dist_.(min)();
    }

    virtual double GetMaxValue(void) NSFX_OVERRIDE
    {
        return dist_.(max)();
    }

    virtual double GetDegreesOfFreedom(void) NSFX_OVERRIDE
    {
        return dist_.n();
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IStudentTDistribution)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<RngType> rng_;
    DistributionType dist_;
};


NSFX_CLOSE_NAMESPACE


#endif // STD_STUDENT_T_DISTRIBUTION_H__A08D5B4E_1B50_43C0_A6A1_F68B757F3B5C

