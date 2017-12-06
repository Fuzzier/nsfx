/**
 * @file
 *
 * @brief Testing support for network simulation frameworks.
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

#ifndef TOOL_TYPE_H__D71D998A_95DB_4354_80D8_560904B9071A
#define TOOL_TYPE_H__D71D998A_95DB_4354_80D8_560904B9071A


#include <nsfx/test/config.h>
#include <ostream>


NSFX_TEST_OPEN_NAMESPACE /*{{{*/


////////////////////////////////////////////////////////////////////////////////
class ToolType /*{{{*/
{
public:
    enum Value
    {
        PREDICATE = 0, /** IS TRUE */
        EQ,            /** EQUAL */
        NE,            /** NOT EQUAL */
        LT,            /** LESS THAN */
        LE,            /** LESS EQUAL */
        GT,            /** GREATER THAN */
        GE,            /** GREATER EQUAL */
        AC,            /** ABSOLUTE CLOSENESS */
        RC,            /** RELATIVE CLOSENESS */
        LAST,
    };

    ToolType(Value type = LAST) : type_(type) {}
    explicit ToolType(size_t type) : type_(static_cast<Value>(type)) {}

    operator Value() const { return type_; }

    size_t ToInteger(void) const { return type_; }
    const char* ToString(void) const/*{{{*/
    {
        size_t index = ToInteger();
        static const char* types[] =
        {
            "!!",
            "==",
            "!=",
            "<",
            "<=",
            ">",
            ">=",
            "~#",
            "~%",
            "?",
        };
        if (index >= LAST)
        {
            index = LAST;
        }
        const char* str = types[index];
        return str;
    }/*}}}*/

private:
    Value type_;
}; // class ToolType /*}}}*/


template<class Char, class CharTraits>
inline std::basic_ostream<Char, CharTraits>&
operator<<(std::basic_ostream<Char, CharTraits>& os, const ToolType& type)/*{{{*/
{
    return os << type.ToString();
}/*}}}*/


NSFX_TEST_CLOSE_NAMESPACE /*}}}*/


#endif // TOOL_TYPE_H__D71D998A_95DB_4354_80D8_560904B9071A

