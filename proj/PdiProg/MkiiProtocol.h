
#pragma once

#include "Crc16.h"
#include "ioports.h"
#include "xMega.h"
#include "ProgInterface.h"
#include "TargetDeviceCtrl.h"
#include "constants.h"
#include "DeviceDescriptor.h"
#include "CheckSummUpdater.h"
#include "ProgrammerParameters.h"
#include "containers.h"

namespace MkII
{
	struct MessageHeader
	{
		uint16_t seqNumber;
		uint32_t messageLen;
		uint8_t messageId;
	};


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
	template<
		class HwInterface, 
		class PdiInterface, 
		class SpiInterface = NullProgInterface, 
		class JTAGInterface = NullProgInterface
		>
	class MkIIProtocol
	{
	public:
		typedef BinaryFormater<CheckSummUpdater<WaitAdapter< HwInterface> > > interface;
	private:
		//HW interfaces
		PdiInterface _pdi;
		NullProgInterface _nullProg;
		ProgInterface *_progIface;
		//targets

		XMega::Xmega<interface> _xmega;
		NullTargetDeviceCtrl<interface> _nullTarget;
		TargetDeviceCtrl * _target;
		ProgParameters _params;
		DeviceDescriptor _deviceDescriptor;
		MessageHeader _header;
		Array<256> _pageBuffer;

	public:

		MkIIProtocol()
		:_xmega(&_params, &_deviceDescriptor)
		{
			_params.EmuMode = Unknown;
			_progIface = &_nullProg;
			_target = &_nullTarget;
		}

		void SetMode(EmulatorMode mode)
		{
			//IO::Portb::Write(mode);
			_progIface->Disable();
			switch(mode)
			{
				case PDI_XMEGA:
					_progIface = &_pdi;
					_target = &_xmega;
					break;
				case JTAG_XMEGA:
				case Spi:		//To be impemented
				case JTAG_Mega:	//To be impemented	
				case DebugWire:
				case JTAG_AVR32:
				case Unknown:
				default:
					_progIface = &_nullProg;
					_target = &_nullTarget;
			}
			_progIface->Enable();
			_target->SetProgInterface(_progIface);
		}

		void SendMessageHeader(uint16_t size, Responses response)
		{
			interface::BeginTxFrame();
			interface::Write(uint8_t(MessageStart));
			interface::Write(_header.seqNumber);
			interface::Write(uint32_t(size));
			interface::Write(uint8_t(Token));
			interface::Write(uint8_t(response));
		}

		void SendResponse(Responses response)
		{
			SendMessageHeader(1, response);
			interface::EndTxFrame();
		}

		template<class T>
		void SendResponse(Responses response, const T&value)
		{
			SendMessageHeader(sizeof(value)+1, response);
			interface::Write(value);
			interface::EndTxFrame();
		}

		template<class T>
		void ParamResponse(const T&value)
		{
			SendResponse(RSP_PARAMETER, value);
		}

		void SingOn()
		{
			SendMessageHeader(29, RSP_SIGN_ON);

			interface::Write(uint8_t(1));	// Communications protocol version [BYTE]
			interface::Write(uint8_t(0));	// M_MCU boot-loader FW version [BYTE]
			interface::Write(uint8_t(20));	// M_MCU firmware version (minor) [BYTE]
			interface::Write(uint8_t(6));	// M_MCU firmware version (major) [BYTE]
			interface::Write(uint8_t(2));	// M_MCU hardware version [BYTE]
			interface::Write(uint8_t(0));	// S_MCU boot-loader FW version [BYTE]
			interface::Write(uint8_t(20));	// S_MCU firmware version (minor) [BYTE]
			interface::Write(uint8_t(6));	// S_MCU firmware version (major) [BYTE]
			interface::Write(uint8_t(2));	// S_MCU hardware version [BYTE]
			interface::Puts("123456");		//(USB) EEPROM stored s/n [BYTE] * 6,LSB FIRST
			interface::Puts("JTAGICE mkII");
			interface::Write(uint8_t(0));
			interface::EndTxFrame();
		}

