#include "PWM.h"

void PWM_Init() {
	// Enable GPIO Port E Clock
	// [TODO
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	// Enable TIM1 Clock
	// [TODO]
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	// Configure PE8
	// [TODO]
	GPIOE->MODER &= ~GPIO_MODER_MODE8;
	GPIOE->MODER |= GPIO_MODER_MODE8_1;
	
	GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;
	
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD8;
	
	// Configure PWM Output for TIM1 CH 1N
	// [TODO]
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL8_0; 					//choose the TIME1_CH1N function
	TIM1->CR1 &= ~TIM_CR1_DIR;										//set up count direction
	
	TIM1->PSC = 399;															//set prescaler value
	TIM1->ARR = 99;																//set ARR value
	
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;								//clear output compare mode 1 bits
	
	/* setting output compare mode to PWM mode 1 */
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;							//setting bit 0
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;							//setting bit 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;							//setting bit 2
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_3;							//setting bit 3
	
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;								//setting preload enable bit
	
	TIM1->CCER &= ~TIM_CCER_CC1P;									//setting output polarity to high
	TIM1->CCER |= TIM_CCER_CC1NE; 								//enable channel 1N output
	
	TIM1->BDTR |= TIM_BDTR_MOE;										//enable main output
	TIM1->CCR1 = 99;															//set CCR value
	
	TIM1->CR1 |= TIM_CR1_CEN; 										//counter enable
}