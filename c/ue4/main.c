/**
 *	filename: main.c
 *	Author: Noah Huetter
 *	Email:	noahhuetter@gmail.com 
 */

//================================================================================
// Includes
//================================================================================
#include "header.h"

//================================================================================
// Defines
//================================================================================
#define LEDS PORTC

#define BAUD (57600)
#define UBR_VAL ((F_CPU/16/BAUD)-1)


//================================================================================
// Funtion declarations
//================================================================================
void inti_send_buf();
int serial_putchar(char ch, FILE *unused);
void initIO ();
long getADCBlocking();


//================================================================================
// Main routine
//================================================================================
//
int main()
{	
	long volts;
	int high;
	int low;
	enum LCD_KEY enLcdKey = 0;
	static FILE fd_stdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &fd_stdout;
	static FILE fd_lcdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	FILE *LCD = &fd_lcdout;
	
	inti_send_buf();
	initIO();

	init_lcd();

	initLCDADC();

//	fprintf(LCD, "42 is the answer");

	LEDS=0x55;
//	sei();

	while(1){
		enLcdKey = get_lcd_key(0xff);
		if(enLcdKey == LCD_KEY_SELECT)
		{
			fprintf(LCD, "S" );
		}
		if(enLcdKey == LCD_KEY_LEFT)
		{
			fprintf(LCD, "L" );
		}
		if(enLcdKey == LCD_KEY_DOWN)
		{
			fprintf(LCD, "D" );
		}
		if(enLcdKey == LCD_KEY_UP)
		{
			fprintf(LCD, "U" );
		}
		if(enLcdKey == LCD_KEY_RIGHT)
		{
			fprintf(LCD, "R" );
		}
		if(enLcdKey == LCD_KEY_NONE)
		{
			fprintf(LCD, "N" );
		}

		fprintf(LCD, "\r" );

		volts = getADCBlocking();
		high = volts/1000;
		low = volts%1000;
		fprintf(LCD, "U=%d.%03dV", high, low );
		_delay_ms(200);
		fprintf(LCD, "\n" );


 	}
}



//================================================================================
// Functions
//================================================================================

/**
 * @brief      Inits the serial send buffer
 */
void inti_send_buf()
{
		UBRR0L=UBR_VAL&0xFF;
		UBRR0H=(UBR_VAL>>8)&0xFF;
		UCSR0B=(1<<RXEN0)|(1<<TXEN0);
		UCSR0C=(3<<UCSZ00);
}

/**
 * @brief      Puts char on serial port
 *
 * @param[in]  ch      char
 * @param      unused  FILE stream
 *
 * @return     always 0
 */
int serial_putchar(char ch, FILE *unused)
{
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=ch;
	return 0;
}

/**
 * @brief      Inits IO Ports
 */
void initIO () 
{
	DDRC = 0xff;
}


long getADCBlocking()
{
	ADCSRA|=1<<ADSC; //Read

	while((ADCSRA>>ADSC)&1); //Wait

	return (long)ADC * 5000 / 1024;
}