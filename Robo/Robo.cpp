#include "main.h"


ISR(SIG_INTERRUPT0)
{
	Ir::CaptureHandler();
}

ISR(SIG_OVERFLOW0)
{
	Ir::TimeoutHandler();
}

int main()
{
	
}
