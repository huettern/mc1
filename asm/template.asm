;
; Vorlage.asm
;
; Created: 21.03.2016 11:43:51
; Author : Sven Baumgartner
;
.include "m2560def.inc"

interrupts:
.org 0
	jmp init
.org 0x0002
	jmp ISR_int0
.org 0x0004
	jmp ISR_int1
.org 0x001A
	jmp ISR_Timer2_CompA
.org 0x001C
	jmp ISR_Timer2_CompB
.org 0x001E
	jmp ISR_Timer2_Ovf
.org 0x0020
	jmp ISR_Timer1_Capt
.org 0x0022
	jmp ISR_Timer1_CompA
.org 0x0024
	jmp ISR_Timer1_CompB
.org 0x0026
	jmp ISR_Timer1_CompC
.org 0x0028
	jmp ISR_Timer1_Ovf
.org 0x002A
	jmp ISR_Timer0_CompA
.org 0x002C
	jmp ISR_Timer0_CompB
.org 0x002E
	jmp ISR_Timer0_Ovf


init:
	;Stackpointer init
	ldi r16,high(RAMEND)		
	out SPH,r16
	ldi r16,high(RAMEND)
	out SPH,r16

	;IO Ports init
	ldi r16,0xff				
	out DDRC,r16
	ldi r16,0x00
	out DDRD,r16
	ldi r16,0xff
	out PORTD,r16

	;External interrupt init
	ldi r16,0x0A
	sts EICRA,r16	;interrupt sense control 0-3
	sts EICRB,r16	;interrupt sense control 4-7
	ldi r16,0x00
	out EIMSK,r16	;interrupt mask
	//EIFR = external interrupt flag register


	;Timer init
	ldi r16,0x00	
	out GTCCR,r16	;General Timer Control Reg / Synchron Mode
	
	ldi r16,0x00
	out TCCR0A,r16	;Timer 0 Mode
	ldi r16,0x05
	out TCCR0B,r16	;Timer 0 Control
	ldi r16,0x00
	out TCNT0,r16	;Timer Register
	out OCR0A,r16	;Compare A Reg
	out OCR0B,r16	;Compare B Reg
	ldi r16,0x01
	sts TIMSK0,r16	;Interrupt Mask
	//TIFR0 = interrupt Flag


	;reg init
	ldi r16,0x00;
	sei							;global interrupt enable

main:
	out PORTC,r16
	rjmp main

ISR_int0:
	reti

ISR_int1:
	reti

ISR_Timer2_CompA:
	reti

ISR_Timer2_CompB:
	reti

ISR_Timer2_Ovf:
	reti

ISR_Timer1_Capt:
	reti

ISR_Timer1_CompA:
	reti

ISR_Timer1_CompB:
	reti

ISR_Timer1_CompC:
	reti

ISR_Timer1_Ovf:
	reti

ISR_Timer0_CompA:
	reti

ISR_Timer0_CompB:
	reti

ISR_Timer0_Ovf:
	inc r16
	reti


delay:
	push r16
	push r17
	push r18
	ldi r16,0xff
	ldi r17,0xff
	ldi r18,0x0A
	loop:
		dec r16
		brne loop
		dec r17
		brne loop
		dec r18
		brne loop
	pop r18
	pop r17
	pop r16
	ret

