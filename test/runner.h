/**
 * @file
 *
 * @brief Test support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RUNNER_H__30677E1A_807A_4439_9F3F_93DA9D08841C
#define RUNNER_H__30677E1A_807A_4439_9F3F_93DA9D08841C


#include <nsfx/test/config.h>
#include <nsfx/test/case.h>
#include <nsfx/test/suite.h>
#include <nsfx/test/logger.h>
#include <memory> // unique_ptr


////////////////////////////////////////////////////////////////////////////////
NSFX_TEST_OPEN_NAMESPACE


/**
 * @ingroup Test
 *
 * @brief The test runner.
 */
class Runner
{
private:
    // Constructors.
private:
    Runner(void) :
        activeCase_(nullptr),
        stop_(false),
        numCases_(0),
        numTestedCases_(0),
        nextProgress_(0)
    {
    }

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Runner(const Runner&));
    BOOST_DELETED_FUNCTION(Runner& operator=(const Runner&));


    // Methods.
public:
    Logger* GetLogger(void)
    {
        return &logger_;
    }

    Suite* GetMasterSuite(void)
    {
        return &masterSuite_;
    }

private:
    void SetStopFlag(void)
    {
        stop_ = true;
    }

public:
    bool GetStopFlag(void) const
    {
        return stop_;
    }

private:
    void SetActiveCase(Case* activeCase)
    {
        activeCase_ = activeCase;
    }

    Case* GetActiveCase(void) const
    {
        return activeCase_;
    }

public:
    /**
     * @brief Commit a test result to the active test case.
     *
     * @remarks The result is also logged.
     *
     * A test result is committed when a test assertion has failed.
     * If the test level is \c TestLevel::ASSERT, the stop flag is set.
     */
    void CommitResult(std::unique_ptr<Result>&& result)
    {
        if (result->GetToolLevel() == ToolLevel::ASSERT)
        {
            SetStopFlag();
        }
        ShowResult(&*result);
        GetActiveCase()->AddResult(std::move(result));
    }

    void CommitMessage(const std::string& message)
    {
        ShowMessage(message);
    }

    void Run(void)
    {
        numCases_ = masterSuite_.GetNumberOfCases();
        numTestedCases_ = 0;
        nextProgress_ = 1;
        prevSuite_ = &masterSuite_;
        prevCase_ = nullptr;
        ShowMessage("Test started:");
        RunSuite(masterSuite_);
        ShowSummary();
    }

public:
    void ShowMessage(const std::string& message)
    {
        struct Visitor /*{{{*/
        {
            Visitor(const std::string& message) :
                message_(message)
            {
            }

            void operator()(std::ostream& os) const
            {
                os << message_ << std::endl;
            }

            const std::string& message_;
        }; // struct Visitor /*}}}*/

        logger_.VisitStreams(Visitor(message));
    }

