// BSD 2-Clause License, see github.com/ma16/monte

#include "Logger.h"

Neat::Logger* Neat::Logger::seize()
{
    static Logger logger ;
    return &logger ;
}

void Neat::Logger::post(std::string const &message)
{
    if (this->terminal == nullptr)
    {
	this->buffer.push_back(message) ;
    }
    else
    {
	this->terminal->dispatch(message) ;
    }
}

void Neat::Logger::connect(Terminal *terminal)
{
    if (this->terminal == nullptr && terminal != nullptr)
    {
	while (!this->buffer.empty())
	{
	    auto const &message = this->buffer.front() ;
	    terminal->dispatch(message) ;
	    this->buffer.pop_front() ;
	}
    }
    this->terminal = terminal ;
}
