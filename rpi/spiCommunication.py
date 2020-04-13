# RPi PINOUTS
# MOSI -> GPIO10
# MISO -> GPIO9
# SCK  -> GPIO11
# CE1  -> GPIO7
# CE1  -> GPIO8

# get the GPIO Library and SPI Library
import spidev
import time


#Initialze the SPI 
spi = spidev.SpiDev()

try:
    spi.open(0,0)
    spi.max_speed_hz = 250000
    while True:
        resp = spi.xfer([0x00])
        print("resp: " + str(resp))
        time.sleep(1.0)
        resp = spi.xfer([0x01])
        print("resp: " + str(resp))
        time.sleep(1.0)
    spi.close()
except KeyboardInterrupt:
    # Ctrl+C pressed, so...
    spi.close()

#End of the Script
