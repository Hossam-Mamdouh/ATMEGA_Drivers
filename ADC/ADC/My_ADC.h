/*
 * My_ADC.h
 * ATMEGA32 / ATMEGA16
 * Created: 08/05/2022 1:41:24 am
 * Author: Hossam Mamdouh
 */ 

#ifndef MY_ADC_H_
#define MY_ADC_H_

#include <avr/io.h>
#include <util/delay.h>

// Functions Prototypes
void ADC_Init (void);
uint16_t Read_ADC (unsigned char Channel);

// Functions Definitions.
void ADC_Init (void)
{	
	// ADCSRA bit 7 [ADEN] = 1 to Enable ADC module, ADCSRA Bit 5 [ADATE] = 0
	// to select Single Conversion mode, ADCSRA bits 2,1,0 [ADPS] = 001
	// to set the division factor for the pre scaler to 2.
	ADCSRA = 0x81;
}

uint16_t Read_ADC (unsigned char Channel)
{
	ADMUX  = Channel;         // Select the ADC Channel.
	_delay_us(10);            // For ADC input voltage stabilization
	ADCSRA |= 0x40;           // ADCSRA bit 6 [ADSC] = 1 to start conversion.
	// Wait until ADCSRA bit 5 (end of conversion flag) [ADIF] = 1.
	while ((ADCSRA & 0x10) == 0);
	ADCSRA |= 0x10;           // Reset end of conversion flag.
	return ADCW;              // Return the ADC Data Register.
}

#endif /* MY_ADC_H_ */