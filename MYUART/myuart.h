/*
 * myuart.h
 *
 *  Created on: 2 maj 2015
 *      Author: Slawek
 */

#define UART_BAUD 38400
#define __UBRR ((F_CPU+UART_BAUD*8UL)/(16UL*UART_BAUD)-1)

#define UART_HORIZONTAL_TAB 9
#define UART_CARRIAGE_RETURN 13

#ifndef MYUART_MYUART_H_
#define MYUART_MYUART_H_

void uart_init(void);
void uart_putc( char data );
void uart_puts(char * s);
void uart_puts_P(const char *s);
void uart_putlong(int32_t liczba, uint8_t radix);
void timer_init(void);
#endif /* MYUART_MYUART_H_ */
