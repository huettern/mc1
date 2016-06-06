/**
 *	filename: main.c
 *	Author: Noah Huetter
 *	Email:	noahhuetter@gmail.com 
 */

//================================================================================
// Includes
//================================================================================
#include "header.h"
#include "../menutree_demo/menutree.h"

//================================================================================
// Defines
//===============================================================================


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

unsigned char getButton0();
unsigned char getButton1();
void setLED(unsigned int led);

/* Menu */
void MF_Beep(void);  		
void MF_CountPort(void);			
void MF_LoadmenuLED(void);	
void MF_LoadmenuMain(void);	
void MF_LEDon(void); 		
void MF_LEDoff(void);		
void MF_LEDtoggle(void);		
void MF_LEDdisp(void);


//------------------------------------------------------------------------------
//Definition of the menu tree by declaring and initializing array variables of type MenuItem_T...
struct MenuItem_T MainMenu[] = { 
	{"Beep 0.5s",		 MF_Beep},
	{"Count on PORTC",	 MF_CountPort},
	{"Submenu LED...",	 MF_LoadmenuLED},
};

struct MenuItem_T  SubMenu[] = { 
	{"set LED on", 		MF_LEDon},
	{"set LED off", 	MF_LEDoff},
	{"toggle LED",		MF_LEDtoggle},
	{"Disp LED value",	MF_LEDdisp},
	{"Back to Mainmenu",  MF_LoadmenuMain}
};

//------------------------------------------------------------------------------


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
	// unsigned char t1, t2;
	init();

	// t1 = getButton0();
	// t2 = getButton1();


	// setLED(0x300);

	// fprintf(LCD, "42 is the answer");
	// printf("Tick\n");
	
	// while(1){
	// 	if(getButton0())
	// 	{
	// 		printf("getButton0\r\n");
	// setLED(0xfff);
	// 	}
	// 	if(getButton1())
	// 	{
	// 		printf("getButton1\r\n");
	// setLED(0x000);
	// 	}
	// 	if(tick >= 250)
	// 	{
	// 		tick = 0;
	// 		printf("Tick\n");
	// 	}
 // 	}

	LoadMenu(MainMenu);

	while (1) 
	{
		int buttons;
		_delay_ms(80);			// debounce on key change
		buttons = (getButton1()<<1) | getButton0();
		if (buttons) {
			ProcessMenu(buttons);
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
	// static FILE fd_stdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	// stdout = &fd_stdout;
	static FILE fd_stdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &fd_stdout;
	// static FILE fd_lcdout= FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
	// LCD = &fd_lcdout;
	static FILE fd_lcdout= FDEV_SETUP_STREAM(serial_putchar, NULL, _FDEV_SETUP_WRITE);
	LCD = &fd_lcdout;
	// Send buffer for console
	inti_send_buf();

	// Init IO Ports
	initIO();

	// Init LCD
	init_lcd();
	initLCDADC();

	/* Time init */
	init_timer0_COMPA();
	//init_timer0_OVF();


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
	//PORT A
	//Buttons init
	DDRA	&= ~((1<<BUTTON0)|(1<<BUTTON1)); //buttons auf A.6 & A.7
	PORTA	|= ((1<<BUTTON0)|(1<<BUTTON1)); //set pullups
	//LEDs Init
	DDRA	|= ((1<<LED_HI0)|(1<<LED_HI1));//LEDs auf A.0 & A.1
	
	//Port C LED_PORT
	DDRC = 0xFF; //1=Output
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

unsigned char getButton0(){
	unsigned char but;
	but = ((~PINA & (1<<BUTTON0))>>BUTTON0);
	return but;
}

unsigned char getButton1(){
	unsigned char but;
	but = ((~PINA & (1<<BUTTON1))>>BUTTON1);
	return but;
}

void setLED(unsigned int led){
	unsigned char led_lo;
	unsigned char led_hi;
	//set low byte
	led_lo=(led&0x00FF);
	LED_PORT=led_lo;
	//set high bits
	led_hi=(led>>8);
	led_hi=(led_hi&0x03);
	PORTA = (PORTA&0xFC)|led_hi;
}



//================================================================================
// Menu functions
//================================================================================
 
//------------------------------------------------------------------------------

//Implementation of the examples menu functions...
//Rem: in a real application, implement this and function prototypes in different module(s)

//beep 0.5s at 1kHz
void MF_Beep(void) {
	fprintf(LCD, "MF_Beep\r\n");
}

//count on Port C
void MF_CountPort(void) {
} 

// change to submenu LED
void MF_LoadmenuLED(void) {
	LoadMenu(SubMenu); 
}

// change back to main menu
void MF_LoadmenuMain(void) {
	LoadMenu(MainMenu); 
}

// set LED on
void MF_LEDon(void) {
}

// set LED off
void MF_LEDoff(void) {
}

// toggle LED
void MF_LEDtoggle(void) { 
}

// show LED value and return from Submenu
void MF_LEDdisp(void) { 
	fprintf(LCD,"\nLED is 42"); 
	_delay_ms(1000); 
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
