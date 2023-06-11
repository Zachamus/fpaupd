#include "UART.h"

void UART1_Init(void) {
	// [TODO]
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->CCIPR &= ~(3UL);
	RCC->CCIPR |= 1UL;
	
	
}

void UART2_Init(void) {
	// [TODO]
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR &= ~(3UL << 2);
	RCC->CCIPR |= 1UL << 2;
}

void UART1_GPIO_Init(void) {
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~(3UL << 12);
	GPIOB->MODER &= ~(3UL << 14);
	GPIOB->MODER |= (1UL << 15);
	GPIOB->MODER |= (1UL << 13);
	GPIOB->OSPEEDR |= 3UL << 12;
	GPIOB->OSPEEDR |= 3UL << 14;
	GPIOB->OTYPER &= ~(1UL << 6);
	GPIOB->OTYPER &= ~(1UL << 7);
	GPIOB->PUPDR &= ~(3UL << 12);
	GPIOB->PUPDR &= ~(3UL << 14);
	GPIOB->PUPDR |= 1UL << 12;
	GPIOB->PUPDR |= 1UL << 14;
	GPIOB->AFR[0] &= ~(15UL << 24);
	GPIOB->AFR[0] &= ~(15UL << 28);
	GPIOB->AFR[0] |= (7UL << 24);
	GPIOB->AFR[0] |= (7UL << 28);
}

void UART2_GPIO_Init(void) {
	// [TODO]
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(3UL << 4);
	GPIOA->MODER &= ~(3UL << 6);
	GPIOA->MODER |= 1UL << 5;
	GPIOA->MODER |= 1UL << 7;
	GPIOA->OSPEEDR |= 3UL << 4;
	GPIOA->OSPEEDR |= 3UL << 6;
	GPIOA->OTYPER &= ~(1UL << 2);
	GPIOA->OTYPER &= ~(1UL << 3);
	GPIOA->PUPDR &= ~(3UL << 4);
	GPIOA->PUPDR &= ~(3UL << 6);
	GPIOA->PUPDR |= 1UL << 4;
	GPIOA->PUPDR |= 1UL << 6;
	GPIOA->AFR[0] &= ~(15UL << 8);
	GPIOA->AFR[0] &= ~(15UL << 12);
	GPIOA->AFR[0] |= (7UL << 12);
	GPIOA->AFR[0] |= (7UL << 8);
}


void USART_Init(USART_TypeDef* USARTx) {
	// Disable USART before configuring settings
	USARTx->CR1 &= ~USART_CR1_UE;
	
	// Set Communication Parameters
	USARTx->CR1 &= ~(USART_CR1_M);     // 00 -> 8 Data Bits
	USARTx->CR1 &= ~(USART_CR1_OVER8); // 0 -> Oversampling by 16
	USARTx->CR2 &= ~(USART_CR2_STOP);  // 00 -> 1 Stop Bit
	
	// Set Baud Rate
	// f_CLK = 80 MHz, Baud Rate = 9600 = 80 MHz / DIV -> DIV = 8333 = 0x208D
	USARTx->BRR = 0x208D;
	
	// Enable Transmitter/Receiver
	USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USARTx->ICR &= ~(USART_ICR_ORECF);

	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
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
