/*
 * Stm32_F103C6_gpio_driver.h
 *
 *  Created on: Jul 7, 2023
 *      Author: ahmed osama
 */

#ifndef INC_STM32_F103C6_GPIO_DRIVER_H_
#define INC_STM32_F103C6_GPIO_DRIVER_H_

//-----------------------------
//Includes
//-----------------------------
#include "Stm32_F103x6.h"

//-----------------------------
//pin config structure
//-----------------------------
typedef struct
{
	uint16_t GPIO_PinNumber ; //Specifies the GPIO pins to be configured
							 //this parameter must be set based on @ref GPIO_PINS_define
	uint8_t  GPIO_MODE ;
	uint8_t  GPIO_Output_SPeed ;
}GPIO_PConfig_t;

//@ref GPIO_PINS_State
#define GPIO_PIN_SET	1
#define GPIO_PIN_RESET  0

//@ref GPIO_RETURN_LOCK
#define GPIO_RETURN_LOCK_Enabled	1
#define GPIO_RETURN_LOCK_Error      0

//-----------------------------
//Macros Configuration References
//-----------------------------

//@ref GPIO_PINS_define

#define GPIO_PIN_0                   ((uint16_t)0x0001)
#define GPIO_PIN_1                   ((uint16_t)0x0002)
#define GPIO_PIN_2                   ((uint16_t)0x0004)
#define GPIO_PIN_3                   ((uint16_t)0x0008)
#define GPIO_PIN_4                   ((uint16_t)0x0010)
#define GPIO_PIN_5                   ((uint16_t)0x0020)
#define GPIO_PIN_6                   ((uint16_t)0x0040)
#define GPIO_PIN_7                   ((uint16_t)0x0080)
#define GPIO_PIN_8                   ((uint16_t)0x0100)
#define GPIO_PIN_9                   ((uint16_t)0x0200)
#define GPIO_PIN_10                  ((uint16_t)0x0400)
#define GPIO_PIN_11                  ((uint16_t)0x0800)
#define GPIO_PIN_12                  ((uint16_t)0x1000)
#define GPIO_PIN_13                  ((uint16_t)0x2000)
#define GPIO_PIN_14                  ((uint16_t)0x4000)
#define GPIO_PIN_15                  ((uint16_t)0x8000)
#define GPIO_PIN_ALL                 ((uint16_t)0xFFFF)

#define GPIO_PIN_MASK                0x0000FFFFu

//@ref GPIO_MODE_define

//0: Analog mode
//1: Floating input (reset state)
//2: Input with pull-up / pull-down
//3: Reserved
//4: General purpose output push-pull
//5: General purpose output Open-drain
//6: Alternate function output Push-pull
//7: Alternate function output Open-drain
#define GPIO_MODE_Analog             0x00000000ul //Analog mode
#define GPIO_MODE_INPUT_FLO          0x00000001ul //Floating input (reset state)
#define GPIO_MODE_INPUT_PU           0x00000002ul //Input with pull-up
#define GPIO_MODE_INPUT_PD           0x00000003ul //Input with pull-down
#define GPIO_MODE_OUTPUT_PP          0x00000004ul //General purpose output push-pul
#define GPIO_MODE_OUTPUT_OD          0x00000005ul //General purpose output Open-drain
#define GPIO_MODE_OUTPUT_AF_PP       0x00000006ul //Alternate function output Push-pull
#define GPIO_MODE_OUTPUT_AF_OD       0x00000007ul //Alternate function output Open-drain
#define GPIO_MODE_INPUT_AF           0x00000008ul //Alternate function input

//@ref GPIO_OutputSpeed_define
//1: Output mode, max speed 10 MHz.
//2: Output mode, max speed 2 MHz.
//3: Output mode, max speed 50 MHz.
#define GPIO_OutputSpeed_10M         0x00000001ul //Output mode, max speed 10 MHz.
#define GPIO_OutputSpeed_2M          0x00000002ul //Output mode, max speed 2 MHz.
#define GPIO_OutputSpeed_50M         0x00000003ul //Output mode, max speed 50 MHz.

/*
* ===============================================
* APIs Supported by "MCAL GPIO DRIVER"
* ===============================================
*/

void MCAL_GPIO_Init (GPIO_TypeDef *GPIOx, GPIO_PConfig_t* PinConfig);
void MCAL_GPIO_DeInit (GPIO_TypeDef *GPIOx);

uint8_t MCAL_GPIO_ReadPin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort (GPIO_TypeDef *GPIOx);

void MCAL_GPIO_WritePin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint8_t value);
void MCAL_GPIO_WritePort  (GPIO_TypeDef *GPIOx, uint16_t value);

void MCAL_GPIO_TogglePin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber);
uint8_t MCAL_GPIO_LockPin   (GPIO_TypeDef *GPIOx, uint16_t PinNumber);



#endif /* INC_STM32_F103C6_GPIO_DRIVER_H_ */
