#include <avr/io.h>
#include "usart.h"
#include "util.h"
#include "ports.h"
#include "usbdrv.h"
#include <avr/interrupt.h>  /* for sei() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>

typedef Usart<8, 4> usart;
typedef IO::Pd5 led;


uchar usbFunctionWrite(uchar *data, uchar len)
{
    return 1;
}


uchar usbFunctionRead(uchar *data, uchar len)
{
    return len;
}



void usbFunctionWriteOut(uchar *data, uchar len)
{
}



USB_PUBLIC usbMsgLen_t  usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (usbRequest_t *)data;

    if(rq->bRequest == 0)   /* request using usbFunctionRead()/usbFunctionWrite() */
        return 0xff;
	return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int	main(void)
{
	uchar   i;
	led::SetDirWrite();
	usart::Init(115200);

	usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
	sei();
	for(;;){                /* main event loop */
		usbPoll();
	}
	return 0;
}


