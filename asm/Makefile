# makefile
A=./avra
TTY=/dev/ttyS0

TargetDir=./
TargetName=huetter_noah_p1

all: $(TargetName).hex

$(TargetName).hex: $(TargetName).asm
	$A $(TargetName).asm

install: $(TargetName).hex
 #       avrdude -p $N -c $P -P $(TTY) -U flash:w:beisp1.hex:i
	avrdude -F -c wiring -p ATMEGA2560 -P $(TTY) -b 115200 -U flash:w:$(TargetDir)$(TargetName).hex:i
clean:
	rm -f *~ *.obj *.cof