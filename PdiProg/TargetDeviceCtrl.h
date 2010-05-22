#pragma once
#include "ProgInterface.h"
#include "DeviceDescriptor.h"
#include "ProgrammerParameters.h"

class TargetDeviceCtrl
{
public:
	TargetDeviceCtrl(ProgParameters *progParams, DeviceDescriptor *deviceDescroptor)
	{
		_deviceDescriptor = deviceDescroptor;
		_progParams = progParams;
	}

	void SetProgInterface(ProgInterface *iface)
	{
		_progIface = iface;
	}
	virtual void EnterProgMode()=0;
	virtual void LeaveProgMode()=0;
	virtual uint32_t GetJTAGID()=0;
	virtual void ReadMem(uint8_t memType, uint32_t size, uint32_t address) = 0;
	virtual bool ReadMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address) = 0;
	virtual bool WriteMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address) = 0;
	virtual bool Erase(uint8_t memType, uint32_t address)= 0;
protected:
	ProgInterface *_progIface;
	DeviceDescriptor *_deviceDescriptor;
	ProgParameters *_progParams;
};

template<class Comm>
class NullTargetDeviceCtrl : public TargetDeviceCtrl
{
public:
	NullTargetDeviceCtrl()
		:TargetDeviceCtrl(0,0)
	{}

	virtual void EnterProgMode(){}
	virtual void LeaveProgMode(){}
	virtual uint32_t GetJTAGID()
	{
		return 0x0974C03F;
	}

	virtual void ReadMem(uint8_t memType, uint32_t size, uint32_t address)
	{
		for(uint32_t i=0; i<size; i++)
		{
			Comm::Write(0xff);
		}
	}

	virtual bool ReadMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address)
	{
		return false;
	}

	virtual bool WriteMem(uint8_t memType, uint8_t *buffer, uint32_t size, uint32_t address)
	{}

	virtual bool Erase(uint8_t memType, uint32_t address)
	{}
};
