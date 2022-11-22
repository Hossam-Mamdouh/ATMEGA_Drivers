/*
 * USART.c
 *
 * Created: 11/22/2022 5:03:15 PM
 * Author : Hossam Mamdouh
 */

#define F_CPU  8000000UL      // Set Clock Frequency tO be 8MHZ.

#include <avr/io.h>           // Basic microcontroller definitions.
#include <avr/interrupt.h>
#include "My_USART.h"         // USART module library.
#include "My_LCD.h"           // LCD module library.

int main(void)
{
    sei();                    // Enable Global interrupts.
	LCD_Init(4);              // Initialize LCD module in 4-bit mode.
	
	/* 
     * Initialize USART module with the following configurations:-
	 * Baud Rate = 9600, 0: Asynchronous mode, 8: 8-bit DataSize.
	 * 0: No Parity, 1: 1 Stop bit, 1: Enable Interrupts.
	 */
	USART_Init(9600, 0, 8, 0, 1, 1);
	
	while (1) 
    {
    }
}

// Interrupt Service Routine for USART Receive Complete Interrupt RXC.
ISR(USART_RXC_vect)
{
	uint8_t Data[100];
	// Read data from the USART module.
	USART_Receive_String(Data);
	
    // Echo the data from the USART module.
	USART_Transmit_Byte(0x0A);   // Print new line.
	USART_Transmit_String(Data); // Print the Data.
	
	// Display the data from the USART module on the LCD.
	LCD_Go_To(1,5);       // Set the cursor at Row 1, Column 2.
    LCD_Display_String((char *)Data);	
	_delay_ms(1000);      // Wait for 1 second.
	LCD_Clear();          // Clear the LCD.
}

