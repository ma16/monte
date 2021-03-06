// BSD 2-Clause License, see github.com/ma16/monte

// --------------------------------------------------------------------
// BCM2835 ARM Peripherals: chapter 14: Free Running ARM Counter
// --------------------------------------------------------------------

#ifndef _Rpi_Counter_h_
#define _Rpi_Counter_h_

#include <Neat/cast.h>
#include "Peripheral.h"

namespace Rpi
{
  struct Counter
  {
    bool enabled() const
    {
      return 0 != (page->at<0x408/4>() & (1u<<9)) ;
    }
    
    void enable(bool on)
    {
      auto w = page->at<0x408/4>() ;
      uint32_t i = 1 ; i <<= 9 ;
      w &= ~i ; // := 0
      i = on ; i <<= 9 ;
      w |=  i ; // := {on}
      page->at<0x408/4>() = w ;
    }

    using Div = uint8_t ; 

    Div prescaler() const
    {
      return static_cast<Div>(page->at<0x408/4>() >> 16) ;
    }
    
    void prescale(Div div)
    {
      auto w = page->at<0x408/4>() ;
      uint32_t i = 0xff ; i <<= 16 ;
      w &= ~i ; // := 0
      i = div ; i <<= 16 ;
      w |=  i ; // := {div}
      page->at<0x408/4>() = w ;
    }
    
    uint32_t clock() const
    {
      return page->at<0x420/4>() ;
    }

    double frequency() const ;
    
    Counter(Peripheral *p) : page(p->page(Peripheral::PNo::make<0x00b>())) {}
  
  private:

    std::shared_ptr<Page> page ; 
  } ;
}

// [todo]
// introduce chrono::clock-like implementation for the two Pi clocks
// (i.e. for the BCM timer and the ARM counter). ponder the effects of
// clock resolution (accuracy), granularity and cost-per-call. at least
// introduce wrapper classes for time-point and duration instaed of
// using ambiguous uint32_t types.

#endif // _Rpi_Counter_h_
