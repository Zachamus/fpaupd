/*
 * ECE 153B - Spring 2023
 *
 * Name(s): Tanish Kumar, Zach Reiber
 * Lab: Final Project
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "DMA.h"
#include "UART.h"
#include "motor.h"
#include "SPI.h"
#include "I2C.h"
#include "accelerometer.h"
#include <stdio.h>
#include <stdbool.h>

static char buffer[IO_SIZE];
	double x;
	double y;
	double z;
	uint32_t open = 1;
	uint32_t console = 2;
	uint32_t wait = 0;
void UART_onInput(char* inputs, uint32_t size) {
	//TODO

	if (inputs[0] == '1'){
		console = 1;
		sprintf(buffer, "Door closing\n");
		UART_print(buffer);
	}
	
	else if (inputs[0] == '0'){
		console = 0;
		sprintf(buffer, "Door opening\n");
		UART_print(buffer);
	}
	else
	{
		sprintf(buffer, "Please enter a 0 or 1\n");
		UART_print(buffer);
	}

	

	
	}

int main(void) {
// Switch System Clock = 80 MHz
System_Clock_Init();
Motor_Init();
SysTick_Init();
UART2_GPIO_Init();
UART2_Init();
LED_Init();
SPI1_GPIO_Init();
SPI1_Init();
initAcc();
I2C_GPIO_Init();
I2C_Initialization();


sprintf(buffer, "Program Starts.\r\n");
UART_print(buffer);

	uint8_t SlaveAddress;
	uint8_t Data_Receive;
	uint8_t prev = 0;
	uint8_t Data_Send;
	setDire(2);
while(1) {

	
	Green_LED_Toggle();
	readValues(&x, &y, &z);
	//sprintf(buffer, "Acceleration: %.2f, %.2f, %.2f\n", x, y, z);
	//UART_print(buffer);

	
	SlaveAddress = 72UL << 1;
	I2C_SendData(I2C1, SlaveAddress, &Data_Send, 1);
	I2C_ReceiveData(I2C1, SlaveAddress, &Data_Receive, 1);
	//sprintf(buffer, "Temperature: %d\n", Data_Receive);
	//UART_print(buffer);

	
	
	
	delay(100);
	if(Data_Receive != prev)
	{
		 sprintf(buffer, "Temperature: %d\n", Data_Receive);
		 //UART_print(buffer);
		 if(Data_Receive > 24 && open == 1 && wait == 0)
		 {
			
				 sprintf(buffer, "Temperature is %d. Door opening\n", Data_Receive);
				 UART_print(buffer);
				 open = 0;
		 }
		 
		 else if(Data_Receive < 20 && open == 0 && wait == 0)
		 {
				 sprintf(buffer, "Temperature is %d. Door closing\n", Data_Receive);
				 UART_print(buffer);
				 open = 1;
		 }
		 
		 
		 
	}
	prev = Data_Receive;

	
	
	if (console == 1) {
		if (open == 0) {
			open = 1;
		}
		console = 3;
		wait = 1;
	}
	else if (console == 0){
		if (open == 1){
			open = 0;
		}
			console = 3;
			wait = 1;
			
		}
		
	if(open == 1)
	{
		if(y<-0.9 || y > 0.9){
			setDire(2);
		
			if(console == 3)
			{
				sprintf(buffer, "Finished Closing");
				UART_print(buffer);
				console = 2;
				delay(3000);
				open = 0;
				wait = 0;
		}}
		else{
			setDire(0);
		}
	}
	
	if(open == 0)
	{
		if(z<-0.9 || z > 0.9){
			setDire(2);
			if(console == 3)
			{
				sprintf(buffer, "Finished Opening");
				UART_print(buffer);
				console = 2;
				delay(3000);
				open = 1;
				wait = 0;
			}
		}
		else{
			setDire(1);
		}
	}
	
	
	}
	
	
}
