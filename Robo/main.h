#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <iopins.h>
#include <pinlist.h>
#include <timer.h>
#include "ir.h"
#include "dispatcher.h"
#include "Encoder.h"

using namespace IO;

typedef IrReciver<Timers::Timer0, uint16_t, 12> Ir;
typedef Dispatcher<16, 10> Sheduller;
typedef Encoder<uint16_t, PinList<Pa0, Pa1> > Encoder1;

typedef Timers::TimerFreqSetup<Timers::Timer0, 1000> T0Setup;
