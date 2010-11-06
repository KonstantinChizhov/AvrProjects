#pragma once

#if defined(__ICC430__) || defined(__MSP430__)

#include "MSP430/timer.h"

#elif defined(__ICCAVR__) || defined(__AVR__)

#ifdef TCNT0
#define USE_TIMER_0
#endif

#ifdef TCNT1
#define USE_TIMER_1
#endif

#ifdef USE_TIMER_0
#include "AVR/timer0.h"
#endif

#ifdef USE_TIMER_1
#include "AVR/timer1.h"
#endif

#else
#error "Not supported arch."

#endif

