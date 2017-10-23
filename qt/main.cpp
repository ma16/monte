// BSD 2-Clause License, see github.com/ma16/monte

#include "MainWindow.h"
#include <Neat/Logger.h>
#include <Posix/base.h>
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 2 && 0 == strcmp(argv[1],"-v"))
    {
	std::cout
	    << "Qt version: " << qVersion() << '\n'
	    << "Build time: " << __DATE__ << ' ' << __TIME__ << '\n'
	    << "Release: 1 (beta)\n" ;
	return 0 ;
    }
    
    Neat::Logger::seize()->post("Monte has been started") ;

    // "open" peripheral and fall back to real uid
    auto base = Rpi::Peripheral::by_cpuinfo() ;
    auto rpi = Rpi::Peripheral::make(base) ;
    Posix::reset_uid() ;
    // otherwise we need: QApplication::setSetuidAllowed(true)

    QApplication app(argc,argv) ;
    MainWindow w ;
    w.init(rpi.get()) ;
    w.show() ;

    Neat::Logger::seize()->post("Call Qt main loop") ;
    return app.exec() ;
}
