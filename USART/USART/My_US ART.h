#ifndef MY_USART_H_
#define MY_USART_H_

#include <avr/io.h>

// Global Variables.
uint8_t i;

// Functions Prototypes
void UART_Init(unsigned long Baud_Rate);
void UART_Transmit_Byte(uint8_t Data);
uint8_t UART_Receive_Byte(void);
void UART_Transmit_String(uint8_t *Data);
void UART_Receive_String(uint8_t *Data);

// Functions Definitions.
void UART_Init(unsigned long Baud_Rate)
{
	unsigned long Baud_Prescale = (((F_CPU / (Baud_Rate * 16UL))) - 1);
	
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8  used for 9-bit data mode
	 ***********************************************************************/ 
	
	// Enable Interrupts.
	UCSRB = (1 << RXCIE) | (1 << TXCIE);
			
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
	
	// UCSZ2 = 0, UCSZ1 = 1, and UCSZ0 = 1 For 8-bit data mode.
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	
	// Load First 8 bits from the Baud_Prescale inside UBRRL and last 4 bits in UBRRH
	UBRRL = (uint8_t) (Baud_Prescale);
	UBRRH = (uint8_t) (Baud_Prescale >> 8);
}

void UART_Transmit_Byte(uint8_t Data)
{
	// Wait for empty transmit buffer.
	while (!( UCSRA & (1 << UDRE)));
	// Put the required Data in the UDR register
	UDR = Data;
}

uint8_t UART_Receive_Byte(void)
{
	// Wait for data to be received.
	while (!(UCSRA & (1 << RXC)));
	// Get and return received data from buffer.
	return UDR;
}

void UART_Transmit_String(uint8_t *Data)
{
	i = 0;
	while (Data[i] != '\0') 
	{
		UART_Transmit_Byte(Data[i]);
		i++;
	}
}

void UART_Receive_String(uint8_t* Data)
{
	i = 0;
	Data[i] = UART_Receive_Byte();
	while (Data[i] != '#') 
	{
		i++;
		Data[i] = UART_Receive_Byte();
	}
	Data[i] = '\0';
}

#endif /* MY_USART_H_ */