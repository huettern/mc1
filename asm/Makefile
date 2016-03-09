# makefile
A=./avra
N=m2560
P=stk500v2
TTY=/dev/ttyS0

TargetDir=./
TargetName=main

all: main.hex

main.hex: main.asm
	$A main.asm

install:
 #       avrdude -p $N -c $P -P $(TTY) -U flash:w:beisp1.hex:i
	avrdude -F -c wiring -p ATMEGA2560 -P $(TTY) -b 115200 -U flash:w:$(TargetDir)$(TargetName).hex:i
clean:
	rm -f *~ *.obj *.cof