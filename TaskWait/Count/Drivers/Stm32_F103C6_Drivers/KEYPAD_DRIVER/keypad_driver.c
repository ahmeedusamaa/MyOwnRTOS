/*
 * keypad_driver.c
 *
 *  Created on: Jul 13, 2023
 *      Author: ahmed osama
 */



#include "keypad_driver.h"

GPIO_PConfig_t PinCfg;

int Keypad_Row[] = {R0, R1, R2, R3}; //rows of the keypad
int Keypad_Col[] = {C0, C1, C2, C3}; //columns of the keypad

void Keypad_Init(){

	    //set rows output
		PinCfg.GPIO_PinNumber=R0;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=R1;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=R2;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=R3;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		//set columns input
		PinCfg.GPIO_PinNumber=C0;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=C1;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=C2;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		PinCfg.GPIO_PinNumber=C3;
		PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
		PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
		MCAL_GPIO_Init(KEYPAD_PORT, &PinCfg);

		MCAL_GPIO_WritePort(KEYPAD_PORT, 0x1FF);

}


char Keypad_ScanKey()
{

	uint32_t row=0,col=0;
		for (row = 0;row < 4; row++)
		{
			MCAL_GPIO_WritePin(KEYPAD_PORT, R0, GPIO_PIN_SET);
			MCAL_GPIO_WritePin(KEYPAD_PORT, R1, GPIO_PIN_SET);
			MCAL_GPIO_WritePin(KEYPAD_PORT, R2, GPIO_PIN_SET);
			MCAL_GPIO_WritePin(KEYPAD_PORT, R3, GPIO_PIN_SET);

			MCAL_GPIO_WritePin(KEYPAD_PORT, Keypad_Row[row], GPIO_PIN_RESET);

			for(col = 0;col < 4; col++){
				if (! (MCAL_GPIO_ReadPin(KEYPAD_PORT, Keypad_Col[col])))
				{
					while(! (MCAL_GPIO_ReadPin(KEYPAD_PORT, Keypad_Col[col])));
					switch(row){
						case (0):
							if (col == 0)      return '7';
							else if (col == 1) return '8';
							else if (col == 2) return '9';
							else if (col == 3) return '/';
							break;
						case (1):
							if (col == 0)      return '4';
							else if (col == 1) return '5';
							else if (col == 2) return '6';
							else if (col == 3) return '*';
							break;
						case (2):
							if (col == 0)      return '1';
							else if (col == 1) return '2';
							else if (col == 2) return '3';
							else if (col == 3) return '-';
							break;
						case (3):
							if (col == 0) 	   return 'c';
							else if (col == 1) return '0';
							else if (col == 2) return '=';
							else if (col == 3) return '+';
							break;
					}
				}
			}
		}
	return 'A';
}




