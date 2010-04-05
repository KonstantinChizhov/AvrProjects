#include "dialog.h"
#include "ui_dialog.h"
#include "exception.h"
#include <QMessageBox>
#include "demodevice.h"

double scaleValues[10]={0.5, 1.0, 1.5, 2, 3, 4, 6, 8, 10, 15};
QVector<double> speeds;

Dialog::Dialog(QWidget *parent)
        : QDialog(parent), ui(new Ui::DialogClass)
{
    ui->setupUi(this);

    for(int i=0; i<5; i++)
        _chanels.append(&ui->Display->CreateNewCurve(MyGraph::LEFT));

    _chanels[0]->setPen(QPen(Qt::green));
    _chanels[1]->setPen(QPen(Qt::red));
    _chanels[2]->setPen(QPen(Qt::blue));
    _chanels[3]->setPen(QPen(Qt::yellow));
    _chanels[4]->setPen(QPen(Qt::yellow, 2.0));
    _device = new K_Scope();
}

Dialog::~Dialog()
{
    delete ui;
    delete _device;
}

void Dialog::on_Start_clicked()
{
    try{
        if(_device->IsOpen())
        {
            _device->Close();
            ui->Start->setText("Start");
            killTimer(_updateScopeEvent);
        }
        else
        {
            if(!_device->Open())
            {
                QMessageBox::critical(this, "Error", "Device not found");
                return;
            }
            _device->SetADCSpeed(ui->AdcSpeed->value());

            double vref=5.0;
            if(ui->VrefList->currentIndex()==0)
            {
                _device->SetVref(DeviceBase::VRef2_56v);
                vref = 2.56;
            }
            else
            {
                _device->SetVref(DeviceBase::VRef5v);
                vref = 5.0;
            }

            int devider = 0;
            if(ui->Ch1->checkState() == Qt::Checked)
                devider++;
            if(ui->Ch2->checkState() == Qt::Checked)
                devider++;
            if(ui->Ch3->checkState() == Qt::Checked)
                devider++;
            if(ui->Ch4->checkState() == Qt::Checked)
                devider++;

            if(devider == 0)
                devider = 1;

            double width = _device->GetMcuBufferSize() / devider;

            speeds = _device->GetAvaluebleAdcFreqs();
            ui->Display->SetDataRect(QRectF(0, vref, width, -vref));
            ui->Start->setText("Stop");
            _updateScopeEvent=startTimer(1);
        }
    }
    catch(Exception e)
    {
        QMessageBox::critical(this, "Error", e.what());
    }
    catch(...)
    {
        QMessageBox::critical(this, "Error", "Unknown error!");
    }
}

void Dialog::timerEvent(QTimerEvent *event)
{
    try{
        if(event->timerId()==_updateScopeEvent)
        {
            QVector<double> data, data2, data3, data4;

            int count = 0;
            if(ui->Ch1->checkState() == Qt::Checked)
                count++;
            if(ui->Ch2->checkState() == Qt::Checked)
                count++;
            if(ui->Ch3->checkState() == Qt::Checked)
                count++;
            if(ui->Ch4->checkState() == Qt::Checked)
                count++;

            if(count == 0)
                count = 1;

            if(count == 1 )
            {
                _device->GetScopeData(data);
            }
            if(count == 2)
            {
                _device->GetScopeData(data, data2);
            }
            if(count == 3)
            {
                _device->GetScopeData(data, data2, data3);
            }
            if(count == 4)
            {
                _device->GetScopeData(data, data2, data3, data4);
            }

            QVector<double> dif;
            if(ui->Dif->checkState() == Qt::Checked && count>=2)
            {
                dif.resize(data.size());
                for(int i=0; i<data.size(); i++)
                {
                    dif[i] = data[i]-data2[i]+2.5;
                }
            }

            double vref=5.0;
            if(ui->VrefList->currentIndex()==0)
            {
                _device->SetVref(DeviceBase::VRef2_56v);
                vref = 2.56;
            }
            else
            {
                _device->SetVref(DeviceBase::VRef5v);
                vref = 5.0;
            }

            double width = _device->GetMcuBufferSize() / count;

            speeds = _device->GetAvaluebleAdcFreqs();
            ui->Display->SetDataRect(QRectF(0, vref, width, -vref));

            _chanels[0]->SetData(data);
            _chanels[1]->SetData(data2);
            _chanels[2]->SetData(data3);
            _chanels[3]->SetData(data4);
            _chanels[4]->SetData(dif);
            ui->Display->update();
        }
    }
    catch(TimeOutException)
    {
        _device->Close();
        _device->Open();
    }
    catch(Exception e)
    {
        QMessageBox::critical(this, "Error", e.what());
        killTimer(_updateScopeEvent);
    }
    catch(...)
    {
        QMessageBox::critical(this, "Error", "Unknown error!");
        killTimer(_updateScopeEvent);
    }
}



void Dialog::on_AdcSpeed_valueChanged(int value)
{
    _device->SetADCSpeed(value+1);
    ui->SampleRate->display(speeds[value]*0.001);
}

void Dialog::on_XScale_valueChanged(int value)
{
    ui->Display->SetDataScaleX(scaleValues[value]);
}

void Dialog::on_XOffset_valueChanged(int value)
{
    ui->Display->SetDataOffsetX(value);
}

void Dialog::on_Ch1_stateChanged(int )
{

}

void Dialog::on_Ch2_stateChanged(int )
{

}

void Dialog::on_Ch3_stateChanged(int )
{

}

void Dialog::on_Ch4_stateChanged(int )
{

}

void Dialog::on_Ch1Gane_valueChanged(int value)
{
    _chanels[0]->SetScale(scaleValues[value]);
}

void Dialog::on_Ch2Gane_valueChanged(int value)
{
    _chanels[1]->SetScale(scaleValues[value]);
}

void Dialog::on_Ch3Gane_valueChanged(int value)
{

}

void Dialog::on_Ch4Gane_valueChanged(int value)
{

}

void Dialog::on_Ch1Offset_valueChanged(int value)
{
    _chanels[0]->SetOffset(value*0.1);
}

void Dialog::on_Ch2Offset_valueChanged(int value)
{
    _chanels[1]->SetOffset(value*0.1);
}

void Dialog::on_Ch3Offset_valueChanged(int value)
{

}

void Dialog::on_Ch4Offset_valueChanged(int value)
{

}

void Dialog::on_VrefList_activated(int index)
{
    if(index==0)
        _device->SetVref(DeviceBase::VRef2_56v);
    else
        _device->SetVref(DeviceBase::VRef5v);


}
