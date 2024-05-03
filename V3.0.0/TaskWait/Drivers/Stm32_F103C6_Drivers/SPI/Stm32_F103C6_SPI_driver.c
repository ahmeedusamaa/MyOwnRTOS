/*
 * Stm32_F103C6_SPI_driver.c
 *
 *  Created on: Jul 24, 2023
 *      Author: ahmed osama
 */

#include "Stm32_F103C6_SPI_driver.h"
/*
 * =================================================
 * 				  Generic Variable
 * =================================================
 */

SPI_Config* Global_SPI_Config[2] = {NULL,NULL};

/*
 * =================================================
 * 				  Generic Macros
 * =================================================
 */

#define SPI1_Index    0
#define SPI2_Index    1

#define SPI_TXE		  ((uint8_t)0x02)      //Transmit buffer empty
#define SPI_RXNE      ((uint8_t)0x01)	   //Receive buffer not empty
/*
 * =================================================
 * 				     APIs
 * =================================================
 */

void MCAL_SPI_Init(SPI_TypeDef *SPIx, SPI_Config *SPI_config)
{
	//safety for registers
	uint16_t tmpReg_CR1=0;
	uint16_t tmpReg_CR2=0;

	if(SPIx==SPI1)
	{
		Global_SPI_Config[SPI1_Index] = SPI_config;
		RCC_SPI1_CLK_EN();
	}
	else if(SPIx==SPI2)
	{
		Global_SPI_Config[SPI2_Index] = SPI_config;
		RCC_SPI2_CLK_EN();
	}

	//Enable SPI CR1:Bit6
	tmpReg_CR1 = (1<<6);
	//Master or Slave
	tmpReg_CR1 |= SPI_config->SPI_Mode;
	//SPI Communication Mode
	tmpReg_CR1 |= SPI_config->Communication_Mode;
	//SPI Frame Format
	tmpReg_CR1 |= SPI_config->FrameFormat;
	//SPI Data Size
	tmpReg_CR1 |= SPI_config->DataSize;
	//SPI Clk Polarity
	tmpReg_CR1 |= SPI_config->ClkPolarity;
	//SPI Clk Phase
	tmpReg_CR1 |= SPI_config->ClkPhase;
	//NSS
	if(SPI_config->NSS==SPI_NSS_HW_Master_output_Enable)
	{
		tmpReg_CR2 |= SPI_config->NSS;
	}
	else if(SPI_config->NSS==SPI_NSS_HW_Master_output_Disable)
	{
		tmpReg_CR2 &= SPI_config->NSS;

	}
	else
	{
		tmpReg_CR1 |= SPI_config->NSS;
	}
	//SPI Prescaler
	tmpReg_CR1 |= SPI_config->SPI_Prescaler;
	//SPI Enable interrupt
	if(SPI_config->IRQ_Enable !=  SPI_IRQ_Enable_NONE)
	{
		tmpReg_CR2 |= SPI_config->IRQ_Enable;
		if(SPIx==SPI1)
		{
			NVIC_IRQ35_SPI1_Enable;
		}
		else if(SPIx==SPI2)
		{
			NVIC_IRQ36_SPI2_Enable;
		}

	}

	SPIx->CR1=tmpReg_CR1;
	SPIx->CR2=tmpReg_CR2;

}
void MCAL_SPI_DeInit(SPI_TypeDef *SPIx)
{

	if(SPIx==SPI1)
	{
		NVIC_IRQ35_SPI1_Disable;
		RCC_SPI1_CLK_Reset();
	}
	else if(SPIx==SPI2)
	{
		NVIC_IRQ36_SPI2_Disable;
		RCC_SPI2_CLK_Reset();
	}

}

void MCAL_SPI_SendData(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn)
{
	if(pollingEn==PollingEnable)
	{
		while(!(SPIx->SR & SPI_TXE));
	}
	SPIx->DR= *pTxBuffer;
}
void MCAL_SPI_ReceiveData(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn)
{
	if(pollingEn==PollingEnable)

	{
		while(!(SPIx->SR & SPI_RXNE));
	}
	*pTxBuffer = SPIx->DR ;
}
void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn)
{
	if(pollingEn==PollingEnable)
	{
		while(!(SPIx->SR & SPI_TXE));
	}
	SPIx->DR= *pTxBuffer;
	if(pollingEn==PollingEnable)

	{
		while(!(SPIx->SR & SPI_RXNE));
	}
	*pTxBuffer = SPIx->DR ;
}

/*
 * ===========================================================
 *                         IRQ
 * ===========================================================
 */

void SPI1_IRQHandler (void)
{
	struct S_IRQ_SRC irq_src;
	Global_SPI_Config[SPI1_Index]->P_IRQ_CallBack(irq_src);

}

void SPI2_IRQHandler (void)
{
	struct S_IRQ_SRC irq_src;
	Global_SPI_Config[SPI2_Index]->P_IRQ_CallBack(irq_src);
}

void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx)
{
	GPIO_PConfig_t PinCfg;

	if(SPIx==SPI1)
	{
		if(Global_SPI_Config[SPI1_Index]->SPI_Mode==SPI_Mode_Master)
		{
			//PA4 : SPI1_NSS
			switch (Global_SPI_Config[SPI1_Index]->NSS)
			{
			case SPI_NSS_HW_Master_output_Disable:
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);

				break;
			case SPI_NSS_HW_Master_output_Enable:
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);

				break;
			}
			PinCfg.GPIO_PinNumber = GPIO_PIN_5;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_6;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_7;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
		else
		{
			if(Global_SPI_Config[SPI1_Index]->NSS == SPI_NSS_HW_Slave)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			PinCfg.GPIO_PinNumber = GPIO_PIN_5;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);


			PinCfg.GPIO_PinNumber = GPIO_PIN_6;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_7;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

		}

	}
	else if(SPIx==SPI2)
	{

		if(Global_SPI_Config[SPI2_Index]->SPI_Mode==SPI_Mode_Master)
		{
			//PA4 : SPI1_NSS
			switch (Global_SPI_Config[SPI2_Index]->NSS)
			{
			case SPI_NSS_HW_Master_output_Disable:
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);

				break;
			case SPI_NSS_HW_Master_output_Enable:
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);

				break;
			}
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_14;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_15;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
		else
		{
			if(Global_SPI_Config[SPI2_Index]->NSS == SPI_NSS_HW_Slave)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);


			PinCfg.GPIO_PinNumber = GPIO_PIN_14;
			PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			PinCfg.GPIO_PinNumber = GPIO_PIN_15;
			PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

		}

	}
}
