#ifndef POS_CALC
#define POS_CALC

#include <avr/io.h>

#define ACC_CORRECTION_SIZE 10
#define ACC_CORRECTION_FACTOR 70
#define ADC_SCROLL_ARR_SIZE 200
#define ADC_UPPER_LIMIT 2300
#define ADC_LOWER_LIMIT 300
#define TIMER_PRESCALER 600
#define PIN_LEFT_CLICK PB1
#define PIN_RIGHT_CLICK PB0
#define PIN_LEFT_CLICK_STATE PINB & (1<<PIN_LEFT_CLICK)
#define PIN_RIGHT_CLICK_STATE PINB & (1<<PIN_RIGHT_CLICK)
#define POS_PRESC 350000
#define RESOLUTION 5
#define DATA_TO_SEND_SIZE 22
#define GYRO_CALIBRATION_SIZE 1000


typedef struct {
	int16_t valADC[ADC_SCROLL_ARR_SIZE];
	int16_t valADCavg;
	int16_t valADCavgTemp0;
	int16_t valADCavgTemp1;
	int16_t valADCavgTempOUT;
	int16_t ADCCount;

	int16_t accX, accY, accZ;
	int16_t accXRot, accYRot, accZRot;
	float angleAccY, angleAccX, angleAccZ;

	int16_t sinGAccX, sinGAccY, sinGAccZ;
	int16_t corrAccDataX[ACC_CORRECTION_SIZE];
	int16_t corrAccDataY[ACC_CORRECTION_SIZE];
	int16_t corrAccDataZ[ACC_CORRECTION_SIZE];
	uint8_t accCount;

	int16_t gyroX, gyroY, gyroZ;
	float angleGyroX, angleGyroY, angleGyroZ;

	int16_t gravElemX;
	int16_t gravElemZ;

	float velX, velXPrev, velXAvg, velXAvgArr[ACC_CORRECTION_SIZE];
	float velY, velYPrev, velYAvg, velYAvgArr[ACC_CORRECTION_SIZE];
	int32_t posX, posXPrev;
	int32_t posZ, posZPrev;
	int32_t posXdivided;
	int32_t posZdivided;

	int16_t posXcalibration;
	int16_t posZcalibration;

	uint8_t leftButtonState;
}HandPos;

void fillHandPos(HandPos * vhand,int16_t vaccX,int16_t vaccY,int16_t vaccZ,
							int16_t vgyroX, int16_t vgyroY, int16_t vgyroZ);
char* dataToSend(HandPos vhand);
void rotateCoordinate(int16_t vaccA, int16_t vaccB,int16_t *vaccARot, int16_t *vaccBRot, float vangleRotAxis);
void fillDataToSend(char *arr,int16_t arrSize,HandPos * vhand);
void initClickPins();
void gyroCalibration(HandPos* vhand);
uint16_t POW(uint8_t base, uint8_t power);

#endif /* I2C_SPI_H_ */
