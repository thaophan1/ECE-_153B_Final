#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <stdint.h>
#include "stm32l476xx.h"

void Init_Nunchuk(I2C_TypeDef *I2Cx, uint8_t SAD_W);
void Read_Nunchuk(I2C_TypeDef *I2Cx, uint8_t *pData);

#endif