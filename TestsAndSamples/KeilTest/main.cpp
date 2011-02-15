#define STM32F10X_LD_VL

#include <stm32f10x.h>

#define USE_PORTA
#include <iopins.h>
using namespace IO;

extern "C" void SystemInit()
{

}

int main()
{
		Pa1::Set();
}
