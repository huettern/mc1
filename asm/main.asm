;
; asm_v2_lauflicht.asm
;
; Created: 02.03.2016 11:49:38
; Author : noahh_000
;
.include "m2560def.inc"

init:
	; PortC output LEDs
    ldi r16, 0xff
	out DDRC, r16
	ldi r16, 0b00000011
	out PORTC, r16
	; PortA 2 low bits output
	ldi r16, 0b00000011
	out DDRA, r16
	ldi r16, 0b00000001
	out PORTA, r16
	; init stack pointer
	ldi r16, high(RAMEND)
	out SPH, r16
	ldi r16, low(RAMEND)
	out SPL, r16


; Replace with your application code
left:
	in r16, PORTC
	rol r16
	out PORTC, r16
	call wait
	ldi r30, 0b11000000
	cpse r16, r30
	rjmp left

right:
	in r16, PORTC
	ror r16
	out PORTC, r16
	call wait
	ldi r30, 0b00000011
	cpse r16, r30
	rjmp right
	rjmp left


wait:
	ldi r20, 2
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
