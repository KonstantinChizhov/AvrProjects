#pragma once

#include "TargetDeviceCtrl.h"
#include "ProgInterface.h"
#include "constants.h"
#include "Pdi.h"

namespace XMega
{
	enum Commands
	{
		CMD_NOOP                   = 0x00,
		CMD_READNVM                = 0x43,
		CMD_LOADFLASHPAGEBUFF      = 0x23,
		CMD_WRITEFLASHPAGE         = 0x2E,
		CMD_FLASHCRC               = 0x78,
		CMD_WRITEAPPSECPAGE        = 0x24,
		CMD_APPCRC                 = 0x38,
		CMD_ERASEBOOTSEC           = 0x68,
		CMD_WRITEBOOTSECPAGE       = 0x2C,
		CMD_BOOTCRC                = 0x39,
		CMD_READUSERSIG            = 0x03,
		CMD_WRITEUSERSIG           = 0x1A,
		CMD_READCALIBRATION        = 0x02,
		CMD_READFUSE               = 0x07,
		CMD_WRITEFUSE              = 0x4C,
		CMD_WRITELOCK              = 0x08,
		CMD_LOADEEPROMPAGEBUFF     = 0x33,
		CMD_WRITEEEPROMPAGE        = 0x34,
		CMD_READEEPROM             = 0x06,

		CMD_CHIPERASE              = 0x40,
		CMD_ERASEUSERSIG           = 0x18,
		CMD_ERASEAPPSEC            = 0x20,
		CMD_ERASEAPPSECPAGE        = 0x22,
		CMD_ERASEWRITEAPPSECPAGE   = 0x25,
		CMD_ERASEFLASHPAGEBUFF     = 0x26,
		CMD_ERASEBOOTSECPAGE       = 0x2A,
		CMD_ERASEFLASHPAGE         = 0x2B,
		CMD_ERASEWRITEBOOTSECPAGE  = 0x2D,
		CMD_ERASEWRITEFLASH        = 0x2F,
		CMD_ERASEEEPROMPAGEBUFF    = 0x36,
		CMD_ERASEEEPROM            = 0x30,
		CMD_ERASEEEPROMPAGE        = 0x32,
		CMD_ERASEWRITEEEPROMPAGE   = 0x35
	};

