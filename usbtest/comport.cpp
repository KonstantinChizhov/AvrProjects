
#include "ComPort.h"
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <string>
#include <stdexcept>

ComPort::ComPort(wchar_t *portName, unsigned int baund)
{
    hCOM = CreateFileW(portName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,  NULL, OPEN_EXISTING,  0,  NULL);

    if(hCOM == INVALID_HANDLE_VALUE)
        throw std::runtime_error ("Unable to open port ");
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
    //if(ComState.cbInQue < size) return false;
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
