/*
 * I2C_Slave_EEPROM.h
 *
 *  Created on: Aug 3, 2023
 *      Author: ahmed osama
 */

#ifndef INC_I2C_SLAVE_EEPROM_H_
#define INC_I2C_SLAVE_EEPROM_H_

#include "Stm32_F103C6_I2C_driver.h"

/*
 * E2PROM is an i2c slave
 * Idle Mode: device is in high-impedance state and waits for data
 * Master Transmitter Mode: device transmits data to slave
 * Master Receiver Mode: device receive data from slave
 */

#define EEPROM_Slave_address 0x2A

void eeprom_init(void);
uint8_t eeprom_write_Nbytes(unsigned int Memory_address, uint8_t* Bytes, uint8_t dataLength);
uint8_t eeprom_read_byte(unsigned int Memory_address, uint8_t* Bytes, uint8_t dataLength);

#endif /* INC_I2C_SLAVE_EEPROM_H_ */