		void SelfTest()
		{
			uint8_t flags = interface::Read();
			SendMessageHeader(9, RSP_SELFTEST);
			for(uint8_t i=0; i<8; i++)
			{
				interface::Write(flags&1);
				flags >>= 1;
			}
			interface::EndTxFrame();
		}

		void PollInterface()
		{
			uint8_t ch;
			if(interface::Getch(ch))
				return;
			
			if(ch != MessageStart)
				return;
		
			_header.seqNumber = interface::ReadU16();
			
			_header.messageLen = interface::ReadU32();

			if(interface::Read() != Token)
				return;

			_header.messageId = interface::Read();
		
			ProcessCommand();
			//uint16_t crc = interface::ReadU16();
		}

		void GetParameter()
		{
			uint8_t parameter = interface::Read();
			//IO::Portb::Write(parameter);
			switch(parameter)
			{
				case BaudRate:
					ParamResponse<uint8_t>(0x04);
					break;
				case OCD_Vtarget:
					ParamResponse<uint16_t>(0x0ce4);//3.3 v
					break;
				case DaisyChainInfo:
					ParamResponse<ProgDaisyChainInfo>(_params.ChainInfo );
					break;
				case ExternalReset:
					ParamResponse<uint8_t>(_params.ExternalReset);
					break;
				case EmulatorMODE:
					ParamResponse<uint8_t>(_params.EmuMode);
					break;
				case OCD_JTAG_Clock:
					ParamResponse<uint8_t>(_params.JTAG_Clock);
					break;
				case FlashPageSize:
					ParamResponse<uint16_t>(_params.FlashPageSize);
					break;
				case EEPROMPageSize:
					ParamResponse<uint8_t>(_params.EEPROMPageSize);
					break;
				case BootAddress:
					ParamResponse<uint32_t>(_params.BootAddress);
					break;
				case JTAGID:
					ParamResponse<uint32_t>(_target->GetJTAGID());
					break;
				case PDI_NVM_Offset:
					ParamResponse<uint32_t>(_params.PDI_NVM_Offset);
					break;
 				case PDI_FlashOffset:
					ParamResponse<uint32_t>(_params.PDI_FlashOffset);
					break;
	 			case PDI_FlashBootOffset:
					ParamResponse<uint32_t>(_params.PDI_FlashBootOffset);
					break;

				default:
					//IO::Portb::Write(parameter);
					SendResponse(RSP_ILLEGAL_PARAMETER);
			}			
		}

		void SetParameter()
		{
			uint8_t parameter = interface::Read();
			
			switch(parameter)
			{
				case DaisyChainInfo:
					interface::Read(_params.ChainInfo);
					break;
				case ExternalReset:
					_params.ExternalReset = interface::Read();
					break;
				case OCD_JTAG_Clock:
					_params.JTAG_Clock =interface:: Read();
					break;
				case EmulatorMODE:
					_params.EmuMode = (EmulatorMode)interface::Read();
					SetMode(_params.EmuMode);
					break;
				case FlashPageSize:
					_params.FlashPageSize = interface::ReadU16();
					break;
				case EEPROMPageSize:
					_params.EEPROMPageSize = interface::Read();
					break;
				case BootAddress:
					_params.BootAddress = interface::ReadU32();
					break;
				case PDI_NVM_Offset:
					_params.PDI_NVM_Offset = interface::ReadU32();
					break;
 				case PDI_FlashOffset:
					_params.PDI_FlashOffset =interface:: ReadU32();
					break;
	 			case PDI_FlashBootOffset:
					_params.PDI_FlashBootOffset = interface::ReadU32();
					break;
				case RunAfterProgramming:
					_params.RunAfterProgramming = interface::Read();
					break;

				case AllowPageProgrammingInScanChain:
					break;
				case BaudRate:
					//SetBaund(); //does not work correctly
					break;
				default:
					//IO::Portb::Write(parameter);
					SendResponse(RSP_ILLEGAL_PARAMETER);
					return;
			}
			SendResponse(RSP_OK); 
		}

		void SetBaund()
		{
			uint8_t baund = interface::Read();
			HwInterface::Disable();
			switch(baund)
			{
				case 0x05: HwInterface::Init(38400);break;
				case 0x06: HwInterface::Init(57600);break;
				case 0x07: HwInterface::Init(115200);break;
				case 0x04:
				default: HwInterface::Init(19200);break;
			}
			SendResponse(RSP_OK); 
		}

