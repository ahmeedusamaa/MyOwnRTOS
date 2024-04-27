/*
 * Stm32_F103C6_EXTI_driver.c
 *
 *  Created on: Jul 9, 2023
 *      Author: ahmed osama
 */

#include "Stm32_F103C6_EXTI_driver.h"
#include "Stm32_F103C6_gpio_driver.h"

/*
*================================================================
*                          Generic Variable
*================================================================
*/
void(* GP_IRQ_CALLBACK[15])(void);




/*
*================================================================
*                          Generic Macros
*================================================================
*/

#define AFIO_GPIO_EXTI_Mapping(x)   ( (x==GPIOA)?0:\
		 	 	 	 	 	 	 	  (x==GPIOB)?1:\
									  (x==GPIOC)?2:\
		 	 	 	 	 	 	 	  (x==GPIOD)?3:0 )

/*
*================================================================
*                          Generic functions
*================================================================
*/

void Enable_NVIC (uint16_t EXTI_Line)
{
	switch (EXTI_Line)
	{
	case 0:
		NVIC_IRQ6_EXTI0_Enable;
		break;
	case 1:
		NVIC_IRQ7_EXTI1_Enable;
		break;
	case 2:
		NVIC_IRQ8_EXTI2_Enable;
		break;
	case 3:
		NVIC_IRQ9_EXTI3_Enable;
		break;
	case 4:
		NVIC_IRQ10_EXTI4_Enable;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI9_5_Enable;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI15_10_Enable;
		break;
	}
}

void Disable_NVIC (uint16_t EXTI_Line)
{
	switch (EXTI_Line)
	{
	case 0:
		NVIC_IRQ6_EXTI0_Disable;
		break;
	case 1:
		NVIC_IRQ7_EXTI1_Disable;
		break;
	case 2:
		NVIC_IRQ8_EXTI2_Disable;
		break;
	case 3:
		NVIC_IRQ9_EXTI3_Disable;
		break;
	case 4:
		NVIC_IRQ10_EXTI4_Disable;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI9_5_Enable;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_15_Disable;
		break;
	}
}


void Update_EXTI(EXTI_PinConfig_t *EXTI_Config)
{
    //	configure GPIO pin to be AF Input -> floating input
	GPIO_PConfig_t pinCfg;
	pinCfg.GPIO_PinNumber=EXTI_Config->EXTI_PIN.GPIO_PIN;
	pinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(EXTI_Config->EXTI_PIN.GPIO_Port, &pinCfg);

	//update AFIO to Route between EXTI line with PORT A,B,C,D
	uint8_t AFIO_EXTICR_index = EXTI_Config->EXTI_PIN.EXTI_InputLineNumber / 4;
	uint8_t AFIO_EXTICR_position= (EXTI_Config->EXTI_PIN.EXTI_InputLineNumber % 4) *4;
	//clear the four bit
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~ (0xF <<AFIO_EXTICR_position);

	AFIO->EXTICR[AFIO_EXTICR_index] |=  ((AFIO_GPIO_EXTI_Mapping(EXTI_Config->EXTI_PIN.GPIO_Port) &0xF) <<AFIO_EXTICR_position);

	//Update Rising or falling Register
	//clear
	EXTI->RTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
	EXTI->FTSR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
	if(EXTI_Config->Trigger_Case==EXTI_Trigger_RISING)
	{
		EXTI->RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}
	else if (EXTI_Config->Trigger_Case==EXTI_Trigger_FALLING)
	{
		EXTI->FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}
	else if (EXTI_Config->Trigger_Case==EXTI_Trigger_RISING_and_FALLING)
	{
		EXTI->RTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
		EXTI->FTSR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}

	//Update IRQ Handling CALLBACK
	GP_IRQ_CALLBACK[EXTI_Config->EXTI_PIN.EXTI_InputLineNumber] = EXTI_Config->P_IRQ_CALLBACK;

	//Enable/Disable IRQ EXTI & NVIC
	if(EXTI_Config->IRQ_EN == EXTI_IRQ_Enable)
	{
		EXTI->IMR |= (1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
		Enable_NVIC(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}
	else
	{
		EXTI->IMR &= ~(1<<EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);
		Disable_NVIC(EXTI_Config->EXTI_PIN.EXTI_InputLineNumber);

	}

}


/*
*================================================================
*                APIS Functions Definitions
*================================================================
*/


/**================================================================
* @Fn                    - MCAL_EXTI_GPIO_DeInit
* @brief  				 - reset EXTI Registers and NVIC corresponding IRQ Mask
* @retval 				 - none
* Note					 - none
*/
void MCAL_EXTI_GPIO_DeInit(void)
{
	EXTI->IMR  =   0x00000000;
	EXTI->EMR  =   0x00000000;
	EXTI->RTSR =   0x00000000;
	EXTI->FTSR =   0x00000000;
	EXTI->SWIER=   0x00000000;
	EXTI->PR   =   0xFFFFFFFF; //rc_W1 cleared by writting 1
	//Disable EXTI IRQ from NVIC
	NVIC_IRQ6_EXTI0_Disable   ;
	NVIC_IRQ7_EXTI1_Disable   ;
	NVIC_IRQ8_EXTI2_Disable   ;
	NVIC_IRQ9_EXTI3_Disable   ;
	NVIC_IRQ10_EXTI4_Disable   ;
	NVIC_IRQ23_EXTI5_9_Disable   ;
	NVIC_IRQ40_EXTI10_15_Disable   ;
}


/**================================================================
* @Fn                        -MCAL_EXTI_Init
* @brief 					 -this is used to inialize EXTI and specify Mask/Trigger condition and IRQ CallBack
* @param [in] 				 -EXTI_Config set by @ref EXTI_define, EXTI_Trigger and EXTI_IRQ_define
* @retval  				     -none
* Note						 -LQFP48 Package has only GPIO A,B,part of C and D
*/
void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);
}
/**================================================================
* @Fn                        -MCAL_EXTI_Init
* @brief 					 -this is used to inialize EXTI and specify Mask/Trigger condition and IRQ CallBack
* @param [in] 				 -EXTI_Config set by @ref EXTI_define, EXTI_Trigger and EXTI_IRQ_define
* @retval  				     -none
* Note						 -LQFP48 Package has only GPIO A,B,part of C and D
*/
void MCAL_EXTI_GPIO_Update(EXTI_PinConfig_t * EXTI_Config)
{
	Update_EXTI(EXTI_Config);
}

