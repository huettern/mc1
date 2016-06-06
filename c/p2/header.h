

#ifndef __HEADER_H__
#define __HEADER_H__

//================================================================================
// Includes
//================================================================================
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../lcd_keypad_m2560/lcd_keypad_m2560.h"
#include "../lcd_keypad_m2560/lcd_keys.h"
#include "../util/interrupts.h"

//================================================================================
// Defines
//================================================================================
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifdef DO_DEBUG
	#define DEBUG(s) {\
			printf( "\n%s:%d\r", s, __FILE__, __LINE__);\
			_delay_ms(1000);\
		}
#else
	#define DEBUG(s) 
#endif

#define SET(b, n) {b|=(1<<(n));}
#define CLR(b, n) {b&=~(1<<(n));}


#define BAUD (57600)
#define UBR_VAL ((F_CPU/16/BAUD)-1)


#define ADC_MUX_MASK ADC0D


#define LED_PORT  PORTC
#define LED_HI0  0x00
#define LED_HI1  0x01

#define BUTTON0 4
#define BUTTON1 5

#define BUTTON_EXECUTE 0x02
#define BUTTON_MENU 0x01

#endif