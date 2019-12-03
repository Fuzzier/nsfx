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


namespace aux
{
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

    static double tgamma(double z)
    {
        return exp(log_gamma(z));
    }
}

struct TestDistributions
{
    static const size_t N = 300000;

    TestDistributions(nsfx::Ptr<nsfx::IRandom> r) : dg_(r) {}

    void Test(void)
    {
        TestUniformUint32Distribution();
        TestUniformInt32Distribution();
        TestUniformUint64Distribution();
        TestUniformInt64Distribution();
        TestUniformDoubleDistribution();
        TestUniformFloatDistribution();
        TestBernoulliDistribution();
        TestBinomialDistribution();
        TestNegativeBinomialDistribution();
        TestGeometricDistribution();
        TestPoissonDistribution();
        TestExponentialDistribution();
        TestGammaDistribution();
        TestWeibullDistribution();
        TestExtremeValueDistribution();
        TestBetaDistribution();
        TestLaplaceDistribution();
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

    void TestUniformUint32Distribution()
    {
        nsfx::Ptr<nsfx::IUniformUint32Distribution> d =
            dg_->CreateUniformUint32Distribution(0, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(), 100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, 0U);
            NSFX_TEST_EXPECT_LE(x, 100U);
        }
        double expected = 100 / 2.0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform uint32";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = dg_->GenerateUniformUint32(0, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, 0U);
            NSFX_TEST_EXPECT_LE(x, 100U);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform uint32";
    }

    void TestUniformInt32Distribution()
    {
        nsfx::Ptr<nsfx::IUniformInt32Distribution> d =
            dg_->CreateUniformInt32Distribution(-100, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),  100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(),  100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            int32_t x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LE(x,  100);
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform int32";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            int32_t x = dg_->GenerateUniformInt32(-100, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LE(x,  100);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform int32";
    }

