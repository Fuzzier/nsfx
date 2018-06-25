/**
 * @file
 *
 * @brief Interoperability support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-25
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef MAKE_INDICES_H__73167B8D_27EB_49D0_A43A_4D32651795AE
#define MAKE_INDICES_H__73167B8D_27EB_49D0_A43A_4D32651795AE


#include <nsfx/interop/config.h>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/insert_range.hpp>


NSFX_OPEN_NAMESPACE
namespace aux {


////////////////////////////////////////////////////////////////////////////////
template<class Sequence>
struct MakeIndices
{
    static_assert(boost::mpl::is_sequence<Sequence>::value, "Unexpected.");
    typedef typename boost::mpl::insert_range<
                boost::mpl::vector_c<size_t>,
                0,
                boost::mpl::range_c<size_t, 0, boost::mpl::size<Sequence>::value>
            >::type  type
};


} // namespace aux
NSFX_CLOSE_NAMESPACE


#endif // MAKE_INDICES_H__73167B8D_27EB_49D0_A43A_4D32651795AE

