avr-gcc -Os -mmcu=atmega328p -I/usr/lib/avr/include -c blink.c
avr-gcc -mmcu=atmega328p -o blink.elf blink.o
avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
sudo avrdude -P /dev/spidev0.1 -c linuxspi -p m328p -U flash:w:blink.hex
