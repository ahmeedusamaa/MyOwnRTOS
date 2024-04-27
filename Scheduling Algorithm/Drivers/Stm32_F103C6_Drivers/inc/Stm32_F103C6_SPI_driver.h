/*
 * Stm32_F103C6_SPI_driver.h
 *
 *  Created on: Jul 24, 2023
 *      Author: ahmed osama
 */

#ifndef INC_STM32_F103C6_SPI_DRIVER_H_
#define INC_STM32_F103C6_SPI_DRIVER_H_

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_gpio_driver.h"

enum Pollingmechanism
{
	PollingEnable,
	PollingDisable
};

struct S_IRQ_SRC                                    //specifies source of interrupt
{
	uint8_t TXE:1;									//TX buffer empty
	uint8_t RXNE:1;									//RX buffer not empty
	uint8_t ERRI:1;									//Error
	uint8_t Reserved:5;

};

typedef struct
{
	uint16_t 			SPI_Mode;  		            //specifies SPI operating mode
											        //This parameter must be set based on @ref SPI_Mode_define

	uint16_t			Communication_Mode;			//specifies Communication Mode
											        //This parameter must be set based on @ref SPI_CommunicationMode_define

	uint16_t			FrameFormat;		        //specifies LSB or MSB
											        //This parameter must be set based on @ref SPI_FrameFormat_define

	uint16_t			DataSize;				    //This parameter must be set based on @ref SPI_DataSize_define

	uint16_t			ClkPolarity;		        //This parameter must be set based on @ref SPI_ClkPolarity_define

	uint16_t			ClkPhase;			        //This parameter must be set based on @ref SPI_ClkPhase_define

	uint16_t 			NSS;		     	        /*specifies whether NSS signal is managed by
													 *Hardware or by Software
													 *This parameter must be set based on @ref SPI_NSS_define
											         */

	uint8_t				SPI_Prescaler;				/* specifies the Baud Rate prescaler value which
	 	 	 	 	 	 	 	 	 	 	 	 	 *configure the transmit and receive clock
													 *This parameter must be set based on @ref SPI_Prescaler_define
												     *you have to configure RCC to enter the correct clock to APB2 >> to SPI
												     */

	uint16_t 			IRQ_Enable;			       					    //This parameter must be set based on @ref SPI_IRQ_Enable_define

	void(*P_IRQ_CallBack)(struct S_IRQ_SRC irq_src);			        //Set the c Function() which will be called once the IRQ Happen

}SPI_Config;

/*
 * ===============================================================
 * 						Reference Macros
 * ===============================================================
 */
//@ref SPI_Mode_define
//SPI_CR1 register
#define SPI_Mode_Slave						     (uint16_t)(0)      		//0: Slave configuration
#define SPI_Mode_Master							 (uint16_t)(1<<2)	        //1: Master configuration

//@ref SPI_CommunicationMode_define
//SPI_CR1 register
#define SPI_CommunicationMode_2Line_TX_RX		 (uint16_t)(0)	            //RXONLY-> 0: Full duplex (Transmit and receive) , BIDIMODE-> 0: 2-line unidirectional data mode selected
#define SPI_CommunicationMode_2Line_RX			 (uint16_t)(1<<10)		    //RXONLY-> 1: Output disabled (Receive-only mode), BIDIMODE-> 0: 2-line unidirectional data mode selected
#define SPI_CommunicationMode_1Line_RX			 (uint16_t)(1<<15)	 	    //BIDIOE-> 0: Output disabled (receive-only mode), BIDIMODE-> 1: 1-line bidirectional data mode selected
#define SPI_CommunicationMode_1Line_TX			 (uint16_t)(1<<15 | 1<<14)  //BIDIOE-> 1: Output enabled (transmit-only mode), BIDIMODE-> 1: 1-line bidirectional data mode selected


//@ref SPI_FrameFormat_define
//SPI_CR1 register
#define SPI_FrameFormat_MSB_First  					 (uint16_t)(0)		    //0: MSB transmitted first
#define SPI_FrameFormat_LSB_First					 (uint16_t)(1<<7)	    //1: LSB transmitted first

//@ref SPI_DataSize_define
//SPI_CR1 register
#define SPI_DataSize_8Bit							 (uint16_t)(0)		    //0: 8-bit data frame format is selected for transmission/reception
#define SPI_DataSize_16Bit							 (uint16_t)(1<<11)		//1: 16-bit data frame format is selected for transmission/reception

//@ref SPI_ClkPolarity_define
//SPI_CR1 register
#define SPI_ClkPolarity_LowIDLE						 (uint16_t)(0)			//0: CK to 0 when idle
#define SPI_ClkPolarity_HighIDLE					 (uint16_t)(1<<1)		//1: CK to 1 when idle

//@ref SPI_ClkPhase_define
//SPI_CR1 register
#define SPI_ClkPhase_First_Edge					     (uint16_t)(0)			//0: The first clock transition is the first data capture edge
#define SPI_ClkPhase_Second_Edge				     (uint16_t)(1<<0)		//1: The second clock transition is the first data capture edge

//@ref SPI_NSS_define
//NSS is Driven by Hardware
#define SPI_NSS_HW_Slave							(uint16_t)(0)			//in SPI_CR1 register
#define SPI_NSS_HW_Master_output_Enable				(uint16_t)(1<<2)        //in SPI_CR2 register
#define SPI_NSS_HW_Master_output_Disable			 ~(uint16_t)(1<<2)		//in SPI_CR2 register
//NSS is Driven by SW
#define SPI_NSS_SW_Internal slave Reset				(uint16_t)(1<<9) 			  //Internal slave enabled, in SPI_CR1 register
#define SPI_NSS_SW_Internal_slave_select		 	(uint16_t)(1<<9 | 1<<8)       //Internal slave disabled, in SPI_CR1 register

//SPI_Prescaler_define
//SPI_CR1 register
#define SPI_Prescaler_2								 (0b000U << 3)
#define SPI_Prescaler_4								 (0b001U << 3)
#define SPI_Prescaler_8								 (0b010U << 3)
#define SPI_Prescaler_16							 (0b011U << 3)
#define SPI_Prescaler_32							 (0b100U << 3)
#define SPI_Prescaler_64							 (0b101U << 3)
#define SPI_Prescaler_128							 (0b110U << 3)
#define SPI_Prescaler_356							 (0b111U << 3)

//@ref SPI_IRQ_Enable_define
//SPI_CR2 register
#define SPI_IRQ_Enable_NONE						     (uint16_t)(0)
#define SPI_IRQ_Enable_TXEIE						 (uint16_t)(1<<7)	//1: TXE interrupt not masked. Used to generate an interrupt request when the TXE flag is set.
#define SPI_IRQ_Enable_RXNEIE	    				 (uint16_t)(1<<6)   //1: RXNE interrupt not masked. Used to generate an interrupt request when the RXNE flag is set.
#define SPI_IRQ_Enable_ERRIE                         (uint16_t)(1<<5)   //1: Error interrupt is enabled


/*
 * ================================================================
 * 						      APIs
 * ================================================================
 */

void MCAL_SPI_Init(SPI_TypeDef *SPIx, SPI_Config *SPI_config);
void MCAL_SPI_DeInit(SPI_TypeDef *SPIx);

void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx);

void MCAL_SPI_SendData(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn);
void MCAL_SPI_ReceiveData(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn);

void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx, uint16_t *pTxBuffer, enum Pollingmechanism pollingEn);



#endif /* INC_STM32_F103C6_SPI_DRIVER_H_ */
