// BSD 2-Clause License, see github.com/ma16/monte

#ifndef INCLUDE_Protocol_OneWire_Bang_crc_h
#define INCLUDE_Protocol_OneWire_Bang_crc_h

#include <bitset>

namespace Protocol { namespace OneWire { namespace Bang { 

template<size_t N> static inline uint8_t crc(std::bitset<N> const &set)
{
    unsigned reg = 0 ;
    for (auto i=0u ; i<set.size() ; ++i)
    {
	auto lsb = 0 != (reg & 0x01) ;
	reg >>= 1 ;
	if (set[i] != lsb)
	    reg ^= 0x8c ;
    }
    return static_cast<uint8_t>(reg) ;
}
    
} } }

#endif // INCLUDE_Protocol_OneWire_Bang_crc_h
