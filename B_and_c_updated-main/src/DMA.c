/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Tanish Kumar, Zach Reiber
 * Section: 
 * Lab: Final Project
 */
 
#include "DMA.h"
#include "SysTimer.h"
#include "UART.h"

void DMA_Init_UARTx(DMA_Channel_TypeDef * tx, USART_TypeDef * uart) {
    
		// Enable the clock for DMA
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;		
		
		// Set delay to 20 us
		USART_Delay(20);
	
		// Disable DMA channel
    tx->CCR &= ~DMA_CCR_EN;

    // Set memory-to-memory mode
    tx->CCR &= ~DMA_CCR_MEM2MEM;

    // Set channel priority to high
    tx->CCR &= ~DMA_CCR_PL;
    tx->CCR |= DMA_CCR_PL_1;
    
    // Set peripheral size to 8-bit
    tx->CCR &= ~DMA_CCR_PSIZE;

    // Set memory size to 8-bit
    tx->CCR &= ~DMA_CCR_MSIZE;

    // Disable peripheral increment mode
    tx->CCR &= ~DMA_CCR_PINC;

    // Enable memory increment mode
    tx->CCR |= DMA_CCR_MINC;

    // Disable circular mode
    tx->CCR &= ~DMA_CCR_CIRC;

    // Set data transfer direction to Peripheral-to-Memory
    tx->CCR |= DMA_CCR_DIR;
    
		// DMA1_CSELR->CSELR |= 2 << 24;
		
    // Configure the peripheral address as the UART data register address
    tx->CPAR = (uint32_t)&(uart->TDR);
        
    // Disable half transfer interrupt
    tx->CCR &= ~(DMA_CCR_TEIE);
    
    // Disable transfer error interrupt
		tx->CCR &= ~(DMA_CCR_HTIE);
		
		// Enable transfer complete interrupt
		tx->CCR &= ~(DMA_CCR_TCIE);
}