/*
*================================================================
*                          ISR functions
*================================================================
*/
void EXTI0_IRQHandler (void)
{
	//cleared by writting 1 in Pending Register (EXTI_PR)
	EXTI->PR |= 1<<0;
	// call IRQ_CALL
	GP_IRQ_CALLBACK[0]();
}
void EXTI1_IRQHandler (void)
{
	//cleared by writting 1 in Pending Register (EXTI_PR)
	EXTI->PR |= 1<<1;
	// call IRQ_CALL
	GP_IRQ_CALLBACK[1]();
}
void EXTI2_IRQHandler (void)
{
	//cleared by writting 1 in Pending Register (EXTI_PR)
	EXTI->PR |= 1<<2;
	// call IRQ_CALL
	GP_IRQ_CALLBACK[2]();
}
void EXTI3_IRQHandler (void)
{
	//cleared by writting 1 in Pending Register (EXTI_PR)
	EXTI->PR |= 1<<3;
	// call IRQ_CALL
	GP_IRQ_CALLBACK[3]();
}
void EXTI4_IRQHandler (void)
{
	//cleared by writting 1 in Pending Register (EXTI_PR)
	EXTI->PR |= 1<<4;
	// call IRQ_CALL
	GP_IRQ_CALLBACK[4]();
}
void EXTI9_5_IRQHandler (void)
{
	if(EXTI->PR & 1<<5) {EXTI->PR |= (1<<5) ; GP_IRQ_CALLBACK[5](); }
	if(EXTI->PR & 1<<6) {EXTI->PR |= (1<<6) ; GP_IRQ_CALLBACK[6](); }
	if(EXTI->PR & 1<<7) {EXTI->PR |= (1<<7) ; GP_IRQ_CALLBACK[7](); }
	if(EXTI->PR & 1<<8) {EXTI->PR |= (1<<8) ; GP_IRQ_CALLBACK[8](); }
	if(EXTI->PR & 1<<9) {EXTI->PR |= (1<<9) ; GP_IRQ_CALLBACK[9](); }

}
void EXTI15_10_IRQHandler (void)
{
	if(EXTI->PR & 1<<10) {EXTI->PR |= (1<<10) ; GP_IRQ_CALLBACK[10](); }
	if(EXTI->PR & 1<<11) {EXTI->PR |= (1<<11) ; GP_IRQ_CALLBACK[11](); }
	if(EXTI->PR & 1<<12) {EXTI->PR |= (1<<12) ; GP_IRQ_CALLBACK[12](); }
	if(EXTI->PR & 1<<13) {EXTI->PR |= (1<<13) ; GP_IRQ_CALLBACK[13](); }
	if(EXTI->PR & 1<<14) {EXTI->PR |= (1<<14) ; GP_IRQ_CALLBACK[14](); }
	if(EXTI->PR & 1<<15) {EXTI->PR |= (1<<15) ; GP_IRQ_CALLBACK[15](); }

}
