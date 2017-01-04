/*
 * pos_calc.c
 *
 *  Created on: 9 pa? 2016
 *      Author: Mariusz
 */

#include "../Includes/pos_calc.h"
#include "../Includes/I2C_SPI.h"
#include "../MYUART/myuart.h"
#include <string.h>
#include <stdlib.h>

#include <limits.h>
const uint32_t posX_coefficient =  (POS_PRESC)/ 768;
const uint32_t posZ_coefficient =  (POS_PRESC)/1366;


void fillHandPos(HandPos * vhand,int16_t vaccX, int16_t vaccY,int16_t vaccZ,
		        				 int16_t vgyroX, int16_t vgyroY, int16_t vgyroZ)
{
	vhand->accX = vaccX;
	vhand->accY = vaccY;
	vhand->accZ = vaccZ;
	vhand->accCount++;
	vhand->accCount %= ACC_CORRECTION_SIZE;
	vhand->corrAccDataX[vhand->accCount] = vaccX;
	vhand->corrAccDataY[vhand->accCount] = vaccY;
	vhand->corrAccDataZ[vhand->accCount] = vaccZ;
	vhand->leftButtonState = (PIN_LEFT_CLICK_STATE)?0:1;

	vhand->posX -= (vgyroX + vhand->posXPrev - 2*vhand->posXcalibration)*RESOLUTION/2;//dzielenie przez okolo 70000 //INT16_MAX*250*3.1415/180/TIMER_PRESCALER*1.19)*250;
	vhand->posZ += (vgyroZ + vhand->posZPrev - 2*vhand->posZcalibration)*RESOLUTION/2 ;//INT16_MAX*250*3.1415/180/TIMER_PRESCALER*1.19 )*250;
	vhand->posXPrev = vgyroX;
	vhand->posZPrev = vgyroZ;

	if(vhand->posX > POS_PRESC)	vhand->posX = POS_PRESC;
	if(vhand->posZ > POS_PRESC)	vhand->posZ = POS_PRESC;
	if(vhand->posX < 0)	vhand->posX = 0;
	if(vhand->posZ < 0)	vhand->posZ = 0;

	vhand->posXdivided = vhand->posX/posX_coefficient;
	vhand->posZdivided = vhand->posZ/posZ_coefficient;


//	uart_putc(13);
//	uart_putc(11);
//	uart_puts(" SPI X divided = ");
//	uart_putlong(vhand->posX,10);
//	uart_putc(9);
//	uart_puts(" SPI Z divided = ");
//	uart_putlong(vhand->posZ,10);

}

void fillDataToSend(char *arr,int16_t arrSize,HandPos * vhand){
	for(int i = 0;;){
		if(i>=arrSize)
			break;
		if(i==0)arr[i]='G';
		if(i==1)arr[i]='X';
		if(i==6)arr[i]='Y';
		if(i==11)arr[i]='L';
		if(i==13)arr[i]='V';
		if(i>=18)arr[i]='A';
		i++;
		if(i==2)
			for(int j=0;j<4;j++){
				int16_t k = (vhand->posXdivided/POW(10,3-j))%10;
				itoa(abs(k),arr+i,10);
				i++;
			};
		if(i==7)
			for(int j=0;j<4;j++){
				int16_t k = (vhand->posZdivided/POW(10,3-j))%10;
				itoa(abs(k),arr+i,10);
				i++;
			};
		if(i==12){
			itoa(abs(vhand->leftButtonState),arr+i,10);
			i++;
		}
		if(i==14)
		for(int j=0;j<4;j++){
			int m = j;
			if(j>0&&vhand->valADCavgTempOUT<0)
				m--;
			int k = (int16_t)(vhand->valADCavgTempOUT/POW(10,2-m))%10;
			if(j==0&&vhand->valADCavgTempOUT<0){
				arr[i]='-';
				itoa(abs(k),arr+i+1,10);
				i++;j++;
			}
			else if(j==0&&vhand->valADCavgTempOUT>=0){
				arr[i]='0';
				itoa(abs(k),arr+i+1,10);
				i++;j++;
			}
			else{
				i++;
				itoa(abs(k),arr+i,10);
			}
		};

	}
}

void initClickPins(){
	//DDRB &= (~(1<<PIN_LEFT_CLICK) & ~(1<<PIN_RIGHT_CLICK));
	PORTB |= (1<<PIN_LEFT_CLICK) | (1<<PIN_RIGHT_CLICK);
}

uint16_t POW(uint8_t base, uint8_t power){

	uint16_t temp = 1;
	while(power--)
		temp*=base;
	return temp;
}

void gyroCalibration(HandPos* vhand){

	int16_t DataGyroTempX = 0;
	int16_t DataGyroTempY = 0;
	int16_t DataGyroTempZ = 0;
	int32_t DataGyroTempXSum = 0;
	int32_t DataGyroTempZSum = 0;

	for(size_t i = 0; i<20; i++)
			getPositionDataSPI(&DataGyroTempX,&DataGyroTempY,&DataGyroTempZ);

	for(size_t i = 0; i<GYRO_CALIBRATION_SIZE; i++){
		getPositionDataSPI(&DataGyroTempX,&DataGyroTempY,&DataGyroTempZ);
		DataGyroTempXSum+=DataGyroTempX;
		DataGyroTempZSum+=DataGyroTempZ;
		uart_putc('\n');
		uart_putc(CARRIAGE_RETURN);
		uart_puts(" SPI X divided = ");
		uart_putlong(DataGyroTempXSum,10);
		uart_putc(HORIZONTAL_TAB);
		uart_puts(" SPI Z divided = ");
		uart_putlong(DataGyroTempZSum,10);
		uart_putc(HORIZONTAL_TAB);
		uart_puts(" I = ");
		uart_putlong(i,10);
	}
	vhand->posXcalibration = DataGyroTempXSum/GYRO_CALIBRATION_SIZE;
	vhand->posZcalibration = DataGyroTempZSum/GYRO_CALIBRATION_SIZE;
	uart_putc('\n');
	uart_putc(CARRIAGE_RETURN);
	uart_puts(" posXcalibration = ");
	uart_putlong(vhand->posXcalibration,10);
	uart_putc(HORIZONTAL_TAB);
	uart_puts(" posZcalibration = ");
	uart_putlong(vhand->posZcalibration,10);
}
