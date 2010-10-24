#include "in430.h"
#include "io430.h"
//nclude "msp430.h"

#include "../mcucpp/iopins.h"

typedef IO::P1_0 LedPwmPin;
typedef IO::P1_3 Button1;
typedef IO::P1_6 DbgPin;
//typedef IO::P1_0 DbgPin2;

const unsigned PowerOffTimeout = 15*10;
const unsigned PwmPeriod = 100;
static volatile unsigned PwmDutyCycle = PwmPeriod / 2;
static volatile unsigned IdleCount = 0;
static volatile bool PwmState = false;
static uint8_t PwmValues[] = {PwmPeriod / 2, PwmPeriod / 3, PwmPeriod / 4, PwmPeriod / 8, PwmPeriod / 16};
static volatile unsigned char PwmIndex = 0;
const uint8_t PwmStepsCount = sizeof(PwmValues)/sizeof(PwmValues[0]);

class Timer0
{
public:
  enum Mode{Stop, ToCCR0, Normal, UpDown};
  enum Divider{Div1, Div2, Div4, Div8};
  enum ClockSrc{MainClock, AuxClock, ExtRising, ExtFailing};
  
  static void Start(Mode mode = Normal, Divider divider = Div1, ClockSrc clock = MainClock)
  {
    uint16_t timerControl = 0;
    switch(mode)
    {
      case Stop: timerControl |= MC_0; break;
      case ToCCR0: timerControl |= MC_1; break;
      case Normal: timerControl |= MC_2; break;
      case UpDown: timerControl |= MC_3; break;
    }
    
    switch(divider)
    {
      case Div1: timerControl |= ID_0; break;
      case Div2: timerControl |= ID_1; break;
      case Div4: timerControl |= ID_2; break;
      case Div8: timerControl |= ID_3; break;
    }
    
    switch(clock)
    {
      case ExtRising: timerControl |= TASSEL_0; break;
      case AuxClock: timerControl |= TASSEL_1; break;
      case MainClock: timerControl |= TASSEL_2; break;
      case ExtFailing: timerControl |= TASSEL_3; break;
    }
        
    TACTL &= 0x03f0;
    TACTL |= timerControl;
  }
  
  static void InterruptEnable()
  {
    TACTL |= TAIE;
  }
  
  template<int Unit>
  static void CCInteruptEnable()
  {
    if(Unit == 0)
      CCTL0 |= CCIE;
    if(Unit == 1)
      CCTL0 |= CCIE;
  }
};

void InitClock()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_16MHZ;                    // Set range
  DCOCTL = CALDCO_16MHZ;                     // Set DCO step + modulation 
}

void SetPwm(unsigned duty)
{
  PwmDutyCycle = duty;
}

void main(void)
{
 // DbgPin::SetDirWrite();
  //DbgPin2::SetDirWrite();
  LedPwmPin::SetDirWrite();
  InitClock();
  Timer0::Start(Timer0::Normal, Timer0::Div8);
  Timer0::InterruptEnable();
  CCR0 = PwmPeriod - PwmDutyCycle;
  Timer0::CCInteruptEnable<0>();
  P1IE |= (1 << Button1::Number);
  P1IES |= (1 << Button1::Number);
  P1IFG = 0;
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
   if(PwmState)
   {
    CCR0 += PwmPeriod - PwmDutyCycle;
    LedPwmPin::Clear();
   }
   else
   {
    CCR0 += PwmDutyCycle;
    LedPwmPin::Set();
   }
   PwmState = !PwmState;
}

#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_B(void)
{
 switch( TAIV )
 {
   
 case 10: 
  // DbgPin::Togle();                  // overflow
   //PwmDutyCycle >>= 1;
   //if(PwmDutyCycle == 0)
     //PwmDutyCycle = PwmPeriod;
   break;
 }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1Int()
{
  DbgPin::Togle(); 
  PwmDutyCycle = PwmValues[PwmIndex++];
  if(PwmIndex >= PwmStepsCount) 
    PwmIndex = 0;
  P1IFG = 0;
}
