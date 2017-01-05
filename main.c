/*
 * main.c
 *
 *  Created on: 7 gru 2016
 *      Author: Mariusz
 */


#include <avr/interrupt.h> //biblioteka z przerwaniami

#include <util/delay.h> //opoznienia
#include <stdlib.h> //standardowa biblioteka
#include <string.h> //lancuchy znakowe
#include "MYUART/myuart.h"
#include "Includes/I2C_SPI.h"
#include "Includes/pos_calc.h"

enum Data{
	X = 0,
	Y,Z
};

int16_t DataGyro[3];
int16_t DataAcc[3];
volatile HandPos * hand = {0} ;
char charToSend[DATA_TO_SEND_SIZE+1];
int main(void){
	//Initialization
	uart_init();
	DDRC |= (1<<PC3);
	PORTC |= (1<<PC3);
	initSPI();
	initClickPins();
	timer_init();
	charToSend[DATA_TO_SEND_SIZE]='\0';
//	gyroCalibration(hand);
	sei();

	while(1){
		PORTC ^= (1<<PC3);

//		getPositionDataACC(&DataAcc[X],&DataAcc[Y],&DataAcc[Z]);
		_delay_ms(500);

//		uart_putc(9);
		//uart_puts("licznik = ");//_delay_ms(10);
		//uart_putlong(a,10);uart_putc(9);

	}
	return 0;
}

ISR(TIMER0_OVF_vect)
{
	uart_puts(" SPI X divided = ");
	//PORTC ^= (1<<PC3);
	getPositionDataGyro(&DataGyro[X],&DataGyro[Y],&DataGyro[Z]);
	getPositionDataACC(&DataAcc[X],&DataAcc[Y],&DataAcc[Z]);
	fillHandPos(hand,DataAcc[X],DataAcc[Y],DataAcc[Z],DataGyro[X],DataGyro[Y],DataGyro[Z]);
	fillDataToSend(charToSend,DATA_TO_SEND_SIZE,hand);
	uart_puts(charToSend);
	uart_putc('\n');
	uart_putc(CARRIAGE_RETURN);

}
