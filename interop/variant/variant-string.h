/**
 * @file
 *
 * @brief Variant support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-24
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef VARIANT_STRING_H__9DD6A503_B8EB_4BCE_90B6_7CD421F10B4C
#define VARIANT_STRING_H__9DD6A503_B8EB_4BCE_90B6_7CD421F10B4C


#include <nsfx/interop/variant/config.h>
#include <cstring> // strlen, memcpy, memcmp


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Variant
 * @brief Allocate a string for variant.
 *
 * @param[in] size  The size of the string,
 *                  <b>including</b> the terminating \c 0.
 *
 * The returned string <b>must</b> be deallocated by \c VariantStringFree().
 */
inline char* VariantStringAllocate(size_t size)
{
    char* result = nullptr;
    if (size > 0)
    {
        result = new char[size];
    }
    return result;
}


/**
 * @ingroup Variant
 * @brief Deallocate a string that was allocated by \c VariantStringAllocate().
 *
 * @param[in] str  The string to deallocate.
 */
inline void VariantStringFree(const char* str) BOOST_NOEXCEPT
{
    if (str)
    {
        delete[] str;
    }
}

/**
 * @ingroup Variant
 * @brief Copy a string.
 *
 * @param[in] str  The string to copy.
 *                 It can be \c nullptr.
 *
 * The returned string <b>must</b> be deallocated by \c VariantStringFree().
 */
inline const char* VariantStringCopy(const char* str)
{
    char* copy = nullptr;
    size_t len = str ? strlen(str) : 0;
    if (len)
    {
        copy = VariantStringAllocate(len + 1);
        std::memcpy(copy, str, len + 1);
    }
    return copy;
}


/**
 * @ingroup Variant
 * @brief Compare two strings.
 *
 * @param[in] s1  A string. It can be \c nullptr.
 * @param[in] s2  A string. It can be \c nullptr.
 *
 * @return \c true if the two strings are the same.
 */
inline bool VariantStringCompare(const char* s1, const char* s2) BOOST_NOEXCEPT
{
    size_t len1 = s1 ? strlen(s1) : 0;
    size_t len2 = s1 ? strlen(s1) : 0;
    return (len1 == len2) && !std::memcmp(s1, s2, len1);
}


NSFX_CLOSE_NAMESPACE


#endif // VARIANT_STRING_H__9DD6A503_B8EB_4BCE_90B6_7CD421F10B4C

