#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>
#include "stm32l476xx.h"

void Accel_Set_Normal_Mode(I2C_TypeDef *I2Cx, uint8_t SAD_W);

#endif