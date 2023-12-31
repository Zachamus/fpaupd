/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void Green_LED_Off(void);
void Green_LED_On(void);
void Green_LED_Toggle(void);

#endif /* __STM32L476G_DISCOVERY_LED_H */
