/*
 * I2C_SPI.c
 *
 *  Created on: 26 sie 2016
 *      Author: Mariusz
 */
#include "../Includes/I2C_SPI.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

uint8_t AccSettings = LSM303_ACC_XYZ_ENABLE | LSM303_ACC_100HZ;

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

//HAL_StatusTypeDef initAccI2C(I2C_HandleTypeDef * hi2c){
//	  return HAL_I2C_Mem_Write(hi2c, LSM303_ACC_ADDRESS, LSM303_ACC_CTRL_REG1_A, 1, &AccSettings, 1, 100);
//}

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
void initGyroSPI(){
	DDRB = (1<<PINMOSI) | (1<<PINSCK) | (1<<PINCS);
	SPCR = ( 1 << SPE ) | ( 1 << MSTR ) /*| ( 1 << SPR1 )*/ | ( 1 << SPR0 );

	SPI_RESET;
	sendSPI(L3GD20_GYRO_CTRL_REG1);
	sendSPI(GyroSettings);
	SPI_SET;
}

void getPositionDataSPI(int16_t *pDataGetXAxis,int16_t *pDataGetYAxis,
		int16_t *pDataGetZAxis){

	int8_t DataGetAxisTempHigh = 0;
	int8_t DataGetAxisTempLow = 0;

	SPI_RESET;
	sendSPI(**(pSendSPI + 2));

	for (int var = 0; var < 3; ++var) {
		sendSPI(255);
		DataGetAxisTempLow = getSPI();
		sendSPI(255);
		DataGetAxisTempHigh = getSPI();

		switch (var){
		case 0:
			*pDataGetXAxis = DataGetAxisTempLow + (DataGetAxisTempHigh<<8);
		//	uart_puts("SPI X = ");
		//	uart_putlong(*pDataGetXAxis,10);
			break;
		case 1:
			*pDataGetYAxis = /*DataGetAxisTempLow +*/ (DataGetAxisTempHigh<<8);
		//	uart_putc(9);
		//	uart_puts(" SPI Y = ");
		//	uart_putlong(*pDataGetYAxis,10);
			break;
		case 2:
			*pDataGetZAxis = /*DataGetAxisTempLow +*/ (DataGetAxisTempHigh <<8);
//			uart_putc(9);
//			uart_puts(" SPI Z = ");
//			uart_putlong(*pDataGetZAxis,10);
//			uart_putc('\n');
//			uart_putc(13);
			break;
		}
	}
	_delay_us(5);

	SPI_SET;
//				uart_putc(9);
//				uart_puts(" SPI Z = ");
//				uart_putlong(*pDataGetZAxis,10);
//				uart_putc('\n');
//				uart_putc(13);
}


