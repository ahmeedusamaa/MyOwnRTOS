/*
 * Stm32_F103C6_USART_driver.c
 *
 *  Created on: Jul 21, 2023
 *      Author: ahmed osama
 */

#include "Stm32_F103C6_USART_driver.h"

UART_Config* Global_UART_config =NULL ;


void MCAL_UART_Init(USART_TypeDef* USARTx, UART_Config* UART_Config)
{

	uint32_t pclk, BRR;
	Global_UART_config = UART_Config;

	// enable the clock for given USART peripheral
	if(USARTx==USART1)
	{
		RCC_USART1_CLK_EN();
	}
	else if(USARTx==USART2)
	{
		RCC_USART2_CLK_EN();
	}
	else if(USARTx==USART3)
	{
		RCC_USART3_CLK_EN();
	}

	//Enable USART module
	USARTx->CR1 |= 1<<13;      //Bit13: UE

	USARTx->CR1 |= UART_Config->USART_Mode;
	USARTx->CR1 |= UART_Config->payload_Length;
	USARTx->CR1 |= UART_Config->Parity;
	USARTx->CR2 |= UART_Config->StopBits;
	USARTx->CR3 |= UART_Config->FlowCtl;

	//configuration of BRR
	if(USARTx==USART1)
	{
		 pclk=MCAL_RCC_GetPCLK2Freq();
	}
	else
	{
		 pclk=MCAL_RCC_GetPCLK1Freq();
	}

	BRR= UART_BRR_Register(pclk, UART_Config->BaudRate);
	USARTx->BRR=BRR;

	//enable / disable Interrupt
	if(UART_Config->IRQ_Enable != UART_IRQ_Enable_NONE)
	{
		USARTx->CR1 |= (UART_Config->IRQ_Enable);
		//enable NVIC
		if(USARTx == USART1)
		{
			NVIC_IRQ37_USART1_Enable;
		}
		else if(USARTx == USART2)
		{
			NVIC_IRQ38_USART2_Enable;
		}
		else if(USARTx == USART3)
		{
			NVIC_IRQ39_USART3_Enable;
		}
	}

}

void MCAL_UART_DeInit(USART_TypeDef* USARTx)
{
	if(USARTx==USART1)
	{
		RCC_USART1_CLK_Reset();
	}
	else if(USARTx==USART2)
	{
		RCC_USART2_CLK_Reset();
	}
	else if(USARTx==USART3)
	{
		RCC_USART3_CLK_Reset();
	}

}

void MCAL_UART_SendData(USART_TypeDef* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn )
{
	//wait until TXE flag is set in the SR
	if(PollingEn == enable)
	{
		while(! (USARTx->SR & 1<<7));
	}

	if(Global_UART_config->payload_Length == UART_payloadLength_9B)
	{
		USARTx->DR = (*pTxBuffer & (uint16_t) 0x1FF);
	}
	else
	{
		USARTx->DR = (*pTxBuffer & (uint8_t) 0xFF);
	}
}

void MCAL_UART_ReceiveData(USART_TypeDef* USARTx, uint16_t* pRxBuffer, enum Polling_mechanism PollingEn )
{
	//wait until RXNE flag is set in the SR
	if(PollingEn == enable)
	{
		while(! (USARTx->SR & 1<<5));
	}
	if(Global_UART_config->payload_Length == UART_payloadLength_9B)
	{
		if(Global_UART_config->Parity==UART_Parity_NONE)
		{
			//no parity so all 9bits are data
			*((uint16_t*) pRxBuffer) = USARTx->DR;
		}
		else
		{
			//Parity is used, so 8Bits are data and 1Bit is parity
			*((uint16_t*) pRxBuffer) = (USARTx->DR & (uint8_t)0xFF);
		}
	}
	else
	{
		if(Global_UART_config->Parity==UART_Parity_NONE)
		{
			//no parity so all 8bits are data
			*((uint16_t*) pRxBuffer) = USARTx->DR;
		}
		else
		{
			//Parity is used, so 7Bits are data and 1Bit is parity
			*((uint16_t*) pRxBuffer) = (USARTx->DR & (uint8_t)0x7F);
		}
	}
}

void MCAL_UART_TC (USART_TypeDef* USARTx)
{
	//wait till TC is set in the SR
	while( ! (USARTx->SR & 1<<6));
}

void MCAL_UART_SetPins(USART_TypeDef* USARTx)
{
	GPIO_PConfig_t PinCfg;

	if(USARTx==USART1)
	{
		//PA9  TX
		PinCfg.GPIO_PinNumber=GPIO_PIN_9;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA10 RX
		PinCfg.GPIO_PinNumber=GPIO_PIN_10;
		PinCfg.GPIO_MODE=GPIO_MODE_INPUT_AF;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA11 CTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_PinNumber=GPIO_PIN_11;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
		//PA12 RTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}

	if(USARTx==USART2)
	{
		//PA2  TX
		PinCfg.GPIO_PinNumber=GPIO_PIN_2;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA3 RX
		PinCfg.GPIO_PinNumber=GPIO_PIN_3;
		PinCfg.GPIO_MODE=GPIO_MODE_INPUT_AF;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA0 CTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_PinNumber=GPIO_PIN_0;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
		//PA1 RTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_PinNumber=GPIO_PIN_1;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}

	if(USARTx==USART3)
	{
		//PB10  TX
		PinCfg.GPIO_PinNumber=GPIO_PIN_10;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		//PB11 RX
		PinCfg.GPIO_PinNumber=GPIO_PIN_11;
		PinCfg.GPIO_MODE=GPIO_MODE_INPUT_AF;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		//PB13 CTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_PinNumber=GPIO_PIN_13;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}
		//PB14 RTS
		if(Global_UART_config->FlowCtl==UART_FlowCtl_RTS || Global_UART_config->FlowCtl==UART_FlowCtl_CTS)
		{
			PinCfg.GPIO_PinNumber=GPIO_PIN_14;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}
	}

}

void MCAL_UART_SendString(USART_TypeDef* USARTx, uint8_t Data[], enum Polling_mechanism PollingEn )
{
    uint32_t itr = 0;

            while(Data[itr] != '\0')
            {
            	MCAL_UART_SendData(USARTx,&Data[itr], enable);
                itr++;
            }
}
//ISR
void USART1_IRQHandler(void)
{
	Global_UART_config->P_IRQ_CallBack();
}
void USART2_IRQHandler(void)
{
	Global_UART_config->P_IRQ_CallBack();
}
void USART3_IRQHandler(void)
{
	Global_UART_config->P_IRQ_CallBack();
}
