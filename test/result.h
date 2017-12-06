/**
 * @file
 *
 * @brief Test support for network simulation frameworks.
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

#ifndef RESULT_H__09E2495D_E83B_455B_824C_6852A1D1622A
#define RESULT_H__09E2495D_E83B_455B_824C_6852A1D1622A


#include <nsfx/test/config.h>
#include <nsfx/test/tool-type.h>
#include <nsfx/test/tool-level.h>
#include <utility>
#include <sstream>


NSFX_TEST_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 *
 * @brief Result of failed test assertions.
 */
class Result /*{{{*/
{
public:
    template<class Desc, class Actual, class Limit, class Tol>
    Result(ToolType type, ToolLevel level, Desc&& description,
           Actual&& actual, Limit&& limit, Tol&& tolerance,
           const char* func, const char* filename, size_t lineno) :
        type_       (type),
        level_      (level),
        description_(std::forward<Desc>(description)),
        actual_     (std::forward<Actual>(actual)),
        limit_      (std::forward<Limit>(limit)),
        tolerance_  (std::forward<Tol>(tolerance)),
        function_   (func),
        filename_   (filename),
        lineno_     (lineno)
    {
    }

    template<class Desc, class Actual, class Limit>
    Result(ToolType type, ToolLevel level, Desc&& description,
           Actual&& actual, Limit&& limit,
           const char* func, const char* filename, size_t lineno) :
        type_       (type),
        level_      (level),
        description_(std::forward<Desc>(description)),
        actual_     (std::forward<Actual>(actual)),
        limit_      (std::forward<Limit>(limit)),
        function_   (func),
        filename_   (filename),
        lineno_     (lineno)
    {
    }

    template<class Desc, class Actual>
    Result(ToolType type, ToolLevel level, Desc&& description,
           Actual&& actual,
           const char* func, const char* filename, size_t lineno) :
        type_       (type),
        level_      (level),
        description_(std::forward<Desc>(description)),
        actual_     (std::forward<Actual>(actual)),
        function_   (func),
        filename_   (filename),
        lineno_     (lineno)
    {
    }

    // Non-copyable.
private:
    BOOST_DELETED_FUNCTION(Result(const Result&));
    BOOST_DELETED_FUNCTION(Result& operator=(const Result&));

    // Movable.
public:
    Result(Result&& rhs) :
        type_        (rhs.type_),
        level_       (rhs.level_),
        description_ (std::move(rhs.description_)),
        actual_      (std::move(rhs.actual_)),
        limit_       (std::move(rhs.limit_)),
        tolerance_   (std::move(rhs.tolerance_)),
        message_     (std::move(rhs.message_)),
        function_    (std::move(rhs.function_)),
        filename_    (std::move(rhs.filename_)),
        lineno_      (rhs.lineno_)
    {
    }

    Result& operator=(Result&& rhs)
    {
        if (this != &rhs)
        {
            type_        = rhs.type_;
            level_       = rhs.level_;
            description_ = std::move(rhs.description_);
            actual_      = std::move(rhs.actual_);
            limit_       = std::move(rhs.limit_);
            tolerance_   = std::move(rhs.tolerance_);
            message_     = std::move(rhs.message_);
            function_    = std::move(rhs.function_);
            filename_    = std::move(rhs.filename_);
            lineno_      = rhs.lineno_;
        }
        return *this;
    }


    // Methods.
public:
    const ToolType& getToolType(void) const
    {
        return type_;
    }

    const ToolLevel& getToolLevel(void) const
    {
        return level_;
    }

    const std::string& getDescription(void) const
    {
        return description_;
    }

    std::string getDetail(void) const
    {
        std::ostringstream oss;
        switch (type_)
        {
        case ToolType::PREDICATE:
            oss << actual_;
            break;

        case ToolType::EQ:
        case ToolType::NE:
        case ToolType::LT:
        case ToolType::LE:
        case ToolType::GT:
        case ToolType::GE:
            oss << actual_ << " " << type_ << " " << limit_;
            break;

        case ToolType::AC:
        case ToolType::RC:
            oss << "|" << actual_ << " - " << limit_ << "| <= " << tolerance_;
            break;

        default:
            break;
        }
        return oss.str();
    }

    const std::string& getActual(void) const
    {
        return actual_;
    }

    const std::string& getLimit(void) const
    {
        return limit_;
    }

    const std::string& getTolerance(void) const
    {
        return tolerance_;
    }

    void setMessage(std::string&& message)
    {
        message_ = std::move(message);
    }

    const std::string& getMessage(void) const
    {
        return message_;
    }

    const std::string& getFunction(void) const
    {
        return function_;
    }

    const std::string& getFileName(void) const
    {
        return filename_;
    }

    size_t getLineNumber(void) const
    {
        return lineno_;
    }


    // Properties.
private:
    ToolType    type_;
    ToolLevel   level_;
    std::string description_;
    std::string actual_;
    std::string limit_;
    std::string tolerance_;
    std::string message_;
    std::string function_;
    std::string filename_;
    size_t      lineno_;

}; // class Result /*}}}*/


NSFX_TEST_CLOSE_NAMESPACE


#endif // RESULT_H__09E2495D_E83B_455B_824C_6852A1D1622A

