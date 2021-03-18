/*
 * ECE 153B - Summer 2020
 *
 * Name(s): Thao Phan
 * Section: Tuesday 7 - 9 PM
 * Lab: 2A
 */

#include "EXTI.h"

void EXTI_Init(void) {
	// Initialize Joystick
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_1;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_1;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE2;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_1;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE3;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_1;
	
	GPIOA->MODER &= ~GPIO_MODER_MODE5;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_1;
	
	// Configure SYSCFG EXTI
	// [TODO]
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;
	
	// Configure EXTI Trigger
	// [TODO]
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	EXTI->FTSR1 |= EXTI_FTSR1_FT2;
	EXTI->FTSR1 |= EXTI_FTSR1_FT3;
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;
	
	// Enable EXTI
	// [TODO]
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	EXTI->IMR1 |= EXTI_IMR1_IM2;
	EXTI->IMR1 |= EXTI_IMR1_IM3;
	EXTI->IMR1 |= EXTI_IMR1_IM5;
	
	// Configure and Enable in NVIC
	// [TODO
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI2_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI3_IRQn, 0);
	
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_SetPriority(EXTI9_5_IRQn, 0);
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)

void ExampleInterruptHandler(void) {
	// Clear interrupt pending bit
	
	// Define behavior that occurs when interrupt occurs
}

void EXTI0_IRQHandler() {
	// Clear interrupr pending bit
	EXTI->PR1 |= EXTI_PR1_PIF0;
	GPIOE->ODR ^= GPIO_ODR_OD12;
}

void EXTI1_IRQHandler() {
	EXTI->PR1 |= EXTI_PR1_PIF1;
}

void EXTI2_IRQHandler() {
	EXTI->PR1 |= EXTI_PR1_PIF2;
}

void EXTI3_IRQHandler() {
	EXTI->PR1 |= EXTI_PR1_PIF3;
}

void EXTI9_5_IRQHandler() {
	EXTI->PR1 |= EXTI_PR1_PIF3;
}