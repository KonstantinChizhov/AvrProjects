//*****************************************************************************
//
// Title		: TextFormater template is used to perform text formatted IO.
// Author		: Konstantin Chizhov
// Date			: 2010
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// As a special exception, you may use this file as part of a free software
// library without restriction. Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License. This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//*****************************************************************************

#pragma once


template<class DATA_SOURCE, uint8_t fieldSize=8>
class TextFormater :public DATA_SOURCE
{
public:

	TextFormater<DATA_SOURCE>& operator<< (int value)
	{
		char buffer[7];
		itoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (long value)
	{
		char buffer[11];
		ltoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (unsigned long value)
	{
		char buffer[11];
		ultoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}
	
	TextFormater<DATA_SOURCE>& operator<< (unsigned value)
	{
		char buffer[7];
		utoa(value, buffer, 10);
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (double value)
	{
		const uint8_t presision=2;
		char buffer[fieldSize+1];
		if(value < Pow<10, fieldSize - presision - 1>::value - 0.1)
		{
			dtostrf(value, fieldSize - presision - 1, presision, buffer);
		}
		else
		{
			dtostre(value, buffer, fieldSize - 6, 0);
		}
		Puts(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator<< (const char *value)
	{
		Puts(value);
		return *this;
	}

	void Write(const void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Putch(((uint8_t*)data)[i]);
		}
	}

	void WriteP(const void *data, uint16_t size)
	{
		uint8_t *ptr=(uint8_t*)data;
		for(uint16_t i=0; i<size; ++i)
		{
			uint8_t c = pgm_read_byte(ptr);
			DATA_SOURCE::Putch(c);
			ptr++;
		}
	}

	void WriteF(const void *data, uint16_t size)
	{
		uint8_t *ptr=(uint8_t*)data;
		for(uint16_t i=0; i<size; ++i)
		{
			uint8_t c = eeprom_read_byte(ptr);
			DATA_SOURCE::Putch(c);
			ptr++;
		}
	}

	void Puts(const char *str)
	{
		while(*str)
		{
			DATA_SOURCE::Putch(*str);
			str++;
		}
	}

	void PutsP(const char *str)
	{
		uint8_t c;
		while((c = pgm_read_byte(str++)))
		{
			DATA_SOURCE::Putch(c);
		}
	}

	void PutsF(const char *str)
	{
		uint8_t c;
		while((c = eeprom_read_byte((uint8_t*)str++)))
		{
			DATA_SOURCE::Putch(c);
		}
	}

	void Read(void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Getch(((uint8_t*)data)[i]);
		}
	}

	void Gets(char *buffer, uint8_t len)
	{
		uint8_t c, pos=0;
		do
		{
			DATA_SOURCE::Getch(c);
			if(c=='\r')
			{
				DATA_SOURCE::Getch(c);
				break;
			}
			buffer[pos++]=c;
			if(pos>=len)break;
		}
		while(c!='\n' && c!=0);
		buffer[pos]=0;
	} 

	TextFormater<DATA_SOURCE>& operator>> (unsigned &value)
	{
		char buffer[7];
		Gets(buffer, sizeof(buffer));
		value = atoi(buffer);
		return *this;
	}

	TextFormater<DATA_SOURCE>& operator>> (unsigned long &value)
	{
		char buffer[11];
		Gets(buffer, sizeof(buffer));
		value = atol(buffer);
		return *this;
	}

	template<class T>
	void Write(T value)
	{
		Write(&value, sizeof(T));
	}

	template<class T>
	T Read()
	{
		T value;
		Read(&value, sizeof(T));
		return value;
	}
};
