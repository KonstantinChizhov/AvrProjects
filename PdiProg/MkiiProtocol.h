
#pragma once

#include "Crc16.h"
#include "ports.h"
#include "xMega.h"
#include "ProgInterface.h"
#include "TargetDeviceCtrl.h"
#include "constants.h"
#include "DeviceDescriptor.h"
#include "CheckSummUpdater.h"

namespace MkII
{
	struct DaisyChainInfoT
	{
		uint8_t UnitsBefore;
		uint8_t UnitsAfter;
		uint8_t BitsBefore;
		uint8_t BitsAfter;
	};

	struct MessageHeader
	{
		uint16_t seqNumber;
		uint32_t messageLen;
		uint8_t messageId;
	};

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------

struct ParametersT
{
	EmulatorMode EmuMode;
	DaisyChainInfoT ChainInfo;
	uint8_t ExternalReset;
	uint8_t JTAG_Clock;
	uint16_t FlashPageSize;
	uint8_t EEPROMPageSize;
	uint32_t BootAddress;
	uint32_t PDI_NVM_Offset;
	uint32_t PDI_FlashOffset;
	uint32_t PDI_FlashBootOffset;
	uint8_t RunAfterProgramming;
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
		typedef CheckSummUpdater<HwInterface> interface;
		private:
		//HW interfaces
		PdiInterface _pdi;
		NullProgInterface _nullProg;
		ProgInterface *_progIface;
		//targets

		XMega::Xmega<CheckSummUpdater<HwInterface> > _xmega;
		NullTargetDeviceCtrl _nullTarget;
		TargetDeviceCtrl * _target;

	public:
		void SetMode(EmulatorMode mode)
		{
			switch(mode)
			{
				case PDI_XMEGA:
				case JTAG_XMEGA:
					_progIface = &_pdi;
					_target = &_xmega;
					break;
				case Spi:		//To be impemented
				case JTAG_Mega:	//To be impemented	
				case DebugWire:
				case JTAG_AVR32:
				case Unknown:
				default:
					_progIface = &_nullProg;
					_target = &_nullTarget;
			}
			_target->SetProgInterface(_progIface);
			_target->SetDeviceDescriptor(&deviceDescriptor);
		}

		void Init()
		{
			params.EmuMode = Unknown;
			_progIface = &_nullProg;
			_target = &_nullTarget;
		}

		void SendMessageHeader(const MessageHeader &header, uint16_t size, Responses response)
		{
			interface::BeginTxFrame();
			interface::Write(uint8_t(MessageStart));
			interface::Write(header.seqNumber);
			interface::Write(size);
			interface::Write(uint8_t(Token));
			interface::Write(uint8_t(response));
		}

		void Send1ByteResponse(const MessageHeader &header, Responses response)
		{
			SendMessageHeader(header, 1, response);
			interface::EndTxFrame();
		}

		template<class T>
		void SendResponse(const MessageHeader &header, Responses response, const T&value)
		{
			SendMessageHeader(header, sizeof(value)+1, response);
			interface::Write(value);
			interface::EndTxFrame();
		}

		template<class T>
		void ParamResponse(const MessageHeader &header, const T&value)
		{
			SendResponse(header, RSP_PARAMETER, value);
		}

		void SingOn(const MessageHeader &header)
		{
			SendMessageHeader(header, 29, RSP_SIGN_ON);

			interface::Write(uint8_t(0));// Communications protocol version [BYTE]
			interface::Write(uint8_t(0));// M_MCU boot-loader FW version [BYTE]
			interface::Write(uint8_t(20));// M_MCU firmware version (minor) [BYTE]
			interface::Write(uint8_t(6));// M_MCU firmware version (major) [BYTE]
			interface::Write(uint8_t(0));// M_MCU hardware version [BYTE]
			interface::Write(uint8_t(0));// S_MCU boot-loader FW version [BYTE]
			interface::Write(uint8_t(20));// S_MCU firmware version (minor) [BYTE]
			interface::Write(uint8_t(6));// S_MCU firmware version (major) [BYTE]
			interface::Write(uint8_t(0));// S_MCU hardware version [BYTE]
			interface::Puts("123456");	//(USB) EEPROM stored s/n [BYTE] * 6,LSB FIRST
			interface::Puts("JTAGICE mkII");
			interface::Write(uint8_t(0));
			interface::EndTxFrame();
		}

