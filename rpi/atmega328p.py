# RPi PINOUTS
# MOSI -> GPIO10
# MISO -> GPIO9
# SCK  -> GPIO11
# CE1  -> GPIO7
# CE1  -> GPIO8

# get the GPIO Library and SPI Library
import spidev
import time

BAUDRATE_MAX    = 250000
BAUDRATE        = 10000

START           = 0xff
CMD_RESET       = 0x00
CMD_SET_DATA    = 0x01
CMD_GET_DATA    = 0x02

class ATMega328():

    instance = None

    @classmethod
    def get_instance(cls):
        if cls.instance is None:
            cls.instance = ATMega328()

    def __init__(self):
        #Initialze the SPI 
        self.spi = spidev.SpiDev()
        self.spi.open(0,0)
        self.spi.max_speed_hz = BAUDRATE_MAX

    def close(self):
        self.spi.close()

    def digitalWrite(self, addr, value):
        resp = self.spi.xfer([START, CMD_SET_DATA, addr, value, 0], BAUDRATE)
 
    def digitalRead(self, addr):
        resp = self.spi.xfer([START, CMD_GET_DATA, addr, 0, 0], BAUDRATE)
        return resp[3]

    def analogRead(self, addr):
        resp = self.spi.xfer([START, CMD_GET_DATA, addr, 0, 0], BAUDRATE)
        return resp[3]

