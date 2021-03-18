#include "Nunchuk.h"
#include "I2C.h"

void Init_Nunchuk(I2C_TypeDef *I2Cx, uint8_t SAD_W) {
	uint8_t Setup_Bytes[2] = {0xF0, 0x55};
	I2C_SendData(I2Cx, SAD_W, Setup_Bytes, 2);
	
	Setup_Bytes[0] = 0xFB;
	Setup_Bytes[1] = 0x00;
	I2C_SendData(I2Cx, SAD_W, Setup_Bytes, 2);
}

void Read_Nunchuk(I2C_TypeDef *I2Cx, uint8_t *pData) {
	uint8_t SAD_W = 0xA4;
	uint8_t SAD_R = 0xA5;
	uint8_t Conversion_Command = 0x00;
	I2C_SendData(I2Cx, SAD_W, &Conversion_Command, 1);
	I2C_ReceiveData(I2Cx, SAD_R, pData, 6);

}