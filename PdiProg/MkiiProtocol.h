
#pragma once

#include "crc16.h"

namespace MkII
{
	enum SlaveMode
	{
		Stopped, 
		Running, 
		Programming
	};

	enum {MessageStart = 27, Token = 14};

	enum Commands
	{
		CMND_SIGN_OFF = 0x00,
		CMND_GET_SIGN_ON = 0x01,
		CMND_SET_PARAMETER = 0x02,
		CMND_GET_PARAMETER = 0x03,
		CMND_WRITE_MEMORY = 0x04,
		CMND_READ_MEMORY = 0x05,
		CMND_WRITE_PC = 0x06,
		CMND_READ_PC = 0x07,
		CMND_GO = 0x08,
		CMND_SINGLE_STEP = 0x09,
		CMND_FORCED_STOP = 0x0A,
		CMND_RESET = 0x0B,
		CMND_SET_DEVICE_DESCRIPTOR = 0x0C,
		CMND_ERASEPAGE_SPM = 0x0D,
		CMND_GET_SYNC 0x0F,
		CMND_SELFTEST = 0x10,
		CMND_SET_BREAK = 0x11,
		CMND_GET_BREAK = 0x12,
		CMD_CHIP_ERASE = 0x13
		CMND_ENTER_PROGMODE = 0x14,
		CMND_LEAVE_PROGMODE = 0x15,
		CMND_CLR_BREAK = 0x1A,
		CMND_RUN_TO_ADDR = 0x1C,
		CMND_SPI_CMD = 0x1D,
		CMND_CLEAR_EVENTS = 0x22,
		CMND_RESTORE_TARGET = 0x23,
		CMND_ISP_PACKET = 0x2F,
		CMND_JTAG_INSTR = 0x24,
		CMND_JTAG_DATA = 0x25,
		CMND_JTAG_SAB_WRITE = 0x28,
		CMND_JTAG_SAB_READ = 0x29,
		CMND_JTAG_BLOCK_READ = 0x2C,
		CMND_JTAG_BLOCK_WRITE = 0x2D,
		CMND_XMEGA_ERASE  = 0x34
	};
	
	enum Responses
	{
		RSP_OK  = 0x80,
		RSP_FAILED = 0xA0,
		RSP_ILLEGAL_PARAMETER = 0xA1,
		RSP_PARAMETER = 0x81,
		RSP_ILLEGAL_MEMORY_TYPE = 0xA2,
		RSP_ILLEGAL_MEMORY_RANGE = 0xA3,
		RSP_MEMORY = 0x82,
		RSP_GET_BREAK = 0x83,
		RSP_ILLEGAL_EMULATOR_MODE = 0xA4,
		RSP_ILLEGAL_MCU_STATE = 0xA5,
		RSP_PC = 0x84,
		RSP_SELFTEST = 0x85,
		RSP_SPI_DATA = 0x88,
		RSP_ILLEGAL_COMMAND = 0xAA,
		RSP_ILLEGAL_VALUE = 0xA6,
		RSP_SIGN_ON = 0x86,
		RSP_ILLEGAL_BREAKPOINT = 0xA8,
		RSP_ILLEGAL_JTAG_ID = 0xA9,
		RSP_NO_TARGET_POWER = 0xAB,
		RSP_DEBUGWIRE_SYNC_FAILED = 0xAC,
		RSP_ILLEGAL_POWER_STATE = 0xAD,
		RSP_SCAN_CHAIN_READ = 0x87		
	};

	enum Events
	{
		EVT_BREAK = 0xE0,
		EVT_TARGET_POWER_ON = 0xE4,
		EVT_TARGET_POWER_OFF = 0xE5,
		EVT_EXT_RESET = 0xE7,
		EVT_TARGET_SLEEP = 0xE8,
		EVT_TARGET_WAKEUP = 0xE9,
		EVT_POWER_ERROR_STATE = 0xEA,
		EVT_POWER_OK = 0xEB,
		EVT_IDR_DIRTY = 0xEC,
		EVT_ERROR_PHY_FORCE_BREAK_TIMEOUT = 0xE2,
		EVT_ERROR_PHY_RELEASE_BREAK_TIMEOUT = 0xE3,
		EVT_ERROR_PHY_MAX_BIT_LENGTH_DIFF = 0xED,
		EVT_ERROR_PHY_SYNC_TIMEOUT  = 0xF0,
		EVT_ERROR_PHY_SYNC_TIMEOUT_BAUD = 0xF4,
		EVT_ERROR_PHY_SYNC_OUT_OF_RANGE = 0xF5,
		EVT_ERROR_PHY_SYNC_WAIT_TIMEOUT = 0xF6,
		EVT_ERROR_PHY_RECEIVE_TIMEOUT = 0xF7,
		EVT_ERROR_PHY_RECEIVED_BREAK = 0xF8,
		EVT_ERROR_PHY_OPT_RECEIVE_TIMEOUT = 0xF9,
		EVT_ERROR_PHY_OPT_RECEIVED_BREAK = 0xFA,
		EVT_RESULT_PHY_NO_ACTIVITY = 0xFB
	};

