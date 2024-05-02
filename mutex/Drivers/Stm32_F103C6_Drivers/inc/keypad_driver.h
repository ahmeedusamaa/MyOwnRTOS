/*
 * keypad_driver.h
 *
 *  Created on: Jul 13, 2023
 *      Author: ahmed osama
 */

#ifndef INC_KEYPAD_DRIVER_H_
#define INC_KEYPAD_DRIVER_H_


#include "Stm32_F103x6.h"
#include "Stm32_F103C6_gpio_driver.h"

#define R0 GPIO_PIN_0
#define R1 GPIO_PIN_1
#define R2 GPIO_PIN_3
#define R3 GPIO_PIN_4
#define C0 GPIO_PIN_5
#define C1 GPIO_PIN_6
#define C2 GPIO_PIN_7
#define C3 GPIO_PIN_8

#define KEYPAD_PORT GPIOB

/*
 * APIs
 */
void Keypad_Init();
//char to return ASCII
char Keypad_ScanKey();

#endif /* INC_KEYPAD_DRIVER_H_ */
