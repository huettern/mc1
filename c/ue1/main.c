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
void init ();

void inti_send_buf();
int serial_putchar(char ch, FILE *unused);
void initIO ();

unsigned char readButtons ();

FILE* LCD;

//================================================================================
// Main routine
//================================================================================
//
int main()
{	
	unsigned char btns;
	unsigned char btns_old = 0x00;
	init();

	fprintf(LCD, "42 is the answer");

	PORTB |= (1<<PB4); // LCD Backlight switch
//	sei();

	while(1){
		btns = readButtons();
		if(((btns ^ btns_old) == 0x01) && (btns == 0x01))
		{
			// we have toggle of btn0
			PORTA^=0x02;
		}
		if(((btns ^ btns_old) == 0x02) && (btns == 0x02))
		{
			// we have toggle of btn1
			PORTC++;
		}
		btns_old=btns;
		// printf("Hello World\r\n");
		_delay_ms(50);
 	}
}



//================================================================================
// Functions
//================================================================================

/**
 * @brief      Inits everything
 */
void init ()
{
	static FILE fd_stdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &fd_stdout;
	static FILE fd_lcdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	LCD = &fd_lcdout;
	
	inti_send_buf();
	initIO();

	init_lcd();
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
	DDRB |= (1<<PB4);	// LCD Backlight switch
	     
	// two buttons on PA4 and PA5  
	// CLR(DDRA, PA4);     
	// CLR(DDRA, PA5); 
	// SET(PORTA, PA4);
	// SET(PORTA, PA5);
	DDRA |= (1<<PA0) | (1<<PA1);
	DDRA &= ~((1<<PA4) | (1<<PA5));
	PORTA |= (1<<PA4) | (1<<PA5);
}

/**
 * @brief      Reads the two buttons on Port A
 *
 * @return     buttons on bit 0 and 1
 */
unsigned char readButtons () 
{
	unsigned char buttons;
	buttons = PINA;

	buttons ^= 0xff;

	buttons &= 0x30;

	buttons >>= 4;

	return buttons;
}