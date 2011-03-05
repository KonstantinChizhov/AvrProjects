#pragma once
#include <stdint.h>

#ifdef __AVR__
enum
{	
	PlatformCyslesPerDelayLoop32 = 6,	
	PlatformCyslesPerDelayLoop16 = 4,	
	PlatformCyslesPerDelayLoop8 = 3
};

inline void PlatformDelayCycle32(uint32_t delayLoops)
{	
	union
	{
		struct
		{
			uint16_t word0;
			uint8_t byte2;
			uint8_t byte3;
		} value;
		uint32_t dword;	
	};	
	dword = delayLoops;	
	__asm volatile (		
		"1: sbiw %0,1" "\n\t"		
		"sbc %1,r1" "\n\t"		
		"sbc %2,r1" "\n\t"		
		"brne 1b"		
		: "=w" (value.word0), "=r" (value.byte2), "=r" (value.byte3)		
		: "0" (delayLoops)	
	);
}

inline void PlatformDelayCycle16(uint16_t delayLoops)
{	
	__asm volatile (
		"1: sbiw %0,1" "\n\t"
		"brne 1b"
		: "=w" (delayLoops)
		: "0" (delayLoops)
	);
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	__asm volatile (
		"1: dec %0" "\n\t"
		"brne 1b"
		: "=r" (delayLoops)
		: "0" (delayLoops)	
	);
}
#elif defined(__ICCAVR__)

#include <intrinsics.h>
enum
{	
	PlatformCyslesPerDelayLoop32 = 1,	
	PlatformCyslesPerDelayLoop16 = 1,	
	PlatformCyslesPerDelayLoop8 = 1
};

inline void PlatformDelayCycle32(uint32_t delayLoops)
{	
	__delay_cycles(delayLoops); 
}

inline void PlatformDelayCycle16(uint16_t delayLoops)
{	
	__delay_cycles(delayLoops); 
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	__delay_cycles(delayLoops); 
}
#endif