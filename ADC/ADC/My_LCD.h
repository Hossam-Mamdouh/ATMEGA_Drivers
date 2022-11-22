/*
 * My_LCD.h
 * For ATMEGA32 / ATMEGA16
 * Created: 08/05/2022 12:14:56 am
 * Author: Hossam-Mamdouh
 */ 

#ifndef LCD_H_
#define LCD_H_

#include <util/delay.h>
#include <stdlib.h>

volatile unsigned char Data_Mode;

// LCD HW Pins
#define RS PB0
#define RW PB1
#define EN PB2
#define LCD_CTRL_PORT        PORTB
#define LCD_CTRL_PORT_DIR    DDRB
#define LCD_DATA_PORT        PORTB
#define LCD_DATA_PORT_DIR    DDRB

// LCD Commands
#define Clear                0x01
#define Eight_bit_Mode		 0x38
#define Init_Four_bit_Mode   0x32
#define Four_bit_Mode		 0x28
#define Cursor_OFF		     0x0C
#define Cursor_ON		     0x0E
#define Cursor_Location      0x80

// Functions Prototypes
void LCD_Init(unsigned char DataMode);
void LCD_Command(uint8_t Command);
void LCD_Display_Char(uint8_t Data);
void LCD_Display_String(const char *Data);
void LCD_Go_To(uint8_t Row,uint8_t Column);
void LCD_Display_Intger(int Data);
void LCD_Clear(void);

// Functions Definitions.

/* 
NAME       : LCD_Init
DESCRIPTION: Initialize the LCD module
INPUTS:
             Data Mode [4: for 4-bit mode , 8: for 8-bit mode] 
OUTPUTS:
             NONE.		 
*/
void LCD_Init(unsigned char DataMode)
{
	Data_Mode = DataMode;
	if (DataMode == 4) // Initialize LCD in 4-bit mode
	{
	    // Configure the LCD Port as Output Port.
		LCD_DATA_PORT_DIR = 0xF0;
		// Configure the Control pins (E,RS,RW) as Output pins.
		LCD_CTRL_PORT_DIR |= (1<<EN) | (1<<RS) | (1<<RW);
		// Send for 4 bit initialization of LCD.
		LCD_Command(Init_Four_bit_Mode);
		// use 2-line lcd + 4-bit Data Mode + 5*8 dot display Mode.
		LCD_Command(Four_bit_Mode);
		//Set Cursor OFF.
		LCD_Command(Cursor_OFF);
		// Clear LCD at the beginning.
		LCD_Command(Clear);
		LCD_Command(Cursor_Location);
	}
	else if (Data_Mode == 8) // Initialize LCD in 8-bit mode
	{
		// Configure the Data port (D0 --> D7) as Output port.
		LCD_DATA_PORT_DIR |= 0xFF;
		// Configure the Control pins (E,RS,RW) as Output pins.
		LCD_CTRL_PORT_DIR |= (1<<EN) | (1<<RS) | (1<<RW);
		// use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode.
		LCD_Command(Eight_bit_Mode);
		//Set Cursor OFF.
		LCD_Command(Cursor_OFF);
		// Clear LCD at the beginning.
		LCD_Command(Clear);
	}
}

/* 
NAME       : LCD_Command
DESCRIPTION: Send a Command to the LCD
INPUTS:
             Command [Clear, Eight_bit_Mode, Init_Four_bit_Mode, Four_bit_Mode, Cursor_OFF, Cursor_ON, Cursor_Location] 
OUTPUTS:
             NONE.		 
*/
void LCD_Command(uint8_t Command)
{
	if (Data_Mode == 4)
	{
		// Send the Upper 4 bits of the required Command to the data bus (D4 --> D7)
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (Command & 0xF0);
		
		// Instruction Mode (RS = 0).
		LCD_CTRL_PORT &= ~(1 << RS);
		// Writing to LCD (RW = 0).
		LCD_CTRL_PORT &= ~(1 << RW);
		_delay_ms(1);
		// Enable LCD (EN = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_us(1);
		
		// Disable LCD (EN = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
		_delay_ms(1);
		
		// Send the Lower 4 bits of the required Command to the data bus (D4 --> D7)
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (Command << 4);
		
		// Enable LCD (EN = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_us(1);
		
		// Disable LCD (EN = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
	}
	else if (Data_Mode == 8)
	{
		// Send the required Command to the data bus D0 --> D7.
	    LCD_DATA_PORT = Command;
		_delay_ms(5);
		
		// Instruction Mode (RS = 0).
		LCD_CTRL_PORT &= ~(1 << RS);
		// Writing to LCD (RW = 0).
		LCD_CTRL_PORT &= ~(1 << RW);
		_delay_ms(1);
		// Enable LCD (E = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_ms(3);
		// Disable LCD (E = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
		_delay_ms(5);
	}
}

