/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-22
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"


class StatisticsProvider :
    public ISchedulerUser,
    public IInitializable
{
  typedef StatisticsProvider  ThisClass;

public:
    StatisticsProvider(void) :
        probes_(/* container = */ this),
        generateSampleEventSink_(/* controller = */this, /* o = */this,
                                 /* ptmf = */&ThisClass::OnGenerateSample)
    {}

    virtual ~StatisticsProvider(void) {}

    void Use(Ptr<IScheduler> scheduler)
    {
        scheduler_ = scheduler;
    }

    void Initialize(void)
    {
        // Add a probe.
        sample_ = probes_.GetImpl()->Add("sample size");

        random_ = CreateObject<IRandom>("edu.uestc.nsfx.Xoshiro256Plus01Engine");
        exp_ = random_->CreateExponentialDistribution(/* lambda = */1.0);

        scheduler_->ScheduleIn(Seconds(1),
            generateSampleEventSink_.GetImpl()->GetIntf());
    }

private:
    void OnGenerateSample(void)
    {
        double sample = exp_->Generate();

        // Generate a sample point.
        sample_->Fire(sample);

        scheduler_->ScheduleIn(Seconds(1),
            generateSampleEventSink_.GetImpl()->GetIntf());
    }

    NSFX_INTERFACE_MAP_BEGIN(StatisticsProvider)
        NSFX_INTERFACE_ENTRY(ISchedulerUser)
        NSFX_INTERFACE_ENTRY(IInitializable)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IProbeContainer, &probes_)
    NSFX_INTERFACE_MAP_END()

private:
    Ptr<IScheduler> scheduler_;

    Ptr<IRandom> random_;
    Ptr<IExponentialDistribution> exp_;

    MemberAggObject<ProbeContainer> probes_;
    Ptr<Probe> sample_;

    // -> OnGenerateSample().
    MutualObject<MemberFunctionBasedEventSink<IEventSink<>, ThisClass>>
        generateSampleEventSink_;
};

NSFX_REGISTER_CLASS(StatisticsProvider,
                    "edu.uestc.nsfx.tutorial.StatisticsProvider");

