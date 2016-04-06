;
; Vorlage.asm
;
; Created: 05.04.2016 22:10:01
; Author : Noah Huetter
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

	
	; PortC output LEDs
    ldi r16, 0xff
	out DDRC, r16
	ldi r16, 0b00000000
	out PORTC, r16

	; PortA 2 low bits output
	ldi r16, 0b00000011
	out DDRA, r16
	ldi r16, 0b00000000
	out PORTA, r16

	; PortA 2,3 input pull enable
	sbi PORTA, 4
	sbi PORTA, 5

	;External interrupt init
	;ldi r16,0x0A
	;sts EICRA,r16	;interrupt sense control 0-3
	;sts EICRB,r16	;interrupt sense control 4-7
	;ldi r16,0x00
	;out EIMSK,r16	;interrupt mask
	;EIFR = external interrupt flag register


	;Timer presclae config
	ldi r16,0x00	
	out GTCCR,r16	;General Timer Control Reg / Synchron Mode
	
	;Timer0 init
	ldi r16,0x00
	sts TCCR1A,r16	;Timer 0 Mode
	ldi r16,0x05
	sts TCCR1B,r16	;Timer 0 Control clk/1024 prescale
	ldi r16,0x00
	sts TCCR1C,r16	;Timer 0 Control 
	
	;Timer1 init
	ldi r16,0x00
	sts TCNT1H,r16	;Timer count Register set to 0
	sts TCNT1L,r16	;Timer count Register set to 0
	ldi r16,0x3d	; high
	ldi r17,0x09	; low
	sts OCR1AH,r16	;Compare A Reg set to 0
	sts OCR1AL,r17	
	ldi r16,0	; high
	ldi r17,0	; low
	sts OCR1BH,r16	;Compare B Reg set to 0
	sts OCR1BL,r17	
	ldi r16,0	; high
	ldi r17,0	; low
	sts OCR1CH,r16	;Compare C Reg set to 0
	sts OCR1CL,r17	

	ldi r16,0	; high
	ldi r17,0	; low
	sts ICR1H,r16	;Input capture Reg set to 0
	sts ICR1L,r17	

	ldi r16,0x02
	sts TIMSK1,r16	;Interrupt Mask, all off
	;TIFR0 = interrupt Flag

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
	;push r16
	in r16, PORTC
	inc r16
	out PORTC, r16
	ldi r17, 0
	sts TCNT1L, r17
	sts TCNT1H, r17
	;pop r16
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

