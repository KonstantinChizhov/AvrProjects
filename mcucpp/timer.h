#pragma once

#if defined(__ICC430__) || defined(__MSP430__)
#include "MSP430/timer.h"
#elif defined(__ICCAVR__) || defined(__AVR__)
#include "AVR/timer0.h"
#else
#error "Not supported arch."

#endif

#endif //TCNT1
