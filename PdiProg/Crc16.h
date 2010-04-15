#ifndef CRC16_H
#define CRC16_H
#include <inttypes.h>

uint16_t Crc16(const uint8_t* message, uint16_t length, uint16_t crc);
uint16_t Crc16_0x8408(uint8_t newchar, uint16_t crcval);

template<class T>
uint16_t Crc16(const T &val, uint16_t crc)
{
	return Crc16((const uint8_t*)&val, sizeof(T), crc);
}

#endif
