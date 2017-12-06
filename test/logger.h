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

#ifndef LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503
#define LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503


#include <nsfx/test/config.h>
#include <nsfx/test/visitor-concept.h>
#include <ostream>
#include <fstream>
#include <vector>
#include <memory>


NSFX_TEST_OPEN_NAMESPACE /*{{{*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Log sink.
 */
class StreamSink /*{{{*/
{
public:
    StreamSink(std::ostream& os) :/*{{{*/
        os_(os)
    {
    }/*}}}*/

public:
    virtual ~StreamSink(void) {}

    // Methods./*{{{*/
public:
    std::ostream& GetStream(void) const /*{{{*/
    {
        return os_;
    }/*}}}*/

    /*}}}*/

    // Properties./*{{{*/
private:
    std::ostream& os_;

    /*}}}*/
}; // class LogSink /*}}}*/


////////////////////////////////////////////////////////////////////////////////
class FileSink : public StreamSink /*{{{*/
{
public:
    FileSink(const std::string& filename, std::ios_base::openmode mode = std::ios_base::out) :/*{{{*/
        StreamSink(ofs_),
        ofs_(filename, mode)
    {
    }/*}}}*/

    virtual ~FileSink(void) {}

    // Properties./*{{{*/
private:
    std::ofstream ofs_;
    /*}}}*/
}; // class StreamSink /*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Testing
 *
 * @brief Output formatted test results to log sinks.
 */
class Logger /*{{{*/
{
public:
    typedef std::vector<std::unique_ptr<StreamSink> > SinkContainerType;

    // Methods./*{{{*/
public:
    void AddStreamSink(std::ostream& os)/*{{{*/
    {
        std::unique_ptr<StreamSink> sink(new StreamSink(os));
        sinks_.push_back(std::move(sink));
    }/*}}}*/

    void AddFileSink(const std::string& filename, std::ios_base::openmode mode = std::ios_base::out)/*{{{*/
    {
        std::unique_ptr<StreamSink> sink(new FileSink(filename, mode));
        sinks_.push_back(std::move(sink));
    }/*}}}*/

    template<class StreamVisitor>
    void VisitStreams(StreamVisitor&& visitor) /*{{{*/
    {
        BOOST_CONCEPT_ASSERT((VisitorConcept<StreamVisitor, std::ostream&>));
        for (auto it = sinks_.cbegin(); it != sinks_.cend(); ++it)
        {
            const std::unique_ptr<StreamSink>& sink = *it;
            visitor(sink->GetStream());
        }
    }/*}}}*/

    /*}}}*/

    // Properties./*{{{*/
private:
    SinkContainerType sinks_;

    /*}}}*/

}; // class Logger /*}}}*/


NSFX_TEST_CLOSE_NAMESPACE /*}}}*/


#endif // LOGGER_H__325C0CE8_FB30_418E_B823_6365EBDF8503

