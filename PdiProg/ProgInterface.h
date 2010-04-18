#pragma once

class ProgInterface
{
public:
	virtual void Enable()=0;
	virtual void Disable()=0;
	virtual void Write(uint8_t c)=0;
	virtual uint8_t Read()=0;
	virtual void Reset()=0;
	virtual void Break()=0;
	virtual void Idle()=0;
};

class NullProgInterface :public ProgInterface
{
	virtual void Enable(){}
	virtual void Disable(){}
	virtual void Write(uint8_t c){}
	virtual uint8_t Read(){return 0xff;}
	virtual void Reset(){}
	virtual void Break(){}
	virtual void Idle(){}
};
