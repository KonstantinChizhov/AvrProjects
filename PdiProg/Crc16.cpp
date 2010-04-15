#include "Crc16.h"

inline uint16_t CrcTable0x8408(uint16_t v)
{
    for (uint8_t i = 8; i--; )
        v = v & 1 ? (v >> 1) ^ 0x8408 : v >> 1;
    return v;
}

uint16_t Crc16_0x8408(uint8_t newchar, uint16_t crcval)
{
    return (crcval >> 8) ^ CrcTable0x8408((crcval ^ newchar) & 0x00ff);
}

uint16_t Crc16(const uint8_t* message, uint16_t length, uint16_t crc)
{
    for(uint16_t i = 0; i < length; i++)
    {
       crc = Crc16_0x8408(message[i], crc);
    }
    return crc;
}