		void SelfTest(const MessageHeader &header)
		{
			uint8_t flags = interface::Read<uint8_t>();
			SendMessageHeader(header, 9, RSP_SELFTEST);
			for(uint8_t i=0; i<8; i++)
			{
				interface::Write(flags&1);
				flags >>= 1;
			}
			interface::EndTxFrame();
		}

		void PollInterface()
		{
			if(Read<uint8_t>() != MessageStart)
				return;
			
			header.seqNumber = Read<uint16_t>();
			header.messageLen = Read<uint32_t>();
			if(Read<uint8_t>() != Token)
				return;
			header.messageId = Read<uint8_t>();
			
			ProcessCommand(header);
			uint16_t crc = Read<uint16_t>();
		}

		void GetParameter(const MessageHeader &header)
		{
			uint8_t parameter = Read<uint8_t>();
			//IO::Portb::Write(parameter);
			switch(parameter)
			{
				case BaudRate:
					ParamResponse<uint8_t>(header, 0x04);
					break;
				case OCD_Vtarget:
					ParamResponse<uint16_t>(header, 0x0ce4);//3.3 v
					break;
				case DaisyChainInfo:
					ParamResponse<DaisyChainInfoT>(header, params.ChainInfo );
					break;
				case ExternalReset:
					ParamResponse<uint8_t>(header, params.ExternalReset);
					break;
				case EmulatorMODE:
					ParamResponse<uint8_t>(header, params.EmuMode);
					break;
				case OCD_JTAG_Clock:
					ParamResponse<uint8_t>(header, params.JTAG_Clock);
					break;
				case FlashPageSize:
					ParamResponse<uint16_t>(header, params.FlashPageSize);
					break;
				case EEPROMPageSize:
					ParamResponse<uint8_t>(header, params.EEPROMPageSize);
					break;
				case BootAddress:
					ParamResponse<uint32_t>(header, params.BootAddress);
					break;
				case JTAGID:
					ParamResponse<uint32_t>(header, _target->GetJTAGID());
					break;
				case PDI_NVM_Offset:
					ParamResponse<uint32_t>(header, params.PDI_NVM_Offset);
					break;
 				case PDI_FlashOffset:
					ParamResponse<uint32_t>(header, params.PDI_FlashOffset);
					break;
	 			case PDI_FlashBootOffset:
					ParamResponse<uint32_t>(header, params.PDI_FlashBootOffset);
					break;

				default:
					//::Portb::Write(parameter);
					Send1ByteResponse(header, RSP_ILLEGAL_PARAMETER);
			}			
		}

		void SetParameter(const MessageHeader &header)
		{
			uint8_t parameter = Read<uint8_t>();
			
			switch(parameter)
			{
				case DaisyChainInfo:
					params.ChainInfo = Read<DaisyChainInfoT>();
					break;
				case ExternalReset:
					params.ExternalReset = Read<uint8_t>();
					break;
				case OCD_JTAG_Clock:
					params.JTAG_Clock = Read<uint8_t>();
					break;
				case EmulatorMODE:
					params.EmuMode = (EmulatorMode)Read<uint8_t>();
					SetMode(params.EmuMode);
					break;
				case FlashPageSize:
					params.FlashPageSize = Read<uint16_t>();
					break;
				case EEPROMPageSize:
					params.EEPROMPageSize = Read<uint8_t>();
					break;
				case BootAddress:
					params.BootAddress = Read<uint32_t>();
					break;
				case PDI_NVM_Offset:
					params.PDI_NVM_Offset = Read<uint32_t>();
					break;
 				case PDI_FlashOffset:
					params.PDI_FlashOffset = Read<uint32_t>();
					break;
	 			case PDI_FlashBootOffset:
					params.PDI_FlashBootOffset = Read<uint32_t>();
					break;
				case RunAfterProgramming:
					params.RunAfterProgramming = Read<uint8_t>();
					break;

				case AllowPageProgrammingInScanChain:
					break;
				case BaudRate:
					//SetBaund(); //does not work correctly
					return;
				default:
					//IO::Portb::Write(parameter);
					Send1ByteResponse(header, RSP_ILLEGAL_PARAMETER);
			}
			Send1ByteResponse(header, RSP_OK); 
		}

