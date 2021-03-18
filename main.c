/*
 * ECE 153B - Summer 2020
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32l476xx.h"
#include "EXTI.h"
#include "Motor.h"
#include "SysClock.h"
#include "PWM.h"
#include "I2C.h"
#include "LCD.h"
#include "Accelerometer.h"
#include "Nunchuk.h"
#include "Sonar.h"
#include "UART.h"
#include "LED.h"
#include "Accel.h"

int volatile distance = 0;

int main() {
	// Initialization
	LCD_Initialization();
	LCD_Clear();
	
	Init_Sonar();
	
	System_Clock_Init();
	EXTI_Init();
	//Car_Wheels_Init();
	//PWM_Init();
	
	I2C_GPIO_Init();
	I2C_Initialization();
	
	LED_Init();
	Init_USARTx(2);
	
	uint8_t accel_addr, nunchuk_addr;
	uint8_t X_L_Reg = 0x29;
	uint8_t accel_data;
	uint8_t nunchuk_data[6];
	uint8_t bz;
	
	// accel_addr = 001100x for accelerometer
	// x = 1 if SD0 pad is connected to voltage supply
	// x = 0 is SD0 pad is connected to ground

	accel_addr = 0x18 << 1;
	nunchuk_addr = 0x52 << 1;
	uint8_t conversion = 0xFA;
	int distance;
	char message[6];
	char rxByte;
	
	// Nunchuk Setup
	Init_Nunchuk(I2C1, nunchuk_addr);
	I2C_SendData(I2C1, nunchuk_addr, &conversion, 1);
	I2C_ReceiveData(I2C1, nunchuk_addr | 1, nunchuk_data, 6);
	Read_Nunchuk(I2C1, nunchuk_data);
	
	while(1) {
		distance = timeInterval / 58;
		
		Read_Nunchuk(I2C1, nunchuk_data);
		
		bz = nunchuk_data[5] & 0x1;
		//printf("x: %d | y: %d | bz: %d\n", nunchuk_data[0], nunchuk_data[1], bz);
		printf("distance: %d\n", distance);
		if (!bz)
		{
			Left_Forward();
			Right_Forward();
		}
		
		Red_LED_On();
	}
	
	return 0;
}
