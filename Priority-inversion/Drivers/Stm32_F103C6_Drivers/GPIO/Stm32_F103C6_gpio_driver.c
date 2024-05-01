/*
 * Stm32_F103C6_gpio_driver.c
 *
 *  Created on: Jul 7, 2023
 *      Author: ahmed osama
 */
#include "Stm32_F103C6_gpio_driver.h"

uint8_t Get_CRLH_Position(uint16_t PinNumber)
{
	switch(PinNumber)
	{
		case GPIO_PIN_0 : return 0;  break;
		case GPIO_PIN_1 : return 4;  break;
		case GPIO_PIN_2 : return 8;  break;
		case GPIO_PIN_3 : return 12; break;
		case GPIO_PIN_4 : return 16; break;
		case GPIO_PIN_5 : return 20; break;
		case GPIO_PIN_6 : return 24; break;
		case GPIO_PIN_7 : return 28; break;
		case GPIO_PIN_8 : return 0;  break;
		case GPIO_PIN_9 : return 4;  break;
		case GPIO_PIN_10: return 8;  break;
		case GPIO_PIN_11: return 12; break;
		case GPIO_PIN_12: return 16; break;
		case GPIO_PIN_13: return 20; break;
		case GPIO_PIN_14: return 24; break;
		case GPIO_PIN_15: return 28; break;
	}
	return 0;
}

