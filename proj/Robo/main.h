#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <iopins.h>
#include <pinlist.h>
#include <timers.h>
#include <timer_utils.h>
#include <Rfm70.h>
#include <dispatcher.h>
#include <Encoder.h>
#include "MotorDriver.h"
#include <delay.h>
#include <spi.h>
#include "protocol.h"

using namespace IO;

//typedef IrReciver<Timers::Timer0, uint16_t, 12> Ir;
typedef Dispatcher<16, 10> Sheduller;
typedef Encoder<uint16_t, PinList<Pc0, Pc1>, PinList<Pc2, Pc3> > Encoder1;

typedef Timers::TimerFreqSetup<Timers::Timer0, 1000> T0Setup;

typedef SimpleDriver<Pb6, Pb7> LeftMotor;
typedef SimpleDriver<Pd5, Pd6> RightMotor;

typedef Pd7 BoostPin;
typedef SoftSpi<Pb3, Pb4, Pb5> Spi;

typedef Rfm70<Spi,
			Pb2, //CSN
			Pb1, //En
			NullPin //Irq
			> Transiver;

