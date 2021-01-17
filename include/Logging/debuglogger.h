#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include "basic_debuglog_stream.h"

#include <iostream>
#include <fstream>

template<class charT>
class log_to_cerr : public basic_log_function<charT>
{
public:
	typename basic_log_function<charT>::result_type operator()(typename basic_log_function<charT>::first_argument_type context, typename basic_log_function<charT>::second_argument_type output)
	{
		std::cerr << context << output;
	}
};

template<class charT>
class log_to_file : public basic_log_function<charT>
{
public:
    typename basic_log_function<charT>::result_type operator()(typename basic_log_function<charT>::second_argument_type context, typename basic_log_function<charT>::second_argument_type output)
    {    
		std::basic_ofstream<charT> fs(GetLogfilename().c_str(), std::ios_base::app);
        if (!fs)
            throw std::invalid_argument(("Cannot open filestream for " + GetLogfilename()).c_str());
        else
            fs << context << output;
    }
private:
    const std::string GetLogfilename()
    { 
        return std::string("d:\\temp\\debug.log");
    }
};

template<class charT>
class MonoStateFunctor 
{
public:
    void operator()(const charT * const context, 
                    const charT * const message)
    {
        std::basic_ofstream<charT> fs(filename_.c_str(),
                                      std::ios_base::app);
        if (!fs)
            throw std::invalid_argument(("cannot open filestream for " + filename_).c_str());
        else
            fs << context << message;		
    }
	
    void setFilename(const std::string &filename)
    {
        filename_ = filename;
    }
    const std::string getFilename() const
    {
        return filename_;
    }
private:
    static std::string filename_;		
};

typedef MonoStateFunctor<char> log_to_file_ex;
typedef basic_debuglog_stream<char, log_to_file_ex> FileDebugLoggerEx;

typedef basic_debuglog_stream<char, log_to_file<char> > FileDebugLogger;
typedef basic_debuglog_stream<char, log_to_cerr<char> > ErrDebugLogger;

//
// defines for quick access to the debug logger 
// 
// RAWLOG    ... output debug messages without any prefix
// CTXRAWLOG ... output debug messages with the context string prefixed
// CTXLOG    ... output debug messages with filename, linenumber and context string prefixed
// LOG       ... output debug messages with filename and linenumber prefixed
//
//#define RAWLOG() DebugLogger().get()
//#define CTXRAWLOG(text) DebugLogger(text).get()
//#define CTXLOG(text) DebugLogger(text, __FILE__, __LINE__).get()

#endif 