void MCAL_GPIO_Init (GPIO_TypeDef *GPIOx, GPIO_PConfig_t* PinConfig)
{
	uint8_t PIN_Config=0;

	//Port configuration register low (GPIOx_CRL) Configure PINS from 0 to 7
	//Port configuration register high (GPIOx_CRH) Configure PINS from 8 to 15
	volatile uint32_t* Config_Register=NULL;
	Config_Register= (PinConfig->GPIO_PinNumber < GPIO_PIN_8) ? &GPIOx->CRL : &GPIOx->CRH ;

	//clear CNF[1:0] MODE[0:0]
	*(Config_Register) &= ~ (0xf<< Get_CRLH_Position(PinConfig->GPIO_PinNumber));

	//if pin is output
	if(PinConfig->GPIO_MODE==GPIO_MODE_OUTPUT_PP || PinConfig->GPIO_MODE==GPIO_MODE_OUTPUT_OD ||  PinConfig->GPIO_MODE==GPIO_MODE_OUTPUT_AF_PP || PinConfig->GPIO_MODE==GPIO_MODE_OUTPUT_AF_OD  )
	{
		//set CNF[1:0] MODE[1:0]
		PIN_Config =( (((PinConfig->GPIO_MODE - 4) << 2 )| (PinConfig->GPIO_Output_SPeed)) & 0x0F);

	}

	//if pin is input
	else //MODE = 00: Input mode (reset state)
	{
		if(PinConfig->GPIO_MODE==GPIO_MODE_Analog || PinConfig->GPIO_MODE==GPIO_MODE_INPUT_FLO)
		{
			//set CNF[1:0] MODE[1:0]
			PIN_Config =( (((PinConfig->GPIO_MODE) << 2 )| 0x0) & 0x0F);

		}
		else if(PinConfig->GPIO_MODE==GPIO_MODE_INPUT_AF)                   //consider it input floating
		{
			//set CNF[1:0] MODE[1:0]
			PIN_Config =( (((GPIO_MODE_INPUT_FLO) << 2 )| (0x0)) & 0x0F);

		}
		else
		{
			//set CNF[1:0] MODE[1:0]
			PIN_Config =( (((GPIO_MODE_INPUT_PU) << 2 )| (0x0)) & 0x0F);
			//PD or PP
			if(PinConfig->GPIO_MODE==GPIO_MODE_INPUT_PU)
			{
				//in Table 20. port bit configuration table set PxODR 1 for input pull_up
				GPIOx->ODR |=  PinConfig->GPIO_PinNumber;
			}
			else
			{
				//in Table 20. port bit configuration table set PxODR 0 for input pull_down
				GPIOx->ODR &= ~(PinConfig->GPIO_PinNumber);
			}
		}
	}
	//write on CRL or CRH
	*(Config_Register) |= PIN_Config << Get_CRLH_Position(PinConfig->GPIO_PinNumber);

}
void MCAL_GPIO_DeInit (GPIO_TypeDef *GPIOx)
{

//	GPIOx->BRR  0x00000000;
//	GPIOx->BSRR 0x00000000;
//	GPIOx->CRH  0x44444444;
//	GPIOx->CRL  0x44444444;
//	GPIOx->IDR  READ ONLY;
//	GPIOx->LCKR 0x00000000;
//	GPIOx->ODR  0x00000000;

		//or use Reset Controller
		//APB2 peripheral reset register (RCC_APB2RSTR)
		// set and clear by software.
	if(GPIOx == GPIOA)
	{
		RCC->APB2RSTR |=   (1<<2);
		RCC->APB2RSTR &= ~ (1<<2);

	}
	else if(GPIOx == GPIOB)
	{
		RCC->APB2RSTR |=   (1<<3);
		RCC->APB2RSTR &= ~ (1<<3);
	}
	else if(GPIOx == GPIOC)
	{
		RCC->APB2RSTR |=   (1<<4);
		RCC->APB2RSTR &= ~ (1<<4);
	}
	else if(GPIOx == GPIOD)
	{
		RCC->APB2RSTR |=   (1<<5);
		RCC->APB2RSTR &= ~ (1<<5);
	}
	else if(GPIOx == GPIOE)
	{
		RCC->APB2RSTR |=   (1<<6);
		RCC->APB2RSTR &= ~ (1<<6);
	}

}
uint8_t MCAL_GPIO_ReadPin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
	uint8_t bitStatus;
	if( (GPIOx->IDR & PinNumber) != (uint32_t) GPIO_PIN_RESET)
	{
		bitStatus = GPIO_PIN_SET;
	}
	else
	{
		bitStatus = GPIO_PIN_RESET;
	}
	return bitStatus;
}
uint16_t MCAL_GPIO_ReadPort (GPIO_TypeDef *GPIOx)
{
	uint16_t PortStatus;
	PortStatus = (uint16_t)GPIOx->IDR;
	return PortStatus;
}
void MCAL_GPIO_WritePin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber, uint8_t value)
{
	if(value != GPIO_PIN_RESET)
	{
		GPIOx->ODR |= PinNumber;
	}
	else
	{
		GPIOx->ODR &= ~ PinNumber;
	}

}
void MCAL_GPIO_WritePort  (GPIO_TypeDef *GPIOx, uint16_t value)
{
	GPIOx->ODR= (uint32_t)value;
}
void MCAL_GPIO_TogglePin  (GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
	GPIOx->ODR ^= PinNumber;
}
uint8_t MCAL_GPIO_LockPin (GPIO_TypeDef *GPIOx, uint16_t PinNumber)
{
/*
	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	0: Port configuration lock key not active
	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	LOCK key writing sequence:
	Write 1
	Write 0
	Write 1
	Read 0
	Read 1 (this read is optional but confirms that the lock is active)
	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	Any error in the lock sequence will abort the lock.
	Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	These bits are read write but can only be written when the LCKK bit is 0.
	0: Port configuration not locked
	1: Port configuration locked.
*/
	volatile uint32_t LCKK = 1<<16;
	LCKK |= PinNumber;
	// Write 1
	GPIOx->LCKR= LCKK;
	//Write 0
	GPIOx->LCKR= PinNumber;
	//Write 1
	GPIOx->LCKR= LCKK;
	//Read 0
	LCKK= GPIOx->LCKR;
	//Read 1
	if( (uint32_t) GPIOx->LCKR & (1<<16 ))
	{
		return GPIO_RETURN_LOCK_Enabled;
	}
	else
	{
		return GPIO_RETURN_LOCK_Error;
	}

}

