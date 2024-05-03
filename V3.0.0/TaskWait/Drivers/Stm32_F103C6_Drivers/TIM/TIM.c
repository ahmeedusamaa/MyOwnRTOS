/*
 * TIM.c
 *
 *  Created on: Mar 3, 2024
 *      Author: ahmed osama
 */

#include "TIM.h"
#include "Stm32_F103C6_gpio_driver.h"


//General-purpose timers (TIM2 to TIM5)

void PWM_Init(General_purpose_TIM2_5_TypeDef* TIMx, TIM_Config* TIM_Config)
{
	//TIMx->CR1 	&= ~(1<<0);
	uint32_t ARR = TIM_Config->TIM_ARR ;
	uint32_t duty_cycle  =  TIM_Config->TIM_duty_cycle_value * ARR;
	uint32_t CC = duty_cycle / 100;

	GPIO_PConfig_t PinConfig;
	if(TIMx == TIM2)
	{
		TIMx->CR1 	|= TIM_Config->TIM_ARPE;
		TIMx->CR1   |= TIM_Config->TIM_direction;
		TIMx->CR1	|= TIM_Config->TIM_mode_selection;


		switch(TIM_Config->TIM_Channel_number)
		{
			case 3:
				PinConfig.GPIO_PinNumber=GPIO_PIN_2;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOA, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_3_EN;
				TIMx->CCMR2 |= TIM_CH3_PWM_mode1;
				TIMx->CCR3 = CC;
				break;

			case 4:
				PinConfig.GPIO_PinNumber=GPIO_PIN_3;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOA, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_4_EN;
				TIMx->CCMR2 |= TIM_CH4_PWM_mode1;
				TIMx->CCR4 = CC;
				break;
		}
	}

	if(TIMx == TIM3)
	{
		TIMx->CR1 	|= TIM_Config->TIM_ARPE;
		TIMx->CR1   |= TIM_Config->TIM_direction;
		TIMx->CR1	|= TIM_Config->TIM_mode_selection;

		switch(TIM_Config->TIM_Channel_number)
		{
			case 1:
				PinConfig.GPIO_PinNumber=GPIO_PIN_ALL;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOA, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_1_EN;
				TIMx->CCMR1 |= TIM_CH1_PWM_mode1;
				TIMx->CCR1 = CC;
				break;

			case 2:
				PinConfig.GPIO_PinNumber=GPIO_PIN_7;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOA, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_2_EN;
				TIMx->CCMR1 |= TIM_CH2_PWM_mode1;
				TIMx->CCR2 = CC;
				break;

			case 3:
				PinConfig.GPIO_PinNumber=GPIO_PIN_0;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOB, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_3_EN;
				TIMx->CCMR2 |= TIM_CH3_PWM_mode1;
				TIMx->CCR3 = CC;
				break;

			case 4:
				PinConfig.GPIO_PinNumber=GPIO_PIN_1;
				PinConfig.GPIO_MODE= GPIO_MODE_OUTPUT_AF_PP;
				PinConfig.GPIO_Output_SPeed=GPIO_OutputSpeed_50M;
				MCAL_GPIO_Init(GPIOB, &PinConfig);

				TIMx->CCER |= TIM_CC_Channel_4_EN;
				TIMx->CCMR2 |= TIM_CH4_PWM_mode1;
				TIMx->CCR4 = CC;
				break;
		}

	}

	//The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
	TIMx->PSC = TIM_Config->TIM_prescaler ;

	//ARR is the value to be loaded in the actual auto-reload register.
	TIMx->ARR = TIM_Config->TIM_ARR ;

	//TIMx->CNT
	//Holds the value that the counter is counting (heartbeat)

	TIMx->EGR |= (1<<0);

	/*
	Bit 0 CEN: Counter enable
	0: Counter disabled
	1: Counter enabled
	*/
	TIMx->CR1 |= (1<<0);

}
