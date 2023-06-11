/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include "motor.h"
#include <stdio.h>

int main(void) {
	char ch;
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	Motor_Init();
	SysTick_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);//TODO
	char rxByte = '0';
	
	printf("Program Starts.\r\n");
	printf("Please Enter 0 to open door, or 1 to close door \n");

	while(1) {
		
		scanf("%s",&rxByte);
		
		if (rxByte == '0') {
			setDire(0);
			UART2_GPIO_Init();
		UART2_Init();
		USART_Init(USART2);

			continue;
			
		}
		else if (rxByte == '1'){
			setDire(1);
			UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
			
	
			continue;
		}
		
		else {
			printf("Please enter either a 0 or a 1 \n");
			UART2_GPIO_Init();
		UART2_Init();
		USART_Init(USART2);
			

			continue;
		}
	}
}


