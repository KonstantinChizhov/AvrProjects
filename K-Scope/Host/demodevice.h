#ifndef DEMODEVICE_H
#define DEMODEVICE_H
#include "devicebase.h"

class DemoDevice : public DeviceBase
{
public:
    DemoDevice();
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
    void WriteDAC(unsigned value);
    double VoltsPerUnit();
    bool IsOpen();
private:
    bool _isOpen;
    enum VRef _vref;
    unsigned  _adcSpeed;
};

#endif // DEMODEVICE_H
