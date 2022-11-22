/*
 * My_USART.h
 * ATMEGA32 / ATMEGA16
 * Created: 08/05/2022 1:07:50 am
 * Author: Hossam-Mamdouh
 */ 


#ifndef MY_USART_H_
#define MY_USART_H_

#include <avr/io.h>

// Functions Prototypes
void USART_Init(unsigned long Baud_Rate, unsigned char Operation_Mode, unsigned char Data_Size, unsigned char Parity, unsigned char Stop_Bit, unsigned char Interrupts);
void USART_Transmit_Byte(uint8_t Data);
uint8_t USART_Receive_Byte(void);
void USART_Transmit_String(uint8_t *Data);
void USART_Receive_String(uint8_t *Data);

// Functions Definitions.
void USART_Init(unsigned long Baud_Rate, unsigned char Operation_Mode, unsigned char Data_Size, unsigned char Parity, unsigned char Stop_Bit, unsigned char Interrupts)
{
	unsigned long Baud_Prescale = (((F_CPU / (Baud_Rate * 16UL))) - 1);
	
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART Rx Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8  used for 9-bit data mode
	 ***********************************************************************/ 
	if (Interrupts == 1)
	{
		// Enable Interrupts.
		UCSRB = (1 << RXCIE) | (1 << TXCIE);
	}
	// Enable Receiver and Transmitter.
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	if (Operation_Mode == 1)
	{
		// Set Synchronous Operation
		UCSRC = (1 << UMSEL);
	}
	
	if (Parity == 1)
	{
		// Set Odd Parity
		UCSRC = (1 << UPM1) | (1 << UPM0);
	}	

	if (Parity == 2)
	{
		// Set Even Parity
		UCSRC = (1 << UPM1);
	}
		
	if (Data_Size == 6)
	{
		// UCSZ2 = 0, UCSZ1 = 0, and UCSZ0 = 1 For 6-bit data mode.
	    UCSRC = (1 << URSEL) | (1 << UCSZ0);
	}
	
	if (Data_Size == 7)
	{
		// UCSZ2 = 0, UCSZ1 = 1, and UCSZ0 = 0 For 7-bit data mode.
	    UCSRC = (1 << URSEL) | (1 << UCSZ1);
	}
	
	if (Data_Size == 8)
	{
		// UCSZ2 = 0, UCSZ1 = 1, and UCSZ0 = 1 For 8-bit data mode.
	    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	}
	
	if (Data_Size == 9)
	{
		// UCSZ2 = 1, UCSZ1 = 1, and UCSZ0 = 1 For 8-bit data mode.
		UCSRB = (1 << UCSZ2);
	    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	}
	if (Stop_Bit == 2)
	{
		// Set 2 Stop bits
		UCSRC = (1 << USBS);
	}	
	
	// Load First 8 bits from the Baud_Prescale inside UBRRL and last 4 bits in UBRRH
	UBRRH = Baud_Prescale >> 8;
	UBRRL = Baud_Prescale;
}

void USART_Transmit_Byte(uint8_t Data)
{
	// Wait for empty transmit buffer.
	while (!( UCSRA & (1 << UDRE)));
	// Put the required Data in the UDR register
	UDR = Data;
}

uint8_t USART_Receive_Byte(void)
{
	// Wait for data to be received.
	while (!(UCSRA & (1 << RXC)));
	// Get and return received data from buffer.
	return UDR;
}

void USART_Transmit_String(uint8_t *Data)
{
	uint8_t i = 0;
	while (Data[i] != '\0') 
	{
		USART_Transmit_Byte(Data[i]);
		i++;
	}
}

void USART_Receive_String(uint8_t* Data)
{
	uint8_t i = 0;
	Data[i] = USART_Receive_Byte();
	while (Data[i] != 0x0A)         // Exit when the user presses Enter.
	{
		i++;
		Data[i] = USART_Receive_Byte();
	}
	Data[i] = '\0';
}

#endif /* MY_USART_H_ */