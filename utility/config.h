/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-03-14
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__90EA23F9_092A_4E77_9D6A_5C3030AD361B
#define CONFIG_H__90EA23F9_092A_4E77_9D6A_5C3030AD361B


#include <nsfx/config.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Utility
 *
 */


/**
 * @page tags Tags and their notations
 *
 * # Function overloading
 *
 * C++ supports function overloading.
 * That is, several functions can share the same name, and perform the same task
 * under different conditions.
 *
 * For example, `Write()` function is to write formatted data into an output file.
 * However, it shall be able to deal with different types of data.
 *
 * On method is to specify the data type as a runtime variable, and pass data as
 * `void*`, e.g., `Write(const void* data, size_t size, int type)`.
 * This method requires runtime validation of the data against the type.
 * To support more data types, the function have to be modified intrusively.
 *
 * Another method is function overloading.
 * Users can define several overloaded functions that deals with different types.
 * e.g., `Write(uint8_t)`, `Write(double)`, `Write(const char*)`, ...
 * This method requires compile time selection of an overloaded function.
 * To support more data types, more overloaded functions can be defined.
 *
 * # Tags
 *
 * ## What are tags?
 *
 * *Tags* are types that are used to choose an overloaded function at compile time.
 *
 * A tag is most commonly defined as a *trivial structure*.
 * For example,
 * @code{.cpp}
 * // Define tags.
 * struct foo_t {};
 * struct bar_t {};
 *
 * // Define overloaded functions.
 * void Quz(foo_t);
 * void Quz(bar_t);
 *
 * // Use tag to choose overloaded functions.
 * Quz(foo_t()); // Invoke foo version by creating a instance of foo_t.
 * Quz(bar_t()); // Invoke bar version by creating a instance of bar_t.
 * @endcode
 *
 * A tag can also be defined as an `enumeration` with *only one* value.
 * The value can be used to choose the overloaded function, since it carries
 * the type.
 * For example,
 * @code{.cpp}
 * // Define tags.
 * enum round_downward_t
 * { round_downward };
 * enum round_to_nearest_t
 * { round_to_nearest };
 *
 * // Define overloaded functions.
 * void Quz(round_downward_t);
 * void Quz(round_to_nearest_t);
 *
 * // Use tag to choose overloaded functions.
 * Quz(round_downward);   // The type of the value chooses the correct function.
 * Quz(round_to_nearest);
 * @endcode
 *
 * ## Notations of tags
 *
 * In C++, classes usually have data members and public functions to fulfill
 * certain tasks.
 * However, a tag is **not** a common class, and its name **does not** adopt *CamelCase*.
 * Instead, the name of a tag adopts *snake_case*.
 *
 * ### Structures
 *
 * For structures, the names usually ends with "`_t`".
 *
 * ### Enumerations
 *
 * For enumerations, the names usually ends with "`_t`".
 * The name of the single value is simple the name without "`_t`".
 *
 * ## Notations of meta-programming entities
 *
 * Similarly, for *type traits* and other meta-programming entities, *snake_case* is
 * also adopted.
 *
 * ### Type traits
 *
 * For type traits, the names usually ends with "`_traits`".
 *
 * ### Meta-functions
 *
 * For meta-functions, the names usually ends with "`_f`".
 *
 * ## How to use tags?
 *
 * The general rules are as follows.
 * * Overload the functions under the same name, but with different tags.
 * * Define meta-functions to generate the tags.
 * * The caller generates tags, and invoke the correct overloaded function.
 *
 * For example, a value is stored in memory in *native-endian* order.
 * To write the value to a file in *native-endian*, *big-endian* or *little-endian*
 * order, the functions can be defined as follows:
 * * `Write()`
 * * `WriteB()`
 * * `WriteL()`
 *
 * However, there are only two cases.
 * * The input and output have the same endianness, and the data is output as is.
 * * The input and output have different endianness, and the data is output in
 *   reversed byte order.
 *
 * Thus, the underlying code only has to provide two underlying functions.
 * * `WriteS()`: write with the same endian order.
 * * `WriteR()`: write with reversed endian order.
 *
 * Usually one has to check the native endianness at runtime, or define macros
 * and use `#if` to invoke the correct underlying function.
 * However, both methods have their cons and pros.
 *
 * In C++, if the compiler has the ability to detect the endianness, or anything
 * of interest, *template* can be used to invoke the correct underlying function
 * at compile time.
 *
 * To do so, the underlying functions need to be overloaded under the same name.
 * * `Write(same_byte_order_t)`: write with native endian order.
 * * `Write(reverse_byte_order_t)`: write with reversed endian order.
 *
 * Then, the caller function needs to *generate a tag*.
 * This usually involves *meta-functions*.
 *
 * For example, in function `WriteB()`, if `endian_traits<big_endian_t>::is_native`
 * is `false`, then the function shall call the underlying `WriteR()` to reverse
 * the byte order.
 * Otherwise, the function shall call the underlying `WriteS()` to output data
 * as is.
 *
 * A meta-function can be defined to generate the tag.
 * For example,
 * @code{.cpp}
 * template<bool same_order = true>
 * struct byte_order_f
 * {
 *     typedef same_byte_order_t type;
 * };
 *
 * template<>
 * struct byte_order_f<false>
 * {
 *     typedef reverse_byte_order_t type;
 * };
 * @endcode
 *
 * Then, `WriteB()` can be writen as follows.
 * @code{.cpp}
 * void WriteB(uint8_t data)
 * {
 *     typedef byte_order_f<endian_traits<big_endian_t>::is_native>::type  tag;
 *     Write(data, tag());
 * }
 * @endcode
 */


#endif // CONFIG_H__90EA23F9_092A_4E77_9D6A_5C3030AD361B

