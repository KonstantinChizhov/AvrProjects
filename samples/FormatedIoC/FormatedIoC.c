#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

static const long baund = 115200;

static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
                                     _FDEV_SETUP_WRITE);

static FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, 
                                     _FDEV_SETUP_READ);

static int
uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}

static int
uart_getchar(FILE *stream)
{
	unsigned long timeout = 10000;
	while((UCSRA & (1 << RXC)) == 0)
	{
		if(timeout-- == 0) return EOF;
	}
	return UDR;
}

void init_uart()
{
	const unsigned int ubrr = (F_CPU/16/baund-1);
	const unsigned int ubrr2x =	(F_CPU/8/baund-1);
	const unsigned long rbaund = (F_CPU/16/(ubrr+1));	
	const unsigned long rbaund2x = (F_CPU/8/(ubrr2x+1));

	unsigned long err1;
	if(baund > rbaund)
		err1 = (baund - rbaund)*1000/baund;
	else
		err1 = (rbaund - baund)*1000/rbaund;

	unsigned long err2;
	if(baund > rbaund2x)
		err2 = (baund - rbaund2x)*1000/baund;
	else
		err2 = (rbaund2x - baund)*1000/rbaund2x;

	unsigned int ubrrToUse;
	if(err1 > err2)
	{
		UCSRA = (1 << U2X);
		ubrrToUse = ubrr2x;
	}
	else
	{
		UCSRA = (0x00);
		ubrrToUse = ubrr;
	}
	UBRRL = (ubrrToUse);
	UBRRH = (ubrrToUse>>8);

	UCSRB = (0x00); 
	UCSRC = (URSEL | (1 << UCSZ1) | (1 << UCSZ0));
	UCSRB = ( (1 << RXCIE) | (0 << TXCIE) | (1 << UDRIE) | (1 << RXEN) | (1 << TXEN));
}

int main(void)
{
	init_uart();
	stdout = &mystdout;
	stdin = &mystdin;
	int v;
	
	while(1)
	{
		scanf("%d", &v);
		printf("You entred %d\n", v);
	}
	return 0;
}
