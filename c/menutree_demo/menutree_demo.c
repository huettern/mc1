/* MenuTreeDemo.c 
 * Simple LCD menu example based on structs 
 * (c) 2013-2016 by Matthias Meier
 * Released under the GPL V3
 * Remark: declaring all strings using PSTR() in PROGMEM and printing by printf_P("%S") would save SRAM space...
 *	but unfortunately PROGMEM isn'nt allowd in structs, so string constants would have to be declared outside them
 */
 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../lcd_keypad_m2560/lcd_keypad_m2560.h"
#include "board.h"
#include "menutree.h"

//------------------------------------------------------------------------------

//menu text and and prototype function for example menu (implemented below)

void MF_Beep(void);  		
void MF_CountPort(void);			
void MF_LoadmenuLED(void);	
void MF_LoadmenuMain(void);	
void MF_LEDon(void); 		
void MF_LEDoff(void);		
void MF_LEDtoggle(void);		
void MF_LEDdisp(void);

static char read_buttons(void);

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

//Implementation of the examples menu functions...
//Rem: in a real application, implement this and function prototypes in different module(s)

//beep 0.5s at 1kHz
void MF_Beep(void) {
	int i; 
	for (i=0;i<1000;i++) {
		BEEPER_PORT ^= BEEPER_MASK; 
		_delay_ms(0.5);
	} 
}

//count on Port C
void MF_CountPort(void) {
	PORTC++;
	printf("\nPORTC = %d",PORTC); 
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
	LED |= LED_MASK; 
}

// set LED off
void MF_LEDoff(void) { 
	LED &= ~LED_MASK; 
}

// toggle LED
void MF_LEDtoggle(void) { 
	LED ^= LED_MASK; 
}

// show LED value and return from Submenu
void MF_LEDdisp(void) { 
	printf("\nLED is %s", (LED & LED_MASK) ? "on" : "off"); 
	_delay_ms(1000); 
}

//------------------------------------------------------------------------------

int main()
{
	static FILE lcd_fd = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &lcd_fd;	//Standardausgabe auf obigen Stream setzen
	init_lcd();

	// initialisation of example hardware used
	LED_DDR = LED_MASK;	//set LED as output
	BEEPER_DDR |= BEEPER_MASK;	//set Beeper as output
	BUTTON_PORT |= BUTTONS_ALL;
	DDRC = 0xff;

	printf("MenuTreeDemo\rhit key to start");
	while (!read_buttons()) ;

	LoadMenu(MainMenu);

	while (1) 
	{
		int buttons;
		buttons = read_buttons();
		if (buttons) {
			ProcessMenu(buttons);
		}
	}

}

//------------------------------------------------------------------------------


static char read_buttons(void)
{
	static char buttons_old;	//static -> old button state since last call
	char buttons_now, buttons_new;
	buttons_now = ~BUTTON_PINS & BUTTONS_ALL; 	// read actual button state (buttons are inverted)
	buttons_new = ~buttons_old & buttons_now;	//additions since last update
	if (buttons_old != buttons_now) {
		buttons_old = buttons_now; //save for next update
		_delay_ms(10);			// debounce on key change
	}
	return buttons_new;
}
