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
void initTimer();

//================================================================================
// Static data
//================================================================================
static char seconds, minutes, hours;
volatile static char update = 0;


//================================================================================
// Main routine
//================================================================================
//
int main()
{	
	static char s[] = "Hello";
	static FILE fd_stdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &fd_stdout;
	static FILE fd_lcdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	FILE *LCD = &fd_lcdout;
	
	inti_send_buf();
	initIO();
	init_lcd();
	initLCDADC();

	initTimer();

	//fprintf(LCD, "42 is the answer");

	hours = 9;
	minutes = 38;
	seconds = 0;

	//sei();

	s[5]='!';
	s[6]='o';
	fprintf(LCD,"%s", s);

	while(1){
		
 	}
}



//================================================================================
// Functions
//================================================================================

void initTimer()
{
	TCCR0A = 1<<WGM01;
	TCCR0B = 1<<CS02;
	TIMSK0 = 1<<OCIE0A;
	OCR0A = 250-1;
}

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

ISR(TIMER0_COMPA_vect) 
{
	static unsigned char tics;
	if(++tics == 250)
	{
		tics = 0;
		if(++seconds == 60)
		{
			seconds = 0;
			if(++minutes == 60) 
			{
				minutes = 0;
				if(++hours == 24)
				{
					hours = 0;
				} 
			}
		}
		update = 1;
	}	
}