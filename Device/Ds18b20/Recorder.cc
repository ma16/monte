// BSD 2-Clause License, see github.com/ma16/monte

#include "Recorder.h"
#include <Neat/Logger.h>

#include <cstring> // memcpy

#include <Protocol/OneWire/Bang/crc.h>
#include <Protocol/OneWire/Bang/Error.h>
#include <Protocol/OneWire/Bang/Signaling.h>

template<size_t N>
static std::array<uint8_t,(N+7)/8> toByteA(std::bitset<N> const &set)
{
    std::array<uint8_t,(N+7)/8> byteA ;
    for (size_t i=0 ; 8*i<N ; ++i)
    {
	uint8_t byte = 0 ;
	for (size_t j=0 ; 8*i+j<N ; ++j)
	    byte = static_cast<uint8_t>(byte | (set[8*i+j] << j)) ;
	byteA[i] = byte ;
    }
    return byteA ;
}

template<size_t N>
static std::string toHexStr(std::array<uint8_t,N> const &byteA)
{
    std::ostringstream os ;
    os << std::hex ;
    auto i = byteA.cbegin() ;
    if (i != byteA.cend())
    {
	os << (*i)/16 << (*i)%16 ;
	while (++i != byteA.cend())
	    os << ':' << (*i)/16 << (*i)%16 ;
    }
    return os.str() ;
}    

using Host = Device::Ds18b20::Bang ;

using Error = Protocol::OneWire::Bang::Error ;

using Recorder = Device::Ds18b20::Recorder ;

using Timing = Protocol::OneWire::Bang::Timing ;

std::unique_ptr<Recorder> Recorder::make(Rpi::Peripheral *rpi,Rpi::Pin pin) 
{
    Rpi::Gpio gpio(rpi) ;

    gpio.setMode(pin,Rpi::Gpio::Mode::In) ;

    gpio.setOutput(pin,Rpi::Gpio::Output::Lo) ;
  
    auto frequency = Rpi::Counter(rpi).frequency() ;

    auto timing = Timing::xlat(frequency) ;
    
    auto master = new Master(rpi,pin,timing) ;

    auto recorder = new Recorder(std::unique_ptr<Master>(master)) ;

    return std::unique_ptr<Recorder>(recorder) ;
}

boost::optional<double> Recorder::sample()
{
    auto pad = this->readPad() ; 
    this->convert() ;
    // [todo]
    // * this might have failed. so the next call will return an old
    //   value w/o any chance for the caller to be aware of this.
    // * the read + convert sequence is sub-optimal. If you want to
    //   measure the temperature every 10 minutes, you would get
    //   a 10 minutes old value. For now, we leave this as it is, since
    //   it is ok if you want to sample every second.

    if (!pad)
	return boost::none ;

    auto byteA = toByteA(*pad) ;
    int16_t fractions ;
    memcpy(&fractions,&byteA[0],2) ;
    auto temp = 1.0 / 16 * fractions ;
    std::ostringstream os ;
    os << "Recorded temperature:" << temp << "°C" ;
    Neat::Logger::seize()->post(os.str()) ;
    
    return temp ; 
}

void Recorder::convert()
{
    auto retryN = 10 ;
    auto notPresentN = 1 ;
  Retry:
    try
    {
	Host(this->master.get()).convert(boost::none) ;
    }
    catch (Error &error)
    {
	Neat::Logger::seize()->post(std::string("convert:") + error.what()) ;
	switch (error.type())
	{
	case Error::Type::Retry:
	    if (0 != retryN--) goto Retry ;
	    break ;
	case Error::Type::NotPresent: 
	    if (0 != notPresentN--) goto Retry ;
	    break ;
	default: break ;
	}
	Neat::Logger::seize()->post("no temperature conversion initiated") ;
    }
}

boost::optional<Host::Pad> Recorder::readPad()
{
    auto retryN = 10 ;
    auto notPresentN = 1 ;
    auto crcN = 1 ;
  Retry:
    try
    {
	auto pad = Host(this->master.get()).readPad(boost::none) ;
	auto crc = Protocol::OneWire::Bang::crc(pad) ;
	if (crc == 0)
	    return pad ;
	auto byteA = toByteA(pad) ;
	Neat::Logger::seize()->post("readPad:CRC error:" + toHexStr(byteA)) ;
	if (0 != crcN--) goto Retry ;
    }
    catch (Error &error)
    {
	Neat::Logger::seize()->post(std::string("readPad:") + error.what()) ;
	switch (error.type())
	{
	case Error::Type::Retry:
	    if (0 != retryN--) goto Retry ;
	    break ;
	case Error::Type::NotPresent: 
	    if (0 != notPresentN--) goto Retry ;
	    break ;
	default: break ;
	}
    }
    Neat::Logger::seize()->post("temperature readout failed") ;
    return boost::none ;
}
