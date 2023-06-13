#include "SPI.h"
#include "SysTimer.h"

void SPI1_GPIO_Init(void) {
	// Enable the GPIO Clock
	// Set PA4, PB3, PB4, and PB5 to Alternative Functions, and configure their AFR to SPI1
	// Set GPIO Pins to: Very High Output speed, Output Type Push-Pull, and No Pull-Up/Down
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOA->MODER &= ~(3UL << 8);
	GPIOB->MODER &= ~(3UL << 8);
	GPIOB->MODER &= ~(3UL << 6);
	GPIOB->MODER &= ~(3UL << 10);
	GPIOA->MODER |= 1UL << 9;
	GPIOB->MODER |= 1UL << 9;
	GPIOB->MODER |= 1UL << 11;
	GPIOB->MODER |= 1UL << 7;
	GPIOA->AFR[0] &= ~(15UL << 16);
	GPIOB->AFR[0] &= ~(15UL << 16);
	GPIOB->AFR[0] &= ~(15UL << 12);
	GPIOB->AFR[0] &= ~(15UL << 20);
	GPIOB->AFR[0] |= 5UL << 12;
	GPIOB->AFR[0] |= 5UL << 16;
	GPIOB->AFR[0] |= 5UL << 20;
	GPIOA->AFR[0] |= 5UL << 16;
	GPIOA->OSPEEDR |= 3UL << 8;
	GPIOB->OSPEEDR |= 3UL << 8;
	GPIOB->OSPEEDR |= 3UL << 6;
	GPIOB->OSPEEDR |= 3UL << 10	;
	GPIOA->PUPDR &= ~(3UL << 8);
	GPIOB->PUPDR &= ~(3UL << 8);
	GPIOB->PUPDR &= ~(3UL << 6);
	GPIOB->PUPDR &= ~(3UL << 10);
	GPIOA->OTYPER &= ~(1UL << 4);
	GPIOB->OTYPER &= ~(1UL << 3);
	GPIOB->OTYPER &= ~(1UL << 4);
	GPIOB->OTYPER &= ~(1UL << 5);
		
}
	


void SPI1_Init(void){
	// Enable SPI clock and Reset SPI
	RCC->APB2ENR |= 1UL << 12;
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	SPI1->CR1 &= ~(1UL << 6);
	SPI1->CR1 &= ~(1UL << 10);
	SPI1->CR1 &= ~(1UL << 15);
	SPI1->CR1 &= ~(1UL << 14);
	SPI1->CR1 &= ~(1UL << 7);
	SPI1->CR2 |= (15UL << 8);
	SPI1->CR2 &= ~(1UL << 4);
	SPI1->CR1 |= 1UL;
	SPI1->CR1 |= 1UL << 1;
	SPI1->CR1 |= 3UL << 3;
	SPI1->CR1 &= ~(1UL << 13);
	SPI1->CR1 |= 1UL << 2;
	SPI1->CR1 &= ~(1UL << 9);
	SPI1->CR2 |= 1UL << 3;
	SPI1->CR2 &= ~(1UL << 12);
	SPI1->CR2 |= 1UL << 2;
	SPI1->CR1 |= (1UL << 6);
	
	// Disable SPI
	// Configure for Full Duplex Communication
	// Configure for 2-line Unidirectional Data Mode
	// Disable Output in Bidirectional Mode
	// Set Frame Format: MSB First, 16-bit, Motorola Mode
	// Configure Clock. Read DataSheet for required value
	// Set Baud Rate Prescaler to 16
	// Disable Hardware CRC Calculation
	// Set as Master
	// Disable Software Slave Management
	// Enable NSS Pulse Management
	// Enable Output
	// Set FIFO Reception Threshold to 1/2
	// Enable SPI
	
}

uint16_t SPI_Transfer_Data(uint16_t write_data){
	
	while ((SPI1->SR & 1UL << 1) != 2) {
	}		
	// Wait for TXE (Transmit buffer empty)
	SPI1->DR = write_data;
	// Write data
	// Wait for not busy
	
	while (((SPI1->SR & (1UL << 7)) == 128)) {
	}
	while ((SPI1->SR & 1UL) != 1) {
		
	}
	uint16_t data = (uint16_t)SPI1->DR;
	
	// Wait for RXNE (Receive buffer not empty)
	// Read data
	return data; // TODO
}
