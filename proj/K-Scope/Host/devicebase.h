#ifndef DEVICEBASE_H
#define DEVICEBASE_H
#include <QVector>

class DeviceBase
{
public:
    enum VRef{VRef2_56v=0, VRef5v=1};
    DeviceBase();
    virtual ~DeviceBase(){}
    virtual bool Open()=0;
    virtual void Close()=0;
    virtual void SetVref(VRef value)=0;
    virtual void SetADCSpeed(unsigned value)=0;
    virtual double GetFreq()=0;
    virtual int GetMcuFreq()=0;
    virtual int GetMcuBufferSize()=0;
    virtual  QVector<double> GetAvaluebleAdcFreqs()=0;
    virtual QVector<double> GetAdcValues()=0;
    virtual void GetScopeData(QVector<double> &ch1, int size=-1)=0;
    virtual void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, int size=-1)=0;
    virtual void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3,int size=-1)=0;
    virtual void GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3, QVector<double> &ch4, int size=-1)=0;
    virtual void WriteDAC(unsigned value)=0;
    virtual double VoltsPerUnit()=0;
    virtual  bool IsOpen()=0;

};

#endif // DEVICEBASE_H
