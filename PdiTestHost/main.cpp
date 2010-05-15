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

    dcb.BaudRate = baund;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hCOM, &dcb);

    GetCommTimeouts(hCOM,&CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 500;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    CommTimeouts.ReadTotalTimeoutConstant = 500;
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
    if(Errors)
        std::cout << "Error = " << Errors << std::endl;
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

ComPort port(L"COM1", 115200);

        enum
        {
                CMD_LDS               = 0x00,
                CMD_LD                = 0x20,
                CMD_STS               = 0x40,
                CMD_ST                = 0x60,
                CMD_LDCS              = 0x80,
                CMD_REPEAT            = 0xA0,
                CMD_STCS              = 0xC0,
                CMD_KEY               = 0xE0,

                STATUS_REG            = 0x0,
                RESET_REG             = 0x1,
                CTRL_REG              = 0x2,

                STATUS_NVM            = 0x02,
                RESET_KEY             = 0x59,

                DATSIZE_1BYTE         = 0x0,
                DATSIZE_2BYTES        = 0x1,
                DATSIZE_3BYTES        = 0x2,
                DATSIZE_4BYTES        = 0x3,

                POINTER_INDIRECT      = 0x0,
                POINTER_INDIRECT_PI   = 0x1,
                POINTER_DIRECT        = 0x2
        };

void WriteCmd(uint8_t cmd)
{
    //port.WriteBuffer("SW", 2);
    port.WriteBuffer(&cmd, 1);
}

template<class T>
void WriteCmd(uint8_t cmd, T value)
{
    port.WriteBuffer("SW", 2);
    port.WriteBuffer(&cmd, 1);
    port.WriteBuffer(&value, sizeof(T));
}

uint8_t Read()
{
    int16_t value=0;
    if(!port.ReadBuffer(&value, 1))
        std::cout << "timeout" << std::endl;
    else
        std::cout << "value = " << value << std::endl;
    port.Purge();
    return value;
}

int main(int argc, char *argv[])
{
    int16_t value=0;
    uint8_t cmd = 0;
    for(;;)
    {
       //WriteCmd('A');
       Read();
       Sleep(10);
    }


    system("pause");
}
