/*
 * ECE 153B - Winter 2023
 *
 * Lab: 6A
 */

#include "stm32l476xx.h"
#include "motor.h"

static const uint32_t MASK = 65535;//TODO
static const uint32_t HalfStep[8] = {32,288,256,320,64,576,512,544};//TODO
static const uint32_t AndvsOr[8] = {1,0,1,0,1,0,1,0};

static volatile int8_t dire = 0;
static volatile int8_t step = 0;
static volatile int8_t andorstep = 0;

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
	if (dire == 0) {
		step += 1;
		step = step % 8;
		if (AndvsOr[andorstep] == 1) {
			GPIOC->ODR |= HalfStep[step];
			
		}
		else {
			GPIOC->ODR &= HalfStep[step];
			
		}
		andorstep += 1;
		andorstep = andorstep % 8;
	
}
	
else if (dire == 1) {
	step -= 1;
	if (step < 0) {
		step = 7;
	}
	if (AndvsOr[andorstep] == 1) {
		GPIOC->ODR |= HalfStep[step];
		
	}
	else{
		GPIOC->ODR &= HalfStep[step];
	}
	andorstep -= 1;
	if (andorstep < 0) {
		andorstep = 7;
	}
	
	
	else if (dire == 2) {
	}
	
	
	
	
}
	}

void setDire(int8_t direction) {
	//TODO
	dire = direction;
}
