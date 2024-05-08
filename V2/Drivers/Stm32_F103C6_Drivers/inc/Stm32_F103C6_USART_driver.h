/*
 * Stm32_F103C6_USART_driver.h
 *
 *  Created on: Jul 21, 2023
 *      Author: ahmed osama
 */

#ifndef INC_STM32_F103C6_USART_DRIVER_H_
#define INC_STM32_F103C6_USART_DRIVER_H_

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_RCC_driver.h"
#include "Stm32_F103C6_gpio_driver.h"

typedef struct
{
	uint8_t 			USART_Mode;  		//specifies TX/RX Enable/Disable
											//This parameter must be set based on @ref UART_Mode_define

	uint32_t			BaudRate;			//specifies Baud Rate
											//This parameter must be set based on @ref UART_BaudRate_define

	uint8_t				payload_Length;		//specifies number of data transmitted or received
											//This parameter must be set based on @ref UART_payloadLength_define

	uint8_t				Parity;				//specifies Parity mode
											//This parameter must be set based on @ref UART_Parity_define

	uint8_t				StopBits;			//specifies number of Stop Bits
											//This parameter must be set based on @ref UART_StopBits_define

	uint8_t				FlowCtl;			//specifies Flow Control enabled or disabled
											//This parameter must be set based on @ref UART_FlowCtl_define

	uint8_t 			IRQ_Enable;			// UART IRQ TX/RX enabled or disabled
											//This parameter must be set based on @ref UART_IRQ_Enable_define

	void(*P_IRQ_CallBack)(void);			//Set the c Function() which will be called once the IRQ Happen

}UART_Config;

//----------------------------------------------------------------
//==================== Reference Macros ===========================
//----------------------------------------------------------------

//@ref UART_Mode_define
#define UART_Mode_TX 				(uint32_t)(1<<3)  // Bit 3 TE: Transmitter enable
#define UART_Mode_RX 				(uint32_t)(1<<2)  // Bit 2 RE: Receiver enable
#define UART_Mode_TX_Rx 			(uint32_t)(1<<3 | 1<<2) // Bit 3 TE: Transmitter enable ,Bit 2 RE: Receiver enable

//@ref UART_BaudRate_define
#define UART_BaudRate_2400 						2400
#define UART_BaudRate_9600 						9600
#define UART_BaudRate_19200 					19200
#define UART_BaudRate_57600 					57600
#define UART_BaudRate_115200 					115200
#define UART_BaudRate_230400 					230400
#define UART_BaudRate_460800 					460800
#define UART_BaudRate_921600 					921600
#define UART_BaudRate_2250000		 			2250000
#define UART_BaudRate_4500000 					4500000

//@ref UART_payloadLength_define
#define UART_payloadLength_8B						(uint32_t)(0) 			  // 0: 1 Start bit, 8 Data bits, n Stop bit
#define UART_payloadLength_9B						(uint32_t)(1<<12)		  // 1: 1 Start bit, 9 Data bits, n Stop bit

//@ref UART_Parity_define
#define UART_Parity_NONE						(uint32_t)(0)			  //Bit10-> 0:parity control disabled
#define UART_Parity_Even						(uint32_t)(1<<10)		  //Bit9-> 0: Even parity, Bit10-> 1:parity control enabled
#define UART_Parity_Odd							(uint32_t)(1<<9 | 1<<10)  //Bit9->1: Odd parity, Bit10-> 1:parity control enabled

//@ref UART_StopBits_define
#define  UART_StopBits_1						(uint32_t)(0)			  //00: 1 Stop bit
#define  UART_StopBits_Half						(uint32_t)(1<<12)		  //01: 0.5 Stop bit
#define  UART_StopBits_2						(uint32_t)(2<<13)	   	  //10: 2 Stop bits
#define  UART_StopBits_one_and_Half						(uint32_t)(3<<12)         //11: 1.5 Stop bit

//@ref UART_FlowCtl_define
#define UART_FlowCtl_NONE						(uint32_t)(0)			  //Bit9-> 0: CTS hardware flow control disabled, Bit8->0: RTS hardware flow control disabled
#define UART_FlowCtl_RTS						(uint32_t)(1<<8)		  //Bit8-> 1: RTS interrupt enabled
#define UART_FlowCtl_CTS						(uint32_t)(1<<9)		  //Bit9-> 1: CTS mode enabled
#define UART_FlowCtl_RTS_CTS					(uint32_t)(1<<8 | 1<<9)	  //Bit9-> 1: CTS mode enabled, Bit8->1: RTS interrupt enabled

//@ref UART_IRQ_Enable_define
#define UART_IRQ_Enable_NONE					(uint32_t)(0)
#define UART_IRQ_Enable_TXE						(uint32_t)(1<<7)          //Transmit data register empty
#define UART_IRQ_Enable_TC						(uint32_t)(1<<6)		  //transmission complete
#define UART_IRQ_Enable_RXNIE					(uint32_t)(1<<5)
#define UART_IRQ_Enable_PE						(uint32_t)(1<<8)		  //Parity error

enum Polling_mechanism
{
	enable,
	disable
};


#define USARTDIV(PCLK,BAUD)					(uint32_t) (PCLK/(16*BAUD))
#define USARTDIV_MUL100(PCLK,BAUD)			(uint32_t) (25*PCLK/(4*BAUD))
#define Mantissa_MUL100(PCLK,BAUD)			(uint32_t) (USARTDIV(PCLK,BAUD)*100)
#define Mantissa(PCLK,BAUD)					(uint32_t) (USARTDIV(PCLK,BAUD))
#define DIV_Fraction(PCLK,BAUD)				(uint32_t) (((USARTDIV_MUL100(PCLK,BAUD) - Mantissa_MUL100(PCLK,BAUD) ) * 16) / 100)
#define UART_BRR_Register(PCLK,BAUD)		( (Mantissa(PCLK,BAUD) ) <<4 ) | ( (DIV_Fraction(PCLK,BAUD)) & 0xF )

/*
 *===============================================================
 * 				APIs Supported by "MCAL GPIO DRIVER"
 *===============================================================
 */

void MCAL_UART_Init(USART_TypeDef* USARTx, UART_Config* UART_Config);
void MCAL_UART_DeInit(USART_TypeDef* USARTx);
void MCAL_UART_SetPins(USART_TypeDef* USARTx);
void MCAL_UART_SendData(USART_TypeDef* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn );
void MCAL_UART_ReceiveData(USART_TypeDef* USARTx, uint16_t* pRxBuffer, enum Polling_mechanism PollingEn );
void MCAL_UART_TC (USART_TypeDef* USARTx);
void MCAL_UART_SendString(USART_TypeDef* USARTx, uint8_t Data[], enum Polling_mechanism PollingEn );

#endif /* INC_STM32_F103C6_USART_DRIVER_H_ */
