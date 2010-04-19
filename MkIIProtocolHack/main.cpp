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


void usbFunctionWriteOut(uchar *data, uchar len)
{
	for(uint8_t i=0; i<min<uint8_t>(8, len); i++)
		usart::Putch(data[i]);
}



USB_PUBLIC usbMsgLen_t  usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (usbRequest_t *)data;

    if(rq->bRequest == 0)   /* request using usbFunctionRead()/usbFunctionWrite() */
        return 0xff;
	return 0;   /* default for not implemented requests: return no data back to host */
}

// -------------------------------------------------------------------------

inline void BulkWrite()
{
	if(usbInterruptIsReady())// only if previous data was sent
	{               
    	uchar *p;
    	uchar len = getInterruptData(&p);   // obtain chunk of max 8 bytes
    	if(len > 0)                         // only send if we have data
       	usbSetInterrupt(p, len);
	}
}

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
	for(;;) /* main event loop */
	{               
		
		BulkWrite();
		usbPoll();
	}
	return 0;
}


