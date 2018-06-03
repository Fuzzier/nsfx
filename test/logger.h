/**
 * @file
 *
 * @brief Testing support for network simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503
#define LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503


#include <nsfx/test/config.h>
#include <nsfx/test/visitor-concept.h>
#include <ostream>
#include <fstream>
#include <memory>


NSFX_TEST_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 *
 * @brief Log sink that consumes test results.
 */
class StreamSink /*{{{*/
{
public:
    StreamSink(std::ostream& os) :
        os_(os)
    {
    }

    BOOST_DELETED_FUNCTION(StreamSink(const StreamSink&));
    BOOST_DELETED_FUNCTION(StreamSink& operator=(const StreamSink&));


public:
    virtual ~StreamSink(void) BOOST_NOEXCEPT_OR_NOTHROW {}


    // Methods.
public:
    std::ostream& GetStream(void) const
    {
        return os_;
    }


    // Properties.
private:
    std::ostream& os_;

}; // class StreamSink /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 *
 * @brief File-based log sink that consumes test results.
 */
class FileSink : public StreamSink /*{{{*/
{
public:
    FileSink(const std::string& filename, std::ios_base::openmode mode = std::ios_base::out) :
        StreamSink(ofs_),
        ofs_(filename, mode)
    {
    }

    virtual ~FileSink(void) {}

    BOOST_DELETED_FUNCTION(FileSink(const FileSink&));
    BOOST_DELETED_FUNCTION(FileSink& operator=(const FileSink&));


    // Methods.
public:
    std::ostream& GetStream(void)
    {
        return *static_cast<std::ostream*>(&ofs_);
    }


    // Properties.
private:
    std::ofstream ofs_;

}; // class FileSink /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Test
 *
 * @brief A collection of log sinks that allows visitors to visit each log sink.
 */
class Logger /*{{{*/
{
public:
    typedef std::unique_ptr<StreamSink>  StreamSinkPtr;
    typedef vector<StreamSinkPtr>  SinkContainerType;


    // Methods.
public:
    void AddStreamSink(std::ostream& os)
    {
        sinks_.emplace_back(new StreamSink(os));
    }

    void AddFileSink(const std::string& filename, std::ios_base::openmode mode = std::ios_base::out)
    {
        sinks_.emplace_back(new FileSink(filename, mode));
    }

    template<class StreamVisitor>
    void VisitStreams(StreamVisitor&& visitor)
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<StreamVisitor, std::ostream&>));
        for (auto it = sinks_.cbegin(); it != sinks_.cend(); ++it)
        {
            const StreamSinkPtr& sink = *it;
            visitor(sink->GetStream());
        }
    }


    // Properties.
private:
    SinkContainerType sinks_;

}; // class Logger /*}}}*/


NSFX_TEST_CLOSE_NAMESPACE


#endif // LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503