    void TestUniformUint64Distribution()
    {
        nsfx::Ptr<nsfx::IUniformUint64Distribution> d =
            dg_->CreateUniformUint64Distribution(0, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(), 100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint64_t x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, 0U);
            NSFX_TEST_EXPECT_LE(x, 100U);
        }
        double expected = 100 / 2.0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform uint64";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint64_t x = dg_->GenerateUniformInt64(0, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, 0);
            NSFX_TEST_EXPECT_LE(x, 100);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform uint64";
    }

    void TestUniformInt64Distribution()
    {
        nsfx::Ptr<nsfx::IUniformInt64Distribution> d =
            dg_->CreateUniformInt64Distribution(-100, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),  100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(),  100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            int64_t x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LE(x,  100);
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform int64";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            int64_t x = dg_->GenerateUniformInt64(-100, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LE(x,  100);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform int64";
    }

    void TestUniformDoubleDistribution(void)
    {
        nsfx::Ptr<nsfx::IUniformDoubleDistribution> d =
            dg_->CreateUniformDoubleDistribution(-100, 100);
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
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform double";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateUniformDouble(-100, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LT(x,  100);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform double";
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateUniform01();
            count += x;
            NSFX_TEST_EXPECT_GE(x, 0);
            NSFX_TEST_EXPECT_LT(x, 1);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform double 01";
    }

    void TestUniformFloatDistribution(void)
    {
        nsfx::Ptr<nsfx::IUniformFloatDistribution> d =
            dg_->CreateUniformFloatDistribution(-100, 100);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(),  100);
        NSFX_TEST_EXPECT_EQ(d->GetLowerBound(), -100);
        NSFX_TEST_EXPECT_EQ(d->GetUpperBound(),  100);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            float x = d->Generate();
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LT(x,  100);
        }
        double expected = 0;
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform float";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            float x = dg_->GenerateUniformFloat(-100, 100);
            count += x;
            NSFX_TEST_EXPECT_GE(x, -100);
            NSFX_TEST_EXPECT_LT(x,  100);
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 1) << "uniform float";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            bool x = dg_->GenerateBernoulli(0.25);
            if (x)
            {
                ++count;
            }
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Bernoulli";
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
            uint64_t x = d->Generate();
            count += x;
        }
        double expected = 1000 * 0.25;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "binomial";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint64_t x = dg_->GenerateBinomial(1000, 0.25);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "binomial";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = dg_->GenerateNegativeBinomial(100, 0.25);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "negative binomial";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = dg_->GenerateGeometric(0.25);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "geometric";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t x = dg_->GeneratePoisson(1.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Poisson";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateExponential(1.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "exponential";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateGamma(2.0, 3.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "gamma";
    }

    void TestWeibullDistribution(void)
    {
        nsfx::Ptr<nsfx::IWeibullDistribution> d =
            dg_->CreateWeibullDistribution(2.0, 3.0);
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
        double expected = 3.0 * aux::tgamma(1 + 1 / 2.0);
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Weibull";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateWeibull(2.0, 3.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "Weibull";
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
        // Euler–Mascheroni constant
        static const double EulerMascheroniConstant = 0.5772156649015328606;
        double expected = 2.0 + 3.0 * EulerMascheroniConstant;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "extreme value";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateExtremeValue(2.0, 3.0);
            count += x;
        }
        // Euler–Mascheroni constant
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "extreme value";
    }

    void TestBetaDistribution(void)
    {
        nsfx::Ptr<nsfx::IBetaDistribution> d =
            dg_->CreateBetaDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 1);
        NSFX_TEST_EXPECT_EQ(d->GetAlpha(), 2.0);
        NSFX_TEST_EXPECT_EQ(d->GetBeta(), 3.0);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 2.0 / (2.0 + 3.0);
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "beta";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateBeta(2.0, 3.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "beta";
    }

    void TestLaplaceDistribution(void)
    {
        nsfx::Ptr<nsfx::ILaplaceDistribution> d =
            dg_->CreateLaplaceDistribution(1.0, 0.5);
        NSFX_TEST_EXPECT_LE(d->GetMinValue(),
                            -(std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_GE(d->GetMaxValue(),
                            (std::numeric_limits<double>::max)());
        NSFX_TEST_EXPECT_EQ(d->GetMean(), 1.0);
        NSFX_TEST_EXPECT_EQ(d->GetScale(), 0.5);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = d->Generate();
            count += x;
        }
        double expected = 1.0;
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "laplace";
        d->Reset();
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateLaplace(1.0, 0.5);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "laplace";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateNormal(2.0, 3.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.01) << "normal";
    }

    void TestLognormalDistribution(void)
    {
        nsfx::Ptr<nsfx::ILognormalDistribution> d =
            dg_->CreateLognormalDistribution(0.1, 1.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateLognormal(0.1, 1.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.1) << "lognormal";
    }

    void TestChiSquaredDistribution(void)
    {
        nsfx::Ptr<nsfx::IChiSquaredDistribution> d =
            dg_->CreateChiSquaredDistribution(2.5);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateChiSquared(2.5);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "chi-squared";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateCauchy(2.0, 3.0);
            count += x;
        }
        // NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "Cauchy";
    }

    void TestFisherFDistribution(void)
    {
        nsfx::Ptr<nsfx::IFisherFDistribution> d =
            dg_->CreateFisherFDistribution(2.0, 3.0);
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateFisherF(2.0, 3.0);
            count += x;
        }
        NSFX_TEST_EXPECT_RC(expected, count / N, 0.2) << "Fisher F";
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateStudentT(2.5);
            count += x;
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "Student t";
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
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 2);
        NSFX_TEST_EXPECT_EQ(d->GetNumValues(), 3);
        NSFX_TEST_EXPECT_RC(d->GetProbability(0), 0.5, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetProbability(1), 0.3, 0.01);
        NSFX_TEST_EXPECT_RC(d->GetProbability(2), 0.2, 0.01);
        double count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            uint64_t x = d->Generate();
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
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 0);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 3);
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
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 1);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 5);
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
        NSFX_TEST_EXPECT_EQ(d->GetMinValue(), 1);
        NSFX_TEST_EXPECT_EQ(d->GetMaxValue(), 5);
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
        ////////////////////
        count = 0;
        for (size_t i = 0; i < N; ++i)
        {
            double x = dg_->GenerateTriangle(1, 2, 5);
            count += x;
        }
        NSFX_TEST_EXPECT_AC(expected, count / N, 0.1) << "triangle";
    }

    nsfx::Ptr<nsfx::IRandom> dg_;
};


NSFX_TEST_SUITE(PRNG)
{
    NSFX_TEST_CASE(Xoshiro256StarstarEngine)
    {
        try
        {
            typedef nsfx::Xoshiro256StarstarEngine Rng;
            nsfx::Ptr<nsfx::IRandomUInt64Generator> rg(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(rg->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(rg->GetMaxValue(),
                                (std::numeric_limits<uint64_t>::max)());
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(rg);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                rg->Generate();
            }
            nsfx::Ptr<nsfx::IRandom> r(pr);
            TestDistributions td(r);
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
            nsfx::Ptr<nsfx::IRandomDoubleGenerator> rg(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(rg->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(rg->GetMaxValue(), 1);
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(rg);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                double d = rg->Generate();
                NSFX_TEST_EXPECT_GE(d, 0);
                NSFX_TEST_EXPECT_LT(d, 1);
            }
            nsfx::Ptr<nsfx::IRandom> r(pr);
            TestDistributions td(r);
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
            nsfx::Ptr<nsfx::IRandomUInt32Generator> rg(new nsfx::Object<Rng>());
            NSFX_TEST_EXPECT_EQ(rg->GetMinValue(), 0);
            NSFX_TEST_EXPECT_EQ(rg->GetMaxValue(),
                                (std::numeric_limits<uint32_t>::max)());
            nsfx::Ptr<nsfx::IPseudoRandomEngine> pr(rg);
            pr->Discard(1000000);
            pr->Seed(2);
            for (size_t i = 0; i < 1000000; ++i)
            {
                rg->Generate();
            }
            nsfx::Ptr<nsfx::IRandom> r(pr);
            TestDistributions td(r);
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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

