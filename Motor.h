#ifndef MOTOR_H
#define MOTOR_H

#define MAX_ARR 0xFFFF
#define MAX_CCR 0x00FF

void Car_Wheels_Init(void);

void Left_Side_PWN_Init(void);
void Right_Side_PWN_Init(void);

void Left_Direction_Init(void);
void Right_Direction_Init(void);

void Left_Forward(void);
void Right_Forward(void);

#endif