#include "SysClock.h"

//******************************************************************************************
// Switch the PLL source from MSI to HSI, and select the PLL as SYSCLK source.
//******************************************************************************************
#include "SysClock.h"

//void System_Clock_Init(void) {
//	// Enable HSI
//	RCC->CR |= RCC_CR_MSION;
//	RCC->CR |= RCC_CR_MSIRGSEL;
//	RCC->CR &= ~RCC_CR_MSIRANGE;
//	while((RCC->CR & RCC_CR_MSIRDY) == 0);
//	
//	// Select MSI as system clock source
//	RCC->CFGR &= ~RCC_CFGR_SW;
//	RCC->CFGR |= RCC_CFGR_SW_MSI;
//	//while((RCC->CFGR & RCC_CFGR_SWS) == 0);
//}

void System_Clock_Init(void) {
		// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0)
		; // Wait until HSI is ready

	// Select HSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0)
		; // Wait until HSI is system clock source
}