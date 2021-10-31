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

BAUDRATE        = 10000

START           = 0xff
CMD_RESET       = 0x00
CMD_SET_DATA    = 0x01
CMD_GET_DATA    = 0x02

def main():
    try:
        spi.open(0,0)
        spi.max_speed_hz = 250000
        while True:
            digitalWrite(0x00, 0x00)
            digitalWrite(0x01, 0x00)
            digitalWrite(0x02, 0x00)
            digitalWrite(0x03, 0x00)
            digitalWrite(0x04, 0x00)
            digitalWrite(0x05, 0x00)
            digitalWrite(0x06, 0x00)
            digitalWrite(0x07, 0x00)
            digitalWrite(0x08, 0x00)
            digitalWrite(0x09, 0x00)
            digitalWrite(0x0a, 0x00)
            time.sleep(1.0)
            digitalWrite(0x00, 0x01)
            digitalWrite(0x01, 0x01)
            digitalWrite(0x02, 0x01)
            digitalWrite(0x03, 0x01)
            digitalWrite(0x04, 0x01)
            digitalWrite(0x05, 0x01)
            digitalWrite(0x06, 0x01)
            digitalWrite(0x07, 0x01)
            digitalWrite(0x08, 0x01)
            digitalWrite(0x09, 0x01)
            digitalWrite(0x0a, 0x01)
            time.sleep(1.0)
            """resp = digitalRead(0x02)
            print("digital read 2: " + str(resp), end='')
            resp = digitalRead(0x03)
            print(" digital read 3: " + str(resp), end='')
            resp = digitalRead(0x04)
            print(" digital read 4: " + str(resp), end='')
            resp = digitalRead(0x05)
            print(" digital read 5: " + str(resp), end='')
            resp = analogRead(0x00)
            print(" analog read 0: " + str(resp), end='')
            resp = analogRead(0x01)
            print("i analog read 1: " + str(resp))"""
        spi.close()
    except KeyboardInterrupt:
        # Ctrl+C pressed, so...
        spi.close()

def digitalWrite(addr, value):
    resp = spi.xfer([START, CMD_SET_DATA, addr, value, 0], BAUDRATE)
 
def digitalRead(addr):
    resp = spi.xfer([START, CMD_GET_DATA, addr, 0, 0], BAUDRATE)
    return resp[3]

def analogRead(addr):
    resp = spi.xfer([START, CMD_GET_DATA, addr, 0, 0], BAUDRATE)
    return resp[3]

if __name__ == "__main__":
    main()
#End of the Script
