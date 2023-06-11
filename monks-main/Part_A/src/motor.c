/*
 * ECE 153B - Winter 2023
 *
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = 65535;
static const uint32_t HalfStep[8] = {32,288,256,320,64,576,512,544};

static int8_t dire = 0;
static volatile uint8_t step = 0;
static int mod = 0;
static int mod2 = 0;
void Motor_Init(void) {	
	
	//TODO
	
	 
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		GPIOC->MODER &= ~(3UL<<10);
		GPIOC->MODER &= ~(3UL<<12);
		GPIOC->MODER &= ~(3UL<<16);
		GPIOC->MODER &= ~(3UL<<18);
	
		GPIOC->MODER |= 1UL<<10;
		GPIOC->MODER |= 1UL<<12;
		GPIOC->MODER |= 1UL<<16;
		GPIOC->MODER |= 1UL<<18;
	
		GPIOC->OSPEEDR &= ~(3UL<<10);
		GPIOC->OSPEEDR &= ~(3UL<<12);
		GPIOC->OSPEEDR &= ~(3UL<<16);
		GPIOC->OSPEEDR &= ~(3UL<<18);
		
		GPIOC->OSPEEDR |= 1UL << 11;
		GPIOC->OSPEEDR |= 1UL << 13;
		GPIOC->OSPEEDR |= 1UL << 17;
		GPIOC->OSPEEDR |= 1UL << 19;
		
		GPIOC->OTYPER &= ~(1UL << 5);
		GPIOC->OTYPER &= ~(1UL << 6);
		GPIOC->OTYPER &= ~(1UL << 8);
		GPIOC->OTYPER &= ~(1UL << 9);
		
		GPIOC->PUPDR &= ~(3UL << 10);
		GPIOC->PUPDR &= ~(3UL << 12);
		GPIOC->PUPDR &= ~(3UL << 16);
		GPIOC->PUPDR &= ~(3UL << 18);
	
}

void rotate(void) {
	//TODO
	GPIOC->ODR &= ~(MASK);
	GPIOC->ODR |= HalfStep[0];
	if (dire == 0) {
	for (int i = 1; i <= 7; i++) {
		if (mod % 2 == 0) {
			GPIOC-> ODR |= HalfStep[i];
			mod += 1;
			for (int k =0; k < 10000; k++) {
			}
			
		}
		else {
			GPIOC-> ODR &= HalfStep[i];
			mod -= 1;
			for (int k =0; k < 10000; k++) {
			}
			
		}
	}
	GPIOC->ODR &= ~(MASK);
mod = 0;
	}		
	else {
		GPIOC->ODR &= ~(MASK);
		for (int j = 7; j > 0; j--) {
			if (mod % 2 == 0) {
				GPIOC-> ODR |= HalfStep[j];
				mod += 1;
				for (int k =0; k < 10000; k++) {
			}
				
				
			}
			else {
				GPIOC-> ODR &= HalfStep[j];
				mod -= 1;
				for (int k =0; k < 10000; k++) {
			}
			}
			
		}
		
		mod = 0;
		GPIOC->ODR &= ~(MASK);
		
		
	}
}

void setDire(int8_t direction) {
	//TODO
	dire = direction;
}
	


