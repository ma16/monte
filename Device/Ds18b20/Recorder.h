// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_Device_Ds18b20_Recorder_h
#define INCLUDE_Device_Ds18b20_Recorder_h

// This is a wrapper around a bit-banged DS18B20 implementation. It
// implements only two functions: convert-T and read scratch-pad. Both
// functions are issued in broadcast mode (i.e. after a skip-ROM-
// command on a single-drop system) so only one DS18B20 may be attached
// to the bus-pin on the wire.
//
// The module has only one method to invoke: sample().
//
// The method reads the scratch-pad and triggers thereafter a
// temperature conversion. So, when the method is called the first
// time (after the DS18B20 was powered-up) it will return the DS18B20
// default temperature (i.e. 85°C).
//
// Access to the 1-wire bus may be accompanied by a tranmission error.
// Besides, there may occur timing problems since this is a bit-banged
// implementation on a non-real-time operating system (Raspbian). At
// least the implementation tries to detect such situations and may
// retry the failed operation. Still, if it doesn't work out, the
// method will return boost::none.

#include "Bang.h"
#include <boost/optional.hpp>

namespace Device { namespace Ds18b20 { 

struct Recorder 
{
    static std::unique_ptr<Recorder> make(Rpi::Peripheral*,Rpi::Pin) ;

    boost::optional<double> sample() ;
    
private:

    using Master = Bang::Master ;
    
    std::unique_ptr<Master> master ;

    Recorder(std::unique_ptr<Master> master) : master(std::move(master)) {}

    void convert() ;

    boost::optional<Bang::Pad> readPad() ;

} ; } }

#endif // INCLUDE_Device_Ds18b20_Recorder_h
