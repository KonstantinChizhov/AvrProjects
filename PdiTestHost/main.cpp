#include <iostream>
#include <windows.h>

#include <stdlib.h>
#include <math.h>

class ComPort
{

public:
    ComPort(wchar_t *portName = L"COM1",  unsigned int baund=115200);
    ~ComPort();
    virtual bool ReadBuffer(void *buffer, unsigned int size);
    virtual unsigned int BytesAvailable();
    bool WriteBuffer(const void *buffer, unsigned int size);
    virtual void Purge();
    void Flush();
protected:
    void *hCOM;
};


ComPort::ComPort(wchar_t *portName, unsigned int baund)
{
    hCOM = CreateFileW(portName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,  NULL, OPEN_EXISTING,  0,  NULL);

    if(hCOM == INVALID_HANDLE_VALUE)
        throw std::string("Unable to open port ");
    DCB dcb={0};
    COMMTIMEOUTS CommTimeouts;
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hCOM, &dcb);

    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hCOM, &dcb);

    GetCommTimeouts(hCOM,&CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 100;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    CommTimeouts.ReadTotalTimeoutConstant = 100;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts(hCOM,&CommTimeouts);
    FlushFileBuffers(hCOM);
    DWORD Errors;
    COMSTAT ComState;
    ClearCommError(hCOM, &Errors, &ComState);
}

ComPort::~ComPort()
{
    CloseHandle(hCOM);
}

void ComPort::Purge()
{
    PurgeComm(hCOM, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

void ComPort::Flush()
{
    FlushFileBuffers(hCOM);
}

bool ComPort::WriteBuffer(const void *buffer, unsigned int size)
{
    DWORD nb;
    bool res = WriteFile(hCOM, buffer, size, &nb, NULL);
    return res && size==nb;
}

bool ComPort::ReadBuffer(void *buffer, unsigned int size)
{
    DWORD nb, Errors;
    COMSTAT ComState;
    ClearCommError(hCOM, &Errors, &ComState);
   //if(ComState.cbInQue < size)
    //{
    //    return false;
    //}
    bool res = ReadFile(hCOM, buffer, size, &nb, NULL);
    return res & nb==size;
}

unsigned int ComPort::BytesAvailable()
{
    DWORD Errors;
    COMSTAT ComState;
    ClearCommError(hCOM, &Errors, &ComState);
    return ComState.cbInQue;
}

inline uint16_t div5(uint16_t num, uint16_t &rem)
{
    uint16_t q = num*0x6667 >> 16;
    q >>= 1;
    rem = num - q*5;
    return q;
}



int main(int argc, char *argv[])
{
    ComPort port;
    int16_t delta=0, old=0;
    for(;;)
    {
       if(port.ReadBuffer(&delta, 2))
       {
           std::cout << delta << "\t";
       }
       else Sleep(1);
       old = delta;
    }


    system("pause");
}
