#pragma once

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
		unsigned int uiSramStartAddr; 		// Start of SRAM
		unsigned char ucResetType; 			// Selects reset type. 0x00
		unsigned char ucPCMaskExtended; 	// For parts with extended PC
		unsigned char ucPCMaskHigh; 		// PC high mask
		unsigned char ucEindAddress; 		// EIND IO address
		unsigned int EECRAddress; 			// EECR IO address
	};
