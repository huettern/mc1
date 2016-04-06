;
; huetter_noah_p1.asm
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


; ===================================================
; INIT
; ===================================================

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

	; PortA 4 input pull enable
	sbi PORTA, 4

	; PortA 7 output of serial data default 1
	sbi DDRA, 7
	sbi PORTA, 7

	; init dataregister
	ldi r20, 0b10101010

	;Timer1 init for baudrate generation
	ldi r16,0x00
	sts TCCR1A,r16	;Timer 0 Mode
	ldi r16,0x01
	sts TCCR1B,r16	;Timer 0 Control clk/1 prescale
	ldi r16,0x00
	sts TCCR1C,r16	;Timer 0 Control 

	ldi r16,0x00
	sts TCNT1H,r16	;Timer count Register set to 0
	sts TCNT1L,r16	;Timer count Register set to 0
	; timer counts qith clk/1 = 62.5ns
	; set compare register to 1667=0x0683 to compare after
	; 1667*62.5ns = 104us = 9615 baud
	; compare gets captured in interrupt
	ldi r16,0x06	; high
	ldi r17,0x83	; low
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

	; init USART1
	ldi r16, 103
	sts UBRR1L, r16
	lds r16, UCSR1B
	ldi r17, (1<<RXEN1)
	or r16, r17
	sts UCSR1B, r16

	;reg init
	ldi r16,0x00;
	sei							;global interrupt enable


; ===================================================
; MAIN
; ===================================================

main:
	call waitForPush
	call sendSerial
	call receiveSerial
	out PORTC, r21
	com r20
	call waitForRelease
	rjmp main

; ===================================================
; RX TX SUBROUTINES
; ===================================================

sendSerial:
	; store send reg
	mov r16, r20
	; send stop bit
	cbi PORTA, 7
	call waitBaud
	; init loop
	ldi r18, 8
	; start loop
sendSerialLoop:
	; output databit
	sbrc r16, 0
	sbi PORTA, 7
	sbrs r16, 0
	cbi PORTA, 7
	; delay
	call waitBaud
	ror r16
	dec r18
	breq sendSerialLoopDone
	rjmp sendSerialLoop
sendSerialLoopDone:
	; send stop bit
	sbi PORTA, 7
	call waitBaud
	; send is done
	ret

receiveSerial:
	; test for rx complete
	lds r16, UCSR1A
	sbrs r16, RXC1
	rjmp receiveSerialFail
	rjmp receiveSerialPass
receiveSerialFail:
	ldi r21, 0x00
	sbi PORTA, 0
	ret
receiveSerialPass:
	; data is now received
	lds r21, UDR1
	cbi PORTA, 0
	ret

; ===================================================
; BAUDRATE GENERATION
; ===================================================

waitBaud:
initBaudWaiter:
	; reset timer counter reg
	ldi r17, 0
	sts TCNT1L, r17
	sts TCNT1H, r17
	; reset r30
	ldi r30, 0x00
waitBaudPoll:
	; polls the r30 reg for timer capture
	ldi r17, 0xff
	cpse r30, r17
	rjmp waitBaudPoll
	ldi r30, 0x00
	ret

; ===================================================
; BUTTON TEST ROUTINES
; ===================================================

waitForPush:
	in r16, PINA
	ror r16
	ror r16
	ror r16
	ror r16
	ldi r17, 0x01
	and r16, r17
	; now btn is on bit 0
	ldi r17, 0x00
	cpse r16, r17
	rjmp waitForPush
	; now btn is pressed
	ret

waitForRelease:
	in r16, PINA
	ror r16
	ror r16
	ror r16
	ror r16
	ldi r17, 0x01
	and r16, r17
	; now btn is on bit 0
	ldi r17, 0x01
	cpse r16, r17
	rjmp waitForRelease
	; now btn is pressed
	ret


; ===================================================
; ISR
; ===================================================

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
	push r17
	ldi r17, 0
	sts TCNT1L, r17
	sts TCNT1H, r17
	; set r30 to indicate baud reached
	ldi r30, 0xff
	pop r17
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
	reti


; ===================================================
; DELAY HELPER FUNTION (not used)
; ===================================================

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

