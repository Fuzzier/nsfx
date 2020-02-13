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

#ifndef SUITE_H__5ED8E64A_19AD_4337_84F2_375936BE9620
#define SUITE_H__5ED8E64A_19AD_4337_84F2_375936BE9620


#include <nsfx/test/config.h>
#include <nsfx/test/visitor-concept.h>
#include <nsfx/test/case.h>
#include <memory> // unique_ptr


NSFX_TEST_OPEN_NAMESPACE


/**
 * @brief A test suite.
 *
 * Child test cases of the test suite run run first,
 * then the child test suites are run.
 *
 * @remarks As `Case` stores a pointer to its parent `Suite`,
 *          the address of `Suite` must be stable.
 *          e.g. `Suite` cannot be stored directly inside a vector,
 *          as the vector may reallocate space, and move `Suite` around.
 *          <p>
 *          The current solution is to store `Suite` objects in an
 *          `unordered_map` to avoid the dirty work of memory management.
 */
class Suite
{
    // Typedefs.
public:
    typedef vector<Case* > CaseVectorType;
    typedef vector<Suite*> SuiteVectorType;

    typedef unordered_map<std::string, std::unique_ptr<Case> > CaseMapType;
    typedef unordered_map<std::string, std::unique_ptr<Suite>> SuiteMapType;

    // Constructors.
public:
    /**
     * @brief Users must use `AddSuite()` to create test suites.
     */
    Suite(void) :
        parent_(nullptr),
        name_("Master")
    {
    }

    /**
     * @brief Users must use `AddSuite()` to create test suites.
     */
    Suite(Suite* parent, const std::string& name) :
        parent_(parent),
        name_(name)
    {
    }

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Suite(const Suite&));
    BOOST_DELETED_FUNCTION(Suite& operator=(const Suite&));

    // Movable.
public:
    Suite(Suite&& rhs) :
        parent_      (rhs.parent_),
        name_        (std::move(rhs.name_)),
        caseVector_  (std::move(rhs.caseVector_)),
        caseMap_     (std::move(rhs.caseMap_)),
        suiteVector_ (std::move(rhs.suiteVector_)),
        suiteMap_    (std::move(rhs.suiteMap_))
    {
    }

    Suite& operator=(Suite&& rhs)
    {
        if (this != &rhs)
        {
            parent_      = rhs.parent_;
            name_        = std::move(rhs.name_);
            caseVector_  = std::move(rhs.caseVector_);
            caseMap_     = std::move(rhs.caseMap_);
            suiteVector_ = std::move(rhs.suiteVector_);
            suiteMap_    = std::move(rhs.suiteMap_);
        }
        return *this;
    }


    // Methods.
public:
    /**
     * @brief Add a child test suite.
     *
     * @return If a test suite with the specified name already exists,
     *         a reference to the existing test suite is returned.
     */
    Suite* AddSuite(const std::string& name)
    {
        Suite* suite = nullptr;
        auto it = suiteMap_.find(name);
        if (it == suiteMap_.end())
        {
            auto result = suiteMap_.emplace(name, new Suite(this, name));
            suite = result.first->second.get();
            suiteVector_.push_back(suite);
        }
        else
        {
            suite = it->second.get();
        }
        return suite;
    }

    /**
     * @brief Add test case.
     */
    template<class Functor>
    Case* AddCase(const std::string& name, Functor&& functor)
    {
        Case* casei = nullptr;
        auto it = caseMap_.find(name);
        if (it == caseMap_.end())
        {
            auto result = caseMap_.emplace(
                name, new Case(this, name, std::forward<Functor>(functor)));
            casei = result.first->second.get();
            caseVector_.push_back(casei);
        }
        else
        {
            casei = it->second.get();
        }
        return casei;
    }

    const std::string& GetName(void) const
    {
        return name_;
    }

    size_t GetNumberOfCases(void) const
    {
        size_t numCases = 0;
        numCases += caseVector_.size();
        for (auto it = suiteVector_.cbegin(); it != suiteVector_.cend(); ++it)
        {
            numCases += (*it)->GetNumberOfCases();
        }
        return numCases;
    }

    size_t GetNumberOfFailures(void) const
    {
        size_t numFailures = 0;
        for (auto it = caseVector_.cbegin(); it != caseVector_.cend(); ++it)
        {
            numFailures += (*it)->GetNumberOfFailures();
        }
        for (auto it = suiteVector_.cbegin(); it != suiteVector_.cend(); ++it)
        {
            numFailures += (*it)->GetNumberOfFailures();
        }
        return numFailures;
    }

    bool IsMaster(void) const
    {
        return !parent_;
    }

    Suite* GetParent(void) const
    {
        return parent_;
    }

    size_t GetDepth(void) const
    {
        size_t depth = 0;
        for (const Suite* suite = this;
             !suite->IsMaster();
             suite = suite->GetParent())
        {
            ++depth;
        }
        return depth;
    }

    template<class CaseVisitor>
    void VisitCases(CaseVisitor&& visitor)
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<CaseVisitor, Case&>));
        for (auto it = caseVector_.begin(); it != caseVector_.end(); ++it)
        {
            Case* casei = *it;
            visitor(*casei);
        }
    }

    template<class SuiteVisitor>
    void VisitSuites(SuiteVisitor&& visitor)
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<SuiteVisitor, Suite&>));
        for (auto it = suiteVector_.begin(); it != suiteVector_.end(); ++it)
        {
            Suite* suite = *it;
            visitor(*suite);
        }
    }


    // Properties.
private:
    Suite* parent_;
    std::string name_;

    CaseVectorType caseVector_;
    CaseMapType    caseMap_;

    SuiteVectorType suiteVector_;
    SuiteMapType    suiteMap_;

}; // class Suite


NSFX_TEST_CLOSE_NAMESPACE


#endif // SUITE_H__5ED8E64A_19AD_4337_84F2_375936BE9620