		void SetBaund()
		{
			uint8_t baund = Read<uint8_t>();
			HwInterface::Disable();
			Send1ByteResponse(header, RSP_OK); 
			switch(baund)
			{
				case 0x05: HwInterface::Init(38400);break;
				case 0x06: HwInterface::Init(57600);break;
				case 0x07: HwInterface::Init(115200);break;
				case 0x04:
				default: HwInterface::Init(19200);break;
			}
		}

		void ProcessCommand(const MessageHeader &header)
		{
			//IO::Portb::Write(header.messageId);
			switch(header.messageId)
			{
				case CMND_SIGN_OFF:
					Send1ByteResponse(header, RSP_OK);
					break;
				case CMND_GET_SIGN_ON:
					SingOn(header); 
					break;

				case CMND_SET_PARAMETER:
					SetParameter(header); 
					break;
				case CMND_GET_PARAMETER:
					GetParameter(header); 
					break;

				case CMND_ENTER_PROGMODE:
					_target->EnterProgMode();
					Send1ByteResponse(header, RSP_OK);
					break;
				case CMND_LEAVE_PROGMODE:
					_target->LeaveProgMode();
					Send1ByteResponse(header, RSP_OK); 
					break;
				break;

				case CMND_SELFTEST:
					SelfTest(header);
				break;

				case CMND_WRITE_MEMORY:
					WriteMem(header);
				break;

				case CMND_READ_MEMORY:
					ReadMem(header);
				break;
				
				case CMND_SET_DEVICE_DESCRIPTOR:
					interface::Read(&deviceDescriptor, sizeof(deviceDescriptor));
					Send1ByteResponse(header, RSP_OK);
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
				case CMND_CLEAR_EVENTS:
				case CMND_RESTORE_TARGET:
				case CMND_ISP_PACKET:
				case CMND_JTAG_INSTR:
				case CMND_JTAG_DATA:
				case CMND_JTAG_SAB_WRITE:
				case CMND_JTAG_SAB_READ:
				case CMND_JTAG_BLOCK_READ:
				case CMND_JTAG_BLOCK_WRITE:
				case CMND_XMEGA_ERASE:

				default:
					//IO::Portb::Write(header.messageId);
					Send1ByteResponse(header, RSP_ILLEGAL_COMMAND);
			}
		}

		void WriteMem(const MessageHeader &header)
		{
			uint8_t memType = Read<uint8_t>();
			uint32_t size = Read<uint32_t>();
			uint32_t address = Read<uint32_t>();
			_target->WriteMem(memType, size, address);

			interface::Read<uint16_t>();//ignore crc

			Send1ByteResponse(header, RSP_OK);
		}

		void ReadMem(const MessageHeader &header)
		{
			uint8_t memType = interface::Read<uint8_t>();
			uint32_t size = interface::Read<uint32_t>();
			uint32_t address = interface::Read<uint32_t>();
			interface::Read<uint16_t>();//ignore crc
			SendMessageHeader(header, size+1, RSP_MEMORY);
			_target->ReadMem(memType, size, address);
			interface::EndTxFrame();
		}

		ParametersT params;
		DeviceDescriptor deviceDescriptor;
		MessageHeader header;
	};

}
