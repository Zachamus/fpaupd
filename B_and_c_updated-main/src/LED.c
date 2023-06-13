/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	// [TODO]
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; 
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	
	// Initialize Green LED
	// [TODO]
		GPIOA->MODER &= ~(3UL<<10);
		GPIOA->MODER |= 1UL<<10;
		GPIOA->PUPDR &= ~(3UL<<10);
		GPIOA->OTYPER &= ~(1UL<<5);
		GPIOA->ODR &= ~(1UL << 5);
}

void Green_LED_Off(void) {
	// [TODO]
		GPIOA->ODR &= ~(1UL << 5);
}

void Green_LED_On(void) {
	// [TODO]
		GPIOA->ODR |= (1UL << 5);
	
}

void Green_LED_Toggle(void) {
	// [TODO]
	GPIOA->ODR ^= (1UL << 5);
}