private:
    void ShowProgress(void)
    {
        struct Visitor /*{{{*/
        {
            Visitor(size_t progress) :
                progress_(progress)
            {
            }

            void operator()(std::ostream& os)
            {
                os << progress_ * 10 << "%" << std::endl;
            }

        private:
            size_t progress_;
        }; // struct Visitor /*}}}*/

        bool show = false;
        while ((double)numTestedCases_ / numCases_ * 10 >= nextProgress_)
        {
            ++nextProgress_;
            show = true;
        }
        if (show)
        {
            logger_.VisitStreams(Visitor(nextProgress_ - 1));
        }
    }

    void ShowResult(Result* result)
    {
        struct Visitor /*{{{*/
        {
            Visitor(Suite* prevSuite, Case* prevCase, Case* activeCase,
                    Result* result) :
                prevSuite_(prevSuite),
                prevCase_(prevCase),
                activeCase_(activeCase),
                result_(result)
            {
            }

            void operator()(std::ostream& os)
            {
                Suite* activeSuite = activeCase_->GetParent();
                size_t depth = activeSuite->GetDepth();
                if (activeSuite != prevSuite_)
                {
                    DisplaySuite(os, activeSuite, depth);
                }
                DisplayCase(os, activeCase_, depth);
                DisplayResult(os, result_, depth);
            }

        private:
            // Precondition: suite is not displayed.
            void DisplaySuite(std::ostream& os, Suite* suite, size_t depth)
            {
                if (!suite->IsMaster())
                {
                    DisplaySuite(os, suite->GetParent(), depth - 1);
                    os << std::string(depth - 1, ' ');
                    os << "+";
                    os << suite->GetName() << std::endl;
                }
            }

            void DisplayCase(std::ostream& os, Case* casei, size_t depth)
            {
                os << std::string(depth, ' ');
                os << "-";
                os << casei->GetName() << std::endl;
            }

            void DisplayResult(std::ostream& os, Result* result, size_t depth)
            {
                os << std::string(depth + 2, ' ');
                // Make clear the position of error.
                os << result->GetFileName();
                os << "(" << result->GetLineNumber() << "): ";
                os << result->GetToolLevel() << ". ";
                os << "\"" << result->GetDescription() << "\" ";
                os << "[" << result->GetDetail() <<  "].";
                if (!result->GetMessage().empty())
                {
                    os << " " << result->GetMessage();
                }
                os << std::endl;
            }

        private:
            Suite*  prevSuite_;
            Case*   prevCase_;
            Case*   activeCase_;
            Result* result_;

        }; // struct Visitor /*}}}*/

        logger_.VisitStreams(Visitor(prevSuite_, prevCase_, activeCase_, result));
        prevCase_ = activeCase_;
        prevSuite_ = prevCase_->GetParent();
    }

    void ShowSummary(void)
    {
        struct Visitor /*{{{*/
        {
            Visitor(bool stopped, Suite* suite) :
                stopped_(stopped),
                suite_(suite)
            {
            }


            void operator()(std::ostream& os)
            {
                if (stopped_)
                {
                    os << "Test stopped due to fatal error." << std::endl;
                }
                os << "Total number of test failures: "
                   << suite_->GetNumberOfFailures() << "." << std::endl;
            }


        private:
            bool   stopped_;
            Suite* suite_;

        }; // struct Visitor /*}}}*/

        logger_.VisitStreams(Visitor(stop_, &masterSuite_));
    }

private:
    void RunSuite(Suite& suite)
    {
        if (!stop_)
        {
            suite.VisitCases([this] (Case& casei) { RunCase(casei); });
        }
        if (!stop_)
        {
            suite.VisitSuites([this] (Suite& suite) { RunSuite(suite); });
        }
    }

    void RunCase(Case& casei)
    {
        if (!stop_)
        {
            SetActiveCase(&casei);
            casei.Run();
            ++numTestedCases_;
            ShowProgress();
        }
    }



    // Singleton.
public:
    static Runner* GetInstance(void)
    {
        static Runner runner;
        return &runner;
    }


    // Properties.
private:
    Logger logger_;
    Suite  masterSuite_;
    Case*  activeCase_;
    bool   stop_;

    // Variables that relates to displaying test progress and results.
    size_t numCases_;
    size_t numTestedCases_;
    size_t nextProgress_;
    /**
     * @brief The previously displayed test suite.
     *
     * All the ancestors of the suite has also been displayed.
     * <p>
     * If the previous test result belongs to the same test suite, then
     * the test suite is not displayed again.
     */
    Suite* prevSuite_;
    /**
     * @brief The previously displayed test case.
     *
     * When a test result is displayed, the test case that contains the
     * test result will be displayed.
     * If the previous test result belongs to the same test case, then
     * the test case is not displayed again.
     */
    Case*  prevCase_;

}; // class Runner


////////////////////////////////////////////////////////////////////////////////
// Utility functions to access the singleton of Runner.
namespace runner
{


inline Logger* GetLogger(void)
{
    return Runner::GetInstance()->GetLogger();
}

inline Suite* GetMasterSuite(void)
{
    return Runner::GetInstance()->GetMasterSuite();
}

inline bool GetStopFlag(void)
{
    return Runner::GetInstance()->GetStopFlag();
}

inline void CommitResult(std::unique_ptr<Result>&& result)
{
    Runner::GetInstance()->CommitResult(std::move(result));
}

inline void ShowMessage(const std::string& message)
{
    Runner::GetInstance()->ShowMessage(message);
}

inline void Run(void)
{
    Runner::GetInstance()->Run();
}


} // namespace runner


NSFX_TEST_CLOSE_NAMESPACE


#endif // RUNNER_H__30677E1A_807A_4439_9F3F_93DA9D08841C

