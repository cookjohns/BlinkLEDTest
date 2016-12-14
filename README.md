# Autonomous Parachute Deployment for UAVs

The purpose of this research is to develop a modular, embedded system that provides for the safe landing of an unmanned aerial vehicle (UAV) upon failure or other undesirable conditions. Such conditions include, but are not limited to, loss of power, destabilized flight, and entrance into restricted airspace.

![alt text](https://drive.google.com/uc?id=0B0RNg9f7VJiEbjNmSU93Mjdoelk)

#### Requires avrdude/avr-gcc toolchain. To install:

1. Install home-brew (for MacOS only)
   ```
   ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/ install)"
   ```

2. Install avr-gcc toolchain

   On MacOS:
   ```
   brew tap osx-cross/avr
   brew install avr-libc
   ```

   On Linux:
   ```
   sudo apt-get install gcc-avr binutils-avr avr-libc
   sudo apt-get install gdb-avr
   ```

3. Install avrdude

   On MacOS:
   ```
   brew install avrdude --with-usb
   ```

   On Linux:
   ```
   sudo apt-get install avrdude
   ```

#### Compilation instructions:
Compile:
```
avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -c systemMain.c -o systemMain.o
```
Link:
```
avr-gcc -Wall -Os -DF_CPU=1000000 -mmcu=attiny85 -o systemMain.elf systemMain.o
```
Generate hex:
```
avr-objcopy -j .text -j .data -O ihex systemMain.elf systemMain.hex
```
Write to chip:
```
avrdude -c usbtiny -p t85 -p attiny85 -U flash:w:systemMain.hex:i
```
