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
#include <nsfx/network/buffer/tag-buffer-storage.h>
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
public:
    /**
     * @brief Create a tag.
     *
     * @param[in] tagId   The id of the tag.
     * @param[in] storage The storage of the tag.
     */
    Tag(size_t tagId, TagBufferStorage* storage) BOOST_NOEXCEPT;

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
     * @brief Get the buffer of the tag.
     */
    template<class T>
    TagBuffer GetBuffer(void) const BOOST_NOEXCEPT;

private:
    /**
     * @brief Make a tag index.
     */
    TagIndex MakeTagIndex(size_t tagStart, size_t tagEnd) const BOOST_NOEXCEPT;

private:
    size_t id_;
    TagBuffer buffer_;

    // Required to access the constructor and MakeTagIndex().
    friend class TagList;
};


////////////////////////////////////////////////////////////////////////////////
inline Tag::Tag(size_t tagId, TagBufferStorage* storage) BOOST_NOEXCEPT :
    id_(tagId),
    storage_(storage)
{
}

inline Tag::~Tag(void)
{
    TagBufferStorage::Release(storage_);
}

inline Tag::Tag(const Tag& rhs) BOOST_NOEXCEPT :
    id_(rhs.id_),
    storage_(rhs.storage_)
{
    TagBufferStorage::AddRef(storage_);
}

inline Tag& Tag::operator=(const Tag& rhs)
{
    if (this != &rhs)
    {
        TagBufferStorage* tmp = storage_;
        id_ = rhs.id_;
        storage_ = rhs.storage_;
        TagBufferStorage::AddRef(storage_);
        TagBufferStorage::Release(tmp);
    }
    return *this;
}

inline size_t Tag::GetId(void) const BOOST_NOEXCEPT
{
    return id_;
}


NSFX_CLOSE_NAMESPACE


#endif // TAG_H__00CF596A_BA0E_4E34_8C66_F41A47718C3F

