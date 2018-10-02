/**
 * @file
 *
 * @brief Fixed length address.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-10
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ADDRESS_CONCEPT_H__068F2DDB_E859_446E_A5CA_94968295F27A
#define ADDRESS_CONCEPT_H__068F2DDB_E859_446E_A5CA_94968295F27A


#include <nsfx/utility/config.h>
#include <boost/concept_check.hpp>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
template<class Address>
class AddressConcept
{
public:
    BOOST_CONCEPT_USAGE(AddressConcept)
    {
        DefaultConstructible();
        Copyable();
        Comparable();
        Arithmetic();
        Hashable();
        Swappable();
        Methods();
    }

    void DefaultConstructible(void)
    {
        Address a;
    }

    void Copyable(void)
    {
        const Address* a = nullptr;
        // Copy construct.
        Address b(*a);
        // Copy assign.
        b = *a;
    }

    void Comparable(void)
    {
        bool result = false;
        const Address* a = nullptr;
        result = (*a == *a);
        result = (*a != *a);
        result = (*a <  *a);
        result = (*a <= *a);
        result = (*a >  *a);
        result = (*a >= *a);
    }

    void Arithmetic(void)
    {
        Address* address = nullptr;

        ++(*address);
        (*address)++;

        --(*address);
        (*address)--;

        *address += 1;
        *address -= 1;

        *address = *address + 1;
        *address = 1 + *address;
        *address = *address - 1;

        auto n = *address - *address;
    }

    void Hashable(void)
    {
        const Address* a = nullptr;
        size_t hash = boost::hash<Address>()(*a);
    }

    void Swappable(void)
    {
        Address* a = nullptr;
        Address* b = nullptr;
        boost::swap(*a, *b);
    }

    void Methods(void)
    {
        Address* a = nullptr;
        *a = Address::Zero();

        size_t bits  = a->GetBitSize();
        size_t bytes = a->GetSize();

    }

};


NSFX_CLOSE_NAMESPACE


#endif // ADDRESS_CONCEPT_H__068F2DDB_E859_446E_A5CA_94968295F27A

