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


#define ADC_MUX_MASK ADC0D

//================================================================================
// Funtion declarations
//================================================================================
void init();
void inti_send_buf();
void initIO ();
void initADC();

void init_timer0_COMPA();
void init_timer0_OVF();


int serial_putchar(char ch, FILE *unused);
unsigned int getADCBlocking();
unsigned int getADCVoltage();

//================================================================================
// Static data
//================================================================================
static FILE *LCD;
volatile static unsigned long long tick = 0;

//================================================================================
// Main routine
//================================================================================
//
int main()
{	
	init();
	printf("Done Init");


	//fprintf(LCD, "42 is the answer");
	while(1){
		if(tick >= 250)
		{
			tick = 0;
			printf("Tick\n");
		}
 	}
}

//================================================================================
// Init Functions
//================================================================================
/* Init routine
 *
 */
void init() 
{
	// Init filestream for console and LCD
	static FILE fd_stdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &fd_stdout;
	static FILE fd_lcdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	LCD = &fd_lcdout;
	// Send buffer for console
	inti_send_buf();

	// Init IO Ports
	initIO();

	// Init LCD
	init_lcd();
	initLCDADC();

	/* Time init */
	//init_timer0_COMPA();
	init_timer0_OVF();


	sei();
}



//================================================================================
// Functions
//================================================================================
/**
 * @brief      Inits the serial send buffer, 57600 baud
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

}


/**
 * @brief      Inits ADC
 */
void initADC ()
{
	//ADC for LCD buttons
	// Is already done in lcd init!
	// ADMUX|=1<<REFS0;
	// ADCSRB|=1<<ACME;
	// ADCSRA|=1<<ADEN|1<<ADSC|1<<ADPS2|1<<ADPS1|1<<ADPS0;
}

/**
 * Returns voltage on the AD pin in mV
 */
unsigned int getADCBlocking()
{
	// Set channel
	ADMUX = (ADMUX & 0xe0) | (ADC_MUX_MASK & 0x1f);
	ADCSRB = (ADCSRB & 0xf7) | ((ADC_MUX_MASK & 0x20)>>2);

	ADCSRA|=1<<ADSC; //Read

	while((ADCSRA>>ADSC)&1); //Wait

	// Set channel back
	ADMUX = (ADMUX & 0xe0) | (0 & 0x1f);
	ADCSRB = (ADCSRB & 0xf7) | ((0 & 0x20)>>2);

	return (unsigned int)ADC * 5000L / 1024;
}

void init_timer0_COMPA() {
    TCCR0A |= (1<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00); //COM -> Clear OC0A on Compare Match / WGM -> CTC-Mode 
    TCCR0B |= (1<<CS02) | (0<<CS01) | (0<<CS00); //Prescaler 256 -> 16us
    OCR0A |= 250 - 1; //16MHz/256/250 --> alle 250 counts in den ISR-Timer -> 4ms Compare
    TIMSK0 |= (0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0); //Timer 0 Enabled
}
 

void init_timer0_OVF() {
    TCCR0B |= (1<<CS02) | (0<<CS01) | (0<<CS00); //Prescaler 256 -> 16us
    TIMSK0 |= (0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0); //Timer 0 Enabled
}


//================================================================================
// Interrupt Functions
//================================================================================
 
ISR(TIMER0_COMPA_vect) {
    //4ms tics
    tick++;
}
 
ISR(TIMER0_OVF_vect) {

}
