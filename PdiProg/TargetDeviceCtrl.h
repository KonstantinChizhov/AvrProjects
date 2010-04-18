#pragma once
#include "ProgInterface.h"

class TargetDeviceCtrl
{
public:
	virtual void SetProgInterface(ProgInterface *iface)
	{
		_iface = iface;
	}
	virtual void EnterProgMode()=0;
	virtual void LeaveProgMode()=0;
	virtual uint32_t GetJTAGID()=0;
	virtual void ReadMem(uint8_t memType, uint32_t size, uint32_t address) = 0;
protected:
	ProgInterface *_iface;
};

class NullTargetDeviceCtrl : public TargetDeviceCtrl
{
	virtual void EnterProgMode(){}
	virtual void LeaveProgMode(){}
	virtual uint32_t GetJTAGID()
	{
		return 0;
	}

	virtual void ReadMem(uint8_t memType, uint32_t size, uint32_t address)
	{}
};
