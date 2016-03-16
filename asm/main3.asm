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
	ldi r16, 0b00000000
	out PORTC, r16
	; PortA 2 low bits output
	ldi r16, 0b00000011
	out DDRA, r16
	ldi r16, 0b00000001
	out PORTA, r16
	; PortA 2,3 input pull enable
	sbi PORTA, 4
	sbi PORTA, 5

	; init stack pointer
	ldi r16, high(RAMEND)
	out SPH, r16
	ldi r16, low(RAMEND)
	out SPL, r16

	ldi r30, 0x00


main:
	call wait
	in r16, PINA
	ror r16
	ror r16
	ror r16
	ror r16
	ldi r17, 0x01
	and r16, r17
	; now the button is on the first bit
	ldi r17, 0x00
	cpse r16, r17
	rjmp main
	; if register is 0, continue here
	inc r30
	out PORTC, r30
	rjmp main




; Replace with your application code
test:
	in r16, PINA
	ror r16
	ror r16
	ldi r17, 0b00000011
	and r16, r17
	ldi r17, 0b00001100
	or r16, r17
	out PORTA, r16
	rjmp test
	


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
