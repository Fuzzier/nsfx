/**
 * @file
 *
 * @brief Test support for network simulation frameworks.
 *
 * @version 1.0
 * @author  Fuzzier Tang <gauchyler@gmail.com>
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
    Logger* getLogger(void)
    {
        return &logger_;
    }

    Suite* getMasterSuite(void)
    {
        return &masterSuite_;
    }

private:
    void setStopFlag(void)
    {
        stop_ = true;
    }

public:
    bool getStopFlag(void) const
    {
        return stop_;
    }

private:
    void setActiveCase(Case* activeCase)
    {
        activeCase_ = activeCase;
    }

    Case* getActiveCase(void) const
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
    void commitResult(Result&& result)
    {
        if (result.getToolLevel() == ToolLevel::ASSERT)
        {
            setStopFlag();
        }
        showResult(result);
        getActiveCase()->addResult(std::move(result));
    }

    void run(void)
    {
        numCases_ = masterSuite_.getNumberOfCases();
        numTestedCases_ = 0;
        nextProgress_ = 1;
        prevSuite_ = &masterSuite_;
        prevCase_ = nullptr;
        showMessage("Test started:");
        runSuite(masterSuite_);
        showSummary();
    }

private:
    void showMessage(const std::string& message)
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

        logger_.visitStreams(Visitor(message));
    }

    void showProgress(void)
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
            logger_.visitStreams(Visitor(nextProgress_ - 1));
        }
    }

    void showResult(const Result& result)
    {
        struct Visitor /*{{{*/
        {
            Visitor(Suite* prevSuite, Case* prevCase, Case* activeCase,
                    const Result& result) :
                prevSuite_(prevSuite),
                prevCase_(prevCase),
                activeCase_(activeCase),
                result_(result)
            {
            }

            void operator()(std::ostream& os)
            {
                Suite* activeSuite = activeCase_->getParent();
                size_t depth = activeSuite->getDepth();
                if (activeSuite != prevSuite_)
                {
                    displaySuite(os, activeSuite, depth);
                }
                displayCase(os, activeCase_, depth);
                displayResult(os, result_, depth);
            }

        private:
            // Precondition: suite is not displayed.
            void displaySuite(std::ostream& os, Suite* suite, size_t depth)
            {
                if (!suite->isMaster())
                {
                    displaySuite(os, suite->getParent(), depth - 1);
                    os << std::string(depth - 1, ' ');
                    os << "+";
                    os << suite->getName() << std::endl;
                }
            }

            void displayCase(std::ostream& os, Case* casei, size_t depth)
            {
                os << std::string(depth, ' ');
                os << "-";
                os << casei->getName() << std::endl;
            }

            void displayResult(std::ostream& os, const Result& result,
                               size_t depth)
            {
                os << std::string(depth + 2, ' ');
                // Make clear the position of error.
                os << result.getFileName();
                os << "(" << result.getLineNumber() << "): ";
                os << result.getToolLevel() << ". ";
                os << "\"" << result.getDescription() << "\" ";
                os << "[" << result.getDetail() <<  "].";
                if (!result.getMessage().empty())
                {
                    os << " " << result.getMessage();
                }
                os << std::endl;
            }

        private:
            Suite* prevSuite_;
            Case*  prevCase_;
            Case*  activeCase_;
            const Result& result_;

        }; // struct Visitor /*}}}*/

        logger_.visitStreams(Visitor(prevSuite_, prevCase_, activeCase_, result));
        prevCase_ = activeCase_;
        prevSuite_ = prevCase_->getParent();
    }

    void showSummary(void)
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
                   << suite_->getNumberOfFailures() << "." << std::endl;
            }


        private:
            bool   stopped_;
            Suite* suite_;

        }; // struct Visitor /*}}}*/

        logger_.visitStreams(Visitor(stop_, &masterSuite_));
    }

private:
    void runSuite(Suite& suite)
    {
        if (!stop_)
        {
            suite.visitCases([this] (Case& casei) { runCase(casei); });
        }
        if (!stop_)
        {
            suite.visitSuites([this] (Suite& suite) { runSuite(suite); });
        }
    }

    void runCase(Case& casei)
    {
        if (!stop_)
        {
            setActiveCase(&casei);
            casei.run();
            ++numTestedCases_;
            showProgress();
        }
    }



    // Singleton.
public:
    static Runner* getInstance(void)
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


Logger* getLogger(void)
{
    return Runner::getInstance()->getLogger();
}

Suite* getMasterSuite(void)
{
    return Runner::getInstance()->getMasterSuite();
}

bool getStopFlag(void)
{
    return Runner::getInstance()->getStopFlag();
}

void commitResult(Result&& result)
{
    Runner::getInstance()->commitResult(std::move(result));
}

void run(void)
{
    Runner::getInstance()->run();
}


} // namespace runner


NSFX_TEST_CLOSE_NAMESPACE


#endif // RUNNER_H__30677E1A_807A_4439_9F3F_93DA9D08841C

