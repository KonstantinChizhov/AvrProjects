/********************************************************************************
** Form generated from reading ui file 'dialog.ui'
**
** Created: Tue 15. Sep 20:30:03 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDial>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "graph.h"

QT_BEGIN_NAMESPACE

class Ui_DialogClass
{
public:
    QGridLayout *gridLayout_4;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    MyGraph *Display;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QCheckBox *Ch1;
    QDial *Ch1Offset;
    QLCDNumber *Ch1Vdiv;
    QDial *Ch2Offset;
    QLCDNumber *Ch2Vdiv;
    QCheckBox *Ch2;
    QCheckBox *Ch3;
    QCheckBox *Ch4;
    QDial *Ch3Offset;
    QDial *Ch4Offset;
    QLCDNumber *Ch3Vdiv;
    QLCDNumber *Ch4Vdiv;
    QDial *Ch1Gane;
    QDial *Ch2Gane;
    QDial *Ch3Gane;
    QDial *Ch4Gane;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QDial *AdcSpeed;
    QDial *XScale;
    QDial *XOffset;
    QVBoxLayout *verticalLayout;
    QPushButton *Start;
    QPushButton *SavePic;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLCDNumber *SampleRate;
    QHBoxLayout *horizontalLayout_4;
    QLabel *VrefLabel;
    QComboBox *VrefList;
    QWidget *tab_2;

    void setupUi(QDialog *DialogClass)
    {
        if (DialogClass->objectName().isEmpty())
            DialogClass->setObjectName(QString::fromUtf8("DialogClass"));
        DialogClass->resize(912, 724);
        gridLayout_4 = new QGridLayout(DialogClass);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setMargin(11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tabWidget = new QTabWidget(DialogClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Display = new MyGraph(tab);
        Display->setObjectName(QString::fromUtf8("Display"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Display->sizePolicy().hasHeightForWidth());
        Display->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(10, 50, 10, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        Display->setPalette(palette);
        Display->setAutoFillBackground(true);
        Display->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(Display);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame_2 = new QFrame(tab);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setSpacing(0);
        gridLayout->setMargin(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Ch1 = new QCheckBox(frame_2);
        Ch1->setObjectName(QString::fromUtf8("Ch1"));
        Ch1->setLayoutDirection(Qt::LeftToRight);
        Ch1->setChecked(true);

        gridLayout->addWidget(Ch1, 0, 1, 1, 1);

        Ch1Offset = new QDial(frame_2);
        Ch1Offset->setObjectName(QString::fromUtf8("Ch1Offset"));
        Ch1Offset->setMinimum(-50);
        Ch1Offset->setMaximum(50);
        Ch1Offset->setNotchTarget(3.7);
        Ch1Offset->setNotchesVisible(true);

        gridLayout->addWidget(Ch1Offset, 2, 1, 1, 1);

        Ch1Vdiv = new QLCDNumber(frame_2);
        Ch1Vdiv->setObjectName(QString::fromUtf8("Ch1Vdiv"));
        Ch1Vdiv->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(Ch1Vdiv, 3, 1, 1, 1);

        Ch2Offset = new QDial(frame_2);
        Ch2Offset->setObjectName(QString::fromUtf8("Ch2Offset"));
        Ch2Offset->setMinimum(-50);
        Ch2Offset->setMaximum(50);
        Ch2Offset->setNotchesVisible(true);

        gridLayout->addWidget(Ch2Offset, 2, 2, 1, 1);

        Ch2Vdiv = new QLCDNumber(frame_2);
        Ch2Vdiv->setObjectName(QString::fromUtf8("Ch2Vdiv"));
        Ch2Vdiv->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(Ch2Vdiv, 3, 2, 1, 1);

        Ch2 = new QCheckBox(frame_2);
        Ch2->setObjectName(QString::fromUtf8("Ch2"));

        gridLayout->addWidget(Ch2, 0, 2, 1, 1);

        Ch3 = new QCheckBox(frame_2);
        Ch3->setObjectName(QString::fromUtf8("Ch3"));

        gridLayout->addWidget(Ch3, 0, 3, 1, 1);

        Ch4 = new QCheckBox(frame_2);
        Ch4->setObjectName(QString::fromUtf8("Ch4"));

        gridLayout->addWidget(Ch4, 0, 4, 1, 1);

        Ch3Offset = new QDial(frame_2);
        Ch3Offset->setObjectName(QString::fromUtf8("Ch3Offset"));
        Ch3Offset->setNotchesVisible(true);

        gridLayout->addWidget(Ch3Offset, 2, 3, 1, 1);

        Ch4Offset = new QDial(frame_2);
        Ch4Offset->setObjectName(QString::fromUtf8("Ch4Offset"));
        Ch4Offset->setNotchesVisible(true);

        gridLayout->addWidget(Ch4Offset, 2, 4, 1, 1);

        Ch3Vdiv = new QLCDNumber(frame_2);
        Ch3Vdiv->setObjectName(QString::fromUtf8("Ch3Vdiv"));
        Ch3Vdiv->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(Ch3Vdiv, 3, 3, 1, 1);

        Ch4Vdiv = new QLCDNumber(frame_2);
        Ch4Vdiv->setObjectName(QString::fromUtf8("Ch4Vdiv"));
        Ch4Vdiv->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(Ch4Vdiv, 3, 4, 1, 1);

        Ch1Gane = new QDial(frame_2);
        Ch1Gane->setObjectName(QString::fromUtf8("Ch1Gane"));
        Ch1Gane->setMaximum(10);
        Ch1Gane->setNotchesVisible(true);

        gridLayout->addWidget(Ch1Gane, 1, 1, 1, 1);

        Ch2Gane = new QDial(frame_2);
        Ch2Gane->setObjectName(QString::fromUtf8("Ch2Gane"));
        Ch2Gane->setMaximum(10);
        Ch2Gane->setNotchesVisible(true);

        gridLayout->addWidget(Ch2Gane, 1, 2, 1, 1);

        Ch3Gane = new QDial(frame_2);
        Ch3Gane->setObjectName(QString::fromUtf8("Ch3Gane"));
        Ch3Gane->setMaximum(10);
        Ch3Gane->setNotchesVisible(true);

        gridLayout->addWidget(Ch3Gane, 1, 3, 1, 1);

        Ch4Gane = new QDial(frame_2);
        Ch4Gane->setObjectName(QString::fromUtf8("Ch4Gane"));
        Ch4Gane->setMaximum(10);
        Ch4Gane->setNotchesVisible(true);

        gridLayout->addWidget(Ch4Gane, 1, 4, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setTextFormat(Qt::AutoText);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);


        horizontalLayout->addWidget(frame_2);

        frame = new QFrame(tab);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 1, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 2, 1, 1);

        AdcSpeed = new QDial(frame);
        AdcSpeed->setObjectName(QString::fromUtf8("AdcSpeed"));
        AdcSpeed->setMaximum(6);
        AdcSpeed->setPageStep(1);
        AdcSpeed->setValue(4);
        AdcSpeed->setNotchesVisible(true);

        gridLayout_2->addWidget(AdcSpeed, 1, 0, 1, 1);

        XScale = new QDial(frame);
        XScale->setObjectName(QString::fromUtf8("XScale"));
        XScale->setMaximum(10);
        XScale->setPageStep(9);
        XScale->setValue(1);
        XScale->setNotchesVisible(true);

        gridLayout_2->addWidget(XScale, 1, 1, 1, 1);

        XOffset = new QDial(frame);
        XOffset->setObjectName(QString::fromUtf8("XOffset"));
        XOffset->setMinimum(-500);
        XOffset->setMaximum(500);
        XOffset->setNotchTarget(50);
        XOffset->setNotchesVisible(true);

        gridLayout_2->addWidget(XOffset, 1, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Start = new QPushButton(frame);
        Start->setObjectName(QString::fromUtf8("Start"));

        verticalLayout->addWidget(Start);

        SavePic = new QPushButton(frame);
        SavePic->setObjectName(QString::fromUtf8("SavePic"));

        verticalLayout->addWidget(SavePic);


        gridLayout_3->addLayout(verticalLayout, 0, 2, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(label_2);

        SampleRate = new QLCDNumber(frame);
        SampleRate->setObjectName(QString::fromUtf8("SampleRate"));
        sizePolicy2.setHeightForWidth(SampleRate->sizePolicy().hasHeightForWidth());
        SampleRate->setSizePolicy(sizePolicy2);
        SampleRate->setMinimumSize(QSize(70, 0));
        SampleRate->setMode(QLCDNumber::Dec);
        SampleRate->setSegmentStyle(QLCDNumber::Flat);
        SampleRate->setProperty("value", QVariant(0));
        SampleRate->setProperty("intValue", QVariant(0));

        horizontalLayout_3->addWidget(SampleRate);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        VrefLabel = new QLabel(frame);
        VrefLabel->setObjectName(QString::fromUtf8("VrefLabel"));
        sizePolicy2.setHeightForWidth(VrefLabel->sizePolicy().hasHeightForWidth());
        VrefLabel->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(VrefLabel);

        VrefList = new QComboBox(frame);
        VrefList->setObjectName(QString::fromUtf8("VrefList"));
        sizePolicy2.setHeightForWidth(VrefList->sizePolicy().hasHeightForWidth());
        VrefList->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(VrefList);


        gridLayout_3->addLayout(horizontalLayout_4, 1, 1, 1, 1);


        horizontalLayout->addWidget(frame);


        verticalLayout_2->addLayout(horizontalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(DialogClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogClass);
    } // setupUi

    void retranslateUi(QDialog *DialogClass)
    {
        DialogClass->setWindowTitle(QApplication::translate("DialogClass", "Dialog", 0, QApplication::UnicodeUTF8));
        Ch1->setText(QApplication::translate("DialogClass", "Ch1", 0, QApplication::UnicodeUTF8));
        Ch2->setText(QApplication::translate("DialogClass", "Ch2", 0, QApplication::UnicodeUTF8));
        Ch3->setText(QApplication::translate("DialogClass", "Ch3", 0, QApplication::UnicodeUTF8));
        Ch4->setText(QApplication::translate("DialogClass", "Ch4", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogClass", "Gane", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogClass", "YOffset", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogClass", "mV/div", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogClass", "AdcSpeed", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogClass", "XScale", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DialogClass", "XOffset", 0, QApplication::UnicodeUTF8));
        Start->setText(QApplication::translate("DialogClass", "Start", 0, QApplication::UnicodeUTF8));
        SavePic->setText(QApplication::translate("DialogClass", "Save Pic", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogClass", "SampleRate, Ksps", 0, QApplication::UnicodeUTF8));
        VrefLabel->setText(QApplication::translate("DialogClass", "Vref, V", 0, QApplication::UnicodeUTF8));
        VrefList->clear();
        VrefList->insertItems(0, QStringList()
         << QApplication::translate("DialogClass", "2.56", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DialogClass", "5", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DialogClass", "Oscilloscope", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DialogClass", "Tab 2", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DialogClass);
    } // retranslateUi

};

namespace Ui {
    class DialogClass: public Ui_DialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
