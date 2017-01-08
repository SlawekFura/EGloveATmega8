/*
 * I2C_SPI.c
 *
 *  Created on: 26 sie 2016
 *      Author: Mariusz
 */
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "../Includes/ADC_SPI.h"
#include "../MYUART/myuart.h"

uint8_t AccSettings = LSM303_ACC_XYZ_ENABLE | LSM303_ACC_100HZ|LSM303_ACC_NOBLOCK;

enum {GyroSettings = L3GD20_GYRO_190HZ_25BW | L3GD20_GYRO_ENABLE | L3GD20_GYRO_ZXY_ENABLE};
uint8_t sendToGyro[10]= {L3GD20_GYRO_CTRL_REG1,
		GyroSettings,
		L3GD20_GYRO_X_L | 128 | L3GD20_GYRO_MS_BIT,//multi reading
		L3GD20_GYRO_X_L | 128,
		L3GD20_GYRO_X_H | 128,
		L3GD20_GYRO_Y_L | 128,
		L3GD20_GYRO_Y_H | 128,
		L3GD20_GYRO_Z_L | 128,
		L3GD20_GYRO_Z_H | 128,
		L3GD20_GYRO_WHO_AM_I | 128};
uint8_t * pSendSPI[10] = {sendToGyro, &sendToGyro[1], &sendToGyro[2],
		&sendToGyro[3], &sendToGyro[4], &sendToGyro[5],
		&sendToGyro[6], &sendToGyro[7], &sendToGyro[8], &sendToGyro[9]};

void sendSPI(int8_t byte){
	SPDR = byte;
	while( (SPSR & (1<<SPIF))==0 );
}

int8_t getSPI(){
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    // Return received data
    return (int8_t)SPDR;
}
void initSPI(){
	DDRB = (1<<PINMOSI) | (1<<PINSCK) | (1<<PINCSGYRO);
	DDRC |= (1<<PINCSACC);
	SPIACC_SET;	SPIGYRO_SET;
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) /*| ( 1 << SPR1 )*/ | ( 1 << SPR0 );

	SPIGYRO_RESET;
	sendSPI(L3GD20_GYRO_CTRL_REG1);
	sendSPI(GyroSettings);

	SPIGYRO_SET;
	_delay_ms(10);

	SPIACC_RESET;
	sendSPI(LSM303_ACC_CTRL_REG1);
	sendSPI(AccSettings);

	uart_putlong(getSPI(),10);
	uart_puts(" ");
	SPIACC_SET;

	_delay_ms(10);

	SPIACC_RESET;
	sendSPI(LSM303_ACC__WHO_AM_I|128);
	sendSPI(255);
	uart_putlong(getSPI(),10);
	SPIACC_SET;
}

void getPositionDataGyro(int16_t *pDataGetXAxis,int16_t *pDataGetYAxis,
		int16_t *pDataGetZAxis){
	int8_t DataGetAxisTempHigh = 0;
	int8_t DataGetAxisTempLow = 0;

	SPIGYRO_RESET;
	sendSPI(**(pSendSPI + 2));
	for (int var = 0; var < 3; ++var) {
		sendSPI(255);
		DataGetAxisTempLow = getSPI();
		sendSPI(255);
		DataGetAxisTempHigh = getSPI();

		switch (var){
		case 0:
			*pDataGetXAxis = DataGetAxisTempLow + (DataGetAxisTempHigh<<8);
			break;
		case 1:
			*pDataGetYAxis = /*DataGetAxisTempLow +*/ (DataGetAxisTempHigh<<8);
			break;
		case 2:
			*pDataGetZAxis = /*DataGetAxisTempLow +*/ (DataGetAxisTempHigh <<8);
			break;
		}
	}
	_delay_us(5);
	SPIGYRO_SET;
//		uart_putlong(*pDataGetXAxis,10);
//		uart_putc(HORIZONTAL_TAB);
//		uart_putlong(*pDataGetYAxis,10);
//		uart_putc(HORIZONTAL_TAB);
//		uart_putlong(*pDataGetZAxis,10);
//		uart_putc('\n');
//		uart_putc(CARRIAGE_RETURN);
}

void getPositionDataACC( int16_t *pDataGetXAxis, int16_t *pDataGetYAxis,
						 int16_t *pDataGetZAxis){

	int8_t DataGetAxisTempHigh = 0;
	int8_t DataGetAxisTempLow = 0;

	SPIACC_RESET;
	sendSPI(LSM303_ACC_X_L | LSM303_ACC_MULTI_READ);
	for (int var = 0; var < 3; ++var) {
		sendSPI(255);
		DataGetAxisTempLow = getSPI();
		sendSPI(255);
		DataGetAxisTempHigh = getSPI();

		switch (var){
		case 0:
			*pDataGetXAxis = DataGetAxisTempLow + (DataGetAxisTempHigh<<8);
			break;
		case 1:
			*pDataGetYAxis = DataGetAxisTempLow + (DataGetAxisTempHigh<<8);
			break;
		case 2:
			*pDataGetZAxis = DataGetAxisTempLow + (DataGetAxisTempHigh <<8);
			break;
		}
	}
	_delay_us(5);
	SPIACC_SET;
//	uart_putlong(*pDataGetXAxis,10);
//	uart_putc(HORIZONTAL_TAB);
//	uart_putlong(*pDataGetYAxis,10);
//	uart_putc(HORIZONTAL_TAB);
//	uart_putlong(*pDataGetZAxis,10);
//	uart_putc('\n');
//	uart_putc(CARRIAGE_RETURN);
}

void initADC(){
ADMUX|=(1<<REFS0)/*|(1<<REFS1)*/|(1<<MUX2)|(1<<MUX0); //Internal 2.56V Voltage Reference with external capacitor at AREF pin, PC5 as analog input
ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); //prescaler + adcON
}

int16_t getAdcScrollData(){

	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
//	uart_puts("ADC = ");
//	uart_putlong(ADCW,10);
//	uart_putc('\n');
//	uart_putc(UART_CARRIAGE_RETURN);
	return ADCW;
}



