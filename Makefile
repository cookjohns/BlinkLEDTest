# Name: Makefile
# Author: Richard Chapman
# Copyright: Auburn University, 2015, S.D.G. 
# License: <insert your license reference her Auburn University, 2015, S.D.G. 

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"

DEVICE     = attiny85 
CLOCK      = 1000000
PROGRAMMER = -c usbtiny #-c stk500v2 -P avrdoper
OBJECTS    = accelTestSmall.o USI_TWI_Master.o 

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	accelTestSmall.hex

accelTestSmall.o:
	$(COMPILE) -c $< -o accelTestSmall.o accelTestSmall.c 

USI_TWI_Master.o:
	avr-gcc  -mmcu=attiny85 -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -MD -MP -MT USI_TWI_Master.o -MF dep/USI_TWI_Master.o.d  -c  USI_TWI_Master.c
 


flash:	all
	$(AVRDUDE) -U flash:w:accelTestSmall.hex:i


# Xcode uses the Makefile targets "", "clean" and "install"
install: flash 


clean:
	rm -f accelTestSmall.hex accelTestSmall.elf $(OBJECTS)

# file targets:
accelTestSmall.elf: $(OBJECTS)
	$(COMPILE) -o accelTestSmall.elf $(OBJECTS)

accelTestSmall.hex: accelTestSmall.elf
	rm -f accelTestSmall.hex
	avr-objcopy -j .text -j .data -O ihex accelTestSmall.elf accelTestSmall.hex
	avr-size --format=avr --mcu=$(DEVICE) accelTestSmall.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	accelTestSmall.elf
	avr-objdump -d accelTestSmall.elf

cpp:
	$(COMPILE) -E accelTestSmall.c
