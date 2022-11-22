/*
 * LCD_8_BIT_MODE.c
 *
 * Created: 22/11/2022 4:13:05 PM
 * Author : Hossam Mamdouh
 */ 

#define F_CPU  8000000UL      // Set Clock Frequency tO be 8MHZ.

#include <avr/io.h>           // Basic microcontroller definitions.
#include "My_LCD.h"           // LCD module library.


int main(void)
{
	LCD_Init(8);              // Initialize LCD module in 8-bit mode.
	
	while (1)
	{
		LCD_Go_To(1,4);       // Set the cursor at Row 1, Column 4.
		LCD_Display_String("Welcome to");
		LCD_Go_To(2,2);       // Set the cursor at Row 1, Column 2.
		LCD_Display_String("LCD 8-bit Mode");
		_delay_ms(1000);      // Wait for 1 second.
		LCD_Clear();          // Clear the LCD.
	}
}