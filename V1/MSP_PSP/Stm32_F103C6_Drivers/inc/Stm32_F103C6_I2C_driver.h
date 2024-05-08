/*
 * Stm32_F103C6_I2C_driver.h
 *
 *  Created on: Jul 31, 2023
 *      Author: ahmed osama
 */

#ifndef INC_STM32_F103C6_I2C_DRIVER_H_
#define INC_STM32_F103C6_I2C_DRIVER_H_

#include "Stm32_F103x6.h"
#include "Stm32_F103C6_gpio_driver.h"
#include "Stm32_F103C6_RCC_driver.h"

struct S_I2C_Slave_Address
{
	uint16_t	Dual_Address_Enable; //1:Enable 0:Disable
	uint16_t	Primary_slave_address;
	uint16_t	Secondary_Slave_address;
	uint16_t	I2C_Addressing_Slave_mode;//@ref I2C_Addressing_Slave_mode_define
};

typedef enum
{
	I2C_EV_STOP,
	I2C_ERROR_AF,
	I2C_EV_ADDR_Matched,
	I2C_EV_DATA_REQ,		//I2C Slave Send Data
	I2C_EV_DATA_RCV			//I2C Slave Receive Data
}Slave_State;

typedef struct
{
	uint32_t   						I2C_CLK_speed;			//specifies I2C clock speed
															//This parameter must be set based on @ref I2C_CLK_speed_define
	uint32_t						I2C_Mode;				//specifies I2C mode
															//This parameter must be set based on @ref I2C_Mode_define
	uint32_t						I2C_Stretch_Mode;		//specifies I2C enable or disable Stretch Mode
															//This parameter must be set based on @ref I2C_Stretch_Mode_define

	uint32_t						I2C_ACK_Control;		//specifies I2C enable or disable Acknowledge
															//This parameter must be set based on @ref I2C_ACK_Control_define

	struct S_I2C_Slave_Address      I2C_Slave_address;

	uint32_t 						General_Call_Address_Detection;  //@ref I2C_ENGC


	void(* P_Slave_Event_CallBack)(Slave_State state);

}I2C_Config;

/*
 * ===============================================================
 * 						Reference Macros
 * ===============================================================
 */

//@ref I2C_Stretch_Mode_define
//I2C_CR1
#define  I2C_Stretch_Mode_Enable	(0x00000000UL)			//0: Clock stretching enabled
#define  I2C_Stretch_Mode_Disable	I2C_CR1_NOSTRETCH       //1: Clock stretching disabled

/*
 *@ref I2C_CLK_speed_define
 */
//Bits 5:0 FREQ[5:0]: Set Peripheral clock frequency
//Configure the clock control registers --->>>     CCR[11:0]:Thigh = CCR * TPCLK1
//Configure the rise time register	------>>>	  TRISE[5:0]:(1000 ns / 125 ns = 8 + 1)
//enable the peripheral
#define I2C_CLK_speed_SM_50k		(0x50UL)
#define I2C_CLK_speed_SM_100k		(0x100000UL)

//@ref I2C_Mode_define
//I2C_CR1
#define	 I2C_Mode_I2C				0
#define	 I2C_Mode_SMBus				I2C_CR1_SMBUS

//@ref I2C_Addressing_Slave_mode_define
//I2C_OAR1
#define I2C_Addressing_Slave_mode_7Bit		(0x00000000UL)			//7-bit slave address (10-bit address not acknowledged)
#define I2C_Addressing_Slave_mode_10Bit		(uint16_t)(1<<15)		//1: 10-bit slave address (7-bit address not acknowledged)

//@ref I2C_ACK_Control_define
#define I2C_ACK_Control_Enable 				I2C_CR1_ACK
#define I2C_ACK_Control_Disable 			(uint16_t)(0x0UL)

//@ref I2C_ENGC
#define I2C_ENGC_Enable						I2C_CR1_ENGC
#define I2C_ENGC_Disable					0x00000000UL



typedef enum  {	With_Stop , Without_Stop} Stop_Condition;
typedef enum  {start , RepeatedStart} Reapeated_Start;
typedef enum  {Disable , Enable} Functional_State;
typedef enum  {Reset, Set} FlagStatus;
#define  Event_Master_Transmitter_Busy_RegisterEmpty   (uint32_t)(0x00070080UL)
typedef enum
{
	I2C_Flag_BUSY,
	EV1,																//EV1: ADDR=1, cleared by reading SR1 followed by reading SR2
	EV2,																//EV2: RxNE=1 cleared by reading DR register.
	EV4,																//EV4: STOPF=1, cleared by reading SR1 register followed by writing to the CR1 register
	EV5,																//EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address
	EV6,																//EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
	EV7,																//EV7: RxNE=1 cleared by reading DR register.
	EV8,																//EV8: TxE=1, shift register not empty, data register empty, cleared by writing DR register .
	EV8_2,																//EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
	Master_Transmitter_Busy_RegisterEmpty=(uint32_t)(0x00070080UL)		//TRA, BUSY, MSL, TXE flags


} Status;
typedef enum  { I2C_Direction_Transmitter, I2C_Direction_Recieve } I2C_Direction;


/*========================================================
 * 						APIs
 *========================================================
 */
void MCAL_I2C_Init(I2C_TypeDef* I2Cx, I2C_Config* I2C_Config);
void MCAL_I2C_DeInit(I2C_TypeDef* I2Cx);
void MCAL_I2C_GPIO_Set_Pins(I2C_TypeDef* I2Cx);

//Master polling mechanism
void MCAL_I2C_Master_Tx(I2C_TypeDef* I2Cx, uint16_t slaveAddress, uint8_t *dataOut, uint32_t dataLen, Stop_Condition stop,Reapeated_Start start);
void MCAL_I2C_Master_Rx(I2C_TypeDef* I2Cx, uint16_t slaveAddress, uint8_t *dataOut, uint32_t dataLen, Stop_Condition stop,Reapeated_Start start);

//Slave interrupt mechanism


//Generic APIs
void I2C_Generate_Start(I2C_TypeDef* I2Cx,Functional_State Status, Reapeated_Start start);
FlagStatus I2C_Get_Flag_Status(I2C_TypeDef* I2Cx,  Status flag);
void I2C_SendAddress(I2C_TypeDef* I2Cx, uint16_t address, I2C_Direction Dir);  /* I2C_Direction: Read/Write operation? */
void I2C_GenerateStop(I2C_TypeDef *I2Cx,Functional_State State);
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx  , Functional_State State);


#endif /* INC_STM32_F103C6_I2C_DRIVER_H_ */
