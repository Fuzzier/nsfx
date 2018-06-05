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

#ifndef TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F
#define TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F


#include <nsfx/network/config.h>
#include <nsfx/network/packet/tag-storage.h>
#include <nsfx/network/packet/tag-index.h>
#include <boost/type_index.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Network
 * @brief A free tag that is not associated with any bytes.
 */
class Tag
{
    // Xtructors.
private:
    /**
     * @brief Construct a tag.
     *
     * @param[in] tagId   The id of the tag.
     * @param[in] storage <b>Must</b> not be \c nullptr.
     */
    Tag(size_t tagId, TagStorage* storage) BOOST_NOEXCEPT;

public:
    ~Tag(void);

    // Copyable.
public:
    Tag(const Tag& rhs) BOOST_NOEXCEPT;
    Tag& operator=(const Tag& rhs);

public:
    /**
     * @brief Get the id of the tag.
     */
    size_t GetId(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the value type of the tag.
     */
    const boost::typeindex::type_info& GetTypeId(void) const BOOST_NOEXCEPT;

    /**
     * @brief Get the value of the tag.
     *
     * @tparam T The type of the value.
     *           It <b>must</b> have the same type id as the return value of
     *           \c Tag::GetTypeId().
     */
    template<class T>
    const T& GetValue(void) const BOOST_NOEXCEPT;

public:
    /**
     * @brief Make a tag index.
     *
     * This function is provided for \c TagList::Insert().
     * Users <b>shall</b> not use this function to obtain and modify the tag
     * storage.
     *
     * @internal
     */
    TagIndex MakeTagIndex(size_t tagStart, size_t tagEnd) const BOOST_NOEXCEPT;

private:
    size_t id_;
    TagStorage* storage_;

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template<class T, class... Args>
    friend Tag MakeTag(size_t tagId, Args&&... args);
#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag.h>, 0))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
};


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
/**
 * @ingroup Network
 * @brief Make a free tag.
 */
template<class T, class... Args>
Tag MakeTag(size_t tagId, Args&&... args);
#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


////////////////////////////////////////////////////////////////////////////////
inline Tag::Tag(size_t tagId, TagStorage* storage) BOOST_NOEXCEPT :
    id_(tagId),
    storage_(storage)
{
}

inline Tag::~Tag(void)
{
    TagStorage::Release(storage_);
}

inline Tag::Tag(const Tag& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    storage_(rhs.storage_)
{
    TagStorage::AddRef(storage_);
}

inline Tag& Tag::operator=(const Tag& rhs)
{
    if (this != &rhs)
    {
        TagStorage* tmp = storage_;
        id_ = rhs.id_;
        storage_ = rhs.storage_;
        TagStorage::AddRef(storage_);
        TagStorage::Release(tmp);
    }
    return *this;
}

inline size_t Tag::GetId(void) const BOOST_NOEXCEPT
{
    return id_;
}

inline const boost::typeindex::type_info&
Tag::GetTypeId(void) const BOOST_NOEXCEPT
{
    return TagStorage::GetTypeId(storage_);
}

template<class T>
inline const T& Tag::GetValue(void) const BOOST_NOEXCEPT
{
    return TagStorage::GetValue<T>(storage_);
}

inline TagIndex Tag::MakeTagIndex(size_t tagStart, size_t tagEnd) const BOOST_NOEXCEPT
{
    TagIndex idx;
    TagStorage::AddRef(storage_);
    TagIndex::Ctor(&idx, id_, tagStart, tagEnd, storage_);
    return idx;
}


////////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
template<class T, class... Args>
inline Tag MakeTag(size_t tagId, Args&&... args)
{
    TagStorage* storage = TagStorage::Allocate<T>(std::forward<Args>(args)...);
    return Tag(tagId, storage);
}

#else // if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)

# define BOOST_PP_ITERATION_PARAMS_1  (4, (0, NSFX_MAX_ARITY, <nsfx/network/packet/tag.h>, 1))
# include BOOST_PP_ITERATE()

#endif // !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)


NSFX_CLOSE_NAMESPACE


#endif // TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F


////////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_PP_IS_ITERATING)/*{{{*/

# define NSFX_PP_FORWARD(z, n, d)  std::forward<A ## n>(a ## n)

////////////////////////////////////////
# if BOOST_PP_ITERATION_FLAGS() == 0
// template<class T, class A0, class A1, ...>
// friend Tag MakeTag(size_t tagId, A0&& a0, A1&& a1, ...);
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
friend Tag MakeTag(size_t tagId
    BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
    BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a));

////////////////////////////////////////
# elif BOOST_PP_ITERATION_FLAGS() == 1
// template<class T, class A0, class A1, ...>
// inline Tag MakeTag(size_t tagId, A0&& a0, A1&& a1, ...)
template<class T  BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
                  BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
inline Tag MakeTag(size_t tagId
    BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
    BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, &&a))
{
    TagStorage* storage = TagStorage::Allocate<T>(
        BOOST_PP_ENUM(BOOST_PP_ITERATION(), NSFX_PP_FORWARD, ));
    return Tag(tagId, storage);
}

# endif // BOOST_PP_ITERATION_FLAGS() == x

# undef NSFX_PP_FORWARD

#endif // defined(BOOST_PP_IS_ITERATING) /*}}}*/

