/**
 * @file
 *
 * @brief Test xoshiro pseudo-random number generator.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-28
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/random/pseudo-random-generator.h>
#include <iostream>


static double log_gamma(double z)
{
    double z2 = z * z;
    double z3 = z2 * z;
    double z5 = z2 * z3;
    return z * std::log(z)
             - z
             - 0.5 * std::log(z)
             + 0.91893853320467274178032973640562 // ln(2 * M_PI) / 2
             + 1.0 / 12.0 / z
             - 1.0 / 360.0 / z3
             + 1.0 / 1260.0 / z5;
}

static double gamma(double z)
{
    return exp(log_gamma(z));
}

struct TestDistributions
{
    static const uint32_t N = 100000;

    TestDistributions(nsfx::Ptr<nsfx::IRandomDistributionGenerator> dg) : dg_(dg) {}

    void Test(void)
    {
        TestUniformIntDistribution();
        TestUniformRealDistribution();
        TestBernoulliDistribution();
        TestBinomialDistribution();
        TestGeometricDistribution();
        TestNegativeBinomialDistribution();
        TestPoissonDistribution();
        TestExponentialDistribution();
        TestGammaDistribution();
        TestWeibullDistribution();
        TestExtremeValueDistribution();
        TestNormalDistribution();
        TestLognormalDistribution();
        TestChiSquaredDistribution();
        TestCauchyDistribution();
        TestFisherFDistribution();
        TestStudentTDistribution();
        TestDiscreteDistribution();
        TestPiecewiseConstantDistribution();
        TestPiecewiseLinearDistribution();
        TestTriangleDistribution();
    }

    void TestUniformIntDistribution(void)
    {
        nsfx::Ptr<nsfx::IUniformIntDistribution> d =
            dg_->CreateUniformIntDistribution(-100, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),  100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(),  100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            int x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LE(x,  100);
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform int";
        d->Reset();
    }

    void TestUniformRealDistribution(void)
    {
        nsfx::Ptr<nsfx::IUniformRealDistribution> d =
            dg_->CreateUniformRealDistribution(-100, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),  100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(),  100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LT(x,  100);
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform real";
        d->Reset();
    }

    void TestBernoulliDistribution(void)
    {
        nsfx::Ptr<nsfx::IBernoulliDistribution> d =
            dg_->CreateBernoulliDistribution(0.25);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), false);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), true);
        NSFX_TEST_EXPECT_EQ(d->GetProbability(), 0.25);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            bool x = d->Generate();
            if (x)
            {
                ++count;
            }
        }
        double expected = 0.25;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Bernoulli";
        d->Reset();
    }

    void TestBinomialDistribution(void)
    {
        nsfx::Ptr<nsfx::IBinomialDistribution> d =
            dg_->CreateBinomialDistribution(1000, 0.25);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 1000);
        NSFX_TEST_EXPECT_EQ(d->GetNumTrials(), 1000);
        NSFX_TEST_EXPECT_EQ(d->GetProbability(), 0.25);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = d->Generate();
            count += x;
        }
        double expected = 1000 * 0.25;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "binomial";
        d->Reset();
    }

    void TestGeometricDistribution(void)
    {
        nsfx::Ptr<nsfx::IGeometricDistribution> d =
            dg_->CreateGeometricDistribution(0.25);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetProbability(), 0.25);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = d->Generate();
            count += x;
        }
        double expected = (1 - 0.25) / 0.25;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "geometric";
        d->Reset();
    }

    void TestNegativeBinomialDistribution(void)
    {
        nsfx::Ptr<nsfx::INegativeBinomialDistribution> d =
            dg_->CreateNegativeBinomialDistribution(100, 0.25);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetNumTrials(), 100);
        NSFX_TEST_EXPECT_EQ(d->GetProbability(), 0.25);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = d->Generate();
            count += x;
        }
        double expected = 100 * (1 - 0.25) / 0.25;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "negative binomial";
        d->Reset();
    }

    void TestPoissonDistribution(void)
    {
        nsfx::Ptr<nsfx::IPoissonDistribution> d =
            dg_->CreatePoissonDistribution(1.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetMean(), 1.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = d->Generate();
            count += x;
        }
        double expected = 1.0;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Poisson";
        d->Reset();
    }

    void TestExponentialDistribution(void)
    {
        nsfx::Ptr<nsfx::IExponentialDistribution> d =
            dg_->CreateExponentialDistribution(1.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetLambda(), 1.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 1.0;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "exponential";
        d->Reset();
    }

    void TestGammaDistribution(void)
    {
        nsfx::Ptr<nsfx::IGammaDistribution> d =
            dg_->CreateGammaDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetShape(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetScale(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 2.0 * 3.0;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "gamma";
        d->Reset();
    }

    void TestWeibullDistribution(void)
    {
        nsfx::Ptr<nsfx::IWeibullDistribution> d =
            dg_->CreateWeibullDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetShape(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetScale(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 3.0 * gamma(1 + 1 / 2.0);
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Weibull";
        d->Reset();
    }

    void TestExtremeValueDistribution(void)
    {
        nsfx::Ptr<nsfx::IExtremeValueDistribution> d =
            dg_->CreateExtremeValueDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(),
                            -(std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetLocation(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetScale(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 2.0 + 3.0 * 0.5772156649015328606;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "extreme value";
        d->Reset();
    }

    void TestNormalDistribution(void)
    {
        nsfx::Ptr<nsfx::INormalDistribution> d =
            dg_->CreateNormalDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(),
                            -(std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetMean(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetStddev(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 2.0;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "normal";
        d->Reset();
    }

    void TestLognormalDistribution(void)
    {
        nsfx::Ptr<nsfx::ILognormalDistribution> d =
            dg_->CreateLognormalDistribution(0.1, 1.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetMean(), 0.1);
        NSFX_TEST_EXPECT_EQ(d->GetStddev(), 1.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = std::exp(0.1 + 1.0 * 1.0 / 2);
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.1) << "lognormal";
        d->Reset();
    }

    void TestChiSquaredDistribution(void)
    {
        nsfx::Ptr<nsfx::IChiSquaredDistribution> d =
            dg_->CreateChiSquaredDistribution(2.5);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetDegreesOfFreedom(), 2.5);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 2.5;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "chi-squared";
        d->Reset();
    }

    void TestCauchyDistribution(void)
    {
        nsfx::Ptr<nsfx::ICauchyDistribution> d =
            dg_->CreateCauchyDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(),
                            -(std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetLocation(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetScale(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        // double expected = ?;
        // NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "Cauchy";
        d->Reset();
    }

    void TestFisherFDistribution(void)
    {
        nsfx::Ptr<nsfx::IFisherFDistribution> d =
            dg_->CreateFisherFDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetNumerator(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetDenominator(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 3.0 / (3.0 - 2);
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "Fisher F";
        d->Reset();
    }

    void TestStudentTDistribution(void)
    {
        nsfx::Ptr<nsfx::IStudentTDistribution> d =
            dg_->CreateStudentTDistribution(2.5);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(),
                            -(std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetDegreesOfFreedom(), 2.5);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "Student t";
        d->Reset();
    }

    void TestDiscreteDistribution(void)
    {
        nsfx::Ptr<nsfx::IDiscreteDistributionParam> p =
            nsfx::CreateObject<nsfx::IDiscreteDistributionParam>(
                "edu.uestc.nsfx.DiscreteDistributionParam");
        p->AddWeight(5);
        p->AddWeight(3);
        p->AddWeight(2);
        NSFX_TEST_EXPECT_EQ(p->GetNumWeights(), 3);
        nsfx::Ptr<nsfx::IDiscreteDistribution> d =
            dg_->CreateDiscreteDistribution(p);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(), 2);
        NSFX_TEST_EXPECT_EQ(d->GetNumValues(), 3);
        NSFX_TEST_EXPECT_RC(d->GetProbability(0), 0.5, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetProbability(1), 0.3, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetProbability(2), 0.2, 0.01);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 0 * 0.5 + 1 * 0.3 + 2 * 0.2;
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "discrete";
        d->Reset();
    }

    void TestPiecewiseConstantDistribution(void)
    {
        nsfx::Ptr<nsfx::IPiecewiseConstantDistributionParam> p =
            nsfx::CreateObject<nsfx::IPiecewiseConstantDistributionParam>(
                "edu.uestc.nsfx.PiecewiseConstantDistributionParam");
        p->SetLowerBound(0);
        p->AddInterval(1, 0.5);
        p->AddInterval(2, 0.3);
        p->AddInterval(3, 0.2);
        NSFX_TEST_EXPECT_EQ(p->GetNumIntervals(), 3);
        nsfx::Ptr<nsfx::IPiecewiseConstantDistribution> d =
            dg_->CreatePiecewiseConstantDistribution(p);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(), 3);
        NSFX_TEST_EXPECT_EQ(d->GetNumIntervals(), 3);
        NSFX_TEST_EXPECT_RC(d->GetBound(0), 0.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBound(1), 1.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBound(2), 2.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBound(3), 3.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetIntervalDensity(0), 0.5, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetIntervalDensity(1), 0.3, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetIntervalDensity(2), 0.2, 0.01);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = (1*1 - 0*0) * 0.5 / 2
                        + (2*2 - 1*1) * 0.3 / 2
                        + (3*3 - 2*2) * 0.2 / 2;
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "piecewise constant";
        d->Reset();
    }

    void TestPiecewiseLinearDistribution(void)
    {
        nsfx::Ptr<nsfx::IPiecewiseLinearDistributionParam> p =
            nsfx::CreateObject<nsfx::IPiecewiseLinearDistributionParam>(
                "edu.uestc.nsfx.PiecewiseLinearDistributionParam");
        p->AddBound(1, 0);
        p->AddBound(2, 10);
        p->AddBound(5, 0);
        NSFX_TEST_EXPECT_EQ(p->GetNumBounds(), 3);
        nsfx::Ptr<nsfx::IPiecewiseLinearDistribution> d =
            dg_->CreatePiecewiseLinearDistribution(p);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 1);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(), 5);
        NSFX_TEST_EXPECT_EQ(d->GetNumIntervals(), 2);
        NSFX_TEST_EXPECT_RC(d->GetBound(0), 1.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBound(1), 2.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBound(2), 5.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBoundDensity(0), 0.0, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBoundDensity(1), 0.5, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetBoundDensity(2), 0.0, 0.01);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = (1 + 2 + 5) / 3.0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "piecewise linear";
        d->Reset();
    }

    void TestTriangleDistribution(void)
    {
        nsfx::Ptr<nsfx::ITriangleDistribution> d =
            dg_->CreateTriangleDistribution(1, 2, 5);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(), 1);
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(), 5);
        NSFX_TEST_EXPECT_EQ(d->GetA(), 1.0);
        NSFX_TEST_EXPECT_EQ(d->GetB(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetC(), 5.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = (1 + 2 + 5) / 3.0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "triangle";
        d->Reset();
    }

    nsfx::Ptr<nsfx::IRandomDistributionGenerator> dg_;
};


NSFX_TEST_SUITE(PRNG)
{
    NSFX_TEST_CASE(Xoshiro256StarstarEngine)
    {
        try
        {
            typedef nsfx::Xoshiro256StarstarEngine Rng;
            nsfx::Ptr<nsfx::IRandomUInt64Generator> r(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(r->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(r->GetMaxValue(),
                                (std::numeric_limits<uint64_t>::max)());
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(r);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                r->Generate();
            }
            nsfx::Ptr<nsfx::IRandomDistributionGenerator> dg(pr);
            TestDistributions td(dg);
            td.Test();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

    NSFX_TEST_CASE(Xoshiro256Plus01Engine)
    {
        try
        {
            typedef nsfx::Xoshiro256Plus01Engine Rng;
            nsfx::Ptr<nsfx::IRandomDoubleGenerator> r(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(r->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(r->GetMaxValue(), 1);
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(r);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                double d = r->Generate();
                NSFX_TEST_EXPECT_GE(d, 0);
                NSFX_TEST_EXPECT_LT(d, 1);
            }
            nsfx::Ptr<nsfx::IRandomDistributionGenerator> dg(pr);
            TestDistributions td(dg);
            td.Test();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

    NSFX_TEST_CASE(Mt19937Engine)
    {
        try
        {
            typedef nsfx::Mt19937Engine Rng;
            nsfx::Ptr<nsfx::IRandomUInt32Generator> r(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(r->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(r->GetMaxValue(),
                                (std::numeric_limits<uint32_t>::max)());
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(r);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                r->Generate();
            }
            nsfx::Ptr<nsfx::IRandomDistributionGenerator> dg(pr);
            TestDistributions td(dg);
            td.Test();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

