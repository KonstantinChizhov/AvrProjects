#include <QtCore/QCoreApplication>
#include "../Host/k_scope.h"
#include "../Host/exception.h"
#include <iostream>
#include <Windows.h>

void DACTest( K_Scope &device)
{
    std::cout << "\n\n============================================"<< std::endl;
    std::cout << "DAC writing test. " << std::endl;
    std::cout << "============================================"<< std::endl;
    unsigned time = GetTickCount();
    for(int k=0; k<1024; k++)
    {
        std::cout << "WriteDAC(" << k << ")\t";
        device.WriteDAC(k);
        QVector<unsigned short> res = device.GetAdcValuesRaw();
        std::cout << "Read from ADC " << res[0]-k << std::endl;
    }
    time = GetTickCount()-time;
    std::cout << "1024 values writed and readed in " << double(time)/1000.0 << " seconds." << std::endl;
    std::cout << 1024.0/(double(time)/1000.0) << " points per second." << std::endl;

}

void TransferSpeedTest( K_Scope &device, unsigned bufferSize, unsigned count)
{
    std::cout << "\n\n============================================"<< std::endl;
    std::cout << "Checking tranfer speed with buffer size " << bufferSize << std::endl;
    std::cout << "============================================"<< std::endl;
    unsigned time = GetTickCount();
    unsigned bytesRead=0, totalRead=0;

    for(int k=0; k<count; k++)
    {
        std::cout << "Requesting " << bufferSize << " bytes, ";
        bytesRead = device.TestTransfer(bufferSize);
        std::cout << "got " << bytesRead << " bytes" << std::endl;
        totalRead+=bytesRead;
    }
    time = GetTickCount()-time;
    std::cout << totalRead << " bytes readed in " << double(time)/1000.0 << " seconds in " << count << " reads" << std::endl;
    std::cout << "Transfer speed " << double(totalRead)/double(time) << " KBps" << std::endl;
}

void PingSpeedTest( K_Scope &device, unsigned count)
{
    std::cout << "\n\n============================================"<< std::endl;
    std::cout << "Checking ping speed " << std::endl;
    std::cout << "============================================"<< std::endl;
    unsigned time = GetTickCount();

    for(int k=0; k<count; k++)
    {
        std::cout << "Ping\t";
        if(!device.Ping()) break;
    }
    time = GetTickCount()-time;
    std::cout << count << " reads in " <<  double(time)/1000.0 << " seconds"  << std::endl;
    std::cout << count/(double(time)*0.001) << " pings per second"  << std::endl;
}

void GetScopeDataTest( K_Scope &device, unsigned bufferSize, unsigned count)
{
    std::cout << "\n\n============================================"<< std::endl;
    std::cout << "Checking tranfer speed with adc data with buffer size " << bufferSize << std::endl;
    std::cout << "============================================"<< std::endl;
    unsigned time = GetTickCount();
    unsigned bytesRead=0, totalRead=0;
    QVector<double> data;
    for(int k=0; k<count; k++)
    {
        std::cout << "Requesting " << bufferSize << " bytes, ";
        device.GetScopeData(data, bufferSize);
        bytesRead=data.size();
        std::cout << "got " << bytesRead << " bytes" << std::endl;
        totalRead+=bytesRead;
    }
    time = GetTickCount()-time;
    std::cout << totalRead << " bytes readed in " << double(time)/1000.0 << " seconds in " << count << " reads" << std::endl;
    std::cout << "Transfer speed " << double(totalRead)/double(time) << " KBps" << std::endl;
}

void GetScopeDataTest2( K_Scope &device, unsigned bufferSize, unsigned count)
{
    std::cout << "\n\n============================================"<< std::endl;
    std::cout << "Checking tranfer speed with adc data with buffer size " << bufferSize << std::endl;
    std::cout << "============================================"<< std::endl;
    unsigned time = GetTickCount();
    unsigned bytesRead=0, totalRead=0;
    QVector<double> data, data2;

    for(int k=0; k<count; k++)
    {
        std::cout << "Requesting " << bufferSize << " bytes, ";
        device.GetScopeData(data, data2, bufferSize);
        bytesRead=data.size()+data2.size();
        std::cout << "got " << bytesRead << " bytes" << std::endl;
        totalRead+=bytesRead;
    }
    time = GetTickCount()-time;
    std::cout << totalRead << " bytes readed in " << double(time)/1000.0 << " seconds in " << count << " reads" << std::endl;
    std::cout << "Transfer speed " << double(totalRead)/double(time) << " KBps" << std::endl;
}

void Test1()
{
    K_Scope device;
    device.Open();
    std::cout << "K-Scope running at " << device.GetMcuFreq()/1000.0/1000.0 << "MHz" << std::endl;
    device.SetADCSpeed(6);

    std::cout << "Ping\t\t";
    device.Ping();
    std::cout << "OK" << std::endl;

    std::cout << "GetAdcValues\t\t";
    QVector<double> res = device.GetAdcValues();
    std::cout << "OK" << std::endl;
    std::cout << "Values are:" << std::endl;
    for(int i=0; i<res.count(); i++)
        std::cout << res[i] << std::endl;

    PingSpeedTest(device, 1000);
    DACTest(device);
    TransferSpeedTest(device, 8, 100);
    TransferSpeedTest(device, 16, 100);
    TransferSpeedTest(device, 32, 100);
    TransferSpeedTest(device, 64, 100);
    TransferSpeedTest(device, 128, 100);
    TransferSpeedTest(device, 256, 100);
    TransferSpeedTest(device, 512, 100);

    device.SetADCSpeed(2);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

    device.SetADCSpeed(3);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

     device.SetADCSpeed(4);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

     device.SetADCSpeed(5);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

     device.SetADCSpeed(6);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

    device.SetADCSpeed(7);
    GetScopeDataTest(device, 1000, 20);
    GetScopeDataTest2(device, 1000, 20);

}


int main(int argc, char *argv[])
{
    try{
        Test1();
    }
    catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    system("pause");
}
