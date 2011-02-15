#ifndef K_SCOPE_H
#define K_SCOPE_H
#include <QVector>
#include "devicebase.h"

class K_Scope :public DeviceBase
{
public:
    //devicebase members
    K_Scope();
    ~K_Scope();
    bool Open();
    void Close();
    void SetVref(VRef value);
    void SetADCSpeed(unsigned value);
    double GetFreq();
    int GetMcuFreq();
    int GetMcuBufferSize();
    QVector<double> GetAvaluebleAdcFreqs();
    QVector<double> GetAdcValues();
    void GetScopeData(QVector<double> &ch1, int size=-1);
    void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, int size=-1);
    void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3,int size=-1);
    void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3, QVector<double> &ch4, int size=-1);
    void WriteDAC(unsigned value);
    double VoltsPerUnit();
    bool IsOpen();
    //end of devicebase members
    //Testing
    bool Ping();
    unsigned TestTransfer(unsigned byteCount);
    QVector<unsigned short> GetAdcValuesRaw();
protected:
    void CheckOpen();
    struct usb_dev_handle *_dev;
    enum VRef _vref;
    unsigned _adcSpeed;
    unsigned _deviceBufferSize;
};

#endif // K_SCOPE_H
