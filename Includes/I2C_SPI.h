/*
 * I2C_SPI.h
 *
 *  Created on: 26 sie 2016
 *      Author: Mariusz
 */

#ifndef I2C_SPI_H_
#define I2C_SPI_H_

#include <avr/io.h>
#include "../MYUART/myuart.h"


#define LSM303_ACC_ADDRESS (0x1D << 1) // adres akcelerometru: 0011 001x
#define LSM303_ACC_CTRL_REG1 0x20 // rejestr ustawien 1
#define LSM303_ACC_XYZ_ENABLE 0x07 // 0000 0111
#define LSM303_ACC_100HZ 0x60 //  0110 0000
#define LSM303_ACC_NOBLOCK 0x08
#define LSM303_ACC_READ 0x80
#define LSM303_ACC_MULTI_READ 0x80 | 0x40
#define LSM303_ACC_X_H 0x29 // wyzszy bajt danych osi X
#define LSM303_ACC_X_L 0x28 // nizszy bajt danych osi X
#define LSM303_ACC_Y_H 0x2B // wyzszy bajt danych osi Y
#define LSM303_ACC_Y_L 0x2A // nizszy bajt danych osi Y
#define LSM303_ACC_Z_H 0x2D // wyzszy bajt danych osi Z
#define LSM303_ACC_Z_L 0x2C // nizszy bajt danych osi Z
#define LSM303_ACC_RESOLUTION 2
#define LSM303_ACC__WHO_AM_I 0x0F


#define L3GD20_GYRO_CTRL_REG1 0x20 // rejestr ustawien 1
#define L3GD20_GYRO_190HZ_25BW 0x50 // 0101 0000
#define L3GD20_GYRO_ENABLE 0x08 // 0000 1000
#define L3GD20_GYRO_ZXY_ENABLE 0x07 // 0000 0100
#define L3GD20_GYRO_MS_BIT 0x40
#define L3GD20_GYRO_X_H 0x29 // wyzszy bajt danych osi X
#define L3GD20_GYRO_X_L 0x28 // nizszy bajt danych osi X
#define L3GD20_GYRO_Y_H 0x2B // wyzszy bajt danych osi Y
#define L3GD20_GYRO_Y_L 0x2A // nizszy bajt danych osi Y
#define L3GD20_GYRO_Z_H 0x2D // wyzszy bajt danych osi Z
#define L3GD20_GYRO_Z_L 0x2C // nizszy bajt danych osi Z
#define L3GD20_GYRO_WHO_AM_I 0x0F

#define PORTSPI DDRB
#define PORTSSGYRO DDRB
#define PORTSSACC DDRC
#define PINMISO PB4
#define PINMOSI PB3
#define PINSCK PB5

#define PINCSGYRO PB2
#define PINCSACC PC4

#define SPIGYRO_SET PORTB |= (1<<PINCSGYRO)
#define SPIGYRO_RESET PORTB &= ~(1<<PINCSGYRO)

#define SPIACC_SET PORTC |= (1<<PINCSACC)
#define SPIACC_RESET PORTC &= ~(1<<PINCSACC)

uint8_t AccSettings;
uint8_t sendToGyro[10];
uint8_t * pSendSPI[10];

void initSPI();
int8_t getSPI();
void sendSPI(int8_t byte);
void getPositionDataACC( int16_t *pDataGetXAxis, int16_t *pDataGetYAxis,
						 int16_t *pDataGetZAxis);

void getPositionDataGyro(int16_t *pDataGetXAxis,
		int16_t *pDataGetYAxis,  int16_t *pDataGetZAxis);



#endif /* I2C_SPI_H_ */
