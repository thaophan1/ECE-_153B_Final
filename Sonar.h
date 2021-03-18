#ifndef SONAR_H
#define SONAR_H

#include "stm32l476xx.h"

extern uint32_t volatile currentValue;
extern uint32_t volatile lastValue;
extern uint32_t volatile overflowCount;
extern uint32_t volatile timeInterval;

void Input_Capture_Setup(void);
void Trigger_Setup(void);
void Init_Sonar(void);

#endif