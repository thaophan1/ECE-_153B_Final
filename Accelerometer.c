#include "Accelerometer.h"
#include "I2C.h"

void Accel_Set_Normal_Mode(I2C_TypeDef *I2Cx, uint8_t SAD_W) {
	uint8_t CR1_Data[2] = {0x20, 0x23};
	uint8_t CR4_Data[2] = {0x23, 0x08};
	
	I2C_SendData(I2Cx, SAD_W, CR1_Data, 2);
	
	I2C_SendData(I2Cx, SAD_W, CR4_Data, 2);
}