		void ProcessCommand()
		{
			//IO::Portb::Write(_header.messageId);
			switch(_header.messageId)
			{
				case CMND_SIGN_OFF:
					SendResponse(RSP_OK);
					break;
				case CMND_GET_SIGN_ON:
					SingOn(); 
					break;

				case CMND_SET_PARAMETER:
					SetParameter(); 
					break;
				case CMND_GET_PARAMETER:
					GetParameter(); 
					break;

				case CMND_ENTER_PROGMODE:
					_target->EnterProgMode();
					SendResponse(RSP_OK);
					break;
				case CMND_LEAVE_PROGMODE:
					_target->LeaveProgMode();
					SendResponse(RSP_OK); 
					break;
				break;

				case CMND_SELFTEST:
					SelfTest();
				break;

				case CMND_WRITE_MEMORY:
					WriteMem();
				break;

				case CMND_READ_MEMORY:
					ReadMemBuffered();
					//ReadMemStreamed();
				break;
				
				case CMND_SET_DEVICE_DESCRIPTOR:
					interface::Read(&_deviceDescriptor, sizeof(_deviceDescriptor));
					SendResponse(RSP_OK);
				break;
				case CMND_CLEAR_EVENTS:
					SendResponse(RSP_OK);
				break;

				case CMND_XMEGA_ERASE:
					Erase();
				break;

				case CMND_WRITE_PC:
				case CMND_READ_PC:
				case CMND_GO:
				case CMND_SINGLE_STEP:
				case CMND_FORCED_STOP:
				case CMND_RESET:
				case CMND_ERASEPAGE_SPM:
				case CMND_GET_SYNC:
				case CMND_SET_BREAK:
				case CMND_GET_BREAK:
				case CMD_CHIP_ERASE:
				case CMND_CLR_BREAK:
				case CMND_RUN_TO_ADDR:
				case CMND_SPI_CMD:
				case CMND_RESTORE_TARGET:
				case CMND_ISP_PACKET:
				case CMND_JTAG_INSTR:
				case CMND_JTAG_DATA:
				case CMND_JTAG_SAB_WRITE:
				case CMND_JTAG_SAB_READ:
				case CMND_JTAG_BLOCK_READ:
				case CMND_JTAG_BLOCK_WRITE:

				default:
					//IO::Portb::Write(_header.messageId);
					SendResponse(RSP_ILLEGAL_COMMAND);
			}
		}

		void WriteMem()
		{
			uint8_t memType = interface::Read();
			uint32_t size = interface::ReadU32();
			uint32_t address = interface::ReadU32();

			if(size > _pageBuffer.Size())
			{
				SendResponse(RSP_ILLEGAL_MEMORY_RANGE);
				return;
			}
			
			interface::Read(_pageBuffer, size);

			if(!_target->WriteMem(memType, _pageBuffer, size, address))
				SendResponse(RSP_FAILED);
			SendResponse(RSP_OK);
		}

		void ReadMemBuffered()
		{
			uint8_t memType = interface::Read();
			uint32_t size = interface::ReadU32();
			uint32_t address = interface::ReadU32();
			Int32 i;
			i.Dword = address;
			//IO::Portb::Write(memType);
			//IO::Portb::Write(size>>8);
			//IO::Portb::Write(i.Bytes[0]);
			if(size > _pageBuffer.Size())
			{
				SendResponse(RSP_ILLEGAL_MEMORY_RANGE);
				return;
			}

			if(_target->ReadMem(memType, _pageBuffer, size, address))
			{
				SendMessageHeader(size+1, RSP_MEMORY);
				interface::Write(_pageBuffer, size);
				interface::EndTxFrame();
			}
			else
			{
				SendResponse(RSP_FAILED);
			}
			
		}

		void Erase()
		{
			uint8_t memType = interface::Read();
			uint32_t address = interface::ReadU32();

			if(_target->Erase(memType, address))
			{
				SendResponse(RSP_OK);
			}
			else
			{
				SendResponse(RSP_FAILED);
			}
		}

	};

}