	enum MemoryTypes
	{
		IO_SHADOW = 0x30,
		SRAM = 0x20,
		EEPROM = 0x22,
		EVENT = 0x60,
		SPM = 0xA0,
		FLASH_PAGE = 0xB0,
		EEPROM_PAGE = 0xB1,
		FUSE_BITS = 0xB2,
		LOCK_BITS = 0xB3,
		SIGN_JTAG = 0xB4,
		OSCCAL_BYTE = 0xB5,
		CAN = 0xB6,
		XMEGA_APPLICATION_FLASH = 0xC0,
		XMEGA_BOOT_FLASH = 0xC1,
		XMEGA_USER_SIGNATURE = 0xC5,
		XMEGA_CALIBRATION_SIGNATURE = 0xC6
	};

	enum XMegaEraseMode
	{
		XMEGA_ERASE_CHIP = 0x00,
		XMEGA_ERASE_APP = 0x01,
		XMEGA_ERASE_BOOT = 0x02,
		XMEGA_ERASE_EEPROM = 0x03,
		XMEGA_ERASE_APP_PAGE = 0x04,
		XMEGA_ERASE_BOOT_PAGE = 0x05,
		XMEGA_ERASE_EEPROM_PAGE = 0x06,
		XMEGA_ERASE_USERSIG = 0x07
	};

	struct DeviceDescriptor
	{
		unsigned char ucReadIO[8]; 			//LSB = IOloc 0, MSB = IOloc63
		unsigned char ucReadIOShadow[8]; 	//LSB = IOloc 0, MSB = IOloc63
		unsigned char ucWriteIO[8]; 		//LSB = IOloc 0, MSB = IOloc63
		unsigned char ucWriteIOShadow[8]; 	//LSB = IOloc 0, MSB = IOloc63
		unsigned char ucReadExtIO[52]; 		//LSB = IOloc 96, MSB = IOloc511
		unsigned char ucReadIOExtShadow[52]; //LSB = IOloc 96, MSB = IOloc511
		unsigned char ucWriteExtIO[52]; 	//LSB = IOloc 96, MSB = IOloc511
		unsigned char ucWriteIOExtShadow[52];//LSB = IOloc 96, MSB = IOloc511
		unsigned char ucIDRAddress; 		//IDR address
		unsigned char ucSPMCRAddress; 		//SPMCR Register address and dW BasePC
		unsigned long ulBootAddress; 		//Device Boot Loader Start Address
		unsigned char ucRAMPZAddress; 		//RAMPZ Register address in SRAM I/O
		//space
		unsigned int uiFlashPageSize; 		//Device Flash Page Size, Size =
		//2 exp ucFlashPageSize
		unsigned char ucEepromPageSize; 		//Device Eeprom Page Size in bytes
		unsigned int uiUpperExtIOLoc; 		//Topmost (last) extended I/O
		//location, 0 if no external I/O
		unsigned long ulFlashSize; 			//Device Flash Size
		unsigned char ucEepromInst[20]; 	//Instructions for W/R EEPROM
		unsigned char ucFlashInst[3]; 		//Instructions for W/R FLASH
		unsigned char ucSPHaddr; 			// Stack pointer high
		unsigned char ucSPLaddr; 			// Stack pointer low
		unsigned int uiFlashpages; 			// number of pages in flash
		unsigned char ucDWDRAddress; 		// DWDR register address
		unsigned char ucDWBasePC; 			// Base/mask value of the PC
		unsigned char ucAllowFullPageBitstream; // FALSE on ALL new parts
		unsigned int uiStartSmallestBootLoaderSection; //
		unsigned char EnablePageProgramming;// For JTAG parts only,
		// default TRUE
		unsigned char ucCacheType; 			// CacheType_Normal 0x00,
		// CacheType_CAN 0x01,
		unsigned int uiSramStartAddr 		// Start of SRAM
		unsigned char ucResetType; 			// Selects reset type. 0x00
		unsigned char ucPCMaskExtended; 	// For parts with extended PC
		unsigned char ucPCMaskHigh; 		// PC high mask
		unsigned char ucEindAddress; 		// EIND IO address
		unsigned int EECRAddress; 			// EECR IO address
	};
	
	template<class HwInterface>
	class MkIIProtocol
	{
		void Send1ByteResponse(uint8_t sequenceNumber, Responses response)
		{
			uint16_t crc = 0xffff;
			HwInterface::Write<uint8_t>(MessageStart);
			HwInterface::Write<uint16_t>(sequenceNumber);
			HwInterface::Write<uint8_t>(1);
			HwInterface::Write<uint8_t>(Token);
			HwInterface::Write<uint8_t>(response);
			
		}
	};

}
