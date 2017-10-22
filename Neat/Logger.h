// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_Neat_Logger_h
#define INCLUDE_Neat_Logger_h

#include <list>
#include <string>

namespace Neat {
    
struct Logger
{
    static Logger* seize() ; 

    void post(std::string const&) ;

    struct Terminal
    {
	virtual void dispatch(std::string const&) = 0 ;
	virtual ~Terminal() {}
    } ;
    
    void connect(Terminal*) ;

    // [todo] print messages to stdout if buffer not empty when destructed
  
private:

    std::list<std::string> buffer ;

    Terminal *terminal = nullptr ;
    
} ; }

#endif // INCLUDE_Neat_Logger_h
