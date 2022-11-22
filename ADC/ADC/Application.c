/*
 * ADC.c
 *
 * Created: 22/11/2022 4:35:37 PM
 * Author : Hossam Mamdouh
 */ 

#define F_CPU  8000000UL      // Set Clock Frequency tO be 8MHZ.

#include <avr/io.h>           // Basic microcontroller definitions.
#include "My_LCD.h"           // LCD module library.
#include "My_ADC.h"           // ADC module library.

int main(void)
{
	uint16_t ADC_Value;          // Variable to hold the reading of the ADC.
	LCD_Init(4);                 // Initialize LCD module in 4-bit mode.
	ADC_Init();                  // Initialize ADC module.
	
    while (1) 
    {
		ADC_Value = Read_ADC(0); // Read ADC Channel A0.
		LCD_Go_To(1,1);          // Set the cursor at Row 1, Column 4.
		LCD_Display_String("ADC Value = ");
		LCD_Display_Intger(ADC_Value);
		_delay_ms(1000);         // Wait for 1 second.
		LCD_Clear();             // Clear the LCD.
    }
}

