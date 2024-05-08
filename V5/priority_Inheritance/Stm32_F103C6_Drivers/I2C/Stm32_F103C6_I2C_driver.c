/*
 * Stm32_F103C6_I2C_driver.c
 *
 *  Created on: Jul 31, 2023
 *      Author: ahmed osama
 */

#include "Stm32_F103C6_I2C_driver.h"
//Master Polling Mechanism
//Slave interrupt Mechanism

/*========================================================
 * 				   Generic Variables
 *========================================================
 */
I2C_Config Global_I2C_Config[2]={NULL, NULL};

/*========================================================
 * 				   Generic Macros
 *========================================================
 */
#define I2C1_INDEX			0
#define I2C2_INDEX			1



/*========================================================
 * 			APIs Supported by "MCAL I2C DRIVER"
 *========================================================
 */
void MCAL_I2C_Init(I2C_TypeDef* I2Cx, I2C_Config* I2C_Config)
{
	uint32_t pclk1= 8000000UL;
	uint16_t tempReg=0 , freqRange=0;
	uint16_t result=0;
	if(I2Cx==I2C1)
	{
		Global_I2C_Config[I2C1_INDEX]= *I2C_Config;
		RCC_I2C1_CLK_EN();
	}
	else if(I2Cx==I2C2)
	{
		Global_I2C_Config[I2C2_INDEX]= *I2C_Config;
		RCC_I2C2_CLK_EN();
	}
	if (I2C_Config->I2C_Mode == I2C_Mode_I2C)
	{
		/*
		 *   Bits 5:0 FREQ[5:0]: Set Peripheral clock frequency
		 */
		//Get the I2Cx_CR2 value
		tempReg= I2Cx->CR2;
		//Clear frequency FREQ[5:0] bits
		tempReg &= ~(I2C_CR2_FREQ_Msk);
		//Get the pclk1 frequency value
		pclk1= MCAL_RCC_GetPCLK1Freq();
		//Set frequency FREQ[5:0] bits
		freqRange= (uint16_t)(pclk1/1000000);
		tempReg |= freqRange;
		I2Cx->CR2= tempReg;

		//The CCR register must be configured only when the I2C is disabled (PE = 0)
		I2Cx->CR1 &=~(I2C_CR1_PE);

		tempReg=0;
		/*
		 * Configure the clock control registers --->>>CCR[11:0]:Thigh = CCR * TPCLK1
		 */
		//Configure the Speed in Standard mode
		if((I2C_Config->I2C_CLK_speed==I2C_CLK_speed_SM_50k) || (I2C_Config->I2C_CLK_speed==I2C_CLK_speed_SM_100k) )
		{
			//Bit 15 already 0 (0:Sm mode I2C)
			// Tclk/2= CRR * Tpclk1
			// CRR= Tclk/(2*Tpclk1)
			// CRR= Fpclk1/(2*I2C_clk_speed)
			result= (uint16_t)(pclk1/(I2C_Config->I2C_CLK_speed << 1));
			tempReg|=result;
			I2Cx->CCR= tempReg;

			/*
			 * Configure the rise time register	------>>>TRISE[5:0]:(1000 ns / 125 ns = 8 + 1)
			 */

			/*
		For instance: in Sm mode, the maximum allowed SCL rise time is 1000 ns.
		If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and TPCLK1 = 125 ns
		therefore the TRISE[5:0] bits must be programmed with 09h.
		(1000 ns / 125 ns = 8 + 1)
			 */
			I2Cx->TRISE=freqRange+1;
		}
		/********Configuration struct*********/
		tempReg=I2Cx->CR1;
		tempReg |= (I2C_Config->I2C_ACK_Control | I2C_Config->I2C_Mode | I2C_Config->I2C_Stretch_Mode | I2C_Config->General_Call_Address_Detection);

		I2Cx->CR1=tempReg;

		tempReg=0;
		if(I2C_Config->I2C_Slave_address.Dual_Address_Enable==1)
		{
			//Dual addressing mode enable
			tempReg=I2C_OAR2_ENDUAL;

			tempReg |=I2C_Config->I2C_Slave_address.Secondary_Slave_address << I2C_OAR2_ADD2_Pos;
			I2Cx->OAR2=tempReg;
		}
		tempReg= 0;

		tempReg |=I2C_Config->I2C_Slave_address.Primary_slave_address << 1;
		tempReg |=I2C_Config->I2C_Slave_address.I2C_Addressing_Slave_mode ;
		I2Cx->OAR1=tempReg;

	}
	else
	{
		/* Not support I2C_MODE_SMBUS*/
	}
	//Interrupt mode (Slave mode)
	if(I2C_Config->P_Slave_Event_CallBack != NULL) //(Slave mode?)
	{
		I2Cx->CR2 |= I2C_CR2_ITERREN;
		I2Cx->CR2 |= I2C_CR2_ITEVTEN;
		I2Cx->CR2 |= I2C_CR2_ITBUFEN;
		if(I2Cx==I2C1)
		{
			NVIC_IRQ31_I2C1_EV_Enable;
			NVIC_IRQ32_I2C1_ER_Enable;
		}
		else if(I2Cx==I2C2)
		{
			NVIC_IRQ33_I2C2_EV_Enable;
			NVIC_IRQ34_I2C2_ER_Enable;

		}
		I2Cx->SR1=0;
		I2Cx->SR2=0;
	}
	/*
	 * enable the peripheral
	 */
	I2Cx->CR1 |= I2C_CR1_PE ;

}

