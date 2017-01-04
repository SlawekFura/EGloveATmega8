/*
 * myuart.c
 *
 *  Created on: 2 maj 2015
 *      Author: Slawek
 */

#include "avr/io.h"
#include "myuart.h"
#include <avr/pgmspace.h>
#include <stdlib.h>


void uart_init(void)
{
/*Set baud rate */
UBRRH = (uint8_t)(__UBRR>>8);
UBRRL = (uint8_t)__UBRR;
//Enable receiver and transmitter
UCSRB = (1<<RXEN)|(1<<TXEN);
// Set frame format: 8data, 2stop bit
// opcjonalnie
UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0);
}

void uart_putc( char data )
{
while ( !( UCSRA & (1<<UDRE)) );	/* Wait for empty transmit buffer */
UDR = data;						/* Put data into buffer, sends the data */
}

void uart_puts(char * s)
{
	while(*s)
		uart_putc(*s++);
}

void uart_puts_P(const char *s)   //wysy³a ³añcuch z pamiêci ROM do mikrokontrolera
{
	register char c;
	while ((c = pgm_read_byte( s++) )) uart_putc(c);
}

void uart_putlong(int32_t liczba, uint8_t radix)
{
	char buf[17];
	ltoa(liczba, buf, radix);//long to ascii
	uart_puts(buf);
}

void timer_init(void)
{
	TCCR0 |= (1<<CS02) | (1<<CS00);	//prze³¹cza po przepe³nieniu OCR0A
	//TCCR0A |= (1<<WGM01);					//tryb ctc
	//TCCR0B |= (1<<CS02) ;					//preskaler 8
	TIMSK |= (1<<TOIE0);				//zezwolenie na przerwania
}








