#include "demodevice.h"
#include <math.h>
#include <stdlib.h>

DemoDevice::DemoDevice()
{
    this->_isOpen=false;
    SetADCSpeed(32);
    SetVref(VRef5v);
}


bool DemoDevice::Open()
{
    this->_isOpen=true;
    return true;
}

void DemoDevice::Close()
{
this->_isOpen=false;
}

void DemoDevice::SetVref(VRef value)
{
    _vref = value;
}

double DemoDevice::GetFreq()
{
    return GetMcuFreq()/13.0/32.0;
}

int DemoDevice::GetMcuFreq()
{
    return 12000000;
}

int DemoDevice::GetMcuBufferSize()
{
    return 800;
}

QVector<double> DemoDevice::GetAvaluebleAdcFreqs()
{
    QVector<double> result;
    double mcuFreq = GetMcuFreq();
    for(int i=1; i<=8; i++)
    {
        result.append(mcuFreq/13.0/(1<<i));
    }
    return result;
}

QVector<double> DemoDevice::GetAdcValues()
{
    unsigned short values[8];
    for(int i=0; i<8; i++)
        values[i] = 100 + rand()%600;
    QVector<double> result(8);

    double vPerUnit = VoltsPerUnit()*0.25;

    for(int i=0; i<8; i++)
    {
        result[i]=double(values[i])*vPerUnit;
    }
    return result;
}

void DemoDevice::GetScopeData(QVector<double> &ch1, int size)
{
    unsigned char buffer[800];

    ch1.resize(800);

    double phaze=rand()/double(RAND_MAX);
    double amp=100;

    for(unsigned i=0; i<800; i++)
    {
        buffer[i]=(unsigned char)(sin(phaze+i*0.01)*amp)+110+rand()%2;
    }

    double vPerUnit = VoltsPerUnit();

    for(unsigned i=0; i<800; i++)
    {
        ch1[i]=double(buffer[i])*vPerUnit;
    }
    return;
}

void DemoDevice::GetScopeData(QVector<double> &ch1, QVector<double> &ch2, int size)
{
    GetScopeData(ch1);
    GetScopeData(ch2);
}

void  DemoDevice::WriteDAC(unsigned value)
{

}

void DemoDevice::SetADCSpeed(unsigned value)
{
    _adcSpeed = value&0xff;
}

double DemoDevice::VoltsPerUnit()
{
    double vPerUnit = 5.0/256.0;
    if(_vref == VRef2_56v)
        vPerUnit = 2.56/256.0;
    return vPerUnit;
}

bool DemoDevice::IsOpen()
{
    return _isOpen;
}
