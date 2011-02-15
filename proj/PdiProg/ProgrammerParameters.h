#pragma once
#include "constants.h"

//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
struct ProgDaisyChainInfo
{
	uint8_t UnitsBefore;
	uint8_t UnitsAfter;
	uint8_t BitsBefore;
	uint8_t BitsAfter;
};

struct ProgParameters
{
	EmulatorMode EmuMode;
	ProgDaisyChainInfo ChainInfo;
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
