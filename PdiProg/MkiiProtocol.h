
#pragma once

#include "Crc16.h"
#include "ports.h"
#include "xMega.h"
#include "ProgInterface.h"
#include "TargetDeviceCtrl.h"
#include "constants.h"
#include "DeviceDescriptor.h"
#include "CheckSummUpdater.h"
#include "ProgrammerParameters.h"


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
		ProgParameters _params;
		DeviceDescriptor _deviceDescriptor;
		MessageHeader _header;

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
		}

		void SendMessageHeader(const MessageHeader &_header, uint16_t size, Responses response)
		{
			interface::BeginTxFrame();
			interface::Write(uint8_t(MessageStart));
			interface::Write(_header.seqNumber);
			interface::Write(size);
			interface::Write(uint8_t(Token));
			interface::Write(uint8_t(response));
		}

		void Send1ByteResponse(const MessageHeader &_header, Responses response)
		{
			SendMessageHeader(_header, 1, response);
			interface::EndTxFrame();
		}

		template<class T>
		void SendResponse(const MessageHeader &_header, Responses response, const T&value)
		{
			SendMessageHeader(_header, sizeof(value)+1, response);
			interface::Write(value);
			interface::EndTxFrame();
		}

		template<class T>
		void ParamResponse(const MessageHeader &_header, const T&value)
		{
			SendResponse(_header, RSP_PARAMETER, value);
		}

		void SingOn(const MessageHeader &_header)
		{
			SendMessageHeader(_header, 29, RSP_SIGN_ON);

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

		void SelfTest(const MessageHeader &_header)
		{
			uint8_t flags = interface::Read();
			SendMessageHeader(_header, 9, RSP_SELFTEST);
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
			{
				//_progIface->Idle();
				return;
			}

			if(ch != MessageStart)
			{
				//_progIface->Idle();
				return;
			}
			
			_header.seqNumber = interface::ReadU16();
			IO::Portb::Write(_header.seqNumber);
			_header.messageLen = interface::ReadU32();

			if(interface::Read() != Token)
			{
				//_progIface->Idle();
				return;
			}

			_header.messageId = interface::Read();

			ProcessCommand(_header);
			uint16_t crc = interface::ReadU16();
		}

		void GetParameter(const MessageHeader &_header)
		{
			uint8_t parameter = interface::Read();
			//IO::Portb::Write(parameter);
			switch(parameter)
			{
				case BaudRate:
					ParamResponse<uint8_t>(_header, 0x04);
					break;
				case OCD_Vtarget:
					ParamResponse<uint16_t>(_header, 0x0ce4);//3.3 v
					break;
				case DaisyChainInfo:
					ParamResponse<ProgDaisyChainInfo>(_header, _params.ChainInfo );
					break;
				case ExternalReset:
					ParamResponse<uint8_t>(_header, _params.ExternalReset);
					break;
				case EmulatorMODE:
					ParamResponse<uint8_t>(_header, _params.EmuMode);
					break;
				case OCD_JTAG_Clock:
					ParamResponse<uint8_t>(_header, _params.JTAG_Clock);
					break;
				case FlashPageSize:
					ParamResponse<uint16_t>(_header, _params.FlashPageSize);
					break;
				case EEPROMPageSize:
					ParamResponse<uint8_t>(_header, _params.EEPROMPageSize);
					break;
				case BootAddress:
					ParamResponse<uint32_t>(_header, _params.BootAddress);
					break;
				case JTAGID:
					ParamResponse<uint32_t>(_header, _target->GetJTAGID());
					break;
				case PDI_NVM_Offset:
					ParamResponse<uint32_t>(_header, _params.PDI_NVM_Offset);
					break;
 				case PDI_FlashOffset:
					ParamResponse<uint32_t>(_header, _params.PDI_FlashOffset);
					break;
	 			case PDI_FlashBootOffset:
					ParamResponse<uint32_t>(_header, _params.PDI_FlashBootOffset);
					break;

				default:
					//::Portb::Write(parameter);
					Send1ByteResponse(_header, RSP_ILLEGAL_PARAMETER);
			}			
		}

		void SetParameter(const MessageHeader &_header)
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
					_params.BootAddress = interface::Read();
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
					return;
				default:
					//IO::Portb::Write(parameter);
					Send1ByteResponse(_header, RSP_ILLEGAL_PARAMETER);
			}
			Send1ByteResponse(_header, RSP_OK); 
		}

		void SetBaund()
		{
			uint8_t baund = interface::Read();
			HwInterface::Disable();
			Send1ByteResponse(_header, RSP_OK); 
			switch(baund)
			{
				case 0x05: HwInterface::Init(38400);break;
				case 0x06: HwInterface::Init(57600);break;
				case 0x07: HwInterface::Init(115200);break;
				case 0x04:
				default: HwInterface::Init(19200);break;
			}
		}

		void ProcessCommand(const MessageHeader &_header)
		{
			//IO::Portb::Write(_header.messageId);
			switch(_header.messageId)
			{
				case CMND_SIGN_OFF:
					Send1ByteResponse(_header, RSP_OK);
					break;
				case CMND_GET_SIGN_ON:
					SingOn(_header); 
					break;

				case CMND_SET_PARAMETER:
					SetParameter(_header); 
					break;
				case CMND_GET_PARAMETER:
					GetParameter(_header); 
					break;

				case CMND_ENTER_PROGMODE:
					_target->EnterProgMode();
					Send1ByteResponse(_header, RSP_OK);
					break;
				case CMND_LEAVE_PROGMODE:
					_target->LeaveProgMode();
					Send1ByteResponse(_header, RSP_OK); 
					break;
				break;

				case CMND_SELFTEST:
					SelfTest(_header);
				break;

				case CMND_WRITE_MEMORY:
					WriteMem(_header);
				break;

				case CMND_READ_MEMORY:
					ReadMem(_header);
				break;
				
				case CMND_SET_DEVICE_DESCRIPTOR:
					interface::Read(&_deviceDescriptor, sizeof(_deviceDescriptor));
					Send1ByteResponse(_header, RSP_OK);
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
					//IO::Portb::Write(_header.messageId);
					Send1ByteResponse(_header, RSP_ILLEGAL_COMMAND);
			}
		}

		void WriteMem(const MessageHeader &_header)
		{
			uint8_t memType = interface::Read();
			uint32_t size = interface::ReadU32();
			uint32_t address = interface::ReadU32();
			_target->WriteMem(memType, size, address);

			

			Send1ByteResponse(_header, RSP_OK);
		}

		void ReadMem(const MessageHeader &_header)
		{
			uint8_t memType = interface::Read();
			uint32_t size = interface::ReadU32();
			uint32_t address = interface::ReadU32();

			SendMessageHeader(_header, size+1, RSP_MEMORY);
			_target->ReadMem(memType, size, address);
			interface::EndTxFrame();
		}
	};

}
