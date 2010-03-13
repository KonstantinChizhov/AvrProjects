#include "k_scope.h"
#include <usb.h>
#include "../Firmware/requests.h"
#include "../Firmware/USBCONFIG.H"
#include "opendevice.h"
#include "exception.h"

K_Scope::K_Scope()
{
    _dev=0;
    _vref = VRef5v;
    _deviceBufferSize =0;
    SetADCSpeed(5);
}

K_Scope::~K_Scope()
{
    Close();
}

bool K_Scope::Open()
{
    Close();
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    int                 vid, pid;

    usb_init();

    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];

    if(usbOpenDevice(&_dev, vid, vendor, pid, product, NULL, NULL, stdout) != 0)
    {
        _dev=0;
        throw DeviceNotFoundException("K-Scope device is not found.");
    }

    _deviceBufferSize = GetMcuBufferSize();

    return true;
}

void K_Scope::Close()
{
    if(_dev)
        usb_close(_dev);
    _dev=0;
}

void K_Scope::SetVref(VRef value)
{
    _vref = value;
}

double K_Scope::GetFreq()
{
    double mcuFreq = GetMcuFreq();
    double result = mcuFreq/13.0/_adcSpeed;
    return result;
}

int K_Scope::GetMcuFreq()
{
    CheckOpen();
    int value = 0;
    int index = 0;
    char buffer[4]={0};
    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_CPU_FREQ,
                                value,
                                index,
                                buffer,
                                sizeof(buffer),
                                1000);

    ThrowErrCode(count, LOCATION);

    return *(int*)buffer;
}

int K_Scope::GetMcuBufferSize()
{
    CheckOpen();
    int value = 0;
    int index = 0;
    char buffer[4]={0};
    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_BUFFER_SIZE,
                                value,
                                index,
                                buffer,
                                2,
                                1000);

    ThrowErrCode(count, LOCATION);

    return *(int*)buffer;
}

QVector<double> K_Scope::GetAvaluebleAdcFreqs()
{
    QVector<double> result;
    double mcuFreq = GetMcuFreq();
    for(int i=1; i<=8; i++)
    {
        result.append(mcuFreq/13.0/(1<<i));
    }
    return result;
}

QVector<double> K_Scope::GetAdcValues()
{
    CheckOpen();
    int value = (_adcSpeed);
    int index = 0;

    unsigned short buffer[8]={0};

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_1_SAMPLE,
                                value,
                                index,
                                (char*)buffer,
                                sizeof(buffer),
                                1000);

    QVector<double> result(8);
    ThrowErrCode(count, LOCATION);

    double vPerUnit = VoltsPerUnit()*0.25;

    for(int i=0; i<8; i++)
    {
        result[i]=double(buffer[i])*vPerUnit;
    }
    return result;
}

void K_Scope::GetScopeData(QVector<double> &ch1, int size)
{
    CheckOpen();
    unsigned char buffer[_deviceBufferSize];
    int value = (_adcSpeed&7) | (0)<<8;
    int index = 0;
    if(size<=0)
        size=_deviceBufferSize;


    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_1_CHANEL,
                                value,
                                index,
                                (char*)buffer,
                                size,
                                1000);
    ThrowErrCode(count, LOCATION);

    ch1.resize(count);

    double vPerUnit = VoltsPerUnit();

    for(unsigned i=0; i<ch1.size(); i++)
    {
        ch1[i]=double(buffer[i])*vPerUnit;
    }
    return;
}

