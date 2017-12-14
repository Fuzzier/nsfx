/**
 * @file
 *
 * @brief Testing support for network simulation Frameworks.
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

#ifndef VISITOR_CONCEPT_H__A0C7938A_88A6_4FE3_8DE8_A9D12597AC5E
#define VISITOR_CONCEPT_H__A0C7938A_88A6_4FE3_8DE8_A9D12597AC5E


#include <nsfx/test/config.h>
#include <boost/concept_check.hpp>


////////////////////////////////////////////////////////////////////////////////
NSFX_TEST_OPEN_NAMESPACE


/**
 * @ingroup Test
 *
 * @brief Visitor concept check.
 *
 * @tparam Visitor A functor type. An instance of \c Visitor can be invoked
 *                 by passing an lvalue reference of \c Visited.
 *                 A visitor can be stateful.
 * @tparam Visited The visited type.
 *
 * e.g.
 * @code
 * void SuiteVisitor(Suite& );
 * BOOST_CONCEPT_ASSERT((VisitorConcept<SuiteVisitor, Suite&>));
 *
 * void CaseVisitor(Case& );
 * BOOST_CONCEPT_ASSERT((VisitorConcept<CaseVisitor, Case&>));
 *
 * void ResultVisitor(const Result& );
 * BOOST_CONCEPT_ASSERT((VisitorConcept<CaseVisitor, const Result&>));
 *
 * void StreamSinkVisitor(std::ostream& );
 * BOOST_CONCEPT_ASSERT((VisitorConcept<StreamSinkVisitor, std::ostream&>));
 * @endcode
 */
template<class Visitor, class Visited>
class VisitorConcept
{
private:
    typedef typename std::decay<Visitor>::type VisitorType;
    typedef typename std::decay<Visited>::type VisitedType;

public:
    BOOST_CONCEPT_USAGE(VisitorConcept)
    {
        Test();
    }

private:
    void Test(void)
    {
        VisitorType* visitor = nullptr;
        VisitedType* visited = nullptr;
        (*visitor)(*visited);
    }
};


NSFX_TEST_CLOSE_NAMESPACE


#endif // VISITOR_CONCEPT_H__A0C7938A_88A6_4FE3_8DE8_A9D12597AC5E

