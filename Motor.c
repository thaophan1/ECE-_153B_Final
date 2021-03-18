#include "Motor.h"
#include "stm32l476xx.h"

void Car_Wheels_Init(void) 
{
	Left_Side_PWN_Init();
	Right_Side_PWN_Init();
	
	Left_Direction_Init();
	Right_Direction_Init();
}

void Left_Side_PWN_Init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // enabling GPIO clocks
	
	GPIOE->MODER &= ~GPIO_MODER_MODE14;		   // clearing GPIO mode bits
	GPIOE->MODER |= GPIO_MODER_MODE14_1;	   // setting GPIO mode to alternate functions
	GPIOE->AFR[1] = GPIO_AFRH_AFSEL14_0;	   // select TIM1_CH2 as function
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD14;		   // setting no PUPD
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT14;		   // setting push-pull mode
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14; // setting very high output speed
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable timer 1
	
	TIM1->CCMR2 |= (TIM_CCMR2_OC4PE | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2); // setting preload enable bit
	
	TIM1->CCER = TIM_CCER_CC4E; // enabling channel 4 output
	TIM1->CCR4 = MAX_CCR; // setting CCR value
}

void Right_Side_PWN_Init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // enabling GPIO clocks
	
	GPIOE->MODER &= ~GPIO_MODER_MODE13;		   // clearing GPIO mode bits
	GPIOE->MODER |= GPIO_MODER_MODE13_1;	   // setting GPIO mode to alternate functions
	GPIOE->AFR[1] = GPIO_AFRH_AFSEL13_0;	   // select TIM1_CH2 as function
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD13;		   // setting no PUPD
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT13;		   // setting push-pull mode
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13; // setting very high output speed
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable timer 1
	
	TIM1->CCMR2 |= (TIM_CCMR2_OC3PE | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2); // setting preload enable bit
	
	TIM1->CCER = TIM_CCER_CC3E; // enabling channel 3 output
	TIM1->CCR3 = MAX_CCR; // setting CCR value
}

void Left_Direction_Init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~GPIO_MODER_MODE2; // Forward direction pin
	GPIOB->MODER |= GPIO_MODER_MODE2_0;
	GPIOB->MODER &= ~GPIO_MODER_MODE3; // Backward direction pin
	GPIOB->MODER |= GPIO_MODER_MODE3_0;
	
	// start the car in off mode
	GPIOB->ODR &= ~GPIO_ODR_OD2;
	GPIOB->ODR &= ~GPIO_ODR_OD3;
}

void Right_Direction_Init(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	GPIOC->MODER &= ~GPIO_MODER_MODE14; // Forward direction pin
	GPIOC->MODER |= GPIO_MODER_MODE14_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE15; // Backward direction pin
	GPIOC->MODER |= GPIO_MODER_MODE15_0;
	
	// start the car in off mode
	GPIOC->ODR &= ~GPIO_ODR_OD14;
	GPIOC->ODR &= ~GPIO_ODR_OD15;
}

void Left_Forward(void)
{
	GPIOB->ODR |= GPIO_ODR_OD2;
	GPIOB->ODR &= ~GPIO_ODR_OD3;
}

void Right_Forward(void)
{
	GPIOC->ODR |= GPIO_ODR_OD14;
	GPIOC->ODR &= ~GPIO_ODR_OD15;
}