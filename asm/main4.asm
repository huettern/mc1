;
; 
;
; Created: 02.03.2016 11:49:38
; Author : noahh_000
;
.include "m2560def.inc"


.org 0
	jmp init
.org INT0addr
	jmp ISR_INT0
.org INT1addr
	jmp ISR_INT1


init:
	; init stack pointer
	ldi r16, high(RAMEND)
	out SPH, r16
	ldi r16, low(RAMEND)
	out SPL, r16

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

	; Setup INT1 on PORTD1, Arduino Pin 20 and 21
	cbi DDRD, 1
	sbi PORTD, 1
	cbi DDRD, 0
	sbi PORTD, 0

	; Enable external interrupt
	ldi r16, (1 << ISC11) | (1 << ISC01)
	sts EICRA, r16
	in r16, EIMSK
	ori r16, (1<<INT1) | (1<<INT0)
	out EIMSK, r16

	sei		; Global interrupt enable


main:

	rjmp main



wait:
	ldi r20, 20
	ldi r21, 200
	ldi r22, 200
loop1:
	dec r22
	brne loop1
loop2:
	ldi r22, 200
	dec r21
	brne loop1
loop3:
	ldi r21, 200
	ldi r22, 200
	dec r20
	brne loop1
	ret


    ;rjmp start

ISR_INT1:
	; save contents of SREG
	in r16, SREG
	push r16

	; do interrupt stuff
	; read
	in r16, PORTC\

	; modify
	inc r16

	; write
	out PORTC, r16

	; restore contents of SREG
	pop r16
	out SREG, r16

	reti


ISR_INT0:
	; save contents of SREG
	in r16, SREG
	push r16

	; do interrupt stuff
	; read
	in r16, PORTC\

	; modify
	dec r16

	; write
	out PORTC, r16

	; restore contents of SREG
	pop r16
	out SREG, r16

	reti