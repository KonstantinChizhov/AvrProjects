#ifndef __COMPORT_H__
#define __COMPORT_H__



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

#endif // __COMPORT_H__