void MCAL_I2C_DeInit(I2C_TypeDef* I2Cx)
{
	if(I2Cx==I2C1)
	{
		NVIC_IRQ31_I2C1_EV_Disable;
		NVIC_IRQ32_I2C1_ER_Disable;
		RCC_I2C1_CLK_Reset();
	}
	else if(I2Cx==I2C2)
	{
		NVIC_IRQ33_I2C2_EV_Disable;
		NVIC_IRQ34_I2C2_ER_Disable;
		RCC_I2C2_CLK_Reset();
	}

}

void MCAL_I2C_GPIO_Set_Pins(I2C_TypeDef* I2Cx)
{
	GPIO_PConfig_t PinCfg;

	if(I2Cx==I2C1)
	{
		//PB6 : I2C1_SCL -- Alternate function open drain
		PinCfg.GPIO_PinNumber=GPIO_PIN_6;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
		//PB7 : I2C1_SDL -- Alternate function open drain
		PinCfg.GPIO_PinNumber=GPIO_PIN_7;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
	}
	else if(I2Cx==I2C2)
	{
		//PB10: I2C2_SCL -- Alternate function open drain
		PinCfg.GPIO_PinNumber=GPIO_PIN_10;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
		//PB11: I2C2_SDA -- Alternate function open drain
		PinCfg.GPIO_PinNumber=GPIO_PIN_11;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_AF_OD;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);
	}
}

void MCAL_I2C_Master_Tx(I2C_TypeDef* I2Cx, uint16_t slaveAddress, uint8_t *dataOut, uint32_t dataLen, Stop_Condition stop,Reapeated_Start start)
{
	//generate Start condition
	I2C_Generate_Start(I2Cx, Enable, start);

	//wait for EV5
	//EV5: SB=1, cleared by reading SR1 register
	while (! I2C_Get_Flag_Status(I2Cx, EV5) );

	//writing DR register with Address
	I2C_SendAddress(I2Cx, slaveAddress, I2C_Direction_Transmitter);

	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	while(!( I2C_Get_Flag_Status(I2Cx, EV6) ) );

	//EV8_1: TxE=1, shift register empty, data register empty, Master?, Busy:after send address Communication ongoing on the bus By you?
	while(!( I2C_Get_Flag_Status(I2Cx, Master_Transmitter_Busy_RegisterEmpty) ) );

	//write Data1 in DR
	uint32_t i=0;
	for(i=0; i<dataLen; i++)
	{
		I2Cx->DR=dataOut[i];
		//EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register .
		while(!( I2C_Get_Flag_Status(I2Cx, EV8) ) );
	}

	//EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
	while(!( I2C_Get_Flag_Status(I2Cx, EV8_2) ) );

	if(stop==With_Stop)
	{
		I2C_GenerateStop(I2Cx,Enable);
	}
}

void MCAL_I2C_Master_Rx(I2C_TypeDef* I2Cx, uint16_t slaveAddress, uint8_t *dataOut, uint32_t dataLen, Stop_Condition stop,Reapeated_Start start)
{
	uint8_t index= I2Cx==I2C1? I2C1_INDEX:I2C2_INDEX;

	//generate Start condition
	I2C_Generate_Start(I2Cx, Enable, start);

	//wait for EV5
	//EV5: SB=1, cleared by reading SR1 register
	while (! (I2C_Get_Flag_Status(I2Cx, EV5)) );

	//writing DR register with Address
	I2C_SendAddress(I2Cx, slaveAddress, I2C_Direction_Recieve);

	//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	while(!( I2C_Get_Flag_Status(I2Cx, EV6) ) );

	I2C_AcknowledgeConfig(I2Cx,Enable);

	uint32_t i=0;
	//dataLen>0
	if(dataLen)
	{
		//until Len becomes zero
		for(i=dataLen; i>0; i--)
		{
			//wait until RxNE becomes 1
			while (! (I2C_Get_Flag_Status(I2Cx, EV7)) );
			//read the data from data register to buffer
			*dataOut=I2Cx->DR;
			//increment the buffer address
			dataOut++;
		}
		//NACK
		I2C_AcknowledgeConfig(I2Cx,Disable);
	}

	if(stop==With_Stop)
	{
		//Send STOP condition
		I2C_GenerateStop(I2Cx,Enable);
	}
	if(Global_I2C_Config[index].I2C_ACK_Control == I2C_ACK_Control_Enable)
	{
		I2C_AcknowledgeConfig(I2Cx,Enable);
	}

}

