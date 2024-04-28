/*
 * lcd_driver.c
 *
 *  Created on: Jul 11, 2023
 *      Author: ahmed osama
*/
#include <lcd_driver.h>

GPIO_PConfig_t PinCfg ;

void wait_ms(uint32_t time)
{
	int i , j;
	for(i=0 ; i<time ; i++)
		for(j=0 ; j<255 ; j++);
}

void LCD_check_isbusy()
{

	//set PORT to receive data (data direction as input)
	PinCfg.GPIO_PinNumber=GPIO_PIN_0;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_1;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_2;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_3;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_4;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_5;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_6;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_7;
	PinCfg.GPIO_MODE=GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	//put LCD in READ mode
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_SET);
	//put LCD in command mode
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);

	//set and reset enable pin
	LCD_kick();

	//put LCD in WRITE mode
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);


}

void LCD_GPIO_init() {

	PinCfg.GPIO_PinNumber=EN_SWITCH;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PinNumber=RS_SWITCH;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PinNumber=RW_SWITCH;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	// SET 8 PINS AS OUTPUT
	PinCfg.GPIO_PinNumber=GPIO_PIN_0;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_1;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_2;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_3;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_4;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_5;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_6;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);

	PinCfg.GPIO_PinNumber=GPIO_PIN_7;
	PinCfg.GPIO_MODE=GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_Output_SPeed=GPIO_OutputSpeed_10M;
	MCAL_GPIO_Init(LCD_PORT, &PinCfg);


	MCAL_GPIO_WritePin(LCD_CTRL, EN_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);

}

void LCD_INIT()
{
	wait_ms(20);
	LCD_GPIO_init();
	wait_ms(15);
	LCD_clear_screen();

	#ifdef EIGHT_BIT_MODE
	LCD_WRITE_COMMAND(LCD_FUNCTION_8BIT_2LINES);
    #endif

	#ifdef FOUR_BIT_MODE
	LCD_WRITE_COMMAND(0X02);
	LCD_WRITE_COMMAND(LCD_FUNCTION_4BIT_2LINES);
	#endif

	LCD_WRITE_COMMAND(LCD_ENTRY_MODE);
	LCD_WRITE_COMMAND(LCD_BEGIN_AT_FIRST_ROW);
	LCD_WRITE_COMMAND(LCD_DISP_ON_CURSOR_BLINK);
}
void LCD_WRITE_COMMAND(unsigned char command)
{

	#ifdef EIGHT_BIT_MODE
	MCAL_GPIO_WritePort(LCD_PORT, command);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	wait_ms(1);
	LCD_kick();

	#endif
	#ifdef FOUR_BIT_MODE
	MCAL_GPIO_WritePin(LCD_PORT, ((command >> 4) & 0x0F) << DATA_shift);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	LCD_kick();

	MCAL_GPIO_WritePin(LCD_PORT, (command & 0x0F) << DATA_shift);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	wait_ms(1);
	LCD_kick();
	#endif
}
void LCD_WRITE_CHAR(unsigned char character)
{

	#ifdef EIGHT_BIT_MODE
	MCAL_GPIO_WritePort(LCD_PORT, character);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_SET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	wait_ms(1);
	LCD_kick();

	#endif
	#ifdef FOUR_BIT_MODE
	MCAL_GPIO_WritePin(LCD_PORT, ((character >> 4) & 0x0F) << DATA_shift);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_SET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	LCD_kick();

	MCAL_GPIO_WritePin(LCD_PORT, (character & 0x0F) << DATA_shift);
	MCAL_GPIO_WritePin(LCD_CTRL, RS_SWITCH, GPIO_PIN_SET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW_SWITCH, GPIO_PIN_RESET);
	wait_ms(1);
	LCD_kick();

	#endif
}
void LCD_WRITE_STRING(char* string)
{
	uint32_t count=0;
	while (*string != '\0')
	{
		LCD_WRITE_CHAR(*string++);
		count++;
		if(count == 16)
		{
			LCD_GOTO_XY(2, 0);
		}
		else if(count == 32)
		{
			LCD_clear_screen();
			LCD_GOTO_XY(1, 0);
			count=0;
		}
	}


}
void LCD_kick(void)
{
	MCAL_GPIO_WritePin(LCD_CTRL, EN_SWITCH, GPIO_PIN_SET);
	wait_ms(50);
	MCAL_GPIO_WritePin(LCD_CTRL, EN_SWITCH, GPIO_PIN_RESET);
}
void LCD_clear_screen()
{
	LCD_WRITE_COMMAND(LCD_CLEAR_SCREEN);
}
void LCD_GOTO_XY(unsigned char line, unsigned char position)
{
	if(line==1)
	{
		if(position<16 && position>0)
		{
			LCD_WRITE_COMMAND(LCD_BEGIN_AT_FIRST_ROW + position);
		}
	}
	else if(line==2)
	{
		if(position<16 && position>0)
		{
			LCD_WRITE_COMMAND(LCD_BEGIN_AT_SECOND_ROW + position);
		}

	}
}


