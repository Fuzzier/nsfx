/**
 * @file
 *
 * @brief Test support for network simulation frameworks.
 *
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2016-05-11
 *
 * @copyright Copyright (c) 2016.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CASE_H__8BDB5DEB_3A1D_4606_B861_79E2440E8D4C
#define CASE_H__8BDB5DEB_3A1D_4606_B861_79E2440E8D4C


#include <nsfx/test/config.h>
#include <nsfx/test/result.h>
#include <nsfx/test/visitor-concept.h>
#include <memory>


NSFX_TEST_OPEN_NAMESPACE


class Suite;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 *
 * @brief Test case.
 *
 * A test case is a container of testing functions.
 * Also, when a test assertion fails, a test result is generated,
 * which is stored in this object.
 *
 * <p>
 * A test case can be added to only one test suite, as it's constructed with
 * a parent test suite, and there's no way to change the parent test suite.
 */
class Case /*{{{*/
{
public:
    class Impl
    {
    public:
        virtual ~Impl(void) {}

        // Impl operations.
    public:
        /**
         * @brief Run a user-defined test case.
         *
         * @param currentCase The current test case.
         */
        virtual void run(void) = 0;

    }; // class Impl


public:
    /**
     * @brief Users must use \c Suite::addCase() to create test cases.
     */
    template<class Functor>
    Case(Suite* parent, const string& name, Functor&& functor);

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Case(const Case&));
    BOOST_DELETED_FUNCTION(Case& operator=(const Case&));

    // Movable.
public:
    Case(Case&& rhs) :
        name_   (std::move(rhs.name_)),
        parent_ (rhs.parent_),
        impl_   (std::move(rhs.impl_)),
        results_(std::move(rhs.results_))
    {
    }

    Case& operator=(Case&& rhs)
    {
        if (this != &rhs)
        {
            name_    = std::move(rhs.name_);
            parent_  = rhs.parent_;
            impl_    = std::move(rhs.impl_);
            results_ = std::move(rhs.results_);
        }
        return *this;
    }


    // Methods.
public:
    void run(void)
    {
        impl_->run();
    }

    void addResult(Result&& result)
    {
        results_.push_back(std::move(result));
    }

    size_t getNumberOfFailures(void) const
    {
        return results_.size();
    }

    Suite* getParent(void) const
    {
        return parent_;
    }

    const string& getName(void) const
    {
        return name_;
    }

    template<class ResultVisitor>
    void visitResults(const ResultVisitor& visitor)
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<ResultVisitor, const Result&>));
        for (auto it = results_.cbegin(); it != results_.cend(); ++it)
        {
            const Result& result = *it;
            visitor(result);
        }
    }


    // Properties.
private:
    /**
     * @brief The test suite that contains the this test case.
     */
    Suite* parent_;

    /**
     * @brief The name of the test case, which must be unique within it parent
     *        test suite.
     */
    string name_;

    /**
     * @brief The test results of failed tests.
     */
    vector<Result> results_;

    std::unique_ptr<Impl> impl_;

}; // class Case /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Test case implementation that stores a nullary callable object.
 *
 * @tparam Functor The type of the nullary callable object.
 *
 * Not copyable.
 */
template<class Functor>
class CaseImpl : public Case::Impl /*{{{*/
{
private:
    typedef typename std::decay<Functor>::type FunctorType;


public:
    explicit CaseImpl(Functor&& functor) :
        functor_(std::forward<Functor>(functor))
    {
    }

    virtual ~CaseImpl(void) {}

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(CaseImpl(const CaseImpl&));
    BOOST_DELETED_FUNCTION(CaseImpl& operator=(const CaseImpl&));


    // Case::Impl operations.
public:
    virtual void run(void) NSFX_OVERRIDE
    {
        functor_();
    }


    // Properties.
private:
    FunctorType functor_;

}; // class CaseImpl /*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class Functor>
inline Case::Case(Suite* parent, const string& name, Functor&& functor) :
    parent_(parent),
    name_(name),
    impl_(new CaseImpl<Functor>(std::forward<Functor>(functor)))
{
}


NSFX_TEST_CLOSE_NAMESPACE


#endif // CASE_H__8BDB5DEB_3A1D_4606_B861_79E2440E8D4C

