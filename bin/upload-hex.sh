sudo avrdude -P /dev/spidev0.0 -c linuxspi -p m328p -U flash:w:$1
