#include "SPI.h"
#include "SysTimer.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

// LSM303C eCompass (a 3D accelerometer and 3D magnetometer module) SPI Interface: 
//   MAG_DRDY = PC2   MEMS_SCK  = PD1 (SPI2_SCK)   XL_CS  = PE0             
//   MAG_CS   = PC0   MEMS_MOSI = PD4 (SPI2_MOSI)  XL_INT = PE1       
//   MAG_INT  = PC1 
//
// L3GD20 Gyro (three-axis digital output) SPI Interface: 
//   MEMS_SCK  = PD1 (SPI2_SCK)    GYRO_CS   = PD7 (GPIO)
//   MEMS_MOSI = PD4 (SPI2_MOSI)   GYRO_INT1 = PD2
//   MEMS_MISO = PD3 (SPI2_MISO)   GYRO_INT2 = PB8

extern uint8_t Rx1_Counter;
extern uint8_t Rx2_Counter;

void SPI1_GPIO_Init(void) {
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	GPIOE->MODER &= ~GPIO_MODER_MODE13;
	GPIOE->MODER |= GPIO_MODER_MODE13_1;
	GPIOE->MODER &= ~GPIO_MODER_MODE14;
	GPIOE->MODER |= GPIO_MODER_MODE14_1;
	GPIOE->MODER &= ~GPIO_MODER_MODE15;
	GPIOE->MODER |= GPIO_MODER_MODE15_1;
	
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL13;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL13_0 | GPIO_AFRH_AFSEL13_2; // choosing AF5
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL14;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL14_0 | GPIO_AFRH_AFSEL14_2; // choosing AF5
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL15;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2; // choosing AF5
	
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT13;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT14;
	GPIOE->OTYPER &= ~GPIO_OTYPER_OT15;
	
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;
	
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD13;
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD14;
	GPIOE->PUPDR &= ~GPIO_PUPDR_PUPD15;
}

void SPI_Init(void){
	// [TODO] 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1 clock
	
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // reset SPI1 
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; // disable reset SPI1
	
	SPI1->CR1 &= ~SPI_CR1_SPE; // disable SPI
	
	SPI1->CR1 &= ~SPI_CR1_RXONLY; // configure full-duplex communication
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE; // configure communication for 2-line unidirectional data mode
	SPI1->CR1 &= ~SPI_CR1_BIDIOE; // disable output for bidirectional mode
	
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // setting MSB first for frame format
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2; // setting data length to 8 bits
	SPI1->CR2 &= ~SPI_CR2_FRF; // setting frame format to be in Motorola mode
	
	SPI1->CR1 &= ~SPI_CR1_CPOL; // setting clock polarity to 0
	SPI1->CR1 &= ~SPI_CR1_CPHA; // setting clock phase to first clock transition
	
	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1; // setting baud rate prescaler to 16
	SPI1->CR1 &= ~SPI_CR1_CRCEN; // disable CRC calculation
	
	SPI1->CR1 |= SPI_CR1_MSTR; // select device to operate in master mode
	SPI1->CR1 |= SPI_CR1_SSM; // enable software slave management
	SPI1->CR2 |= SPI_CR2_NSSP; // enable NSS pulse management
	SPI1->CR1 |= SPI_CR1_SSI; // enable slave select bit
	SPI1->CR2 |= SPI_CR2_FRXTH; // setting FIFO selection threshold to 1/4
	
	SPI1->CR1 |= SPI_CR1_SPE; // enable SPI
}
 
void SPI_Write(SPI_TypeDef * SPIx, uint8_t *txBuffer, uint8_t * rxBuffer, int size) {
	volatile uint32_t tmpreg; 
	int i = 0;
	for (i = 0; i < size; i++) {
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE );  // Wait for TXE (Transmit buffer empty)
		*((volatile uint8_t*)&SPIx->DR) = txBuffer[i];
		while((SPIx->SR & SPI_SR_RXNE ) != SPI_SR_RXNE); // Wait for RXNE (Receive buffer not empty)
		rxBuffer[i] = *((__IO uint8_t*)&SPIx->DR);
	}
	while( (SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY ); // Wait for BSY flag cleared
}

void SPI_Read(SPI_TypeDef * SPIx, uint8_t *rxBuffer, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE ); // Wait for TXE (Transmit buffer empty)
		*((volatile uint8_t*)&SPIx->DR) = rxBuffer[i];	
		// The clock is controlled by master. Thus the master has to send a byte
		// data to the slave to start the clock. 
		while((SPIx->SR & SPI_SR_RXNE ) != SPI_SR_RXNE); 
		rxBuffer[i] = *((__IO uint8_t*)&SPIx->DR);
	}
	while( (SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY ); // Wait for BSY flag cleared
}

void SPI_Delay(uint32_t us) {
	uint32_t i, j;
	for (i = 0; i < us; i++) {
		for (j = 0; j < 18; j++) // This is an experimental value.
			(void)i;
	}
}
