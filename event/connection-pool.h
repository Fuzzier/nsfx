/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
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

#ifndef CONNECTION_POOL_H__4606C8A1_1EBE_11E9_A0FD_989096D393BD
#define CONNECTION_POOL_H__4606C8A1_1EBE_11E9_A0FD_989096D393BD


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
// ConnectionItemConcept
template<class T>
class ConnectionItemConcept
{
public:
    BOOST_CONCEPT_USAGE(ConnectionItemConcept)
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
// ConnectionItemVisitorConcept.
/**
 * @ingroup Event
 * @brief Connection item visitor concept.
 *
 * @tparam Visitor The type of a callable object.
 *                 The prototype is <code>void(const T&)</code>.
 * @tparam T       The type of items stored in \c ConnectionItemPool.
 */
template<class Visitor, class T>
class ConnectionItemVisitorConcept
{
public:
    BOOST_CONCEPT_USAGE(ConnectionItemVisitorConcept)
    {
        Test();
    }

    void Test(void)
    {
        T* item = nullptr;
        typename std::decay<Visitor>::type* visitor = nullptr;
        (*visitor)(*item);
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
 * The container can be viewed as a vector of slots.
 * Each slot can store an item which holds the information of a connection.
 *
 * When a connection is to be established, the connectable component requests
 * this container to find a free slot to store the information (an item).
 * The 1-based index of the slot is used as the cookie of the connection.
 *
 * When a connection is to be closed, the connectable component requests this
 * container to remove the information (the item) from the slot whose 1-based
 * index matches the cookie value.
 *
 * @tparam T        The type of items.
 *                  It <b>must</b> satisfy \c ConnectionItemConcept.
 *                  The default value of \c T <b>must</b> be \c false, and
 *                  <b>must</b> hold no resources.
 * @tparam capacity The capacity of the container.
 */
template<class T, uint32_t capacity = UINT32_MAX>/*{{{*/
class ConnectionPool
{
    BOOST_CONCEPT_ASSERT((ConnectionItemConcept<T>));
    static_assert(capacity > 0,
                  "Invalid capacity for ConnectionPool class template.");
    static_assert(sizeof (cookie_t) >= sizeof (uint32_t),
                  "Invalid cookie_t type.");

public:
    ConnectionPool(void) :
        size_(0),
        last_(0)
    {
    }

public:
    cookie_t Connect(const T& item)
    {
        if (!item)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        if (size_ == capacity)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        cookie_t cookie = Insert(item);
        if (!cookie)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        return cookie;
    }

    cookie_t Connect(T&& item)
    {
        if (!item)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        if (size_ == capacity)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        cookie_t cookie = Insert(std::move(item));
        if (!cookie)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        return cookie;
    }

    template<class Visitor>
    void Disconnect(cookie_t cookie, Visitor&& visitor)
    {
        BOOST_CONCEPT_ASSERT((ConnectionItemVisitorConcept<Visitor, T>));
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

    void Disconnect(cookie_t cookie)
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
    cookie_t Insert(T_&& item)
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
     * @brief Get the number of connections.
     */
    uint32_t GetNumConnections(void) const BOOST_NOEXCEPT
    {
        return size_;
    }

    const T& GetConnection(cookie_t cookie) const
    {
        if (cookie == 0 || cookie > items_.size())
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        const T& item = items_[--cookie];
        if (!item)
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        return item;
    }

    /**
     * @brief Visit the connections in an arbitrary order.
     *
     * @tparam Visitor A callable object that has a prototype of
     *                 <code>void Visitor(const T&)</code>.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((ConnectionItemVisitorConcept<Visitor, T>));

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
class ConnectionPool<T, /* capacity = */ 1>
{
    BOOST_CONCEPT_ASSERT((ConnectionItemConcept<T>));

public:
    ConnectionPool(void) {}

public:
    cookie_t Connect(const T& item)
    {
        if (!item)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        if (!!item_)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        item_ = item;
        return 1;
    }

    cookie_t Connect(T&& item)
    {
        if (!item)
        {
            BOOST_THROW_EXCEPTION(InvalidArgument());
        }
        if (!!item_)
        {
            BOOST_THROW_EXCEPTION(ConnectionLimit());
        }
        item_ = std::move(item);
        return 1;
    }

    void Disconnect(cookie_t cookie)
    {
        if (cookie == 1)
        {
            item_ = T();
        }
    }

public:
    /**
     * @brief Get the number of connections.
     */
    uint32_t GetNumConnections(void) const BOOST_NOEXCEPT
    {
        return !item_ ? 0 : 1;
    }

    const T& GetConnection(cookie_t cookie) const
    {
        if (cookie != 1 || !item_)
        {
            BOOST_THROW_EXCEPTION(NoConnection());
        }
        return item_;
    }

    /**
     * @brief Visit the connections in an arbitrary order.
     *
     * @tparam Visitor A callable object that has a prototype of
     *                 <code>void Visitor(const T&)</code>.
     */
    template<class Visitor>
    void Visit(Visitor&& visitor) const
    {
        BOOST_CONCEPT_ASSERT((ConnectionItemVisitorConcept<Visitor, T>));
        if (!!item_)
        {
            visitor(item_);
        }
    }

private:
    T item_;

};/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CONNECTION_POOL_H__4606C8A1_1EBE_11E9_A0FD_989096D393BD

