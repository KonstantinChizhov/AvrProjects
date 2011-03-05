#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <iopins.h>
#include <pinlist.h>
#include <timers.h>
#include <timer_utils.h>
#include <rfm70.h>
#include <dispatcher.h>
#include <Encoder.h>
#include "MotorDriver.h"
#include <delay.h>

using namespace IO;

//typedef IrReciver<Timers::Timer0, uint16_t, 12> Ir;
typedef Dispatcher<16, 10> Sheduller;
typedef Encoder<uint16_t, PinList<Pc0, Pc1>, PinList<Pc2, Pc3> > Encoder1;

typedef Timers::TimerFreqSetup<Timers::Timer0, 1000> T0Setup;

typedef SimpleDriver<Pb0, Pb1> LeftMotor;
typedef SimpleDriver<Pb2, Pb3> RightMotor;

//typedef 
