/*
 * Stm32_F103C6_RCC_driver.h
 *
 *  Created on: Jul 21, 2023
 *      Author: ahmed osama
 */

#ifndef INC_STM32_F103C6_RCC_DRIVER_H_
#define INC_STM32_F103C6_RCC_DRIVER_H_

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_gpio_driver.h"

#define HSE_CLK      (uint32_t)16000000
#define HSI_RC_CLK			  (uint32_t)8000000

uint32_t MCAL_RCC_GetSYSFreq(void);
uint32_t MCAL_RCC_GetHCLFreq(void);
uint32_t MCAL_RCC_GetPCLK1Freq(void);
uint32_t MCAL_RCC_GetPCLK2Freq(void);


#endif /* INC_STM32_F103C6_RCC_DRIVER_H_ */
