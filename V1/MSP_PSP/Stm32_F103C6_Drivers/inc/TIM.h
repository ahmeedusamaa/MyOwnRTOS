/*
 * TIM.h
 *
 *  Created on: Mar 3, 2024
 *      Author: ahmed osama
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_RCC_driver.h"
#include "Stm32_F103C6_gpio_driver.h"

//General-purpose timers (TIM2 to TIM5)

typedef struct
{
	uint16_t 			TIM_direction;  		//specifies direction of counter (upcounter or downcounter)
												//This parameter must be set based on @ref TIM_Direction_define

	uint32_t 			TIM_prescaler;  		//specifies prescaler value
												//The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)

	uint32_t 			TIM_ARR;  				//specifies auto-reload value
												//ARR is the value to be loaded in the actual auto-reload register.

	uint16_t 			TIM_mode_selection;  	//specifies Center-aligned mode selection
												//This parameter must be set based on @ref TIM_mode_selection_define

	uint16_t 			TIM_ARPE;  				//specifies TIMx_ARR register is buffered ?
												//This parameter must be set based on @ref TIM_ARPE_define

//	uint16_t 			TIM_CC_Channel_EN;  	//To Enable the Capture/Compare channels
//												//This parameter must be set based on @ref TIM_CC_Channel_EN_define

	uint16_t 			TIM_Channel_number;  	//specifies number of Capture/Compare channel
												//number of channel must be set based on @ref TIM_CC_number_define

	uint32_t 			TIM_duty_cycle_value;  			//specifie duty_cycle

}TIM_Config;

//----------------------------------------------------------------
//==================== Reference Macros ===========================
//----------------------------------------------------------------

//@ref TIM_Direction_define
//CR1
#define TIM_direction_upcounter			   (uint32_t)(0)  // Bit 4 -> 0: Counter used as upcounter
#define TIM_direction_downcounter 		   (uint32_t)(1<<4)  // Bit 4 -> 1: Counter used as downcounter

/*
Bits 6:5 CMS: Center-aligned mode selection
00: Edge-aligned mode. The counter counts up or down depending on the direction bit
(DIR).
01: Center-aligned mode 1. The counter counts up and down alternatively. Output compare
interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
only when the counter is counting down.
10: Center-aligned mode 2. The counter counts up and down alternatively. Output compare
interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
only when the counter is counting up.
11: Center-aligned mode 3. The counter counts up and down alternatively. Output compare
interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
both when the counter is counting up or down.
Note: It is not allowed to switch from edge-aligned mode to center-aligned mode as long as
the counter is enabled (CEN=1)
*/


//@ref TIM_mode_selection_define
//Note: It is not allowed to switch from edge-aligned mode to center-aligned mode as long as the counter is enabled (CEN=1)
//CR1
#define TIM_mode_selection_Edge_aligned 			    (uint32_t)(0)  			 // 00: Edge-aligned mode. The counter counts up or down depending on the direction bit (DIR)
#define TIM_mode_selection_Center_aligned_mode1 		(uint32_t)(1<<5) 		 // 01: Center-aligned mode 1. The counter counts up and down alternatively
#define TIM_mode_selection_Center_aligned_mode2 		(uint32_t)(1<<6)  		 // 10: Center-aligned mode 2. The counter counts up and down alternatively
#define TIM_mode_selection_Center_aligned_mode3 		(uint32_t)(1<<5 | 1<<6)  // 11: Center-aligned mode 3. The counter counts up and down alternatively.

//@ref TIM_ARPE_define
#define TIM_ARPE_Disable			    (uint32_t)(0)     // 0: TIMx_ARR register is not buffered
#define TIM_ARPE_Enable		            (uint32_t)(1<<7)  // 1: TIMx_ARR register is buffered

//@ref TIM_CC_Channel_EN_define
#define TIM_CC_Channel_4_EN			   	(uint32_t)(1<<12)  // Capture/Compare 4 output enable
#define TIM_CC_Channel_3_EN			   	(uint32_t)(1<<8)   // Capture/Compare 3 output enable
#define TIM_CC_Channel_2_EN			   	(uint32_t)(1<<4)   // Capture/Compare 2 output enable
#define TIM_CC_Channel_1_EN			   	(uint32_t)(1<<0)   // Capture/Compare 1 output enable


//@ref TIM_CC_number_define
#define TIM_CC_number1			   		1
#define TIM_CC_number2			   		2
#define TIM_CC_number3			   		3
#define TIM_CC_number4			   		4

//===============================================================

#define TIM_CH1_PWM_mode1			   		(uint32_t)(1<<3 | 1<<5 | 1<<6)  		   //110: PWM mode 1 , Bit 11 OC4PE: Output compare 4 preload enable
#define TIM_CH1_PWM_mode2			   		(uint32_t)(1<<3 | 1<<4 | 1<<5 | 1<<6)  	   //111: PWM mode 2 , Bit 11 OC4PE: Output compare 4 preload enable

#define TIM_CH2_PWM_mode1			   		(uint32_t)(1<<11 | 1<<13 | 1<<14)  		   //110: PWM mode 1 , Bit 11 OC4PE: Output compare 4 preload enable
#define TIM_CH2_PWM_mode2			   		(uint32_t)(1<<11 | 1<<12 | 1<<13 | 1<<14)  //111: PWM mode 2 , Bit 11 OC4PE: Output compare 4 preload enable

#define TIM_CH3_PWM_mode1			   		(uint32_t)(1<<3 | 1<<5 | 1<<6)  		   //110: PWM mode 1 , Bit 11 OC4PE: Output compare 4 preload enable
#define TIM_CH3_PWM_mode2			   		(uint32_t)(1<<3 | 1<<4 | 1<<5 | 1<<6)  	   //111: PWM mode 2 , Bit 11 OC4PE: Output compare 4 preload enable

#define TIM_CH4_PWM_mode1			   		(uint32_t)(1<<11 | 1<<13 | 1<<14)  		   //110: PWM mode 1 , Bit 11 OC4PE: Output compare 4 preload enable
#define TIM_CH4_PWM_mode2			   		(uint32_t)(1<<11 | 1<<12 | 1<<13 | 1<<14)  //111: PWM mode 2 , Bit 11 OC4PE: Output compare 4 preload enable

/*
 *===============================================================
 * 							APIs
 *===============================================================
 */

void PWM_Init(General_purpose_TIM2_5_TypeDef* TIMx, TIM_Config* TIM_Config);


#endif /* INC_TIM_H_ */
