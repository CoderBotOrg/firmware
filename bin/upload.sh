echo "uploading firmware to atmega"
sudo avrdude -P /dev/spidev0.0 -c linuxspi -p m328p -U lfuse:w:0xE2:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -U lock:w:0xFF:m -U flash:w:firmware/atmega.hex
touch firmware/initialised
