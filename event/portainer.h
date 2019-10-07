/**
 * @file
 *
 * @brief Connection support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-01-23
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef PORTAINER_H__5FE8D42F_B6A3_42A3_AC0B_2E75F6E36966
#define PORTAINER_H__5FE8D42F_B6A3_42A3_AC0B_2E75F6E36966


#include <nsfx/event/config.h>
#include <nsfx/event/i-event.h>
#include <nsfx/event/exception.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <boost/concept_check.hpp>
#include <type_traits> // decay
#include <utility>     // move, forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// PortainableItemConcept
template<class T>
class PortainableItemConcept
{
public:
    BOOST_CONCEPT_USAGE(PortainableItemConcept)
    {
        IsDefaultConstructible();
        IsCopyAssignable();
        HasNegateOperator();
    }

    void IsDefaultConstructible(void)
    {
        T t;
    }

    void IsCopyAssignable(void)
    {
        T t1;
        T t2;
        t1 = t2;
    }

    void HasNegateOperator(void)
    {
        T t;
        bool b = !t; // bool operator!()
    }
};


////////////////////////////////////////////////////////////////////////////////
// PortainableItemVisitorConcept.
/**
 * @ingroup Event
 * @brief Portainer item visitor concept.
 *
 * @tparam Visitor The type of a callable object.
 *                 The prototype is `void(const T&)`.
 * @tparam T       The type of items stored in the `Portainer`.
 */
template<class Visitor, class T>
class PortainableItemVisitorConcept
{
public:
    BOOST_CONCEPT_USAGE(PortainableItemVisitorConcept)
    {
        Test();
    }

    void Test(void)
    {
        T* port = nullptr;
        typename std::decay<Visitor>::type* visitor = nullptr;
        (*visitor)(*port);
    }
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Event
 * @brief A vector-based container whose items are identified by cookies.
 *
 * This class is used by connectable components to store the information of
 * connections, and generate cookies for each connection.
 *
 * The container can be viewed as a vector of items (ports).
 * Each port stores the information of a connection.
 *
 * When a connection is to be established, the connectable component requests
 * this container to find a free slot to store the information (a port).
 * The 1-based index of the slot is used as the cookie of the item.
 *
 * When a connection is to be closed, the connectable component requests this
 * container to remove the information (the port) whose 1-based index matches
 * the cookie value.
 *
 * @tparam T        The type of stored items.
 *                  It **must** satisfy `PortainableItemConcept`.
 *                  The default value of `T` **must** be `false`, and **must** hold
 *                  no resources.
 *                  e.g., a smart pointer.
 * @tparam capacity The capacity of the container.
 */
template<class T, uint32_t capacity = UINT32_MAX>/*{{{*/
class Portainer
{
    BOOST_CONCEPT_ASSERT((PortainableItemConcept<T>));
    static_assert(capacity > 0,
                  "Invalid capacity for Portainer class template.");
    static_assert(sizeof (cookie_t) >= sizeof (uint32_t),
                  "Invalid cookie_t type.");

public:
    Portainer(void) :
        size_(0),
        last_(0)
    {
    }

public:
    cookie_t Add(const T& item)
    {
        BOOST_ASSERT(!!item);
        cookie_t cookie = 0;
        if (size_ < capacity)
        {
            cookie = InternalAdd(item);
        }
        return cookie;
    }

    cookie_t Add(T&& item)
    {
        BOOST_ASSERT(!!item);
        cookie_t cookie = 0;
        if (size_ < capacity)
        {
            cookie = InternalAdd(std::move(item));
        }
        return cookie;
    }

    void Remove(cookie_t cookie)
    {
        if (--cookie < items_.size())
        {
            if (!!items_[cookie])
            {
                --size_;
                items_[cookie] = T();  // default constructible, copy assignable
                while (last_ && !items_[last_-1])
                {
                    --last_;
                }
            }
        }
    }

private:
    template<class T_>
    cookie_t InternalAdd(T_&& item)
    {
        cookie_t cookie = 0;
        // If there is a free slot, find and reuse the slot.
        if (size_ < items_.size())
        {
            // Search for a free slot.
            for (uint32_t i = 0; i < items_.size(); ++i)
            {
                // If this is a free slot.
                if (!items_[i])  // bool operator!()
                {
                    items_[i] = std::forward<T_>(item);
                    ++size_;
                    cookie = i + 1;
                    if (last_ < cookie)
                    {
                        last_ = cookie;
                    }
                    break;
                }
            }
        }
        // If the pool is full, try to extend it.
        else // if (size_ == items_.size())
        {
            try
            {
                items_.emplace_back(std::forward<T_>(item));
                ++size_;
                cookie = size_;
                last_  = cookie;
            }
            catch (std::bad_alloc& )
            {
                // Leave cookie to be 0.
            }
        }
        return cookie;
    }

public:
    /**
     * @brief Get the number of items.
     */
    uint32_t GetSize(void) const BOOST_NOEXCEPT
    {
        return size_;
    }

    T Get(cookie_t cookie) const
    {
        return (cookie > 0 && cookie <= items_.size())
             ? items_[--cookie]  // copy assignable
             : T();              // default constructible
    }

    T operator[](cookie_t cookie) const
    {
        return Get(cookie);
    }

    /**
     * @brief Visit the connections in an arbitrary order.
     *
     * @tparam Visitor A callable object that has a prototype of
     *                 `void Visitor(const T&)`.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((PortainableItemVisitorConcept<Visitor, T>));

        for (size_t i = 0; i < last_; ++i)
        {
            if (!!items_[i])
            {
                visitor(items_[i]);
            }
        }
    }

private:
    uint32_t  size_;
    uint32_t  last_; // The cookie of the last non-null item.
    vector<T> items_;

};/*}}}*/


////////////////////////////////////////////////////////////////////////////////
template<class T>/*{{{*/
class Portainer<T, /* capacity = */ 1>
{
    BOOST_CONCEPT_ASSERT((PortainableItemConcept<T>));

public:
    Portainer(void) {}

public:
    cookie_t Add(const T& item)
    {
        BOOST_ASSERT(!!item);
        cookie_t cookie = 0;
        if (!item_)
        {
            item_ = item;
            cookie = 1;
        }
        return cookie;
    }

    cookie_t Add(T&& item)
    {
        BOOST_ASSERT(!!item);
        cookie_t cookie = 0;
        if (!item_)
        {
            item_ = std::move(item);
            cookie = 1;
        }
        return cookie;
    }

    void Remove(cookie_t cookie)
    {
        if (cookie == 1)
        {
            item_ = T();
        }
    }

public:
    /**
     * @brief Get the number of items.
     */
    uint32_t GetSize(void) const BOOST_NOEXCEPT
    {
        return !item_ ? 0 : 1;
    }

    T Get(cookie_t cookie) const
    {
        return (cookie == 1)
             ? item_
             : T();
    }

    T operator[](cookie_t cookie) const
    {
        return Get(cookie);
    }

    /**
     * @brief Visit the connections in an arbitrary order.
     *
     * @tparam Visitor A callable object that has a prototype of
     *                 `void Visitor(const T&)`.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((PortainableItemVisitorConcept<Visitor, T>));
        if (!!item_)
        {
            visitor(item_);
        }
    }

private:
    T item_;

};/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // PORTAINER_H__5FE8D42F_B6A3_42A3_AC0B_2E75F6E36966

