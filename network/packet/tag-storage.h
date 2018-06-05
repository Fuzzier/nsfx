/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef TAG_STORAGE_H__7A03B7EF_CC75_4248_BEF0_B513C79C0D59
#define TAG_STORAGE_H__7A03B7EF_CC75_4248_BEF0_B513C79C0D59


#include <nsfx/network/config.h>
#include <nsfx/network/packet/i-tag.h>
#include <boost/type_index.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <memory>  // unique_ptr
#include <utility> // forward


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief The low-level storage of a tag. POD.
 *
 * @internal
 */
struct TagStorage
{
    refcount_t  refCount_; ///< The reference count of the tag storage.
    ITag*   intf_; ///< Used to virtually destruct the tag.

    /**
     * @brief The space to hold the <code>TypedTag<></code> .
     *
     * It is properly aligned.
     */
    uint8_t bytes_[1];

    // Helper functions.
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class T, class... Args>
    static TagStorage* Allocate(Args&&... args);

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag-storage.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

    static void AddRef(TagStorage* storage) BOOST_NOEXCEPT;
    static void Release(TagStorage* storage);

    static const boost::typeindex::type_info&
    GetTypeId(const TagStorage* storage) BOOST_NOEXCEPT;

    template<class T>
    static const T& GetValue(const TagStorage* storage) BOOST_NOEXCEPT;
};


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<class T, class... Args>
inline TagStorage* TagStorage::Allocate(Args&&... args)
{
    typedef TypedTag<T>  TagType;

    // Allocate storage.
    size_t size = sizeof (TagStorage) - 1 + sizeof (TagType);
    std::unique_ptr<uint8_t[]>  safeguard(new uint8_t[size]);

    // Deallocate in case the constructor of 'TagType' throws.
    TagStorage* storage = reinterpret_cast<TagStorage*>(safeguard.get());

    // Construct the tag in-place, which may throw.
    TagType* tag = reinterpret_cast<TagType*>(storage->bytes_);
    new (tag) TagType(std::forward<Args>(args)...);

    storage->refCount_ = 1;
    storage->intf_     = tag;

    safeguard.release();
    return storage;
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag-storage.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

inline void TagStorage::AddRef(TagStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    ++storage->refCount_;
}

inline void TagStorage::Release(TagStorage* storage)
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->intf_);
    if (--storage->refCount_ == 0)
    {
        // Call the virtual destructor to destruct the value.
        storage->intf_->~ITag();
        // Deallocate the storage.
        uint8_t* p = reinterpret_cast<uint8_t*>(storage);
        delete[] p;
    }
}

inline const boost::typeindex::type_info&
TagStorage::GetTypeId(const TagStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->intf_);
    return storage->intf_->GetTypeId();
}

template<class T>
inline const T& TagStorage::GetValue(const TagStorage* storage) BOOST_NOEXCEPT
{
    BOOST_ASSERT(storage);
    BOOST_ASSERT(storage->intf_);
    BOOST_ASSERT_MSG(storage->intf_->GetTypeId() ==
                     boost::typeindex::type_id<T>(),
                     "Cannot access the tag value, "
                     "since the requested type mismatches the value type.");
    typedef TypedTag<T>  TagType;
    const TagType* tag = static_cast<const TagType*>(storage->intf_);
    return tag->GetValue();
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_STORAGE_H__7A03B7EF_CC75_4248_BEF0_B513C79C0D59


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

////////////////////////////////////////
# if BOOST_PP_ITERATION_FLAGS() == 0
// template<class T, class A0, class A1, ...>
// static TagStorage* Allocate(A0&& a0, A1&& a1, ...);
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
static TagStorage* Allocate(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a));


////////////////////////////////////////
# elif BOOST_PP_ITERATION_FLAGS() == 1
// template<class T, class A0, class A1, ...>
// inline TagStorage* TagStorage::Allocate(A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline TagStorage* TagStorage::Allocate(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    typedef TypedTag<T>  TagType;

    // Allocate storage.
    size_t size = sizeof (TagStorage) -1 + sizeof (TagType);
    std::unique_ptr<uint8_t[]>  safeguard(new uint8_t[size]);

    // Deallocate in case the constructor of 'TagType' throws.
    TagStorage* storage = reinterpret_cast<TagStorage*>(safeguard.get());

    // Construct the tag in-place, which may throw.
    TagType* tag = reinterpret_cast<TagType*>(storage->bytes_);
    new (tag) TagType(BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));

    storage->refCount_ = 1;
    storage->intf_     = tag;

    safeguard.release();
    return storage;
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

