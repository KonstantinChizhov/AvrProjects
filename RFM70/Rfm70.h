#pragma once 

#include <iopins.h>

enum Command
{
	READ_REG        	= 0x00,  // Define read command to register
	WRITE_REG       	= 0x20,  // Define write command to register
	RD_RX_PLOAD     	= 0x61,  // Define RX payload register address
	WR_TX_PLOAD     	= 0xA0,  // Define TX payload register address
	FLUSH_TX        	= 0xE1,  // Define flush TX register command
	FLUSH_RX        	= 0xE2,  // Define flush RX register command
	REUSE_TX_PL     	= 0xE3,  // Define reuse TX payload register command
	W_TX_PAYLOAD_NOACK	= 0xb0,
	W_ACK_PAYLOAD		= 0xa8,
	ACTIVATE			= 0x50,
	R_RX_PL_WID			= 0x60,
	NOP_NOP            	= 0xFF 
};

enum Registers
{
	CONFIG          = 0x00,  // 'Config' register address
	EN_AA           = 0x01,  // 'Enable Auto Acknowledgment' register address
	EN_RXADDR       = 0x02,  // 'Enabled RX addresses' register address
	SETUP_AW        = 0x03,  // 'Setup address width' register address
	SETUP_RETR      = 0x04,  // 'Setup Auto. Retrans' register address
	RF_CH           = 0x05,  // 'RF channel' register address
	RF_SETUP        = 0x06,  // 'RF setup' register address
	STATUS          = 0x07,  // 'Status' register address
	OBSERVE_TX      = 0x08,  // 'Observe TX' register address
	CD              = 0x09,  // 'Carrier Detect' register address
	RX_ADDR_P0      = 0x0A,  // 'RX address pipe0' register address
	RX_ADDR_P1      = 0x0B,  // 'RX address pipe1' register address
	RX_ADDR_P2      = 0x0C,  // 'RX address pipe2' register address
	RX_ADDR_P3      = 0x0D,  // 'RX address pipe3' register address
	RX_ADDR_P4      = 0x0E,  // 'RX address pipe4' register address
	RX_ADDR_P5      = 0x0F,  // 'RX address pipe5' register address
	TX_ADDR         = 0x10,  // 'TX address' register address
	RX_PW_P0        = 0x11,  // 'RX payload width, pipe0' register address
	RX_PW_P1        = 0x12,  // 'RX payload width, pipe1' register address
	RX_PW_P2        = 0x13,  // 'RX payload width, pipe2' register address
	RX_PW_P3        = 0x14,  // 'RX payload width, pipe3' register address
	RX_PW_P4        = 0x15,  // 'RX payload width, pipe4' register address
	RX_PW_P5        = 0x16,  // 'RX payload width, pipe5' register address
	FIFO_STATUS     = 0x17,  // 'FIFO Status Register' register address
	PAYLOAD_WIDTH   = 0x1f   // 'payload length of 256 bytes modes register address
};

enum IntStatus
{
	RX_DR 	= 0x40,
	TX_DS 	= 0x20,
	MAX_RT 	= 0x10
};

enum{ TX_FULL 	= 0x01};

enum FifoStatus
{
	FIFO_STATUS_TX_REUSE 	= 0x40,
	FIFO_STATUS_TX_FULL 	= 0x20,
	FIFO_STATUS_TX_EMPTY 	= 0x10,
	FIFO_STATUS_RX_FULL 	= 0x02,
	FIFO_STATUS_RX_EMPTY 	= 0x01
};

enum{Bank1RegsSize = 14*4 };
const uint8_t Bank1Regs[Bank1RegsSize]=
{
	0xE2, 0x01, 0x4B, 0x40,
	0x00, 0x00, 0x4B, 0xC0,
	0x02, 0x8C, 0xFC, 0xD0,
	0x41, 0x39, 0x00, 0x99,
	0x0B, 0x86, 0x9E, 0xf9,
	0xA6, 0x7F, 0x06, 0x24,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x12, 0x73, 0x00,
	0x36, 0xB4, 0x80, 0x00
};

enum{Reg14Size = 11 };
const uint8_t Bank1_Reg14[Reg14Size]=
{
	0x41,0x20,0x08,0x04,0x81,0x20,0xCF,0xF7,0xFE,0xFF,0xFF
};

template<class Spi>
class Rfm70
{
private:
	void WriteReg(uint8_t reg, uint8_t value)
	{
		Spi::SlaveSelectPin::Clear();
		uint8_t status = Spi::ReadWrite(reg);
		Spi::ReadWrite(value);
		Spi::SlaveSelectPin::Set();
	}

	uint8_t SPI_Read_Reg(uint8_t reg)                               
	{                                                           
		Spi::SlaveSelectPin::Clear();
		uint8_t status = Spi::ReadWrite(reg);
		uint8_t value = Spi::ReadWrite(0);
		Spi::SlaveSelectPin::Set();

		return value;
	}                      
public:

};
