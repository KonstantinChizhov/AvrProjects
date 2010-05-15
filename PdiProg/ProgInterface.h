#pragma once

class ProgInterface
{
public:
	virtual void Enable()=0;
	virtual void Disable()=0;
	virtual void WriteByte(uint8_t c)=0;
	virtual uint8_t ReadByte()=0;
	virtual void Reset()=0;
	virtual void Break()=0;
	
	template<class T>
	void Write(const T &value)
	{
		const uint8_t * rawData = reinterpret_cast<const uint8_t *>(&value);
		Write(rawData, sizeof(T));
	}

	void Write(const uint8_t *value, const size_t size)
	{
		for(uint8_t i=0; i<size; ++i)
			this->WriteByte(value[i]);
	}

	template<class T>
	T Read()
	{
		T value;
		uint8_t * rawData = reinterpret_cast<uint8_t*>(&value);
		Read(rawData, sizeof(T));
		return value;
	}

	void Read(void *value, const size_t size)
	{
		uint8_t * data = reinterpret_cast<uint8_t*>(value);
		for(uint8_t i=0; i<size; ++i)
			data[i] = this->ReadByte();
	}
};

class NullProgInterface :public ProgInterface
{
	virtual void Enable(){}
	virtual void Disable(){}
	virtual void WriteByte(uint8_t c){}
	virtual uint8_t ReadByte(){return 0xff;}
	virtual void Reset(){}
	virtual void Break(){}
};
