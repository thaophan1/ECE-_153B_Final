#include "UART.h"

void Init_USARTx(int x) {

		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);

}

void UART2_Init(void) {
	// [TODO]
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;	// enable USART2 clock 
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;	// setting USART2 clock source as SYSCLK
}

void UART2_GPIO_Init(void) {
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	//enable GPIO_A clock
	
	GPIOA->MODER &= ~GPIO_MODER_MODE2;	// choose alternate function mode
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3;	// choose alternate function mode
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2; // selecting AF7
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2; // selecting AF7
	
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR2;	// configure very high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3;	// configure very high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2; // select push-pull output type
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2; // select pull up mode
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3; // select pull up mode
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]
	USARTx->CR1 &= ~USART_CR1_UE; // disable USART to modify settings
	
	USARTx->CR1 &= ~USART_CR1_M; // clear word length bits, set word length to 8 bits
//	USARTx->CR1 &= ~USART_CR1_M0; // set bit 0 of word length
//	USARTx->CR1 &= ~USART_CR1_M1; // set bit 1 of word length
	
	USARTx->CR1 &= ~USART_CR1_OVER8; // set oversampling mode to 16
	USARTx->CR2 &= ~USART_CR2_STOP; // set number of stop bits to 1
	
	USARTx->BRR = 1667;	// setting USARTDIV so 16 MHz / 1667 = 9600 Baud Rate
	
	USARTx->CR1 |= USART_CR1_TE; // transmitter enable
	USARTx->CR1 |= USART_CR1_RE; // receiver enable
	
	USARTx->CR1 |= USART_CR1_UE; // enable USART
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
