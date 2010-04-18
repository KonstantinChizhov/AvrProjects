#pragma once

#include "TargetDeviceCtrl.h"
#include "ProgInterface.h"
#include "constants.h"

template<class Comm>
class Xmega :public TargetDeviceCtrl
{
	public:

	virtual void EnterProgMode()
	{
	
	}

	virtual void LeaveProgMode()
	{
	
	}

	virtual uint32_t GetJTAGID()
	{
		return 0x0974C03F;
	}

	virtual void ReadMem(uint8_t memType, uint32_t size, uint32_t address)
	{
		switch(memType)
		{
			case SIGN_JTAG:
				Comm::Putch(0x1E);
				Comm::Putch(0x97);
				Comm::Putch(0x4C);
				break;
			case IO_SHADOW:
			case SRAM:
			case EEPROM:
			case EVENT:
			case SPM:
			case FLASH_PAGE:
			case EEPROM_PAGE:
			case FUSE_BITS:
			case LOCK_BITS:
			case OSCCAL_BYTE:
			case CAN:
			case XMEGA_APPLICATION_FLASH:
			case XMEGA_BOOT_FLASH:
			case XMEGA_CALIBRATION_SIGNATURE:
			case XMEGA_USER_SIGNATURE:
			default:
				for(uint32_t i=0; i<size; i++)
				{
					Comm::Putch(0xff);
				}
		}
	}
};