void K_Scope::GetScopeData(QVector<double> &ch1, QVector<double> &ch2, int size)
{
    CheckOpen();
    unsigned char buffer[_deviceBufferSize];
    int value = (_adcSpeed&7) | (0)<<8;
    int index = 0x0100;

    if(size<=0)
        size=_deviceBufferSize;

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_2_CHANEL,
                                value,
                                index,
                                (char*)buffer,
                                size,
                                1000);
    ThrowErrCode(count, LOCATION);

    ch1.resize(count/2+1);
    ch2.resize(count/2+1);

    double vPerUnit = VoltsPerUnit();

    for(unsigned i=0; i<count; i+=2)
    {
        ch1[i/2]=double(buffer[i])*vPerUnit;
        ch2[i/2]=double(buffer[i+1])*vPerUnit;
    }
    return;
}

 void K_Scope::GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3,int size)
 {
    CheckOpen();
    unsigned char buffer[_deviceBufferSize];
    int value = (_adcSpeed&7) | (0)<<8;
    int index = 0x0100;

    if(size<=0)
        size=_deviceBufferSize;

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_3_CHANEL,
                                value,
                                index,
                                (char*)buffer,
                                size,
                                1000);
    ThrowErrCode(count, LOCATION);

    ch1.resize(count/3+1);
    ch2.resize(count/3+1);
    ch3.resize(count/3+1);

    double vPerUnit = VoltsPerUnit();

    for(unsigned i=0; i<count; i+=3)
    {
        ch1[i/3]=double(buffer[i])*vPerUnit;
        ch2[i/3]=double(buffer[i+1])*vPerUnit;
        ch3[i/3]=double(buffer[i+2])*vPerUnit;
    }
    return;
 }

 void K_Scope::GetScopeData(QVector<double> &ch1, QVector<double> &ch2, QVector<double> &ch3, QVector<double> &ch4,int size)
 {
 CheckOpen();
    unsigned char buffer[_deviceBufferSize];
    int value = (_adcSpeed&7) | (0)<<8;
    int index = 0x0100;

    if(size<=0)
        size=_deviceBufferSize;

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_4_CHANEL,
                                value,
                                index,
                                (char*)buffer,
                                size,
                                1000);
    ThrowErrCode(count, LOCATION);

    ch1.resize(count/4+1);
    ch2.resize(count/4+1);
    ch3.resize(count/4+1);
    ch4.resize(count/4+1);

    double vPerUnit = VoltsPerUnit();

    for(unsigned i=0; i<count; i+=4)
    {
        ch1[i/4]=double(buffer[i])*vPerUnit;
        ch2[i/4]=double(buffer[i+1])*vPerUnit;
        ch3[i/4]=double(buffer[i+2])*vPerUnit;
        ch4[i/4]=double(buffer[i+3])*vPerUnit;
    }
    return;
 }

void  K_Scope::WriteDAC(unsigned value)
{
    CheckOpen();
    int index = 0;

    char buffer[16];

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_WRITE_DAC,
                                value,
                                index,
                                buffer,
                                sizeof(buffer),
                                1000);
    ThrowErrCode(count, LOCATION);
}

void K_Scope::SetADCSpeed(unsigned value)
{
    _adcSpeed = value&0x07;
}

double K_Scope::VoltsPerUnit()
{
    double vPerUnit = 5.0/256.0;
    if(_vref == VRef2_56v)
        vPerUnit = 2.56/256.0;
    return vPerUnit;
}

bool K_Scope::IsOpen()
{
    return _dev!=0;
}

void  K_Scope::CheckOpen()
{
    if(!IsOpen())
        throw Exception("Device is not open");
}

bool K_Scope::Ping()
{
    CheckOpen();
    int index = 0;
    int value = 0;
    char buffer[8]={0};
    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_PING,
                                value,
                                index,
                                buffer,
                                sizeof(buffer),
                                1000);
    if(count != 0)
        return false;
    return true;
}

unsigned K_Scope::TestTransfer(unsigned byteCount)
{
    CheckOpen();
    int index = 0;
    int value = 0;
    char buffer[byteCount];

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_TRANSFER_TEST,
                                value,
                                index,
                                buffer,
                                byteCount,
                                1000);

    ThrowErrCode(count, LOCATION);

    return (unsigned)count;
}

QVector<unsigned short> K_Scope::GetAdcValuesRaw()
{
    CheckOpen();
    int value = (_adcSpeed);
    int index = 0;

    unsigned short buffer[8]={0};

    int count = usb_control_msg(_dev,
                                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                                CUSTOM_RQ_GET_ADC_1_SAMPLE,
                                value,
                                index,
                                (char*)buffer,
                                sizeof(buffer),
                                1000);

    ThrowErrCode(count, LOCATION);

    QVector<unsigned short> result(8);
    for(int i=0; i<8; i++)
        result[i]=buffer[i];
    return result;
}
