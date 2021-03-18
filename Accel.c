#include "Accel.h"
#include "SPI.h"

// Modifed from l3gd20.c V2.0.0, COPYRIGHT(c) 2015 STMicroelectronics

void Accel_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Initialize SPI
	SPI1_GPIO_Init();
	SPI_Init();
	
	// Initialize Accel
	Accel_IO_CS_Init();	
	Chip_Init();
}

// Gyroscope IO functions
void Accel_IO_CS_Init(void) {
	// [TODO]
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= GPIO_MODER_MODE1_0;
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT1;
	
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
	
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	
	// Deselect the Gyroscope
	Accel_CS_HIGH;
}

void Accel_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite){
	
	uint8_t rxBuffer[32];
	
	/* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
	*/
	if(NumByteToWrite > 0x01){
		WriteAddr |= (uint8_t) MULTIPLEBYTE_CMD; // Set write mode & auto-increment
	}
	
	// Set chip select Low at the start of the transmission 
	Accel_CS_LOW;  // Gyro CS low
	SPI_Delay(10);  
	
	// Send the Address of the indexed register 
	SPI_Write(SPI1, &WriteAddr, rxBuffer, 1);
	
	// Send the data that will be written into the device (MSB First) 
	SPI_Write(SPI1, pBuffer, rxBuffer, NumByteToWrite);
  
	// Set chip select High at the end of the transmission  
	SPI_Delay(10); // Gyro CS high
	Accel_CS_HIGH;
}

void Accel_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead){

	uint8_t rxBuffer[32];
	
	uint8_t AddrByte = ReadAddr | ((uint8_t) 0xC0);  // set read mode & autoincrement
	
	// Set chip select Low at the start of the transmission 
	Accel_CS_LOW; // Gyro CS low
	SPI_Delay(10);
	
	// Send the Address of the indexed register 
	SPI_Write(SPI1, &AddrByte, rxBuffer, 1);
	
	// Receive the data that will be read from the device (MSB First) 
	SPI_Read(SPI1, pBuffer, NumByteToRead);
  
	// Set chip select High at the end of the transmission  
	SPI_Delay(10); // Gyro CS high
	Accel_CS_HIGH;
}	


void Chip_Init(void) {  
	// [TODO]
	uint8_t buffer;
	
	buffer = 0x23;
	Accel_IO_Write(&buffer, 0x20, 1);
	
	buffer = 0x08;
	Accel_IO_Write(&buffer, 0x23, 1);
}
