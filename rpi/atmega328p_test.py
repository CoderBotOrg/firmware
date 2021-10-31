# RPi PINOUTS
# MOSI -> GPIO10
# MISO -> GPIO9
# SCK  -> GPIO11
# CE1  -> GPIO7
# CE1  -> GPIO8

# get the GPIO Library and SPI Library
import time
from atmega328p import ATMega328

def main():
    try:
        atmega = ATMega328()
        while True:
            for i in range(0, 11):
                atmega.digitalWrite(i, 0x00)
            print("on")
            time.sleep(1.0)
            for i in range(0, 11):
                atmega.digitalWrite(i, 0x01)
            print("off")
            time.sleep(1.0)
            ai1 = atmega.analogRead(0x00)
            ai2 = atmega.analogRead(0x01)
            di3 = atmega.digitalRead(0x02)
            di4 = atmega.digitalRead(0x03)
            di5 = atmega.digitalRead(0x04)
            di6 = atmega.digitalRead(0x05)
            print("AI1: {ai1}, AI2: {ai2}, DI3: {di3}, DI4: {di4}, DI5: {di5}, DI6: {di6}".format(ai1=ai1, ai2=ai2, di3=di3, di4=di4, di5=di5, di6=di6))
            time.sleep(1.0)
        atmega.close()
    except KeyboardInterrupt:
        # Ctrl+C pressed, so...
        atmega.close()

if __name__ == "__main__":
    main()
#End of the Script
