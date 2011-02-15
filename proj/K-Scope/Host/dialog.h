#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>
#include "gr_data.h"
#include <QVector>
#include "k_scope.h"
#include "devicebase.h"

namespace Ui
{
    class DialogClass;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::DialogClass *ui;

private slots:

    void on_VrefList_activated(int index);
    void on_Ch4Offset_valueChanged(int value);
    void on_Ch3Offset_valueChanged(int value);
    void on_Ch2Offset_valueChanged(int value);
    void on_Ch1Offset_valueChanged(int value);
    void on_Ch4Gane_valueChanged(int value);
    void on_Ch3Gane_valueChanged(int value);
    void on_Ch2Gane_valueChanged(int value);
    void on_Ch1Gane_valueChanged(int value);
    void on_Ch4_stateChanged(int );
    void on_Ch3_stateChanged(int );
    void on_Ch2_stateChanged(int );
    void on_Ch1_stateChanged(int );
    void on_XOffset_valueChanged(int value);
    void on_XScale_valueChanged(int value);
    void on_AdcSpeed_valueChanged(int value);
    void on_Start_clicked();
protected:
    void timerEvent(QTimerEvent *event);
    int _updateScopeEvent;
    QVector<Curve*> _chanels;
    DeviceBase *_device;
};

#endif // DIALOG_H