	enum Registers
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
			_progParams->PDI_NVM_Offset = 0x010001C0;
		}

		virtual void EnterProgMode()
		{
			_progIface->Enable();
			_progIface->WriteByte(Pdi::CMD_STCS | Pdi::RESET_REG);	
			_progIface->WriteByte(Pdi::RESET_KEY);

			_progIface->WriteByte(Pdi::CMD_STCS | Pdi::CTRL_REG);	
			_progIface->WriteByte(0x05);

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
			return 0;//0x0974C03F;
		}

		virtual bool ReadMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address)
		{
			if(!Command(CMD_READNVM))
				return false;

			Address(Pdi::CMD_ST | (Pdi::POINTER_DIRECT << 2) | Pdi::DATSIZE_4BYTES, address);
		
			SetRepeat(size - 1);

			_progIface->WriteByte(Pdi::CMD_LD | (Pdi::POINTER_INDIRECT_PI << 2) | Pdi::DATSIZE_1BYTE);

			for(uint32_t i=0; i<size; i++)
			{
				uint8_t c = _progIface->ReadByte();
				buffer[i] = c;
			}

			return true;
		}

		virtual bool WriteMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address)
		{
			switch(memType)
			{
				case FUSE_BITS:
					return WriteLockOrFuse(CMD_WRITEFUSE, buffer, address);
				case LOCK_BITS:
					return WriteLockOrFuse(CMD_WRITELOCK, buffer, address);

				case XMEGA_APPLICATION_FLASH:
					FillPageBuffer(CMD_LOADFLASHPAGEBUFF, buffer, size, address);
					return //ErasePage(CMD_ERASEAPPSECPAGE, address) &&
					 PageWrite(CMD_WRITEAPPSECPAGE, address);

				case XMEGA_BOOT_FLASH:
					FillPageBuffer(CMD_LOADFLASHPAGEBUFF, buffer, size, address);
					return PageWrite(CMD_WRITEBOOTSECPAGE, address);

				case EEPROM:
					FillPageBuffer(CMD_LOADEEPROMPAGEBUFF, buffer, size, address);
					return PageWrite(CMD_ERASEWRITEEEPROMPAGE, address);

				case XMEGA_USER_SIGNATURE:
				case XMEGA_CALIBRATION_SIGNATURE:
				case SIGN_JTAG:
				case IO_SHADOW:
				case SRAM:
				case EVENT:
				case SPM:
				case FLASH_PAGE:
				case EEPROM_PAGE:
				case OSCCAL_BYTE:
				case CAN:
				default:
					return false;
			}
		}

		virtual bool Erase(uint8_t memType, uint32_t address)
		{
			uint8_t eraseCmd;
			switch(memType)
			{
				case XMEGA_ERASE_CHIP:
					return EraseSection(CMD_CHIPERASE);
				case XMEGA_ERASE_APP:
					eraseCmd = CMD_ERASEAPPSEC;
					break;
				case XMEGA_ERASE_BOOT:
					eraseCmd = CMD_ERASEBOOTSEC;
					break;
				case XMEGA_ERASE_EEPROM:
					eraseCmd = CMD_ERASEEEPROM;
				case XMEGA_ERASE_APP_PAGE:
					eraseCmd = CMD_ERASEAPPSECPAGE;
					break;
				case XMEGA_ERASE_BOOT_PAGE:
					eraseCmd = CMD_ERASEBOOTSECPAGE;
					break;
				case XMEGA_ERASE_EEPROM_PAGE:
					eraseCmd = CMD_ERASEEEPROMPAGE;
					break;
				case XMEGA_ERASE_USERSIG:
					eraseCmd = CMD_ERASEUSERSIG;
					break;
				default:
					return false;
			}
			return ErasePage(eraseCmd, address);
		}
	protected:

		bool PageWrite(uint8_t writeCmd, uint32_t address)
		{
			if (!Command(writeCmd))
				return false;

			Address(Pdi::CMD_STS | (Pdi::DATSIZE_4BYTES << 2), address, 0x00);
			return true;
		}

		bool EraseSection(uint8_t eraseCmd)
		{
			if (!Command(eraseCmd))
				return false;
				
			WriteNvmReg(REG_CTRLA, 1 << 0);
	
			if(!WaitWhileBusBusy())
				return false;
	  
			return true;
		}

		bool ErasePage(uint8_t eraseCmd, uint32_t address)
		{
			if (!Command(eraseCmd))
				return false;
				
			Address(Pdi::CMD_STS | (Pdi::DATSIZE_4BYTES << 2), address, 0x00);

			//WriteNvmReg(REG_CTRLA, 1 << 0);

			if (!(WaitWhileBusBusy()))
			  return false;
	  
			return true;
		}

		bool FillPageBuffer(uint8_t bufferCommand, uint8_t *buffer, uint16_t size, uint32_t address)
		{
			if (size)
			{
				if (!Command(bufferCommand))
				  return false;

				Address(Pdi::CMD_ST | (Pdi::POINTER_DIRECT << 2) | Pdi::DATSIZE_4BYTES, address);

				SetRepeat(size - 1);
			
				_progIface->WriteByte(Pdi::CMD_ST | (Pdi::POINTER_INDIRECT_PI << 2) | Pdi::DATSIZE_1BYTE);
				for (uint16_t i = 0; i < size; i++)
				  _progIface->WriteByte(*(buffer++));
			}
			return true;
		}

		bool WriteLockOrFuse(uint8_t command, uint8_t *buffer, uint32_t address)
		{
			if (!Command(command))
				return false;

			Address(Pdi::CMD_STS | (Pdi::DATSIZE_4BYTES << 2), address, *buffer);

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
				_progIface->Write<uint32_t>(XMega::REG_STATUS | _progParams->PDI_NVM_Offset);

				if(!(_progIface->ReadByte() & (1 << 7)))
				{
					return true;
				}
			}
			return false;
		}

		bool Command(uint8_t command)
		{
			return WriteNvmReg(REG_CMD, command);
		}

		bool WriteNvmReg(Registers reg, uint8_t value)
		{
			if (!WaitWhileControllerBusy())
				return false;

			Address(Pdi::CMD_STS | (Pdi::DATSIZE_4BYTES << 2),
					_progParams->PDI_NVM_Offset | reg, 
					value);
		
			return true;
		}

		void Address(uint8_t command, uint32_t address)
		{
			_progIface->WriteByte(command);
			_progIface->Write(address);
		}

		void Address(uint8_t command, uint32_t address, uint8_t value)
		{
			_progIface->WriteByte(command);
			_progIface->Write(address);
			_progIface->WriteByte(value);
		}

		void SetRepeat(uint8_t value)
		{
			_progIface->WriteByte(Pdi::CMD_REPEAT | Pdi::DATSIZE_1BYTE);
			_progIface->WriteByte(value);
		}
	};
}//namespace XMega
