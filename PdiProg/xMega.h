#pragma once

#include "TargetDeviceCtrl.h"
#include "ProgInterface.h"
#include "constants.h"
#include "Pdi.h"

namespace XMega
{
	enum 
	{
		CMD_NOOP                   = 0x00,
		CMD_CHIPERASE              = 0x40,
		CMD_READNVM                = 0x43,
		CMD_LOADFLASHPAGEBUFF      = 0x23,
		CMD_ERASEFLASHPAGEBUFF     = 0x26,
		CMD_ERASEFLASHPAGE         = 0x2B,
		CMD_WRITEFLASHPAGE         = 0x2E,
		CMD_ERASEWRITEFLASH        = 0x2F,
		CMD_FLASHCRC               = 0x78,
		CMD_ERASEAPPSEC            = 0x20,
		CMD_ERASEAPPSECPAGE        = 0x22,
		CMD_WRITEAPPSECPAGE        = 0x24,
		CMD_ERASEWRITEAPPSECPAGE   = 0x25,
		CMD_APPCRC                 = 0x38,
		CMD_ERASEBOOTSEC           = 0x68,
		CMD_ERASEBOOTSECPAGE       = 0x2A,
		CMD_WRITEBOOTSECPAGE       = 0x2C,
		CMD_ERASEWRITEBOOTSECPAGE  = 0x2D,
		CMD_BOOTCRC                = 0x39,
		CMD_READUSERSIG            = 0x03,
		CMD_ERASEUSERSIG           = 0x18,
		CMD_WRITEUSERSIG           = 0x1A,
		CMD_READCALIBRATION        = 0x02,
		CMD_READFUSE               = 0x07,
		CMD_WRITEFUSE              = 0x4C,
		CMD_WRITELOCK              = 0x08,
		CMD_LOADEEPROMPAGEBUFF     = 0x33,
		CMD_ERASEEEPROMPAGEBUFF    = 0x36,
		CMD_ERASEEEPROM            = 0x30,
		CMD_ERASEEEPROMPAGE        = 0x32,
		CMD_WRITEEEPROMPAGE        = 0x34,
		CMD_ERASEWRITEEEPROMPAGE   = 0x35,
		CMD_READEEPROM             = 0x06
	};

	enum
	{
		REG_ADDR0                  = 0x00,
		REG_ADDR1                  = 0x01,
		REG_ADDR2                  = 0x02,
		REG_DAT0                   = 0x04,
		REG_DAT1                   = 0x05,
		REG_DAT2                   = 0x06,
		REG_CMD                    = 0x0A,
		REG_CTRLA                  = 0x0B,
		REG_CTRLB                  = 0x0C,
		REG_INTCTRL                = 0x0D,
		REG_STATUS                 = 0x0F,
		REG_LOCKBITS               = 0x10
	};

	template<class Comm>
	class Xmega :public TargetDeviceCtrl
	{
		public:
		
		Xmega(ProgParameters *progParams, DeviceDescriptor *deviceDescroptor)
			:TargetDeviceCtrl(progParams, deviceDescroptor)
		{		
		}

		virtual void EnterProgMode()
		{
			_progIface->Enable();
			_progIface->WriteByte(Pdi::CMD_STCS | Pdi::RESET_REG);	
			_progIface->WriteByte(Pdi::RESET_KEY);

			//_progIface->Write(Pdi::CMD_STCS | Pdi::CTRL_REG);	
			//_progIface->Write(0x04);
			_progIface->WriteByte(Pdi::CMD_KEY);
			long long key = 0x1289AB45CDD888FFll;
			_progIface->Write(key);
		}

		virtual void LeaveProgMode()
		{
			_progIface->WriteByte(Pdi::CMD_STCS | Pdi::RESET_REG);	
			_progIface->WriteByte(0x00);
			_progIface->Disable();
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
				break;
			}

			if(!ReadMemory(address, size))
			{
				for(uint32_t i=0; i<size; i++)
				{
					Comm::Write(0x00);
				}
			}
		}

		virtual void WriteMem(uint8_t memType, uint32_t size, uint32_t address)
		{
			uint8_t c;
			switch(memType)
			{
				case SIGN_JTAG:
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
						Comm::Getch(c);
					}
			}
		}
	protected:

		bool ReadMemory(uint32_t readAddress, uint16_t size)
		{
			if (!(WaitWhileControllerBusy()))
				return false;

			_progIface->WriteByte(Pdi::CMD_STS | (Pdi::DATSIZE_4BYTES << 2));

			_progIface->Write(_progParams->PDI_NVM_Offset | REG_CMD);

			_progIface->WriteByte(CMD_READNVM);

			_progIface->WriteByte(Pdi::CMD_ST | (Pdi::POINTER_DIRECT << 2) | Pdi::DATSIZE_4BYTES);
			_progIface->Write(readAddress);

			_progIface->WriteByte(Pdi::CMD_REPEAT | Pdi::DATSIZE_1BYTE);
			_progIface->WriteByte(size - 1);

			_progIface->WriteByte(Pdi::CMD_LD | (Pdi::POINTER_INDIRECT_PI << 2) | Pdi::DATSIZE_1BYTE);
			while (size--)
			{
				uint8_t c = _progIface->ReadByte();
				Comm::Write(c);
			}

			return true;
		}

		bool WaitWhileBusBusy()
		{
			uint16_t timeout=500;
			while (timeout--)
			{
				_progIface->Write(Pdi::CMD_LDCS | Pdi::STATUS_REG);
				if (_progIface->ReadByte() & Pdi::STATUS_NVM)
				{
					return true;
				}
			}

			return false;
		}

		bool WaitWhileControllerBusy(void)
		{
			uint16_t timeout=500;
			while (timeout--)
			{
				_progIface->Write(Pdi::CMD_LDS | (Pdi::DATSIZE_4BYTES << 2));
				_progIface->Write<uint32_t>(XMega::REG_STATUS);

				if (!(_progIface->ReadByte() & (1 << 7)))
				{
					return true;
				}
			}
			return false;
		}
	};
}//namespace XMega