void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx  , Functional_State State)
{
	/*Bit 10 ACK: Acknowledge enable
	This bit is set and cleared by software and cleared by hardware when PE=0.
	0: No acknowledge returned
	1: Acknowledge returned after a byte is received (matched address or data)*/

	if(State == Enable)
	{
		I2Cx->CR1 |=(I2C_CR1_ACK);
	}
	else
	{
		I2Cx->CR1 &=~(I2C_CR1_ACK);
	}

}

void I2C_GenerateStop(I2C_TypeDef *I2Cx,Functional_State State)
{
	if(State != Disable)
	{
		//Generate a STOP condition
		I2Cx->CR1 |= I2C_CR1_STOP;
	}
	else
	{
		//Disable a STOP condition
		I2Cx->CR1 &= ~(I2C_CR1_STOP);
	}


}
void I2C_Generate_Start(I2C_TypeDef* I2Cx,Functional_State Status, Reapeated_Start start)
{
	if(start!=RepeatedStart)
	{   //check if Bus is idle
		/************************todo: add timeout (I2CMaster_SetTimeout() )************************/
		while(( I2C_Get_Flag_Status(I2Cx, I2C_Flag_BUSY) ));  //check if Bus is idle
	}
	if(Status != Disable)
	{
		//Generate a START condition
		I2Cx->CR1 |= I2C_CR1_START;
	}
	else
	{
		//Disable a START condition
		I2Cx->CR1 &= ~(I2C_CR1_START);
	}


}

//supported only 7bit mode
void I2C_SendAddress(I2C_TypeDef* I2Cx, uint16_t address, I2C_Direction Dir)
{
	address= (address << 1);
	if(Dir!=I2C_Direction_Transmitter)
	{
		address |= (1<<0);
	}
	else
	{
		address &= ~(1<<0);
	}
	I2Cx->DR=address;
}

FlagStatus I2C_Get_Flag_Status(I2C_TypeDef* I2Cx,  Status flag)
{
	FlagStatus bitStatus=Reset;
	uint16_t dummyRead;
	switch(flag)
	{
	case I2C_Flag_BUSY:
	{
		if((I2Cx->SR2) & I2C_SR2_BUSY)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
		break;
	}
	case EV2:
	{
		if((I2Cx->SR1) & I2C_SR1_RXNE)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
		break;
	}
	case EV4:
	{
		if(I2Cx->SR1 & I2C_SR1_STOPF)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
		I2Cx->CR1 |=I2C_CR1_STOP;
		break;
	}
	case EV5:
	{
		if(I2Cx->SR1 & I2C_SR1_SB)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
		break;
	}
	case EV1:
	case EV6:
	{
		if(I2Cx->SR1 & I2C_SR1_ADDR)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
		//by reading SR2
		dummyRead=(I2Cx->SR2);
		break;

	}
	case EV7:
	{
		if(I2Cx->SR1 & I2C_SR1_RXNE)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
	}
	case EV8:
	{
		if(I2Cx->SR1 & I2C_SR1_TXE)
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
	}
	case EV8_2:
	{
		if((I2Cx->SR1 & I2C_SR1_TXE) && (I2Cx->SR1 & I2C_SR1_BTF))
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
	}
	case Master_Transmitter_Busy_RegisterEmpty:
	{
		uint32_t flag1=0, flag2=0;
		uint32_t lastEvent=0;
		flag1= I2Cx->SR1;
		flag2= I2Cx->SR2;
		flag2= flag2 << 16;
		lastEvent=(flag1 | flag2) & ((uint32_t)0x00FFFFFF);

		//flag = Master_Transmitter_Busy_RegisterEmpty
		if((lastEvent & flag) == flag)	/* flag =  0x00070080UL */
		{
			bitStatus=Set;
		}
		else
		{
			bitStatus=Reset;
		}
	}
	}
	return bitStatus;
}
