/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */


  #include "UART.h"
  #include "DMA.h"
  #include <stdio.h>
  #include <string.h>
  
  
  static volatile DMA_Channel_TypeDef * tx;
  static volatile char inputs[IO_SIZE];
  static volatile uint8_t data_t_0[IO_SIZE];
  static volatile uint8_t data_t_1[IO_SIZE];
  static volatile uint8_t input_size = 0;
  static volatile uint8_t pending_size = 0;
  static volatile uint8_t * active = data_t_0;
  static volatile uint8_t * pending = data_t_1;
  
  #define SEL_0 1
  #define BUF_0_EMPTY 2
  #define BUF_1_EMPTY 4
  #define BUF_0_PENDING 8
  #define BUF_1_PENDING 16
  
  void transfer_data(char ch);
  void on_complete_transfer(void);
  
 void UART1_Init(void) {
	UART1_GPIO_Init();
	tx = DMA1_Channel4;
	 DMA_Init_UARTx(DMA1_Channel4, USART1);
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0; 
	DMA1_Channel4->CMAR = (uint32_t) active; 
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C4S; 
	USART_Init(USART1);
	NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);
}

void UART2_Init(void) {
	UART2_GPIO_Init();
	tx = DMA1_Channel7;
	DMA_Init_UARTx(DMA1_Channel7, USART2);
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	DMA1_Channel7->CMAR = (uint32_t) active; 
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C7S; 
	DMA1_CSELR->CSELR |= DMA_ISR_TCIF7;  

	USART_Init(USART2);
	NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);
}

void UART1_GPIO_Init(void) {
	// Enable GPIO Clock
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
	// Enable GPIO Clock
	
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

void USART_Init(USART_TypeDef * USARTx) {
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
	USARTx->CR3 |= USART_CR3_DMAT;
	USARTx->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE;
	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

  
  /**
   * This function accepts a string that should be sent through UART
  */
  void UART_print(char* data) {
  	//TODO
		uint8_t length = strlen(data);
  	//Transfer char array to buffer
  	//Check DMA status. If DMA is ready, send data
  	//If DMA is not ready, put the data aside
  

  	for(uint8_t i = 0; i < length; i++) {
  		pending[i] = data[i];
  	}
  	pending_size = length;
  
  	// Check if the DMA is ready to start a new transaction
  	if((tx->CCR & DMA_CCR_EN) == 0) {
  		// Switch active and pending buffer
  		uint8_t* temp = active;
  		active = pending;
  		pending = temp;
  
  		// Start DMA transaction
  		tx->CMAR = (uint32_t)active;
  		tx->CNDTR = pending_size;
  		tx->CCR |= DMA_CCR_EN;
  		pending_size = 0;
  	}
  }
  
  /**
   * This function should be invoked when a character is accepted through UART
  */
	
	void transfer_data(char ch) {
  
	inputs[input_size++] = ch;
  if(ch == '\n') {
    UART_onInput((char *) inputs, input_size);
    for(int i = 0; i < IO_SIZE; i++) {
      inputs[i] = 0;
    }
    input_size = 0; // Reset the input buffer size
  }
}

  
  /**
   * This function should be invoked when DMA transaction is completed
  */
  void on_complete_transfer(void) {
  	//TODO
  	// If there are pending data to send, switch active and pending buffer, and send data
  
  	if(pending_size > 0) {
  		// Switch active and pending buffer
  		uint8_t * temp = active;
  		active = pending;
  		pending = temp;
  
  		// Start DMA transaction
  		tx->CMAR = (uint32_t) active;
  		tx->CPAR = (uint32_t)&USART2->TDR;
  		tx->CNDTR = pending_size;
  		tx->CCR |= DMA_CCR_EN;
  		pending_size = 0; // reset the pending buffer
  	}
  }
	
	
void USART1_IRQHandler(void){
	NVIC_ClearPendingIRQ(USART1_IRQn);
	if(USART1->ISR & USART_ISR_TC) {
		USART1->ICR |= USART_ICR_TCCF;
		DMA1_Channel4->CCR &= ~DMA_CCR_EN;
		on_complete_transfer();
	}
	if(USART1->ISR & USART_ISR_RXNE) {
		transfer_data((uint8_t)(USART1->RDR & 0xFF));
	}
}

void USART2_IRQHandler(void){
	NVIC_ClearPendingIRQ(USART2_IRQn);
	if(USART2->ISR & USART_ISR_TC) {
		USART2->ICR |= USART_ICR_TCCF;
		DMA1_Channel7->CCR &= ~DMA_CCR_EN;
		on_complete_transfer();
	}
	if(USART2->ISR & USART_ISR_RXNE) {
		transfer_data((uint8_t)(USART2->RDR));
	}
}