/* 
NAME       : LCD_Display_Char
DESCRIPTION: Display a Character on the LCD
INPUTS:
             Character
OUTPUTS:
             NONE.		 
*/
void LCD_Display_Char(uint8_t Data)
{	
	if (Data_Mode == 4)
	{
		// Send the Upper 4 bits of the required Data to the data bus (D4 --> D7)
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (Data & 0xF0);
		
		// Data Mode (RS = 1).
		LCD_CTRL_PORT |= (1 << RS);
		// Writing to LCD (RW = 0).
		LCD_CTRL_PORT &= ~(1 << RW);
		// Enable LCD (EN = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_us(1);
		// Disable LCD (EN = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
		_delay_us(200);
		
		// Send the Lower 4 bits of the required Data to the data bus (D4 --> D7)
		LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (Data << 4);
		// Enable LCD (EN = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_us(1);
		// Disable LCD (EN = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
		_delay_ms(2);
	}
	
	else if (Data_Mode == 8)
	{
		// Send the required Data to the data bus D0 --> D7.
		LCD_DATA_PORT = Data;
		_delay_ms(5);
		// Data Mode (RS = 1).
		LCD_CTRL_PORT |= (1 << RS);
		// Writing to LCD (RW = 0).
		LCD_CTRL_PORT &= ~(1 << RW);
		_delay_ms(1);
		// Enable LCD (E = 1).
		LCD_CTRL_PORT |= (1 << EN);
		_delay_ms(1);
		// Disable LCD (E = 0).
		LCD_CTRL_PORT &= ~(1 << EN);
		_delay_ms(5);
	}
}

/* 
NAME       : LCD_Display_String
DESCRIPTION: Display a String on the LCD
INPUTS:
            String
OUTPUTS:
            NONE.		 
*/
void LCD_Display_String(const char *Data)
{
	uint8_t i = 0;
	while(Data[i] != '\0')
	{
		LCD_Display_Char(Data[i]);
		i++;
	}
}

/* 
NAME       : LCD_Go_To
DESCRIPTION: Set the cursor at a specific specific location 
INPUTS:
             Row[1 : 4].
	         Column[1 : 20].
OUTPUTS:
             NONE.		 
*/
void LCD_Go_To(uint8_t Row,uint8_t Column)
{
	Row = Row - 1;
	Column = Column - 1;
	uint8_t Address = 0;
	// First, calculate the required Address.
	switch(Row)
	{
		case 0:
		Address = Column;
		break;
		case 1:
		Address = Column + 0x40;
		break;
		case 2:
		Address = Column + 0x10;
		break;
		case 3:
		Address = Column + 0x50;
		break;
	}
	LCD_Command(Address | Cursor_Location);
}

/*
NAME       : LCD_Display_Intger
DESCRIPTION: Display an Intger on the LCD
INPUTS:
            Intger
OUTPUTS:
            NONE.		 
*/
void LCD_Display_Intger(int Data)
{
	char Buffer[16];             // String to hold the ascii result.
	itoa(Data , Buffer , 10);    // 10 for decimal.
	LCD_Display_String(Buffer);
}

/* 
NAME       : LCD_Clear
DESCRIPTION: Clear the LCD and set the cursor at the beginning of the first Row.
INPUTS:
             NONE.
OUTPUTS:
             NONE.		 
*/

void LCD_Clear(void)
{
	// Send Clear LCD command.
	LCD_Command(Clear);
}

#endif /* LCD_H_ */