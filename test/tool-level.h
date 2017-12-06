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

#ifndef TOOL_LEVEL_H__0DDE0C58_9DFD_42D0_B318_A8EB05C57982
#define TOOL_LEVEL_H__0DDE0C58_9DFD_42D0_B318_A8EB05C57982


#include <nsfx/test/config.h>
#include <ostream>


NSFX_TEST_OPEN_NAMESPACE /*{{{*/


////////////////////////////////////////////////////////////////////////////////
class ToolLevel /*{{{*/
{
public:
    enum Value
    {
        EXPECT = 0, /** Non-critical error. */
        ASSERT,     /** Critical error. */
        LAST,
    };

    ToolLevel(Value level = LAST) : level_(level) {}
    explicit ToolLevel(size_t level) : level_(static_cast<Value>(level)) {}

    operator Value() const { return level_; }

    size_t ToInteger(void) const { return level_; }
    const char* ToString(void) const/*{{{*/
    {
        size_t index = ToInteger();
        static const char* levels[] =
        {
            "error",
            "fatal error",
            "?",
        };
        if (index >= LAST)
        {
            index = LAST;
        }
        const char* str = levels[index];
        return str;
    }/*}}}*/

private:
    Value level_;
}; // class ToolLevel /*}}}*/


template<class Char, class CharTraits>
inline std::basic_ostream<Char, CharTraits>&
operator<<(std::basic_ostream<Char, CharTraits>& os, const ToolLevel& level)/*{{{*/
{
    return os << level.ToString();
}/*}}}*/


NSFX_TEST_CLOSE_NAMESPACE /*}}}*/


#endif // TOOL_LEVEL_H__C03D486B_021E_4F81_AE60_E0954C790EB0

