/*
 * I2C_Slave_EEPROM.c
 *
 *  Created on: Aug 3, 2023
 *      Author: ahmed osama
 */


#include "I2C_Slave_EEPROM.h"

void eeprom_Init(void)
{
	I2C_Config I2C1_cfg;
	I2C1_cfg.General_Call_Address_Detection= I2C_ENGC_Enable;
	I2C1_cfg.I2C_ACK_Control= I2C_ACK_Control_Enable;
	I2C1_cfg.I2C_CLK_speed= I2C_CLK_speed_SM_100k;
	I2C1_cfg.I2C_Mode= I2C_Mode_I2C;
	I2C1_cfg.P_Slave_Event_CallBack=NULL;
	I2C1_cfg.I2C_Stretch_Mode=I2C_Stretch_Mode_Enable;

	MCAL_I2C_GPIO_Set_Pins(I2C1);
	MCAL_I2C_Init(I2C1, &I2C1_cfg);

}

uint8_t eeprom_write_Nbytes(unsigned int Memory_address, uint8_t* Bytes, uint8_t dataLength)
{
	uint8_t buffer[256];
	buffer[0]= Memory_address>>8; 	//upper byte memory address
	buffer[1]= Memory_address;		//lower byte memory address
	uint8_t i ;
	//Data
	for(i=2; i < (dataLength+2); i++)
	{
		buffer[i]= Bytes[i-2];
	}
	MCAL_I2C_Master_Tx(I2C1, EEPROM_Slave_address, buffer, (dataLength+2), With_Stop, start);
	return 0;
}
uint8_t eeprom_read_byte(unsigned int Memory_address, uint8_t* Bytes, uint8_t dataLength)
{
	uint8_t buffer[2];
	buffer[0]= Memory_address>>8; 	//upper byte memory address
	buffer[1]= Memory_address;		//lower byte memory address

	MCAL_I2C_Master_Tx(I2C1, EEPROM_Slave_address, buffer, 2, With_Stop, start);
	MCAL_I2C_Master_Rx(I2C1, EEPROM_Slave_address, Bytes, dataLength, With_Stop, RepeatedStart);
	return 0;

}
