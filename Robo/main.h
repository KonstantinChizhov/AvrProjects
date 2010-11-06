#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <iopins.h>
#include <timer.h>
#include "ir.h"
#include "dispatcher.h"

typedef IrReciver<Timers::Timer0, uint16_t, 12> Ir;
typedef Dispatcher<16, 10> Sheduller;
